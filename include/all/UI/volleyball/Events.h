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
    void drop(int eventId, std::string message);
    std::string to_string(int eventId, User* user);
    std::string to_string_short(int id);
    std::string to_string_short_all();

    int find_user(int eventId, User* user);
    int add_user(int eventId, User* user);
    void remove_user(int eventId, User* user);
    nlohmann::json& get_event(User* user);
    nlohmann::json& get_event(int eventId);
    bool set_status(int eventId, User* user, std::string status);
    std::string get_status(int eventId, User* user);
    int actual_size();

private:
    int                                     eventID;
    nlohmann::json                          defaultEvent;
    std::vector<nlohmann::json>             data;
    MASBot*                                 masBot;
    std::string                             path;
};


#endif // !EVENTS_H

