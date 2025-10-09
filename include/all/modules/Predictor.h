#pragma once
#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <chrono>
#include <vector>
#include <string>
#include <iostream>

#include "modules.h"

class Predictor
{
public:
	Predictor();
	std::string predict_date();
	std::string da_net();
private:
	std::vector<std::string> danet;
};


#endif // !PREDICTOR_H

