#ifndef API_H
#define API_H
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "include/crow_all.h"
#include "Item.h"
#include <curl/curl.h>
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
    Item translate(std::string str);
    std::string API::response_openAI(std::string imageURL);

private:
    auth OpenAI;
    std::string response;
};

#endif