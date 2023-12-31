/**
 * @file Router.cpp
 * @author 	Maxwell Ding (jding263)
 * @brief Router class that works for the content of the web server
 * @date 2023-11-28
 *
 * @copyright Copyright (c) 2023
 *
 */

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

        CROW_ROUTE(app, "/uploadImage")
            .methods("POST"_method)([&](const crow::request &req)
                                    {
                                        crow::multipart::message parts(req);
                                        crow::multipart::part image_url = parts.get_part_by_name("URL");
                                        Item response = api.vision_openAI(image_url.body);
                                        // Do something with the response, maybe send it back to the client or process it further
                                        db.insert(response);
                                        return crow::response(200, "Item inserted successfully"); // Example response
                                    });

        CROW_ROUTE(app, "/search")
        ([&](const crow::request &req)
         {
            // get query
            std::string query = req.url_params.get("q") ? req.url_params.get("q") : "",
                        type = req.url_params.get("type") ? req.url_params.get("type") : "";
                        crow::mustache::template_t page = crow::mustache::load("search.html");
            if(type == "json")
            {
                crow::json::wvalue json;
                json["type"] = type;
                json["query"] = query;
                json["result"] = handleQuery(query);
                return crow::response(json);
            } 
            else if (type == "keyword")
            {
                crow::json::wvalue json;
                return crow::response(api.response_openAI(query));
            }
            else if (type == "product")
            {
                page = crow::mustache::load("product.html");
            }
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

        // to handle the POST request sent from the webpage
        // that contains the blob object of the recorded voice
        CROW_ROUTE(app, "/voiceUpload")
            .methods("POST"_method)([&](const crow::request &req)
                                    {
                std::time_t now = std::time(nullptr);
                crow::multipart::message parts(req);
                crow::multipart::part file = parts.get_part_by_name("file");
                std::string file_path = "static/upload/" + std::to_string(std::localtime(&now)->tm_sec) + ".mp3";
                std::string base64 = file.body.erase(0, 22);
                voice.decode_base64_and_write_to_file(file.body, file_path);
                // get domain from the request
                std::string domain = req.get_header_value("Host");
                std::string fullPath = "https://" + domain + "/" + file_path;
                std::string id = voiceAPI.callAPI(fullPath);
                nlohmann::json jsonResponse;
                jsonResponse["Rep_id"] = id;
                // Convert JSON object to string
                std::string jsonString = jsonResponse.dump();

                return crow::response(200, jsonString); });

        // used for refetching the result from the API
        CROW_ROUTE(app, "/refetch/<string>")
        ([&](std::string id)
         {
            nlohmann::json jsonResponse = voiceAPI.refetch(id);
            std::string jsonString = jsonResponse.dump(); // Convert JSON object to string
            crow::response res(200, jsonString);
            res.add_header("Content-Type", "application/json"); // Set the Content-Type header to 'application/json'
            return res; });
        CROW_ROUTE(app, "/docs")
        ([&](const crow::request &req)
         {
            crow::response res;
            res.add_header("Location", "/static/docs/html/annotated.html");
            res.code = 302;
            return res; });
    }
    catch (const std::exception &e) // catch any exceptions
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

/**
 * @brief handleQuery handles the query and returns the result in json format
 *
 * @param query
 * @return crow::json::wvalue
 */
crow::json::wvalue Router::handleQuery(std::string query)
{
    crow::json::wvalue json;
    // Magic! Don't touch
    std::vector<crow::json::wvalue> items_list; // list of wvalue
    std::vector<Item> items = db.get(query);
    if (items.size() == 0)
    {
        return crow::json::wvalue({"No result found."});
    }
    for (Item item : items)
    {
        crow::json::wvalue item_json; // new wvalue
        item_json["name"] = item.name;
        std::vector<crow::json::wvalue> tags_list; // list of wvalue
        for (std::string tag : item.tags)          // iterate through tags
        {
            tags_list.push_back(tag); // push wvalue
        }
        item_json["tags"] = crow::json::wvalue(tags_list); // wvalue of list of wvalue
        item_json["summary"] = item.summary;
        item_json["url"] = item.url;
        items_list.push_back(item_json);
    }

    json = std::move(crow::json::wvalue::list(items_list)); // wvalue of list of wvalue
    return json;
}

/**
 * @brief urlDecode decodes the url
 *
 * @param encoded
 * @return std::string
 */
std::string Router::urlDecode(const std::string &encoded)
{
    std::string decoded;
    decoded.reserve(encoded.length());

    for (size_t i = 0; i < encoded.length(); ++i)
    {
        if (encoded[i] == '%' && i + 2 < encoded.length())
        {
            std::string hex = encoded.substr(i + 1, 2);
            decoded += static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
            i += 2;
        }
        else if (encoded[i] == '+')
        {
            decoded += ' ';
        }
        else
        {
            decoded += encoded[i];
        }
    }

    return decoded;
}
