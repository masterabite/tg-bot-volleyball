#pragma once
#ifndef EVENTS_H
#define EVENTS_H

#include <chrono>
#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

#include <json.hpp>

#include <UI.h>
#include <MASBot.h>

class MasBot;

class Events
{
public:
	Events(MASBot* _masBot, std::string _path);

    void load(std::string _path = "");
    void save(std::string _path = "");

    std::vector<nlohmann::json>& get_data() {return data;}
    void reg_event(std::string parse);
    std::string to_string();

    int find_user(std::string username);
    int add_user(std::string username);
    void remove_user(std::string username);

private:
    int                             eventID;
    nlohmann::json                  defaultEvent;
    std::vector<nlohmann::json>     data;
    MASBot*                         masBot;
    std::string                     path;
};


#endif // !EVENTS_H

