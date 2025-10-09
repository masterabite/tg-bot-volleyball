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
	User(Menu* _menu, MASBot* _masBot);
	User(TgBot::Chat::Ptr _chat, Menu* _menu, MASBot* _masBot);

    Menu*               get_menu();
    TgBot::Chat::Ptr    get_chat();
    nlohmann::json&     get_data();
    MASBot*             get_masBot();
    TgBot::Message::Ptr get_last_sended_menu();

    void set_menu(Menu* _menu);
    void set_chat(TgBot::Chat::Ptr _chat);
    void set_data(nlohmann::json _data);
    void set_masBot(MASBot* _masBot);
    void set_last_sended_menu(TgBot::Message::Ptr _message);
    
    std::chrono::time_point<std::chrono::system_clock>  get_lastTime();
    void set_lastTime(std::chrono::time_point<std::chrono::system_clock>  tp);

private:
	TgBot::Chat::Ptr        chat;
    TgBot::Message::Ptr     lastSendedMessage;
    Menu*                   menuCurrent;
    nlohmann::json          data;
    MASBot*                 masBot;
    std::chrono::time_point<std::chrono::system_clock> lastProcMessageTime;
};


#endif // !USER_H

