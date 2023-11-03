#include "Router.h"

size_t write_callback(char *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

Router::Router() : app(app)
{
    enroute(app);
}

Router::~Router()
{
    std::cout << "Router destroyed" << std::endl;
}

int Router::enroute(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/")
    ([&](const crow::request& req) {
        auto page = crow::mustache::load_text("index.html");
        return page;
    });
    return 0;
}
