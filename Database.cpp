#include "Database.h"
#include <string>

Database::Database()
{
}

void Database::connect()
{
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "hop", "2^WsMm$3UA8uXcYn%U");
    stmt = con->createStatement();
}

crow::response Database::query()
{
    stmt->execute("USE hop");
    res = stmt->executeQuery("SELECT * FROM items");
    json items;
    while (res->next())
    {
        json object;
        object["id"] = res->getString(1);
        object["name"] = res->getString(2);
        object["tags"] = res->getString(3);
        object["summary"] = res->getString(4);
        items.push_back(object);
    }
    delete stmt;
    delete con;
    delete res;
    auto test = crow::response(items.dump());
    test.set_header("Content-Type", "application/json");
    return test;
}


std::string vectorToString(const std::vector<std::string>& vec) {
    std::string result;
    for (const auto& str : vec) {
        result += str; 
    }
    return result;
}


void Database::insert(Item newItem)
{// INSERT INTO items (name, tag, summary) VALUES ('banana', '{"tags": ["yellow", "fruit"]}', 'A sweet and nutritious yellow fruit');
    std::string inputStr = "INSERT INTO items (name, tag, summary) VALUES ('";
    inputStr += newItem.getName();
    inputStr += "', '";
    inputStr += vectorToString(newItem.getTags());
    inputStr += "', '";
    inputStr += newItem.getSummary();
    inputStr += "');";
    stmt->execute("USE hop");
    stmt->execute(inputStr);
    delete stmt;
    return;
}

crow::json::wvalue Database::handleQuery(std::string query)
{
    crow::json::wvalue result;
   
    return result;
}   
