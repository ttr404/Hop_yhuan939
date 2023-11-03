#ifndef ROUTER_H
#define ROUTER_H
#include <curl/curl.h>
#include "include/crow_all.h"

class Router {
    public:
        Router();
        ~Router();
        int enroute(crow::SimpleApp& app);
        crow::json::wvalue handleQuery(std::string query);
        void NavigateTo(std::string route);
    private:
        crow::SimpleApp& app;
        std::string currRoute;
        std::string prevRoute;
        crow::mustache::context ctx;
};
#endif