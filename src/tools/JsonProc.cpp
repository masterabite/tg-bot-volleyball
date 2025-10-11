#include <tools/JsonProc.h>
#include <fstream>

json JsonProc::scan(const std::string path) {

    json jsonRet;

    std::ifstream file(path);
    if (file.is_open()) {
        try {
            jsonRet = json::parse(file);   
        }
        catch (json::exception& e) {
            printf("\t JsonProc::scan_json(...): %s\n", e.what());
        }
    }
    file.close();
    // printf("Scanned json:\n%s\n", jsonRet.dump(4).c_str());
    return jsonRet;
}

void JsonProc::replace(json& baseJson, const json& updateJson) {
    for (auto& [key, value]: updateJson.items()) {
        if (baseJson.count(key)) {
            printf("replacing key \"%s\"\n", key.c_str());
            baseJson[key] = value;
        }
    }
}


void JsonProc::save(const json& jsn, std::string path) {
    std::ofstream jsonFile(path);
    jsonFile << jsn.dump(4);
    jsonFile.close();
}