#include <string>
#include <locale>
#include <random>
#include <time.h>
#include <fstream>
#include <string>

#include <curl/curl.h>
#include <tgbot/tgbot.h>

#include <MASBot.h>

MASBot* masBot;

MASBot::MASBot(std::string token): tgBot(token) {
    srand(time(NULL));
    startTime = time(0) + 1;

    //  __Инициализация__

    if (masBot != nullptr) {
        printf("MASBot already created, freeing...\n");
        delete masBot;
    }
    masBot = this;

    calculator      = new Calculator();
    dayOf           = new DayOf();
    predictor       = new Predictor();


    //  __Логика меню__
    Menu* menuMain      = menu_create("main");
    Menu* menuMainNick   = menu_create(menuMain, "setNick");
    Menu* menuMainEvent         = menu_create(menuMain, "event");
    Menu* menuMainEventReg      = menu_create(menuMain, "reg");
    Menu* menuMainEventList     = menu_create(menuMain, "list");
    

    //  стартовое меню
    menuMain->set_text("Меню\n");

    //  меню установки ника
    menuMainNick->set_text("Как отображать тебя в списках на тренировках?");
    menuMainNick-> proc_message = [](TgBot::Message::Ptr message, User* user) {
        user->get_data()["nickname"] = message->text;
        user->set_menu(user->get_masBot()->get_menu("setNick"));
        std::string rem = "Отлично, теперь будет \"" + message->text + "\"";
        masBot->tgBot.getApi().sendMessage(message->chat->id, rem);
        user->set_menu(user->get_masBot()->get_menu("main"));
        user->get_menu()->send_menu(message, user);
    };
    menuMain->  create_button_link("поменять имя", menuMainNick);



    //     //  предсказание
    // menuMain->          create_button_link("предсказание", menuMainPred);
    // menuMainPred->      set_text("Выбери опцию предсказания\n");
    // menuMainPred->      create_button_link("когда", menuMainPredDate);
    // menuMainPredDate->  set_text("Напиши событие и узнай когда это произойдет\n");
    // menuMainPredDate->proc_message = [](TgBot::Message::Ptr message, User* user) {
    //     std::string ans = user->get_masBot()->get_predictor()->predict_date();
    //     user->get_masBot()->tgBot.getApi().sendMessage(message->chat->id, ans);
    // };
    // menuMainPred->      create_button_link("да/нет", menuMainPredDanet);
    // menuMainPredDanet-> set_text("Напиши утверждение и узнай его достоверность\n");
    // menuMainPredDanet->proc_message = [](TgBot::Message::Ptr message, User* user) {
    //     std::string ans = user->get_masBot()->get_predictor()->da_net();
    //     user->get_masBot()->tgBot.getApi().sendMessage(message->chat->id, ans);
    // };





    //  __Логика тг бота__
    load_users();
    tgBot.getEvents().onAnyMessage([](TgBot::Message::Ptr message) {
        
        if (masBot == nullptr) {
            printf("masBot is NULL\n");
            return;
        } 

        if (message->date < masBot->startTime) {
            // masBot->get_tgbot()->getApi().deleteMessage(message->chat->id, message->messageId);  под вопросом
            return;
        }
        
        User* user = masBot->get_user(message->chat->username);
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - user->get_lastTime();

        if (diff.count() < 0.4) {
            return;
        } 
        
        user->get_menu()->proc_message  (message, user);
        user->set_lastTime(std::chrono::system_clock::now());
    });

    tgBot.getEvents().onCallbackQuery([](TgBot::CallbackQuery::Ptr query) {
        
        if (masBot == nullptr) {
            printf("masBot is NULL\n");
            return;
        } 

        if (query->message->date < masBot->startTime) {
            // masBot->get_tgbot()->getApi().deleteMessage(message->chat->id, message->messageId);  под вопросом
            return;
        }
        
                User* user = masBot->get_user(query->message->chat->username);
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - user->get_lastTime();

        if (diff.count() < 0.4) {
            return;
        } 

        query->message->text = query->data;
        user->get_menu()->proc_command  (query->message, user, query->data);
        user->set_lastTime(std::chrono::system_clock::now());

    });
    
    std::thread th(&MASBot::tgBot_start, this); 
    th.join();
    //  __________________


}

void MASBot::tgBot_start() {
    try {
        printf("Bot username: %s\n", tgBot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(tgBot);
        while (true) {
            longPoll.start();
            printf("saving users\n");
            save_users();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}



MASBot::~MASBot() {
    save_users();
    printf("delete MASBot: %p\n", this);
    for (auto& [name, menu]: menus) {
        delete menu;
    }
    delete calculator;
    delete dayOf;
    delete predictor;
}

Menu* MASBot::menu_create(std::string name) {
    Menu* newMenu = new Menu(name);
    menus[name] = newMenu;
    return newMenu;
}

Menu* MASBot::menu_create(Menu* parent, std::string name) {
    Menu* newMenu = new Menu(parent, name);
    menus[name] = newMenu;
    return newMenu;

}


User* MASBot::get_user(const std::string& username) {
    printf("get user \"%s\"\n", username.c_str());
    if (users.find(username) == users.end()) {
        printf("new user \"%s\"\n", username.c_str());
        users[username] = new User(menus["main"], this);
    }
    printf("%p\n", users[username]);
    return users[username];
}

Menu* MASBot::get_menu(std::string name) {
    if (menus.find(name) == menus.end()) {
        return nullptr;
    }
    return menus[name];
}

std::string MASBot::get_random_phrase() {
    return phrases[rand()%phrases.size()];
}

DayOf* MASBot::get_dayOf() {
    return dayOf;
}

Predictor* MASBot::get_predictor() {
    return predictor;
}

TgBot::Bot* MASBot::get_tgBot() {
    return &tgBot;
}

void MASBot::load_users() {
    using json = nlohmann::json;
    std::ifstream usersFile("users.json");
    if (usersFile.is_open()) {
        try {
            json usersJson = json::parse(usersFile);   
            usersFile.close();
            for (auto& [username, data]: usersJson.items()) {
                if (users.count(username)) {
                    printf("updating user \"%s\"\n", username.c_str());
                    User* user = users[username];
                    user->set_data(data);
                } else {
                    printf("adding user \"%s\"\n", username.c_str());
                    users[username] = new User(menus["empty"], this);
                    users[username]->set_data(data);
                    printf("user \"%s\" added data:\n%s\n", username.c_str(), users[username]->get_data().dump(4).c_str());
                }
            }
        }
        catch (nlohmann::json::exception& e) {
            printf("error: %s\n", e.what());
        }
    }
    usersFile.close();
}

void MASBot::save_users() {
    using json = nlohmann::json;
    json usersJson;
    for (auto& [username, user]: users) {
        usersJson[username] = user->get_data();
    }
    std::ofstream usersFile("users.json");
    usersFile << usersJson.dump(4);
    usersFile.close();
}

time_t& MASBot::get_start_time() {
    return startTime;
}
