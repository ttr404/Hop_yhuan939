#ifndef ROUTER_H
#define ROUTER_H
#include <ctime> // add this line to include the ctime library
#include <regex>
#include <curl/curl.h>
#include "include/crow_all.h"
#include "API.h"
#include "Voice.h"
#include "Database.h"
#include "VoiceAPI.h"
#include <time.h>

class Router {
    public:
        Router();
        ~Router();
        int enroute(crow::SimpleApp& app);
        crow::json::wvalue handleQuery(std::string query);
        std::string urlDecode(const std::string &encoded);
    private:
        crow::SimpleApp& app;
        API api;
        Voice voice;
        Database db;
        std::string currRoute;
        std::string prevRoute;
        std::string id;
        crow::mustache::context ctx;
};
#endif