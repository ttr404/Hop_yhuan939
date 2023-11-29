#ifndef VOICEAPI_H
#define VOICEAPI_H
#include <cstdlib>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

class VoiceAPI {
public:
    static std::string callAPI(std::string &filePath);
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s);
    static std::string refetch(std::string &token, std::string &id);
};
#endif