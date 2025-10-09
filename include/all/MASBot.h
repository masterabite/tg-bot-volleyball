#pragma once
#ifndef MASBOT_H
#define MASBOT_H

#include <vector>
#include <iostream>

#include <tgbot/tgbot.h>

#include <modules.h>

#include <UI.h>

class User;
class Menu;

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

	TgBot::Bot*			get_tgBot();
	time_t&				get_start_time();


	void 		save_users();
	void 		load_users();


private:
	TgBot::Bot 	tgBot;

	Calculator*		calculator;
	DayOf* 			dayOf;
	Predictor* 		predictor;

	std::vector<std::string> phrases;
	std::string help;
	
	std::unordered_map<std::string, Menu*> 	menus;
	std::unordered_map<std::string, User*>	users;

	time_t startTime;

	void tgBot_start();
};



#endif // !MASBOT_H

