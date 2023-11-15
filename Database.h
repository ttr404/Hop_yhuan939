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
#include "include/crow_all.h"
using json = nlohmann::json;

class Database
{
public:
    Database();
    void connect();
    crow::response query();
    void insert(crow::json::rvalue input);

private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
};

#endif