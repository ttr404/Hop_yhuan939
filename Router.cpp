#include "Router.h"

Router::Router() : app(app)
{
}

Router::~Router()
{
    std::cout << "Router destroyed" << std::endl;
}

/**
 * @brief enroute routes the app to the specified path
 *
 * @param app
 * @return int
 */
int Router::enroute(crow::SimpleApp &app)
{
    try
    {
        CROW_ROUTE(app, "/")
        ([&](const crow::request &req)
         {
             auto page = crow::mustache::load("index.html");
             std::time_t now = std::time(nullptr);
             ctx["title"] = "Home - HOP";
             ctx["description"] = "HOP.cheap: Shop without Search, hop in shopping experience.";
             ctx["year"] = std::localtime(&now)->tm_year + 1900;
             return page.render(ctx); });

        CROW_ROUTE(app, "/favicon.ico")
        ([]
         {
            auto favicon = crow::response(200);
            favicon.set_static_file_info("static/favicon.ico");
            return favicon; });

        CROW_ROUTE(app, "/admin")
        ([&](const crow::request &req)
         {
            auto page = crow::mustache::load("dash.html");
            return page.render(ctx); });

        CROW_ROUTE(app, "/search")
        ([&](const crow::request &req)
         {
            // get query
            std::string query = req.url_params.get("q") ? req.url_params.get("q") : "",
                        type = req.url_params.get("type") ? req.url_params.get("type") : "";
            if(type == "json")
            {
                crow::json::wvalue json;
                json["query"] = query;
                json["type"] = type;
                return crow::response(json);
            }
            auto page = crow::mustache::load("search.html");
            ctx["title"] = query + " - HOP";
            ctx["description"] = "Search results for " + query;
            ctx["query"] = query;
            auto response = page.render(ctx);
            return crow::response(response); });

        CROW_ROUTE(app, "/openai/<string>")
        ([&](std::string query)
         { return crow::response(api.response_openAI(query)); });

        CROW_ROUTE(app, "/suggestion")
        ([&]()
         { return crow::response(api.googleTrends()); });

        CROW_ROUTE(app, "/suggestion/<string>")
        ([&](std::string query)
         { return crow::response(api.bingSuggestion(query)); });

        CROW_ROUTE(app, "/db")
        ([&]()
         { return db.getAll(); });
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

std::string Router::handleQuery(std::string query)
{
}