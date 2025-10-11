#pragma once
#ifndef MASBOT_H
#define MASBOT_H

#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

#include <json.hpp>
#include <modules.h>

#include <UI.h>

class User;
class Menu;
class Events;

class MASBot
{
public:
	 MASBot(std::string token);
	~MASBot();

	Menu* 		menu_create(std::string name);
	Menu* 		menu_create(Menu* parent, std::string name);
	User* 				get_user(const std::string& username);
	Menu* 				get_menu(std::string name);
	std::string 		get_random_phrase();
	DayOf* 				get_dayOf();
	Predictor* 			get_predictor();
	Events*				get_events();
	nlohmann::json		get_default_user_data();

	TgBot::Bot*			get_tgBot();
	time_t&				get_start_time();


	void 		save_db();
	void 		load_db();


private:
	TgBot::Bot 	tgBot;

	Calculator*		calculator;
	DayOf* 			dayOf;
	Predictor* 		predictor;

	std::vector<std::string> phrases;
	std::string help;
	
	std::unordered_map<std::string, Menu*> 	menus;
	std::unordered_map<std::string, User*>	users;
	Events* 								events;

	time_t startTime;

	void tgBot_start();
};



#endif // !MASBOT_H

