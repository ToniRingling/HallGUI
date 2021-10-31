#include "CommonFileHandling.h"
#include "iostream"

#include <boost/algorithm/string.hpp>

// Reading

int64_t common_file_handling::readLong(std::istream* readFrom){
    char intReadingBuffer[sizeof(int64_t) / sizeof(char)];
    readFrom->read((char*)&intReadingBuffer, sizeof(int64_t) / sizeof(char));
    return *((int64_t*)(&intReadingBuffer));
}

int32_t common_file_handling::readInt(std::istream* readFrom){
    char intReadingBuffer[sizeof(int32_t) / sizeof(char)];
    readFrom->read((char*)&intReadingBuffer, sizeof(int32_t) / sizeof(char));
    return *((int32_t*)(&intReadingBuffer));
}

std::string* common_file_handling::readString(std::istream* readFrom){
    int length = readInt(readFrom);
    char* buffer = new char[length + 1]; // remember 0-termination
    buffer[length] = 0;
    readFrom->read(buffer, length);
    std::string* data = new std::string(buffer, length);
    delete buffer;
    return data;
}

// Writing

void common_file_handling::writeLong(std::ostream* writeTo, int64_t data){
    writeTo->write((char*)(&data), sizeof(int64_t) / sizeof(char));
}

void common_file_handling::writeInt(std::ostream* writeTo, int32_t data){
    writeTo->write((char*)(&data), sizeof(int32_t) / sizeof(char));
}

void common_file_handling::writeString(std::ostream* writeTo, std::string* data){
    std::string writing = *data;
    boost::erase_all(writing, "\r"); // windows handles just \n fine, but on Linux things sometimes break on \r\n
    writeInt(writeTo, writing.length());
    writeTo->write(writing.c_str(), writing.length());
}
