#ifndef DATABASE_H
#define DATABASE_H
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
#include <set>
#include <algorithm>
#include "include/crow_all.h"
using json = nlohmann::json;

struct Item
{
    std::string name;
    std::vector<std::string> tags;
    std::string summary;
    std::string url;
    Item(){};
    Item(std::string name, std::vector<std::string> tags, std::string summary)
    {
        this->name = name;
        this->tags = tags;
        this->summary = summary;
        this->url = url;
    }
} typedef Item;

class Database
{
public:
    Database();
    ~Database();
    std::vector<Item> get(std::string query = "");
    std::vector<std::string> split(const std::string &s, char delimiter);
    std::string buildQuery(const std::vector<std::string> &words);
    void insert(Item newItem);
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Item item;
};

#endif