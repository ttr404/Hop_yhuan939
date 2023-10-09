#ifndef ROUTER_H
#define ROUTER_H
#include "include/crow_all.h"

// Path: router.h
// Define your endpoint at the root directory

class Router {
    public:
        Router(crow::SimpleApp& app);
        crow::json::wvalue handleQuery(std::string query);
        void NavigateTo(std::string route);
    private:
        crow::SimpleApp& app;
        std::string currRoute;
        std::string prevRoute;
        std::string content;
        std::stringstream buffer;
        crow::mustache::context ctx;
};
#endif