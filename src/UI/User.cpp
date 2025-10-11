#include <UI.h>

User::User(Menu* _menu, MASBot* _masBot) {
    chat            = nullptr;
    menuCurrent     = _menu;
    masBot          = _masBot;
    data            = masBot->get_default_user_data();
    lastProcMessageTime     = std::chrono::system_clock::now();
}

User::User(TgBot::Chat::Ptr _chat, Menu* _menu, MASBot* _masBot) {
    chat            = _chat;
    menuCurrent     = _menu;
    masBot          = _masBot;
    data            = masBot->get_default_user_data();
    lastProcMessageTime     = std::chrono::system_clock::now();
}

Menu* User::get_menu() {
    return menuCurrent;
}

TgBot::Chat::Ptr User::get_chat() {
    return chat;
}

nlohmann::json& User::get_data() {
    return data;    
}

bool User::is_admin() {
    return data["type"].get<std::string>() == "admin";
}

MASBot* User::get_masBot() {    
    return masBot;
}

std::string User::get_username() {
    return username;
}

void User::set_menu(Menu* _menu) {
    menuCurrent = _menu;
}

void User::set_chat(TgBot::Chat::Ptr _chat) {
    chat = _chat;
}

void User::set_data(nlohmann::json _data) {
    if (!_data.is_null()) {
        data = _data;    
    }
}

void User::set_masBot(MASBot* _masBot) {
    masBot = _masBot;    
}

TgBot::Message::Ptr User::get_last_sended_menu() {
    return lastSendedMessage;
}

void User::set_last_sended_menu(TgBot::Message::Ptr _message) {
    lastSendedMessage = _message;
}

void User::set_username(std::string _username) {
    username = _username;
}

std::chrono::time_point<std::chrono::system_clock>  User::get_lastTime() {
    return lastProcMessageTime;
}

void User::set_lastTime(std::chrono::time_point<std::chrono::system_clock> tp) {
    lastProcMessageTime = tp;
}