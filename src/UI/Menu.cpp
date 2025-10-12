#include <UI.h>

#include <unordered_set>

Menu::Menu() {
    menus["this"]    = this;    
    set_default_functions();
}

Menu::Menu(std::string _name) {
    menus["this"]  = this;
    name    = _name;
    text    =  name; 
    title   =  name;
    set_default_functions();
}

Menu::Menu(Menu* parent, std::string _name) {
    menus["this"]    = this;
    menus["parent"]  = parent;
    name    = _name; 
    text    =  name;
    title   = parent->title + "." + name;
    set_default_functions();
}

void Menu::create_button(std::string buttonLabel) {
    create_button(
        buttonLabel,
        [](TgBot::Message::Ptr message, User* user, std::string cmd) {
            user->set_menu(user->get_menu()->get_menu("command_" + cmd));
        }
    );
}



void Menu::create_button(std::string buttonLabel, void (*proc)(TgBot::Message::Ptr message, User* user, std::string cmd)) {

    Menu* buttonMenu = new Menu(this, "command_"+buttonLabel);
    Button* button = new Button(buttonLabel, buttonMenu);

    buttons[button->get_name()] = button;

    commandParser.add_key(button->get_name());
    
    menus["command_" + buttonLabel] = buttonMenu;
    button->command = proc;
}

Button* Menu::create_button_link(std::string buttonLabel, Menu* menuNext) {

    Button* button = new Button(buttonLabel, menuNext);

    buttons[button->get_name()] = button;
    commandParser.add_key(button->get_name());

    button->command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {
        
        Button* button = user->get_menu()->get_button(cmd);
        if (button == nullptr) {
            printf("button \"%s\" is NULL\n", cmd.c_str());
        }
        user->set_menu(user->get_menu()->get_button(cmd)->get_menu_next());
        user->get_menu()->send_menu(message, user);
    };
    return button;
}



// задание функций по умлочанию
void Menu::set_default_functions() {

    send_menu = [](TgBot::Message::Ptr message, User* user) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard = TgBot::InlineKeyboardMarkup::Ptr(new TgBot::InlineKeyboardMarkup);
        for (auto& button: user->get_menu()->get_buttons()) {
            keyboard->inlineKeyboard.push_back({button.second->get_tg_button()});
        }
        TgBot::Message::Ptr lastMenu = user->get_last_sended_menu();
        if (lastMenu != nullptr) {
            user->get_masBot()->get_tgBot()->getApi().deleteMessage(lastMenu->chat->id, lastMenu->messageId);
        }

        user->set_last_sended_menu(
            user->get_masBot()->get_tgBot()->getApi().sendMessage(message->chat->id, user->get_menu()->get_text(), 0, 0, keyboard)
        );
    };
    
    proc_command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {
        
        if (user->get_menu()->buttons.count(cmd) && user->get_menu()->buttons[cmd]->command != nullptr) {
            user->get_menu()->buttons[cmd]->command(message, user, cmd);
        } else {
            printf("command \"%s\" not found in menu \"%s\"\n", cmd.c_str(), user->get_menu()->get_title().c_str());
        }
    };

    proc_message = [](TgBot::Message::Ptr message, User* user) {

        std::string cmd = user->get_menu()->parse_command(message->text);
        if (cmd != "" && user->get_menu()->commandParser.check_key(cmd)) {
            user->get_menu()->proc_command(message, user, cmd);
        } 

        if (cmd == "event" && user->is_admin()) {
            MASBot* masBot = user->get_masBot();
            masBot->get_events()->reg_event(message->text.substr(cmd.size()));
        }

        user->get_menu()->send_menu(message, user);  
    };

    if (menus["parent"] != nullptr) {
        // создаем кнопку "назад"
        create_button_link("Назад 🔙", menus["parent"]); 
    }
}



std::string Menu::parse_command(const std::string& text) {
    std::unordered_set<char> separators = {'\t', '\n', ' '};
    size_t pos = 0;
    for(; pos < text.size() && separators.count(text[pos]); ++pos);
    std::string command = "";
    for(; pos < text.size() && !separators.count(text[pos]); ++pos) {
        command += text[pos];
    }
    to_lower(command);
    return command;
}

void Menu::to_lower(std::string& message) {
    int ch;
    for(int i = 0; i < message.size(); i+=2) {
        if (message[i] == -48 && message[i+1] < -80 && message[i+1] > -113) {
            message[i+1]+=32;
            if (message[i+1] > -65){
                message[i+1]-=64;
                ++message[i];
            }
        }
        if (message[i] == -48 && message[i+1] == -127) {
            message[i]=-47;
            message[i+1]=-111;
        }
    }
}

Menu* Menu::get_menu(std::string menuName) {
    // printf("finding menu \"%s\" in \"%s\"\n", menuName.c_str(), title.c_str());
    if (menus.find(menuName) == menus.end()) {
        // printf("not found\n");
        return nullptr;
    }
    return menus[menuName];
}

Button* Menu::get_button(std::string buttonName) {
    // printf("finding button \"%s\" in \"%s\"\n", buttonName.c_str(), title.c_str());
    if (buttons.find(buttonName) == buttons.end()) {
        // printf("not found\n");
        return nullptr;
    }
    // printf("found\n");
    return buttons[buttonName];
}