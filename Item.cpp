#include "Item.h"

Item::Item(){
}

Item::Item(std::string name, std::vector<std::string> tags, std::string summary){
    this->name = name;
    this->tags = tags;
    this->summary = summary;
}

std::string Item::getName(){
    return this->name;
}

std::vector<std::string> Item::getTags(){
    return this->tags;
}

std::string Item::getSummary(){
    return this->summary;
}

void Item::setName(std::string name){
    this->name = name;
}

void Item::setTags(std::vector<std::string> tags){
    this->tags = tags;
}

void Item::setSummary(std::string summary){
    this->summary = summary;
}




