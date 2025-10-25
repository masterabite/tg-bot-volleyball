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

    std::string totalMessage = "Появилось новое событие!";
    json& event = data.back();
    
    if (event["timeStart"] < time(0)) return;

    std::vector<json> eventList = event["list"];
    for (const auto& [username, user]: masBot->get_users()) { 
        try {
            TgBot::Message::Ptr userMessage = masBot->get_tgBot()->getApi().sendMessage(user->get_chat_id(), totalMessage);
            user->get_menu()->send_menu(userMessage, user);
        }
        catch (TgBot::TgException& e) {
            printf("ERROR try send reg event to user \"%s (%s)\": %s", user->get_list_name().c_str(), user->get_fullname().c_str(), e.what());
        }
    }
}

void Events::drop(std::string message) {
    std::string totalMessage = "Произошла отмена события, причина:"+message;
    json& event = data.back();
    std::vector<json> eventList = event["list"];
    for (int i = 0; i < eventList.size(); ++i) { 
        User* user = masBot->get_user(eventList[i].get<std::string>());
        try {
            TgBot::Message::Ptr userMessage = masBot->get_tgBot()->getApi().sendMessage(user->get_chat_id(), totalMessage);
            user->get_menu()->send_menu(userMessage, user);
        }
        catch (TgBot::TgException& e) {
            printf("ERROR try send drop event to user \"%s (%s)\": %s", user->get_list_name().c_str(), user->get_fullname().c_str(), e.what());
        }
    }
    data.pop_back();
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

        ret += "\n 📃 Записалось: "   + std::to_string(regs) + "/" + std::to_string(places) + "\n";
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

int Events::find_user(User* user) {
    std::string listName = user->get_list_name();

    for (int i = 0; i < data.back()["list"].size(); ++i) {
        if (data.back()["list"][i].get<std::string>() == listName) {
            return i;
        }
    }
    return -1;
} 

int Events::add_user(User* user) {
    if (data.size() <= 1) return -1;
    std::string listName = user->get_list_name();
    data.back()["list"].push_back(listName);
    data.back()["stats"]["wait"].erase(listName);
    data.back()["stats"]["refuse"].erase(listName);
    save();
    return -1+data.back()["list"].size();
}

void Events::remove_user(User* user) {
    int pos = find_user(user);
    if (pos == -1) return;
    data.back()["list"].erase(pos);
    save();
}

bool Events::set_status(User* user, std::string status) {
    if (data.size() <= 1) return false;

    std::string currentStatus = get_status(user);
    json& event = data.back();

    if (status == currentStatus) return false;

    std::string listName = user->get_list_name();
    if (currentStatus == "accept")  remove_user(user);
    if (currentStatus == "wait")    event["stats"]["wait"].erase(listName);
    if (currentStatus == "refuse")  event["stats"]["refuse"].erase(listName);
    event["stats"][status][listName];
    return true;
}

std::string Events::get_status(User* user) {
    
    if (find_user(user) >= 0) return "accept";
    
    std::string status = "none";
    json& event = data.back();
    if (!event.contains("stats")) {
        printf("эвент не содержит \"stats\"!\n");
        return status;
    }
    std::string listName = user->get_list_name();
    json& stats = event["stats"];
    for (const auto& [st, names]: event["stats"].items()) {
        if (names.contains(listName)) return st;
    }
    return status;
}