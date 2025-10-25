#include <UI.h>
#include<tools/JsonProc.h>

User::User(MASBot* _masBot, std::string _username, nlohmann::json _data) {
    menuCurrent     = nullptr;
    masBot          = _masBot;
    data            = masBot->get_default_user_data();
    username        = _username;
    fullname        = (_data.contains("fullname")? _data["fullname"].get<std::string>(): "");
    lastProcMessageTime     = std::chrono::system_clock::from_time_t(1);

    JsonProc::replace(data, _data);

    menuCurrent = _masBot->get_menu(data["menu"].get<std::string>());
}

// User::User(TgBot::Chat::Ptr _chat, Menu* _menu, MASBot* _masBot) {
//     chat            = _chat;
//     menuCurrent     = _menu;
//     masBot          = _masBot;
//     data            = masBot->get_default_user_data();
//     lastProcMessageTime     = std::chrono::system_clock::now();
// }

Menu* User::get_menu() {
    return menuCurrent;
}

int64_t User::get_chat_id() const {
    return data["chatId"].get<int64_t>();
}

nlohmann::json& User::get_data() {
    return data;    
}

std::string User::get_fullname() {
    return fullname;
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

std::string User::get_list_name() {
    return username.empty()? fullname: username;
}

void User::set_menu(Menu* _menu) {
    menuCurrent = _menu;
    data["menu"] = menuCurrent->get_name();
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

void User::set_chat(TgBot::Chat::Ptr _chat) {
    chat = _chat;
    set_fullname(_chat);
    set_listname(_chat);
}

void User::set_fullname(TgBot::Chat::Ptr _chat) {
    fullname = _chat->firstName + " " + chat->lastName;
    data["fullname"] = fullname;
}

void User::set_listname(TgBot::Chat::Ptr _chat) {
    listname = _chat->firstName;
    if (!username.empty()) listname += " @"+username;
    data["listname"] = listname;
}

void User::set_username(std::string _username) {
    username = _username;
}

void User::set_fullname(std::string _fullname) {
    fullname = _fullname;
    data["fullname"] = fullname;
}

std::chrono::time_point<std::chrono::system_clock>  User::get_lastTime() {
    return lastProcMessageTime;
}

void User::set_lastTime(std::chrono::time_point<std::chrono::system_clock> tp) {
    lastProcMessageTime = tp;
}