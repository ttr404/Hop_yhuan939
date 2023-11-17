#include "API.h"
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

std::string API::response_openAI(std::string message)
{
    // CURL *curl;
    // CURLcode res = CURLE_FAILED_INIT; // Default to an error code
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
        const char *data = "{\n    \"model\": \"gpt-3.5-turbo\",\n    \"messages\": [{\"role\": \"system\", \"content\": \"You are a product summary writer.\"},\n        {\"role\": \"user\", \"content\": \"The desctiption below is in the format of json file, now, generate me one new line of description of a backpack so that I could add to the database:: \"},\n        {\"role\": \"user\", \"content\": \"The json has 4 properties: id, name, summary, and tags, and one example of the output should be: 'id':1,'name':banana,'summary':'A sweet and nutritious yellow fruit','tags':{['yellow', 'fruit']}\"\n\n        }]\n  }";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        // Check if curl_easy_perform was successful
        if (res == CURLE_OK)
        {
            std::cout << "Response data: " << response << std::endl;
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

// Item API::translate(CURLcode res){

// }

// int main(){
//     API api;
//     api.response_openAI("message");
//     return 0;
// }