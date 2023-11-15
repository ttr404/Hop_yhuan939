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

void Database::insert(crow::json::rvalue input)
{
    // INSERT INTO items (name, tag, summary) VALUES ('banana', '{"tags": ["yellow", "fruit"]}', 'A sweet and nutritious yellow fruit');
    std::string inputStr = "INSERT INTO items (name, tag, summary) VALUES ('";
    if(input.has("name")){
        inputStr += input["name"].s();
        inputStr += "', '";
    }
    else throw error_at_line;
    if(input.has("tags")){
        inputStr += input["tags"].s();
        inputStr += "', '";
    }
    else throw error_at_line;
    if(input.has("summary")){
        inputStr += input["summary"].s();
        inputStr += "');";
    }
    else throw error_at_line;

    ;

    stmt->execute("USE hop");
    



    delete stmt;
    return;
}
