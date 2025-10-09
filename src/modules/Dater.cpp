#include <modules.h>

const std::string Dater::month[12] = {
    "Января",   "Февраля",  "Марта",
    "Апреля",   "Мая",      "Июня",
    "Июля",     "Августа",  "Сентября",
    "Октября",  "Ноября",   "Декабря"
};

std::string Dater::get_date_string(time_t date) {
    tm* current_time = localtime(&date);

    return 
        std::to_string(current_time->tm_mday) + " " + 
        month[current_time->tm_mon] + " " + 
        std::to_string(current_time->tm_year+1900); 
}
