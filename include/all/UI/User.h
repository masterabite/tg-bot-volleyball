#pragma once
#ifndef USER_H
#define USER_H

#include <chrono>
#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

#include <json.hpp>

#include <UI.h>
#include <MASBot.h>

class Menu;
class MASBot;

class User
{
public:
	User(MASBot* _masBot, std::string _username, nlohmann::json _data);

    Menu*               get_menu();
    nlohmann::json&     get_data();
    MASBot*             get_masBot();
    TgBot::Message::Ptr get_last_sended_menu();
    std::string         get_username();
    std::string         get_list_name();
    int64_t             get_chat_id() const;
    int                 get_event_id() const;

    void set_event_id(int id);
    void set_menu(Menu* _menu);
    void set_data(nlohmann::json _data);
    void set_masBot(MASBot* _masBot);
    void set_last_sended_menu(TgBot::Message::Ptr _message);
    void set_chat(TgBot::Chat::Ptr _chat);
    void set_username(std::string _username);
    void set_fullname(std::string _fullname);
    void set_fullname(TgBot::Chat::Ptr _chat);
    void set_listname(TgBot::Chat::Ptr _chat);
    
    std::chrono::time_point<std::chrono::system_clock>  get_lastTime();
    std::string get_fullname();
    void set_lastTime(std::chrono::time_point<std::chrono::system_clock>  tp);

    bool is_admin();

private:
    std::string             username;
    std::string             fullname;
    std::string             listname;
	int64_t                 chatId;
    TgBot::Message::Ptr     lastSendedMessage;
    TgBot::Chat::Ptr        chat;
    Menu*                   menuCurrent;
    nlohmann::json          data;
    MASBot*                 masBot;
    std::chrono::time_point<std::chrono::system_clock> lastProcMessageTime;
};


#endif // !USER_H

