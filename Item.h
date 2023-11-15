#ifndef ITEM_H
#define ITEM_H
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <error.h>
#include <vector>
#include "include/crow_all.h"

class Item
{
public:
    Item();
    Item(std::string name, std::vector<std::string> tags, std::string summary);
    std::string getName();
    std::vector<std::string> getTags();
    std::string getSummary();
    void setName(std::string name);
    void setTags(std::vector<std::string> tags);
    void setSummary(std::string summary);

private:
    std::string name;
    std::vector<std::string> tags;
    std::string summary;
};

#endif