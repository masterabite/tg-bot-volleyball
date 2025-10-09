#pragma once
#ifndef URLPARSER_H
#define URLPARSER_H

#include <iostream>

class URLParser
{
public:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
	static std::string get_url_content(const std::string& url);
	static void download_save_file(const std::string& fileUrl, const std::string& outputFilePath);
private:	
};

#endif // !URLPARSER_H

