/**
 * @file Database.cpp
 * @author Yulun Feng (yfeng445)
 * @brief A database class that handles all the database operations
 * @date 2023-11-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Database.h"
#include <string>

/**
 * @brief Construct a new Database:: Database object
 *
 */
Database::Database()
{
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "hop", "2^WsMm$3UA8uXcYn%U");
    stmt = con->createStatement();
}

/**
 * @brief Destroy the Database:: Database object
 *
 */
Database::~Database()
{
    delete stmt;
    delete con;
    delete driver;
}

/**
 * @brief get items from the database
 *
 * @param query
 * @return std::vector<Item>
 */
std::vector<Item> Database::get(std::string query)
{
    std::string originalQuery = query;
    std::vector<Item> items, results;
    stmt->execute("USE hop");
    if (query.length() == 0)
    {
        res = stmt->executeQuery("SELECT * FROM items");
    }
    else
    {
        // split query into words
        std::vector<std::string> words;
        std::string word;
        // split query by "+"
        for (int i = 0; i < query.length(); i++)
        {
            if (query[i] == '+' || query[i] == ' ' || i == query.length() - 1)
            {
                words.push_back(word);
                word = "";
            }
            else
            {
                word += query[i];
            }
        }


        query = "";
        for (int i = 0; i < words.size(); i++)
        {
            query += " name LIKE '%" + words[i] + "%'";
            if (i != words.size() - 1)
            {
                query += " OR";
            }
        }
        results = getResult(query);
        items.insert(items.end(), results.begin(), results.end());

        // build query
        query = "";
        for (int i = 0; i < words.size(); i++)
        {
            query += " tags LIKE '%" + words[i] + "%'";
            if (i != words.size() - 1)
            {
                query += " OR";
            }
        }
        results = getResult(query);
        items.insert(items.end(), results.begin(), results.end());
    }
    // remove duplicates
    for(int i = 0; i < items.size(); i++)
    {
        for(int j = i + 1; j < items.size(); j++)
        {
            if(items[i].name == items[j].name)
            {
                items.erase(items.begin() + j);
                j--;
            }
        }
    }
    return items;
}

std::vector<Item> Database::getResult(std::string query)
{
    std::vector<Item> items;
    res = stmt->executeQuery("SELECT * FROM items WHERE " + query);
    while (res->next())
    {
        Item item;
        item.url = res->getString(5);         // Updated column index
        item.name = res->getString(2);        // Updated column index
        item.summary = res->getString(4);     // Updated column index
        std::string tags = res->getString(3); // Updated column index
        std::string tag;
        for (int i = 0; i < tags.length(); i++)
        {
            if (tags[i] == ',' || tags[i] == ']')
            {
                item.tags.push_back(tag);
                tag = "";
            }
            else if (tags[i] == '[' || tags[i] == '"')
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

/**
 * @brief convert a vector of strings to a string
 *
 * @param vec
 * @return std::string
 */
std::string vectorToString(const std::vector<std::string> &vec)
{
    std::string result = "[\"";
    for (const auto &str : vec)
    {
        result += str;
        result += "\",\"";
    }
    result += "\"]";
    return result;
}

std::string removeChar(std::string str)
{
    char charToRemove = '\'';
    std::string result;
    for (char c : str)
    {
        if (c != charToRemove)
        {
            result += c;
        }
    }
    return result;
}

/**
 * @brief Insert a new item into the database
 *
 * @param newItem
 */
void Database::insert(Item newItem)
{
    std::string inputStr = "INSERT INTO items (name, tags, summary, url) VALUES ('";
    inputStr += newItem.name;
    inputStr += "', '";
    inputStr += vectorToString(newItem.tags);
    inputStr += "', '";
    inputStr += removeChar(newItem.summary);
    inputStr += "', '";
    inputStr += newItem.url;
    inputStr += "');";
    std::cout << inputStr << std::endl;
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
