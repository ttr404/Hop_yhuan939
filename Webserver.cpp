#include "Webserver.h"

Webserver::Webserver()
{
    router.enroute(app);
    std::cout << "Webserver created" << std::endl;
}
Webserver::~Webserver()
{
    std::cout << "Webserver destroyed" << std::endl;
}
void Webserver::run(int port)
{
    app.port(port).multithreaded().run();
    std::cout << "Webserver running on port " << port << std::endl;
}
void Webserver::stop()
{
    std::cout << "Webserver stopped" << std::endl;
}