#pragma once
#ifndef DATER_H
#define DATER_H

#include <iostream>
#include <time.h>
#include <string>

class Dater
{
public:
	static std::string 	get_date_string(time_t date);
	static std::string 	get_date_time_string(time_t date);
	static std::string  get_date_time_duration_string(time_t time1, time_t time2);

	static time_t 		get_date_time(std::string date);
private:
	static const std::string month[12];
	static const std::string days[7];
};


#endif // !DATER_H

