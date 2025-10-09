#pragma once
#ifndef MENU_H
#define MENU_H

#include <chrono>
#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

#include <json.hpp>

#include <MASBot.h>
#include <UI.h>
#include <structures/CharTree/CharTree.h>

class MASBot;
class Button; 
class User;

class Menu
{
public:
    Menu();
    Menu(std::string _name);
    Menu(Menu* _parent, std::string _name);

    void set_default_functions();

    void create_button(std::string buttonLabel);
    void create_button(std::string buttonLabel, void (*proc)(TgBot::Message::Ptr message, User* user, std::string cmd));
    void create_button_link(std::string buttonName, Menu* menuNext);


    Menu*       get_menu    (std::string menuName);
    Button*     get_button  (std::string buttonName);

    std::unordered_map<std::string, Button*>& get_buttons() {return buttons;}

    std::string get_title() {return title;}

    std::string get_name() {return name;}

    void set_text(std::string _text) {text=_text;}
    std::string get_text() {return text;}
    
    std::string parse_command(const std::string& text);

    void to_lower(std::string& s);

    //  динамические функции
    void (*send_menu)   (TgBot::Message::Ptr message, User* user) = nullptr;
    void (*proc_message)(TgBot::Message::Ptr message, User* user) = nullptr;
    void (*proc_command)(TgBot::Message::Ptr message, User* user, std::string cmd) = nullptr;

private:
    std::string                             name;
    std::string                             title;
    std::string                             text;

    std::unordered_map<
        std::string, 
        Button*
    >                                       buttons;

    std::unordered_map<std::string, Menu*>  menus;
    CharTree                                commandParser;            
};


#endif // !MENU_H

