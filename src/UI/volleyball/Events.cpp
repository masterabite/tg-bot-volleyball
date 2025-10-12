#include <UI/volleyball/Events.h>
#include <tools/JsonProc.h>
#include <modules/Dater.h>

Events::Events(MASBot* _masBot, std::string _path): masBot(_masBot), path(_path) {
    load();
}

void Events::load(std::string _path) {
    if (_path.empty()) _path = path;
    nlohmann::json eventsJson = JsonProc::scan(_path);
    eventID = eventsJson["eventID"].get<int>();
    data = eventsJson["data"];
    defaultEvent = data[0];
}

void Events::save(std::string _path) {
    if (_path.empty()) _path = path;
    nlohmann::json eventsJson;
    eventsJson["eventID"] = eventID;
    eventsJson["data"] = data;
    JsonProc::save(eventsJson, _path);
}

void Events::reg_event(std::string parseString) {
    data.push_back(defaultEvent);
    nlohmann::json& newEvent = data.back();
    newEvent["ID"] = ++eventID;
    
    try {
        nlohmann::json updateEvent = nlohmann::json::parse(parseString);

        //  меняем даты на секунды
        time_t timeStart    = Dater::get_date_time(updateEvent["timeStart"].get<std::string>());
        time_t timeEnd      = Dater::get_date_time(updateEvent["timeEnd"].get<std::string>());
        updateEvent["timeStart"]    = timeStart;
        updateEvent["timeEnd"]      = timeEnd;

        JsonProc::replace(newEvent, updateEvent);
    } 
    catch (json::exception& e) {
        printf("\t json::parse(...): %s\n", e.what());
    }
    save();
}

std::string Events::to_string() {
    std::string ret = "";
    for (int i = 0; i < data.size(); ++i) {
        json& event = data[i];
        if (event["timeStart"].get<int>() < time(0)) continue;
        ret += "\n";
        ret += "_________🏐" + event["type"].get<std::string>() + "🏐_________";
        ret += "\n 🏢 " + event["place"].get<std::string>();
        ret += "\n 🕜 "    + Dater::get_date_time_duration_string(event["timeStart"].get<int>(), event["timeEnd"].get<int>()); 

        //  Записи
        int places = event["places"].get<int>();
        int regs = event["list"].size();

        ret += "\n 📃 Места: "   + std::to_string(regs) + "/" + std::to_string(places);
        std::string emoji;
        float workload = 1.f*regs/places;
        if (workload < 0.3f)    emoji = " [⚪ Почти никого]"; else 
        if (workload < 0.6f)    emoji = " [🟢 Мало людей]"; else      
        if (workload < 0.8f)    emoji = " [⏳ Места заканчиваются]"; else   
        if (workload < 1.1f)    emoji = " [🔥 Людей хватает]"; else
                                emoji = " [💀 Перегруз]";
        ret += emoji;
        ret += "\n_____________________________";
    }
    return ret;
}

int Events::find_user(std::string username) {
    for (int i = 0; i < data.back()["list"].size(); ++i) {
        if (data.back()["list"][i].get<std::string>() == username) {
            return i;
        }
    }
    return -1;
} 

int Events::add_user(std::string username) {
    data.back()["list"].push_back(username);
    save();
    return -1+data.back()["list"].size();
}

void Events::remove_user(std::string username) {
    int pos = find_user(username);
    if (pos == -1) return;
    data.back()["list"].erase(pos);
    save();
}