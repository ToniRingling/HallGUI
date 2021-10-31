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

    enum oldOptionType : int{
        oldInvalid = -1,
        oldSelection = 0,
        oldTextInput = 1,
        oldTextAreaInput = 2,
        oldCheckBox = 3,
    };

    enum newOptionType : int{
        newInvalid = -1,
        newSelection = 0,
        newTextInput = 1,
        newTextAreaInput = 2,
        newCheckBox = 3,
    };

    // This will take the given stream and advance it reading and upgrading the next option, giving it back as a string.
    std::string updatedOption(std::istringstream* readingStream){
        std::ostringstream optionStream;
        writeString(&optionStream, readString(readingStream)); // name
        writeString(&optionStream, readString(readingStream)); // description
        writeInt(&optionStream, readInt(readingStream)); // importance
        int32_t type = readInt(readingStream);
        if(type == oldSelection) {
            writeInt(&optionStream, newSelection); // type
            int choiceNumber = readInt(readingStream);
            writeInt(&optionStream, choiceNumber);
            for(int a = 0; a < choiceNumber; a++) {
                writeString(&optionStream, readString(readingStream)); // a selectable option
            }
            writeString(&optionStream, readString(readingStream)); // the current value
        } else if(type == oldTextInput) {
            writeInt(&optionStream, newTextInput); // type
            writeString(&optionStream, readString(readingStream)); // just the standard contents
        } else if(type == oldTextAreaInput) {
            writeInt(&optionStream, newTextAreaInput); // type
            writeString(&optionStream, readString(readingStream)); // just the standard contents
        } else if(type == oldCheckBox) {
            writeInt(&optionStream, newCheckBox); // type
            writeInt(&optionStream, readInt(readingStream)); // standard activation
        }
        return optionStream.str();
    }
}

namespace node_file_version_upgrading{
    bool updatePreVersionNodeFile(std::string* toUpdate){
        // This version servers to facilitate easier upgrading for future versions.
        // As such we add a version string and lengths for inputs, outputs and options.
        std::istringstream readingStream = std::istringstream(*toUpdate); // Should cause a copy, as such we can overwrite it directly.
        std::ostringstream writingStream;

        writeString(&writingStream, "HGNV-0"); // Adding version

        writeString(&writingStream, readString(&readingStream)); // name
        writeString(&writingStream, readString(&readingStream)); // description

        {
            int32_t inputCount = readInt(&readingStream);
            writeInt(&writingStream, inputCount);
            for(int32_t inputNumber = 0; inputNumber < inputCount; inputNumber++) {
                std::ostringstream subStream;
                writeString(&subStream, readString(&readingStream)); // name
                writeString(&subStream, readString(&readingStream)); // description
                writeString(&subStream, readString(&readingStream)); // type
                writeString(&writingStream, subStream.str()); // Now with a length for the input.
            }
        }

        {
            int32_t outputCount = readInt(&readingStream);
            writeInt(&writingStream, outputCount);
            for(int32_t outputNumber = 0; outputNumber < outputCount; outputNumber++) {
                std::ostringstream subStream;
                writeString(&subStream, readString(&readingStream)); // name
                writeString(&subStream, readString(&readingStream)); // description
                writeString(&subStream, readString(&readingStream)); // type
                writeString(&writingStream, subStream.str()); // Now with a length for the output.
            }
        }

        {
            int32_t optionCount = readInt(&readingStream);
            writeInt(&writingStream, optionCount);
            for(int32_t optionNumber = 0; optionNumber < optionCount; optionNumber++) {
                std::string option = updatedOption(&readingStream);
                writeString(&writingStream, option); // Now with a length for the option.
            }
        }

        writeString(&writingStream, readString(&readingStream)); // task
        writeInt(&writingStream, readInt(&readingStream)); // size mode
        writeInt(&writingStream, readInt(&readingStream)); // xSize
        writeInt(&writingStream, readInt(&readingStream)); // ySize

        writeString(&writingStream, readString(&readingStream)); // run script
        writeString(&writingStream, readString(&readingStream)); // init script


        {
            int32_t importCount = readInt(&readingStream);
            writeInt(&writingStream, importCount);
            for(int32_t importNumber = 0; importNumber < importCount; importNumber++) {
                writeString(&writingStream, readString(&readingStream)); // Note that imports will be removed again in the next version, so we do not bother giving them total lengths (as they are just single strings anyway).
            }
        }

        *toUpdate = writingStream.str();

        // Unless I am mistaken, stringstreams are not closed.
        return true;
    }
};
