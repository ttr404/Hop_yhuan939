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
            ctx["title"] = "Home - HOP";
            ctx["description"] = "HOP.cheap: Shop without Search, hop in shopping experience.";
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

        // new added
        CROW_ROUTE(app, "/openai")
            .methods("POST"_method)([this](const crow::request &req)
                                    {
            // Extract the prompt from POST data
            auto x = crow::json::load(req.body);
            if (!x)
                return crow::response(400);

            std::string prompt = x["type"].s();

            // Make the API request to OpenAI
            std::string response = RequestToOpenAI(prompt);

            // Send the response back to the client
            return crow::response{response}; });

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

// Function that makes a request to OpenAI API
std::string Router::RequestToOpenAI(const std::string &prompt)
{
    // const std::string api_key = std::getenv("OPENAI_API_KEY");

    const std::string api_key = "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"; // You should use an environment variable or a config for this!
    const char *url = "https://api.openai.com/v1/engines/davinci-codex/completions";

    CURL *curl;
    CURLcode res;
    std::string response;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    if (curl)
    {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string auth_header = "Authorization: Bearer " + api_key;
        headers = curl_slist_append(headers, auth_header.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        // Convert your parameters to JSON
        crow::json::wvalue json_payload = {
            {"prompt", prompt},
            {"max_tokens", 150},
            {"model", "gpt-3.5-turbo"},
            {"messages", {{"role", "system"}, {"content", "You are a summary writer."}, {"role", "user"}, {"content", "Write me a summary for tag red, bag, schoolbag, backpack"}, {"role", "assistant"}, {"content", "This is a summary about a red backpack: Embrace the boldness of red with this versatile and durable backpack designed to cater to every student's needs. Our red schoolbag isn't just a statement piece, but a practical companion for the daily hustle of academic life. It features multiple compartments to keep books, devices, and personal items organized. The padded straps provide comfort for those long days, while the sturdy fabric ensures it can handle the weight of textbooks and laptops alike. Whether it's for school, travel, or casual use, this backpack combines functionality with style, making it an essential for anyone who wants to stand out in a crowd while having everything they need within reach."}, {"role", "user"}, {"content", "Write me a summary for keychain"}}}
            // Example parameter, add others as needed
        };
        // std::string post_data = json_payload.dump();

        // Set cURL options for POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 1L);
    }
    std::cout << response;
    return response;
}