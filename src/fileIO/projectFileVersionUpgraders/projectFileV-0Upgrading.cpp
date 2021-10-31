#include "projectFileVersionUpgrading.h"

#include <sstream>
#include <boost/algorithm/string.hpp>

#include "../ProjectFileHandling.h"

namespace { // Anonymous namespace - these functions are only available in this file.
    // We attach the adjusted read functions for the old format and the adjusted write functions for the new format here.

    int64_t readLong(std::istream* readFrom){
        char intReadingBuffer[sizeof(int64_t) / sizeof(char)];
        readFrom->read((char*)&intReadingBuffer, sizeof(int64_t) / sizeof(char));
        return *((int64_t*)(&intReadingBuffer));
    }

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


    void writeLong(std::ostream* writeTo, int64_t data){
        writeTo->write((char*)(&data), sizeof(int64_t) / sizeof(char));
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

namespace project_file_version_upgrading{
    bool updateV0ProjectFile(std::string* toUpdate){
        // This version serves to remove unused parts of the previous versions.
        // Furthermore project options are removed because they are a horrible feature.
        std::istringstream readingStream = std::istringstream(*toUpdate); // Should cause a copy, as such we can overwrite it directly.
        std::ostringstream writingStream;

        readString(&readingStream); // Ignoring old version.
        writeString(&writingStream, "HGPV-1"); // Writing new version

        writeString(&writingStream, readString(&readingStream)); // name
        readString(&readingStream); // description is not yet used, we chuck it for now
        writeString(&writingStream, readString(&readingStream)); // answers


        // Now all the project options which we just ignore because we do not want them anymore.
        int32_t optionCount = readInt(&readingStream);
        for(int32_t optionNumber = 0; optionNumber < optionCount; optionNumber++){
            readString(&readingStream); // Just throw the option away.
        }

        writeLong(&writingStream, readLong(&readingStream)); // editor offsets
        writeLong(&writingStream, readLong(&readingStream));

        // Now for the contained nodes
        int32_t nodeCount = readInt(&readingStream);
        writeInt(&writingStream, nodeCount);
        for(int32_t nodeNumber = 0; nodeNumber < nodeCount; nodeNumber++){
            writeString(&writingStream, readString(&readingStream)); // We just write them back. Updates are done by nodeFileVersionUpgrading.
            writeInt(&writingStream, readInt(&readingStream)); // node x
            writeInt(&writingStream, readInt(&readingStream)); // node y
        }

        // Nodes are through, now to connections.

        int32_t connectionCount = readInt(&readingStream);
        writeInt(&writingStream, connectionCount);
        for(int32_t connectionNumber = 0; connectionNumber < connectionCount; connectionNumber++){
            writeInt(&writingStream, readInt(&readingStream)); // Type of starting port.
            writeInt(&writingStream, readInt(&readingStream)); // Number of starting node.
            writeInt(&writingStream, readInt(&readingStream)); // Number of starting port on node.
            writeInt(&writingStream, readInt(&readingStream)); // Type of ending port.
            writeInt(&writingStream, readInt(&readingStream)); // Number of ending node.
            writeInt(&writingStream, readInt(&readingStream)); // Number of ending port on node.
        }

        *toUpdate = writingStream.str();
        // Unless I am mistaken, stringstreams are not closed.
        return true;
    }
};
