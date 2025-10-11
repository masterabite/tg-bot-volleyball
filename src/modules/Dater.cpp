#include <modules.h>
#include <sstream>
#include <iomanip>
#include <ctime>

const std::string Dater::month[12] = {
    "Января",   "Февраля",  "Марта",
    "Апреля",   "Мая",      "Июня",
    "Июля",     "Августа",  "Сентября",
    "Октября",  "Ноября",   "Декабря"
};

const std::string Dater::days[7] = {
    "Вс",
    "Пн",   "Вт",  "Ср",
    "Чт",   "Пт",  "Сб",
};

std::string Dater::get_date_string(time_t date) {
    std::tm current_time;
    localtime_r(&date, &current_time); // Безопасная версия

    return 
        std::to_string(current_time.tm_mday) + " " + 
        month[current_time.tm_mon] + " " + 
        std::to_string(current_time.tm_year + 1900); 
}

std::string Dater::get_date_time_string(time_t date) {
    time_t currentTime = time(nullptr);
    
    std::tm dateTime;
    std::tm currentDateTime;
    
    // Безопасное получение локального времени
    localtime_r(&date, &dateTime);
    localtime_r(&currentTime, &currentDateTime);

    std::string res = "[";
    res += std::to_string(dateTime.tm_mday) + "." + 
           std::to_string(dateTime.tm_mon + 1);
    
    if (dateTime.tm_year != currentDateTime.tm_year) {
        res += "." + std::to_string(dateTime.tm_year + 1900); 
    }

    // Форматируем время с ведущими нулями
    res += " | " + days[dateTime.tm_wday] + "] ";
    res +=  
           (dateTime.tm_hour < 10 ? "0" : "") + std::to_string(dateTime.tm_hour) + ":" +
           (dateTime.tm_min < 10 ? "0" : "") + std::to_string(dateTime.tm_min);
    res += "";
    return res;
}

std::string Dater::get_date_time_duration_string(time_t time1, time_t time2) {
    std::string str1 = get_date_time_string(time1);
    std::string str2 = get_date_time_string(time2);
    size_t posHour = 0;
    for (int i = 0; i < str1.size(); ++i) {
        if (str1[i]==' ') posHour = i;
    }

    std::string res = str1 + "-" + str2.substr(posHour+1);

    return res;
}


time_t Dater::get_date_time(std::string dateStr) {
    
    int day, month, year, hour, minute;
    if (sscanf(dateStr.c_str(), "%d.%d.%d %d:%d", 
               &day, &month, &year, &hour, &minute) != 5) {
        printf("Dater::get_date_time():\t Parse failed!\n");
        return time(nullptr);
    }
    
    std::tm tm = {};
    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;
    
    time_t result = mktime(&tm);
    
    // Проверяем корректность преобразования
    if (result == -1) {
        printf("mktime failed! Using current time.\n");
        return time(nullptr);
    }
    
    return result;
}