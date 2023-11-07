#include "Router.h"

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

int Router::enroute(crow::SimpleApp &app)
{
    try
    {
        CROW_ROUTE(app, "/")
        ([&](const crow::request &req)
         {
            auto page = crow::mustache::load("index.html");
            ctx["title"] = "HOP - Home";
            ctx["description"] = "HOP.cheap: Shop without Search, hop in shopping experience.";
            return page.render(ctx);
        });
        
        CROW_ROUTE(app, "/admin")
        ([&](const crow::request &req)
         {
            auto page = crow::mustache::load("dash.html");
            return page.render(ctx);
        });

        CROW_ROUTE(app, "/autocomplete/<string>")
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
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
