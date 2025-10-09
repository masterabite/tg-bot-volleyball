#pragma once
#ifndef DAYOF_H
#define DAYOF_H

#include <iostream>

#include "modules.h"

class DayOf
{
public:
	DayOf();
	std::string& getDays();
	bool update();
private:	
	time_t lastDay;
	std::string days;
};

#endif // !DAYOF_H

