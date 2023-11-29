// used for sending a curl request to the API
// here the OpenAI Whisper model is being run at Replicate
#include "VoiceAPI.h"

// get the file from the path and upload it to the API
// the idea is: get the file path, then send a curl request to the API
// the API will then return a JSON object with the text that contains the transcribed text
// there might be delays for a few seconds, refetching is required
// the text will then be parsed and sent to the frontend as a string
std::string VoiceAPI::callAPI(std::string &filePath)
{
    CURL *curl = curl_easy_init();
    // storing the response from the API
    std::string readBuffer = "";
    std::string url = "https://api.replicate.com/v1/deployments/mdyude/whisper-3307/predictions";
    std::string postFields = "{\"input\": {\"audio\": \"" + filePath + "\"}}";
    std::string token = "r8_X1ErZq5FO5J4XZc00HphUvYTWLr3zdP1utzio";
    // std::string headers = "Authorization: Token " + token;
    // this is fucking culprit fuck fuck FUCKKKKKKKKKKK
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Token " + token).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);


    // execute the CURL command
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        // cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        try {   
            auto jsonResponse = nlohmann::json::parse(readBuffer);
            std::cout << "Response: " << jsonResponse.dump(4) << std::endl;
            if (jsonResponse["status"] != "succeeded") {
                // const std::chrono::milliseconds retryDelay(3000); 
                // sleep(5);
                std::string id = jsonResponse["id"];
                // std::string ret;
                return id;
                // for (int i = 0; i < 5; i ++) {
                //     // std::this_thread::sleep_for(retryDelay);
                //     sleep(3);
                //     ret = refetch(token, id);
                //     if (ret != "qwertyuiopasdfghjklzxcvbnm") {
                //         return ret;
                //     }
                // }
            }
        }
        catch (nlohmann::json::parse_error &e)
        {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }
    // curl_slist_free_all(headers);
    return "qwertyuiopasdfghjklzxcvbnm";
}

// function for refetching the response
// as it might take a couple seconds to
std::string VoiceAPI::refetch(std::string &token, std::string &id) {
    CURL *curlRe = curl_easy_init();
    std::string readBuffer;
    std::string url = "https://api.replicate.com/v1/predictions/" + id;
    std::cout << "Refetching..." << std::endl;
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Token " + token).c_str());
    curl_easy_setopt(curlRe, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlRe, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curlRe, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curlRe, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curlRe, CURLOPT_FORBID_REUSE, 1);

    CURLcode res = curl_easy_perform(curlRe);

    if (res != CURLE_OK) {
        std::cerr << "refetech failed: " << curl_easy_strerror(res) << std::endl;
    }
    else {
        try {
            auto jsonResponse = nlohmann::json::parse(readBuffer);
            std::cout << "Refetched Response: " << jsonResponse.dump(4) << std::endl;
            if (jsonResponse["status"] == "succeeded")
            {
                auto output = jsonResponse["output"];
                auto transcript = output["transcript"];
                std::cout << "Refetch succeeded" << std::endl;
                std::cout << "Response: " << jsonResponse.dump(4) << std::endl;
                std::cout << "Response: " << transcript << std::endl;
                curl_slist_free_all(headers);
                curl_easy_cleanup(curlRe);
                return transcript;
            }
        }
        catch (nlohmann::json::parse_error &e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(curlRe);
    return "qwertyuiopasdfghjklzxcvbnm";
}

// callback function for curl
size_t VoiceAPI::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size * nmemb;
    try
    {
        s->append((char *)contents, newLength);
        return newLength;
    }
    catch (std::bad_alloc &e)
    {
        // handle memory problem
        return 0;
    }
}

// https://dev-x.hop.cheap/static/upload/57.mp3
// curl -s -X POST \
  -d '{"input": {"audio": "..."}}' \
  -H "Authorization: Token $REPLICATE_API_TOKEN" \
  "https://api.replicate.com/v1/deployments/mdyude/whisper-3307/predictions"
