#include "nodeFileVersionUpgrading.h"

#include <sstream>
#include <boost/algorithm/string.hpp>

#include "../NodeFileHandling.h"

namespace { // Anonymous namespace - these functions are only available in this file.
    // We attach the adjusted read functions for the old format and the adjusted write functions for the new format here.

    int32_t readInt(std::istream* readFrom){
        char intReadingBuffer[sizeof(int32_t) / sizeof(char)];
        readFrom->read((char*)&intReadingBuffer, sizeof(int32_t) / sizeof(char));
        return *((int32_t*)(&intReadingBuffer));
    }

    std::string readString(std::istream* readFrom){
        int length = readInt(readFrom);
        char* buffer = new char[length + 1]; // remember 0-termination
        buffer[length] = 0;
        readFrom->read(buffer, length);
        std::string data = std::string(buffer, length);
        delete buffer;
        return data;
    }


    void writeInt(std::ostream* writeTo, int32_t data){
        writeTo->write((char*)(&data), sizeof(int32_t) / sizeof(char));
    }

    void writeString(std::ostream* writeTo, std::string data){
        boost::erase_all(data, "\r"); // windows handles just \n fine, but on Linux things sometimes break on \r\n
        writeInt(writeTo, data.length());
        writeTo->write(data.c_str(), data.length());
    }
}

namespace node_file_version_upgrading{
    bool updateV0NodeFile(std::string* toUpdate){
        // This version serves to remove unused parts of the previous versions.
        std::istringstream readingStream = std::istringstream(*toUpdate); // Should cause a copy, as such we can overwrite it directly.
        std::ostringstream writingStream;

        readString(&readingStream); // Replacing version.
        writeString(&writingStream, "HGNV-1");

        writeString(&writingStream, readString(&readingStream)); // name
        writeString(&writingStream, readString(&readingStream)); // description

        {
            int32_t inputCount = readInt(&readingStream);
            writeInt(&writingStream, inputCount);
            for(int32_t inputNumber = 0; inputNumber < inputCount; inputNumber++) {
                writeString(&writingStream, readString(&readingStream)); // Write Input back.
            }
        }

        {
            int32_t outputCount = readInt(&readingStream);
            writeInt(&writingStream, outputCount);
            for(int32_t outputNumber = 0; outputNumber < outputCount; outputNumber++) {
                writeString(&writingStream, readString(&readingStream)); // Write Output back.
            }
        }

        {
            int32_t optionCount = readInt(&readingStream);
            writeInt(&writingStream, optionCount);
            for(int32_t optionNumber = 0; optionNumber < optionCount; optionNumber++) {
                writeString(&writingStream, readString(&readingStream)); // Write Option back.
            }
        }

        writeString(&writingStream, readString(&readingStream)); // task

        // Following attributes were meant for placement in HTML context, but for now there is still only vertical ordering. As such we remove them.
        readInt(&readingStream); // size mode
        readInt(&readingStream); // xSize
        readInt(&readingStream); // ySize

        writeString(&writingStream, readString(&readingStream)); // run script
        writeString(&writingStream, readString(&readingStream)); // init script


        // We also just remove the import strings because they are not used yet.
        {
            int32_t importCount = readInt(&readingStream);
            for(int32_t importNumber = 0; importNumber < importCount; importNumber++) {
                readString(&readingStream);
            }
        }

        *toUpdate = writingStream.str();

        // Unless I am mistaken, stringstreams are not closed.
        return true;
    }
};
