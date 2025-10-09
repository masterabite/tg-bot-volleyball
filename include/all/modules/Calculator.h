#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <iostream>
#include <unordered_map>

class Calculator
{
public:
	Calculator();
	int prior(char c);
	bool isFunction(const std::string& str);
	double parseToDouble(const std::string& str);
	std::string cleanExp(std::string str);
	double calc(std::string str);
private:
	std::string signs;
	bool allowedChars[256];
	std::unordered_map<std::string,	double(*)(double)			> functions;
	std::unordered_map<char,		double(*)(double, double)	> operationFunctions;
};


#endif // !CALCULATOR_H

