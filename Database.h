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
#include "Item.h"
#include "include/crow_all.h"
using json = nlohmann::json;

class Database
{
public:
    Database();
    void connect();
    crow::response getAll();
    void insert(Item newItem);
    crow::response handleQuery(std::string query);

private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
};

#endif