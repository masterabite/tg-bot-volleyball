#pragma once
#ifndef JSONPROC_H
#define JSONPROC_H

#include <chrono>
#include <vector>
#include <iostream>

#include <json.hpp>
using json = nlohmann::json;
class JsonProc
{
public:
	static  json    scan(std::string path);
    static  void    replace(json& baseJson, const json& updateJson);
    static  void    save(const json& jsn, std::string path);
};


#endif // !JSONPROC_H

