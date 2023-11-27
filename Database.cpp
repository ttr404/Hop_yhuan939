#include "Database.h"
#include <string>

Database::Database()
{
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "hop", "2^WsMm$3UA8uXcYn%U");
    stmt = con->createStatement();
}

Database::~Database()
{
    delete stmt;
    delete con;
    delete driver;
}

std::vector<Item> Database::get(std::string query)
{
    int col;
    std::vector<Item> items;
    stmt->execute("USE hop");
    if (query.length() == 0) 
    {
        res = stmt->executeQuery("SELECT * FROM items");
    } else {
        res = stmt->executeQuery("SELECT * FROM items WHERE tags LIKE '%" + query + "%'");
    }
    col = res->getMetaData()->getColumnCount();
    while (res->next())
    {
        Item item;
        item.name = res->getString(2); // Updated column index
        item.summary = res->getString(4); // Updated column index
        std::string tags = res->getString(3); // Updated column index
        std::string tag;
        for (int i = 0; i < tags.length(); i++)
        {
            if (tags[i] == ',' || tags[i] == ']' )
            {
                item.tags.push_back(tag);
                tag = "";
            }
            else if (tags[i] == '['|| tags[i] == '"')
            {
                continue;
            }
            else
            {
                tag += tags[i];
            }
        }
        items.push_back(item);
    }
    return items;
}


std::string vectorToString(const std::vector<std::string>& vec) {
    std::string result = "{'tags': ['}";
    for (const auto& str : vec) {
        result += str; 
        result += "','";
    }
    result += "']}";
    return result;
}


void Database::insert(Item newItem)
{// INSERT INTO items (name, tags, summary, url) VALUES ('name', '{"tags": ["tag1", "tag2"]}', 'This is a description for the item.', "this is the url of the item");
// INSERT INTO items (name, tags, summary) VALUES ('Modern Home Office Setup', 'home officedesk setupcomputerworkstationoffice suppliesproductivity', 'A well-organized home office setup featuring a desktop computer with statistical data displayed on the monitor. The desk is equipped with essential office supplies, including a note pad, smart phone, decorative plants, and stationery. This setup is ideal for professionals or students who require a dedicated space for work or study.');   
// INSERT INTO items (name, tags, summary) VALUES ('Modern Home Office Setup', '{"tags": ["home", "officedesk", "setupcomputerworkstationoffice", "suppliesproductivity"]}, 'A well-organized home office setup featuring a desktop computer with statistical data displayed on the monitor. The desk is equipped with essential office supplies, including a note pad, smart phone, decorative plants, and stationery. This setup is ideal for professionals or students who require a dedicated space for work or study.');   
   
    std::string inputStr = "INSERT INTO items (name, tags, summary) VALUES ('";
    inputStr += newItem.name;
    inputStr += "', '";
    inputStr += vectorToString(newItem.tags);
    inputStr += "', '";
    inputStr += newItem.summary;
    inputStr += "');";
    std::cout<<inputStr<<std::endl;
    stmt->execute("USE hop");
    stmt->execute(inputStr);
    delete stmt;
    return;
}

// crow::response Database::handleQuery(std::string query)
// {
//     stmt->execute("USE hop");
//     res = stmt->executeQuery(query);
//     json items;
//     while (res->next())
//     {
//         json object;
//         object["id"] = res->getString(1);
//         object["name"] = res->getString(2);
//         object["tags"] = res->getString(3);
//         object["summary"] = res->getString(4);
//         items.push_back(object);
//     }
//     delete stmt;
//     delete con;
//     delete res;
//     auto test = crow::response(items.dump());
//     test.set_header("Content-Type", "application/json");
//     return test;
// }   
