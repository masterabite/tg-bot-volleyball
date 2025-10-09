
#include <iostream>
#include <curl/curl.h>
#include <cpr/cpr.h>
#include <modules.h>

size_t URLParser::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string URLParser::get_url_content(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Устанавливаем заголовки (например, для API Telegram)
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return readBuffer;
}

void URLParser::download_save_file(const std::string& fileUrl, const std::string& outputFilePath) {
     // Выполняем HTTP GET-запрос
    cpr::Response response = cpr::Get(cpr::Url{fileUrl});

    // Проверяем успешность запроса
    if (response.status_code == 200) {
        // Открываем файл для записи в бинарном режиме
        std::ofstream file(outputFilePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << outputFilePath << std::endl;
            return;
        }

        // Записываем данные в файл
        file.write(response.text.c_str(), response.text.size());
        file.close();

        std::cout << "File successfully saved to: " << outputFilePath << std::endl;
    } else {
        std::cerr << "Failed to download file. HTTP status code: " << response.status_code << std::endl;
    }
}