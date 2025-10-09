
#include <iostream>
#include <string>
#include <locale>
#include <random>
#include <curl/curl.h>
#include <tgbot/tgbot.h>
#include <modules.h>

DayOf::DayOf() {
    URLParser* urlParser    = new URLParser();
    lastDay                 = 0;
    days                    = "";
}

std::string& DayOf::getDays() {
    // if (update()) {
    //     printf("updated, now days: \"%s\"\n", days.c_str());
    // } else {
    //     printf("not updated, now days: \"%s\"\n", days.c_str());
    // }
    update();
    return days;
}

bool DayOf::update() {

    long day = std::time(0)/60/60/24;

    if (lastDay != 0 && day == lastDay) {
        return false;
    }
    
    std::string ans = URLParser::get_url_content("https://kakoysegodnyaprazdnik.ru/");
    if (ans.find("<title>Проверка безопасности подключения к сайту</title>") != std::string::npos) {
        days = "Запрос с сайта отклонен.\nЯ не прошел проверку на человека :(";
        return false;   
    }
    
    lastDay = day;
    days = Dater::get_date_string(lastDay*24*60*60) + ":\n";

    size_t pos = ans.find("<span itemprop=\"text\">"), nextPos1;
    for (int i = 0; i < 10 && pos != std::string::npos; ++i) {
        pos += 22;
        nextPos1 = ans.find("</span>", pos);
        if (nextPos1 != std::string::npos) {
            
            days += "-    " + ans.substr(pos, nextPos1-pos) + ";\n";
            pos = ans.find("<span itemprop=\"text\">", nextPos1+1);
        }
    }
    return true;
}