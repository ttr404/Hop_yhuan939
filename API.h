#ifndef API_H
#define API_H
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "include/crow_all.h"
#include "Item.h"
#include <curl/curl.h>
using json = nlohmann::json;

class API
{
public:
    API();
    std::string response_openAI(std::string message);
    Item translate(std::string str);
private:
    std::string openAI_link;
    std::string openAI_key;
    std::string response_data;
};

#endif