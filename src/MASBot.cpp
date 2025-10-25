#include <string>
#include <locale>
#include <random>
#include <time.h>
#include <fstream>
#include <string>

#include <curl/curl.h>
#include <tgbot/tgbot.h>
#include <tools/JsonProc.h>

#include <MASBot.h>

MASBot* masBot;

MASBot::MASBot(std::string token): tgBot(token) {

    defaultUser = JsonProc::scan("default_user.json");

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
    events          = new Events(this, "events.json");


    //  __Логика меню__
    Menu* menuMain      = menu_create("main");
    Menu* menuMainNick   = menu_create(menuMain, "setNick");
    Menu* menuMainEvent         = menu_create(menuMain, "event");
    Menu* menuMainEventReg      = menu_create(menuMain, "reg");
    Menu* menuMainEventUnreg    = menu_create(menuMain, "unreg");
    Menu* menuMainEventList     = menu_create(menuMain, "list");
    

    //  стартовое меню
    menuMain->set_text("[ бот команды ]\n");
    
        //  Ближайшая тренировка
    menuMain->          create_button_link("Ближайшее событие", menuMainEvent);
    menuMainEvent->send_menu = [](TgBot::Message::Ptr message, User* user) {
        TgBot::InlineKeyboardMarkup::Ptr keyboard = TgBot::InlineKeyboardMarkup::Ptr(new TgBot::InlineKeyboardMarkup);
        for (auto& button: user->get_menu()->get_buttons()) {
            keyboard->inlineKeyboard.push_back({button.second->get_tg_button()});
        }
        TgBot::Message::Ptr lastMenu = user->get_last_sended_menu();
        if (lastMenu != nullptr) {
            try {
                user->get_masBot()->get_tgBot()->getApi().deleteMessage(lastMenu->chat->id, lastMenu->messageId);
            }
            catch (TgBot::TgException& e) {
                printf("error delete message to user %s: %s\n", user->get_list_name().c_str(), e.what());
            }
        }
        MASBot* masBot = user->get_masBot();
        if (!masBot) return;
        Events* events = masBot->get_events();
        std::vector<json>& eventsList = events->get_data();
        std::string persText = "Актуальное событие:\n";

        time_t timeStart = events->get_data().back()["timeStart"].get<time_t>();
        printf("%ld < %ld?\n", time(0), timeStart);
        if (time(0) > timeStart) {
            user->set_last_sended_menu(
                user->get_masBot()->get_tgBot()->getApi().sendMessage(message->chat->id, "событий нет", 0, 0, keyboard)
            );
            return;
        }
        persText += events->to_string() + "\n";

        int pos = events->find_user(user);
        persText += "Ваш статус: ";
        if (pos == -1) {
            std::string status = events->get_status(user);
            if (status == "none") {
                persText += "[👀 зритель]";
            } else
            if (status == "refuse") {
                persText += "[🛑 отказался]";
            } else 
            if (status == "wait") {
                persText += "[🤔 думаю]";
            } 
        } else {
            persText += "[😎 записан #" + std::to_string(pos+1) + "]";
            if (pos+1 > eventsList.back()["places"].get<int>()) {
                persText += "\n⚠Вы находитесь в резерве⚠";
            }
        }
        persText += "\n";

        std::vector<json> userList = eventsList.back()["list"].get<std::vector<json>>();
        int places = events->get_data().back()["places"].get<int>();
        for (int i = 0; i < std::max(userList.size(), size_t(places)); ++i) {
            persText += "\n" + std::to_string(i+1) + ". ";
            if (i < userList.size()) {
                persText += userList[i].get<std::string>();

                if (i == pos) {
                    persText += " <- это вы";
                }
            }
            if (i+1 == places) {
                persText += "\n-----------------------";
            }
        }

        user->set_last_sended_menu(
            user->get_masBot()->get_tgBot()->getApi().sendMessage(message->chat->id, persText, 0, 0, keyboard)
        );
    };


    Button* refuseButton = menuMainEvent->create_button_link("Не смогу 🛑", menuMainEventList);
    refuseButton->command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {
        Events* events = masBot->get_events();
        if (events->set_status(user, "refuse")) {
            user->get_menu()->send_menu(message, user);
        }
    };

    Button* waitButton = menuMainEvent->create_button_link("Возможно смогу 🤔", menuMainEventList);
    waitButton->command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {
        Events* events = masBot->get_events();
        if (events->set_status(user, "wait")) {
            user->get_menu()->send_menu(message, user);
        }
    };

    // Button* unsubButton = menuMainEvent->create_button_link("Отписаться ❌", menuMainEventUnreg);
    // unsubButton->command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {

    //     MASBot* masBot = user->get_masBot();
    //     if (!masBot) return;
    //     Events* events = masBot->get_events();
    //     std::vector<json>& eventsList = events->get_data();
    //     json& event = eventsList.back();
    //     std::string persText = "";
    //     // persText += events->to_string() + "\n";

    //     int pos = events->find_user(user);
    //     if (pos == -1) {
    //         persText += "Вы не записаны на событие!";
    //     } else {
    //         events->remove_user(user);
            
    //         // отправляем юзеру хорошие новости
    //         if (event["places"] <= event["list"].size()) {
    //                 std::string nextUsername = event["list"][event["places"].get<int>()-1].get<std::string>();
    //                 User* nextUser = masBot->get_user(nextUsername);
    //                 std::string nextUserMessage = "Кто-то отписался от события, теперь ты в основе!";
    //                 try {   
    //                     TgBot::Message::Ptr nextUserMessagePtr = user->get_masBot()->get_tgBot()->getApi().sendMessage(nextUser->get_chat_id(), nextUserMessage);
    //                     nextUser->set_menu(masBot->get_menu("event"));
    //                     nextUser->get_menu()->send_menu(nextUserMessagePtr, nextUser);
    //                 }
    //                 catch (TgBot::TgException& e) {
    //                     printf("error send message to user %s: %s\n", user->get_list_name().c_str(), e.what());
    //                 }
    //         }

    //         persText += "Вы успешно отписались от события!";
    //     }

    //     // user->get_masBot()->get_tgBot()->getApi().sendMessage(message->chat->id, persText, 0, 0);
    //     if (pos != -1) {
    //         user->get_menu()->send_menu(message, user);
    //     } 
    // };

    Button* subButton = menuMainEvent->create_button_link("Записаться ✅", menuMainEventReg);
    subButton->command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {

        MASBot* masBot = user->get_masBot();
        if (!masBot) return;
        Events* events = masBot->get_events();
        std::vector<json>& eventsList = events->get_data();
        std::string persText = "";
        // persText += events->to_string() + "\n";

        int pos = events->find_user(user);
        if (pos != -1) {
            persText += "Вы уже записаны на событие!";
        } else {
            events->add_user(user);
            persText += "Вы успешно записались на событие!";
        }

        // user->get_masBot()->get_tgBot()->getApi().sendMessage(message->chat->id, persText, 0, 0);
        if (pos == -1) {
            user->get_menu()->send_menu(message, user);
        }
    };
    
    Button* updateButton = menuMainEvent->create_button_link("Обновить 🔃", menuMainEventList);
    updateButton->command = [](TgBot::Message::Ptr message, User* user, std::string cmd) {
        user->get_menu()->send_menu(message, user);
    };




    //  __Логика тг бота__
    load_db();
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
        user->get_data()["chatId"] = message->chat->id;
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
            save_db();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}



