#pragma once
#ifndef BUTTON_H
#define BUTTON_H

#include <chrono>
#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

#include <json.hpp>

#include "Menu.h"

class Menu;
class User;

class Button
{
public:
    Button(std::string _label, Menu* _menuNext);
    ~Button();
    TgBot::InlineKeyboardButton::Ptr get_tg_button();
    std::string get_name();
    Menu* get_menu_next();
    
    void (*command) (
        TgBot::Message::Ptr message, 
        User* user,
        std::string cmd
    ) = nullptr;

private:
    std::string                         name;
    std::string                         label;
    Menu*                               menuNext;
    TgBot::InlineKeyboardButton::Ptr    tgButton;
};


#endif // !BUTTON_H

