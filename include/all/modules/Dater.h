#pragma once
#ifndef DATER_H
#define DATER_H

#include <iostream>
#include <time.h>
#include <string>

class Dater
{
public:
	static std::string get_date_string(time_t date);
private:
	static const std::string month[12];
};


#endif // !DATER_H

