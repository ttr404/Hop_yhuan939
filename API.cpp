#include "API.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <algorithm>
using json = nlohmann::json;

// #include "Item.h"

size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size * nmemb;
    try
    {
        s->append((char *)contents, newLength);
        return newLength;
    }
    catch (std::bad_alloc &e)
    {
        // Handle memory allocation error
        return 0;
    }
}

API::API()
{
    OpenAI = {
        "OpenAI",
        "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",
        "https://api.openai.com/v1/chat/completions"};
    GoogleTrends = {
        "GoogleTrends",
        "",
        "https://trends.google.com/trends/trendingsearches/daily/rss?geo=CA"};
    BingSuggestion = {
        "BingSuggestion",
        "",
        "https://www.bing.com/osjson.aspx?query="};
    // openAI_link= "https://api.openai.com/v1/chat/completions";
    // openAI_key="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";
}

void API::extractResponseData(const std::string& responseData, std::string& name, std::string& summary, std::vector<std::string>& tags)
{
    try {
        auto responseJson = json::parse(responseData);

        auto choices = responseJson["choices"];
        if (choices.is_array() && !choices.empty())
        {
            std::string content = choices[0]["message"]["content"];

            // Extract name
            auto nameStart = content.find("'name':") + 8;
            auto nameEnd = content.find("',", nameStart);
            name = content.substr(nameStart, nameEnd - nameStart);
            name.erase(std::remove(name.begin(), name.end(), '\''), name.end()); // Remove single quotes

            // Extract summary
            auto summaryStart = content.find("'summary':") + 11;
            auto summaryEnd = content.find("',", summaryStart);
            summary = content.substr(summaryStart, summaryEnd - summaryStart);
            summary.erase(std::remove(summary.begin(), summary.end(), '\''), summary.end()); // Remove single quotes

            // Extract tags
            auto tagsStart = content.find("['") + 2;
            auto tagsEnd = content.find("']", tagsStart);
            std::string tagsStr = content.substr(tagsStart, tagsEnd - tagsStart);

            std::istringstream tagStream(tagsStr);
            std::string tag;
            while (std::getline(tagStream, tag, ',')) {
                tag.erase(std::remove_if(tag.begin(), tag.end(), [](char c) { return c == ' ' || c == '\''; }), tag.end()); // Remove spaces and single quotes
                tags.push_back(tag);
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing response data: " << e.what() << std::endl;
    }
}


std::string API::response_openAI(std::string message)
{
    // CURL *curl;
    // CURLcode res = CURLE_FAILED_INIT; // Default to an error code
    std::string name;
    std::string summary;
    std::vector<std::string> tags;
    response = "";
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, OpenAI.url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        char header_string[256]; // Adjust the size as necessary
        sprintf(header_string, "Authorization: Bearer %s", OpenAI.key);
        headers = curl_slist_append(headers, header_string);
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Cookie: __cf_bm=a2G2DPqwgZfjno6TxGTGlr67X7QEWfuocCUMkPt_J.A-1700083564-0-ASn1MqABc/z7RkLFqTNrDWdVFQzKDKEb2oaXS7hBu/lnsNncEUfdGrm0vqxQHVYaOJ7pwOUPpjelWc0Bunjszkw=; _cfuvid=WoTQbHaZY_70._GaSXA3ATmR9Or895X_RssxxmJeGz0-1700081522623-0-604800000");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        char data[1024]; // Adjust the size as necessary
        sprintf(data, "{\n    \"model\": \"gpt-3.5-turbo\",\n    \"messages\": [{\"role\": \"system\", \"content\": \"You are a product summary writer.\"},\n        {\"role\": \"user\", \"content\": \"The description below is in the format of a JSON file, now, generate me one new line of description of a %s so that I could add to the database:: \"},\n        {\"role\": \"user\", \"content\": \"The json has 4 properties: id, name, summary, and tags, and one example of the output should be: 'id':1,'name':banana,'summary':'A sweet and nutritious yellow fruit','tags':{['yellow', 'fruit']}\"\n\n        }]\n  }", message.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        // Check if curl_easy_perform was successful
        if (res == CURLE_OK)
        {
            extractResponseData(response, name, summary, tags);

            // Output the extracted data for verification
            std::cout << response << std::endl;
            std::cout << "Name: " << name << std::endl;
            std::cout << "Summary: " << summary << std::endl;
            std::cout << "Tags: ";
            for (const auto& tag : tags) {
                std::cout << tag << " ";
            }
            std::cout << std::endl;
        }
        else
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    else
    {
        std::cerr << "Error initializing curl" << std::endl;
    }
    return response;
}

std::string API::googleTrends()
{
    response = "";
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, GoogleTrends.url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

std::string API::bingSuggestion(std::string query)
{
    response = "";
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, (BingSuggestion.url + query).c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

std::string API::vision_openAI(std::string imageURL)
{
    std::string name;
    std::string summary;
    std::vector<std::string> tags;
    std::string response="";
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions?Authorization=Bearer%20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&Content-Type=application%2Fjson");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Authorization: Bearer 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");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Cookie: __cf_bm=h.pFjbb9HoNL1BPhLTPuk7McIEU4vJBuGxDjgprcejk-1700687871-0-Afy9GnYFrq+bLZg0laFb1BQ8znMhEpNwwv+kr+sHa0to3tL67n5jFiEMGFX2X3IWNaghreRzhqoT8jM/bRbXlT8=; _cfuvid=Qpi3Eam2TduP9HHXfuzsfiDW1RO20Sn_c8nGObMw260-1700687871631-0-604800000");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    char data[4096]; // Increase the buffer size if needed
    sprintf(data, "{\n    \"model\": \"gpt-4-vision-preview\",\n    \"messages\": [\n      {\n        \"role\": \"user\",\n        \"content\": [\n          {\n            \"type\": \"text\",\n            \"text\": \"Analyze this image and generate a product description in a structured JSON format with properties: id, name, summary, and tags.\"\n          },\n          {\n            \"type\": \"image_url\",\n            \"image_url\": {\n              \"url\": \"%s\"\n            }\n          }\n        ]\n      }\n    ],\n    \"max_tokens\": 300\n  }", imageURL.c_str());

    
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        


    res = curl_easy_perform(curl);
    if (res == CURLE_OK)
        {
            extractImageData(response, name, summary, tags);

            // Output the extracted data for verification
            std::cout << response << std::endl;
            std::cout << "Name: " << name << std::endl;
            std::cout << "Summary: " << summary << std::endl;
            std::cout << "Tags: ";
            for (const auto& tag : tags) {
                std::cout << tag << " ";
            }
            std::cout << std::endl;
        }
        else
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

    }
    curl_easy_cleanup(curl);
    return response;
}

void API::extractImageData(const std::string& responseData, std::string& name, std::string& summary, std::vector<std::string>& tags)
{
    try {
        auto responseJson = json::parse(responseData);

        auto choices = responseJson["choices"];
        if (choices.is_array() && !choices.empty())
        {
            // Remove the code block markers
            std::string content = choices[0]["message"]["content"];
            size_t codeBlockStart = content.find("```json") + 7;
            size_t codeBlockEnd = content.rfind("```");
            if (codeBlockEnd != std::string::npos && codeBlockStart < codeBlockEnd) {
                content = content.substr(codeBlockStart, codeBlockEnd - codeBlockStart);

                // Parse the JSON content
                auto productJson = json::parse(content);

                // Extract name, summary, and tags
                name = productJson["name"];
                summary = productJson["summary"];
                for (const auto& tag : productJson["tags"]) {
                    tags.push_back(tag.get<std::string>());
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error parsing response data: " << e.what() << std::endl;
    }
}



// int main(){
//     API api;
//     api.response_openAI("message");
//     return 0;
// }
