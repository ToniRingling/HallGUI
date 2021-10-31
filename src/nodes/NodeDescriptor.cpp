#include "NodeDescriptor.h"

NodeDescriptor::NodeDescriptor(std::string name, std::string description, std::string path){
    this->name = name;
    this->description = description;
    this->path = path;
}

NodeDescriptor::~NodeDescriptor(){

}

std::string NodeDescriptor::getName(){
    return name;
}

std::string NodeDescriptor::getDescription(){
    return description;
}

std::string NodeDescriptor::getPath(){
    return path;
}
