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
        ret += "_____🏐" + event["type"].get<std::string>() + "🏐_____";
        ret += "\n 🏢 " + event["place"].get<std::string>();
        ret += "\n 🕜 "    + Dater::get_date_time_duration_string(event["timeStart"].get<int>(), event["timeEnd"].get<int>()); 
        ret += "\n 📃 Записей: "   + std::to_string(event["list"].size()) + "/" + std::to_string(event["places"].get<int>());
        ret += std::to_string(event["list"].size()) < std::to_string(event["places"].get<int>())? "✅": "❗";
        ret += "\n____________________";
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