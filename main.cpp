#include <iostream>
#include <string>
#include "include/crow_all.h"
#include <curl/curl.h>
#include "router.h"

size_t write_callback(char *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

int main()
{
    crow::SimpleApp app; // define your crow application
    std::string content;
    std::stringstream buffer;
    crow::mustache::context ctx;

    // define your endpoint at the root directory
    CROW_ROUTE(app, "/")
    ([&content, &buffer, &ctx]()
     {
        std::ifstream html("public/index.html");
        buffer.str("");
        buffer << html.rdbuf();
        content = buffer.str();
        ctx["title"] = "HOP.cheap: Shop without Search - Hop in shopping experience";
        return crow::mustache::compile(content).render(ctx); });

    // TODO: delete this route
    CROW_ROUTE(app, "/drink/<int>")
    ([](int count)
     {
        if (count > 100) {
            return crow::response(400);
        }
        std::ostringstream os;
        os << count << " bottle" << (count > 1 ? "s" : "") << " of beer!";
        return crow::response(os.str()); });

    // TODO: move to router class
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

    // TODO: move to router class
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

    // set the port, set the app to run on multiple threads, and run the app
    app.port(8080).multithreaded().run();
}