MASBot::~MASBot() {
    save_db();
    printf("delete MASBot: %p\n", this);
    for (auto& [name, menu]: menus) {
        delete menu;
    }
    delete calculator;
    delete dayOf;
    delete predictor;
    delete events;
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
        users[username] = new User(this, username, {});
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

Events* MASBot::get_events() {
    return events;
}

json MASBot::get_default_user_data() {
    return defaultUser;
}

TgBot::Bot* MASBot::get_tgBot() {
    return &tgBot;
}

const std::unordered_map<std::string, User*>&	MASBot::get_users() {
    return users;
}

void MASBot::load_db() {
    nlohmann::json usersJson = JsonProc::scan("users.json");
    for (auto& [username, data]: usersJson.items()) {
        User* user;
        if (!users.count(username)) {
            printf("adding user \"%s\"\n", username.c_str());
            users[username] = new User(this, username, data);
            printf("user \"%s\" added data:\n%s\n", username.c_str(), users[username]->get_data().dump(4).c_str());
        }
    }
    events->load("events.json");
}

void MASBot::save_db() {
    using json = nlohmann::json;
    json usersJson;
    for (auto& [username, user]: users) {
        usersJson[username] = user->get_data();
    }
    JsonProc::save(usersJson, "users.json");
    events->save("events.json");
}

time_t& MASBot::get_start_time() {
    return startTime;
}
