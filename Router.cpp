#include "Router.h"

/**
 * @brief write_callback writes data and re
 *
 * @param ptr
 * @param size
 * @param nmemb
 * @param data
 * @return size_t
 */
size_t write_callback(char *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

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

        CROW_ROUTE(app, "/suggestion")
        ([]()
         {
        std::string url = "https://trends.google.com/trends/trendingsearches/daily/rss?geo=CA";
        CURL *curl;
        CURLcode res;
        std::string response;
        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        return crow::response(response); });

        CROW_ROUTE(app, "/suggestion/<string>")
        ([](std::string query)
         {
            std::string url = "https://www.bing.com/osjson.aspx?query=" + query;
            CURL *curl;
            CURLcode res;
            std::string response;
            curl = curl_easy_init();
            if (curl)
            {
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
            }
            return crow::response(response); });

        

        CROW_ROUTE(app, "/db")
        ([]()
         {
            Database db;
            db.connect();
            return db.query(); });
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

crow::json::wvalue handleQuery(std::string query)
{
}