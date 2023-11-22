#ifndef API_H
#define API_H
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "include/crow_all.h"
#include <curl/curl.h>
#include "Database.h"

using json = nlohmann::json;

struct
{
    std::string name;
    std::string key;
    std::string url;
} typedef auth;

class API
{
public:
    API();
    std::string response_openAI(std::string message);
    std::string googleTrends();
    std::string bingSuggestion(std::string query);
    Item translate(std::string str);
    std::string vision_openAI(std::string imageURL);
    void extractResponseData(const std::string& responseData, std::string& name, std::string& summary, std::vector<std::string>& tags);
    void extractImageData(const std::string& responseData, std::string& name, std::string& summary, std::vector<std::string>& tags);


private:
    CURL *curl;
    CURLcode res;
    std::string response;
    auth OpenAI;
    auth GoogleTrends;
    auth BingSuggestion;
};

#endif