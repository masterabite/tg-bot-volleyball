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

    int eventId = event["ID"].get<int>();
    for (const auto& [username, user]: masBot->get_users()) { 
        try {
            user->set_event_id(eventId);
            TgBot::Message::Ptr userMessage = masBot->get_tgBot()->getApi().sendMessage(user->get_chat_id(), totalMessage);
            user->get_menu()->send_menu(userMessage, user);
        }
        catch (TgBot::TgException& e) {
            printf("ERROR try send reg event to user \"%s (%s)\": %s", user->get_list_name().c_str(), user->get_fullname().c_str(), e.what());
        }
    }
}

void Events::drop(int eventId, std::string message) {
    std::string totalMessage = "Произошла отмена события:\n" + to_string_short(eventId) + "причина:"+message;
    json& event = get_event(eventId);
    std::vector<json> eventList = event["list"];
    for (int i = 0; i < eventList.size(); ++i) { 
        User* user = masBot->get_user_by_list_name(eventList[i].get<std::string>());
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

std::string Events::to_string(int eventId, User* user) {
    std::string ret = "";
    json& event = get_event(eventId);
    if (event["timeStart"].get<int>() < time(0)) return ret;

    ret += to_string_short(eventId);

    //  Записи
    int places = event["places"].get<int>();
    int regs = event["list"].size();
    int pos = find_user(eventId, user);
    ret += "Ваш статус: ";
    if (pos == -1) {
        std::string status = get_status(eventId, user);
        if (status == "none") {
            ret += "[👀 зритель]";
        } else
        if (status == "refuse") {
            ret += "[🛑 не смогу]";
        } else 
        if (status == "wait") {
            ret += "[🤔 думаю]";
        } 
    } else {
        ret += "[😎 записан #" + std::to_string(pos+1) + "]";
        if (pos+1 > event["places"].get<int>()) {
            ret += "\n⚠Вы находитесь в резерве⚠";
        }
    }
    ret += "\n";
         
    std::vector<json> userList = event["list"].get<std::vector<json>>();
    for (int i = 0; i < std::max(userList.size(), size_t(places)); ++i) {
        ret += "\n" + std::to_string(i+1) + ". ";
        if (i < userList.size()) {
            ret += userList[i].get<std::string>();

            if (i == pos) {
                ret += " <- это вы";
            }
        }
        if (i+1 == places) {
            ret += "\n-----------------------";
        }
    } ret += "\n";

    if (user->is_admin()) {
        json& stats = event["stats"];
        for (const auto& [st, names]: stats.items()) {
            ret += st + ":\n";
            for (const auto& [name, data]: names.items()) {
                ret += "- " + name + "\n";
            }
        }
    }
    return ret;
}

std::string Events::to_string_short(int id) {
    std::string ret = "";
    
    json& event = get_event(id);
    if (event["timeStart"].get<int>() < time(0)) return ret;
    ret += "\n#️⃣: " + std::to_string(event["ID"].get<int>()) + "\n";
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
    ret += "\n_____________________________\n";
    
    return ret;
}

std::string Events::to_string_short_all() {
    std::string ret = "";
    for (json& event: data) {
        ret += to_string_short(event["ID"]);
    }
    return ret;
}

int Events::find_user(int eventId, User* user) {
    std::string listName = user->get_list_name();
    json& event = get_event(eventId);
    for (int i = 0; i < event["list"].size(); ++i) {
        if (event["list"][i].get<std::string>() == listName) {
            return i;
        }
    }
    return -1;
} 

int Events::add_user(int eventId, User* user) {
    if (data.size() <= 1) return -1;
    std::string listName = user->get_list_name();
    json& event = get_event(eventId);
    event["list"].push_back(listName);
    event["stats"]["wait"].erase(listName);
    event["stats"]["refuse"].erase(listName);
    save();
    return -1+event["list"].size();
}

void Events::remove_user(int eventId, User* user) {
    int pos = find_user(eventId, user);
    if (pos == -1) return;
    json& event = get_event(eventId);
    event["list"].erase(pos);

    // отправляем юзеру хорошие новости
    if (event["places"] <= event["list"].size()) {
            std::string nextListName = event["list"][event["places"].get<int>()-1].get<std::string>();
            User* nextUser = masBot->get_user_by_list_name(nextListName);
            std::string nextUserMessage = "Кто-то отписался от события\n" + to_string_short(eventId) + "\nТеперь ты в основе!";
            try {   
                TgBot::Message::Ptr nextUserMessagePtr = user->get_masBot()->get_tgBot()->getApi().sendMessage(nextUser->get_chat_id(), nextUserMessage);
                nextUser->set_menu(masBot->get_menu("event"));
                nextUser->get_menu()->send_menu(nextUserMessagePtr, nextUser);
            }
            catch (TgBot::TgException& e) {
                printf("error send message to user %s: %s\n", nextUser->get_list_name().c_str(), e.what());
            }
    }

    save();
}

bool Events::set_status(int eventId, User* user, std::string status) {
    if (data.size() <= 1) return false;

    std::string currentStatus = get_status(eventId, user);
    json& event = get_event(eventId);

    if (status == currentStatus) return false;

    std::string listName = user->get_list_name();
    if (currentStatus == "accept")  {
        remove_user(eventId, user);
    }
    if (currentStatus == "wait")    event["stats"]["wait"].erase(listName);
    if (currentStatus == "refuse")  event["stats"]["refuse"].erase(listName);
    event["stats"][status][listName];
    return true;
}

std::string Events::get_status(int eventId, User* user) {
    
    if (find_user(eventId, user) >= 0) return "accept";
    
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

json& Events::get_event(User* user) {
    return get_event(user->get_event_id());
}

json& Events::get_event(int eventId) {
    for (int i = 0; i < data.size(); ++i) {
        if (data[i]["ID"] == eventId) return data[i];
    }
    return data[0];
}

int Events::actual_size() {
    int actualSize = 0;
    for (int i = 0; i < data.size(); ++i) {
        if (data[i]["timeStart"].get<int>() < time(0)) continue;
        ++actualSize;
    }
    return actualSize;
}