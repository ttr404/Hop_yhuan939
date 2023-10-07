#include <iostream>
#include <filesystem>
#include "include/crow_all.h"


int main() {
    crow::SimpleApp app; //define your crow application
    std::string content;
    std::stringstream buffer;
    crow::mustache::context ctx;

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/")([&content, &buffer, &ctx]() {
        std::ifstream html("public/index.html");
        buffer.str("");
        buffer << html.rdbuf();
        content = buffer.str();
        ctx["title"] = "HOP.cheap: Shop without Search - Hop in shopping experience";
        return crow::mustache::compile(content).render(ctx);
    });

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/drink/<int>")([](int count) {
        if (count > 100) {
            return crow::response(400);
        }
        std::ostringstream os;
        os << count << " bottle" << (count > 1 ? "s" : "") << " of beer!";
        return crow::response(os.str());
    });


    //set the port, set the app to run on multiple threads, and run the app
    app.port(8081).multithreaded().run();
}