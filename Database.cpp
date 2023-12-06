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
    con->setSchema("hop");
}

/**
 * @brief Destroy the Database:: Database object
 *
 */
Database::~Database()
{
    // deconstruct the database
    delete con;
    delete driver;
}

std::vector<Item> Database::get(const std::string query)
{
    std::vector<Item> items;
    sql::PreparedStatement *stmt;

    if (query.empty())
    {
        stmt = con->prepareStatement("SELECT * FROM items");
        res = stmt->executeQuery();
        // res = stmt->executeQuery("SELECT * FROM items");
        // logic to add items from res to vector
        while(res->next())
        {
            Item item;
            item.name = res->getString(2);
            item.tags = split(res->getString(3), ',');
            item.summary = res->getString(4);
            item.url = res->getString(5);
            items.push_back(item);
        }
    }
    else
    {
        std::vector<std::string> words = split(query, ' ');
        std::string sqlQuery = buildQuery(words);
        // res = stmt->executeQuery(sqlQuery);
        stmt = con->prepareStatement(sqlQuery);
        res = stmt->executeQuery();
        // logic to add items from res to vector
        while(res->next())
        {
            Item item;
            item.name = res->getString(2);
            item.tags = split(res->getString(3), ',');
            item.summary = res->getString(4);
            item.url = res->getString(5);
            items.push_back(item);
        }
    }

    // free up memory
    delete stmt;
    return items;
}

/**
 * @brief split a string by a delimiter
 *
 * @param s
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> Database::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief build a query from a vector of words
 *
 * @param words
 * @return std::string
 */
std::string Database::buildQuery(const std::vector<std::string> &words)
{
    std::string query = "SELECT DISTINCT * FROM items WHERE ";
    for (size_t i = 0; i < words.size(); ++i)
    {
        query += "(name LIKE '%" + words[i] + "%' OR tags LIKE '%" + words[i] + "%')";
        if (i != words.size() - 1)
        {
            query += " OR ";
        }
    }
    return query;
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
    sql::PreparedStatement *stmt;
    inputStr += newItem.name;
    inputStr += "', '";
    inputStr += vectorToString(newItem.tags);
    inputStr += "', '";
    inputStr += removeChar(newItem.summary);
    inputStr += "', '";
    inputStr += newItem.url;
    inputStr += "');";
    std::cout << inputStr << std::endl;
    stmt = con->prepareStatement(inputStr);
    stmt->execute();
    // free up memory
    delete stmt;
    return;
}