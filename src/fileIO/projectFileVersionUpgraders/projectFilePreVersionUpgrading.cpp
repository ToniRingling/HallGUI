#include "projectFileVersionUpgrading.h"

#include <sstream>
#include <boost/algorithm/string.hpp>

#include "../ProjectFileHandling.h"

namespace { // Anonymous namespace - these functions are only available in this file.
    // We attach the adjusted read functions for the old format and the adjusted write functions for the new format here.

    int64_t projectReadLong(std::istream* readFrom){
        char intReadingBuffer[sizeof(int64_t) / sizeof(char)];
        readFrom->read((char*)&intReadingBuffer, sizeof(int64_t) / sizeof(char));
        return *((int64_t*)(&intReadingBuffer));
    }

    int32_t projectReadInt(std::istream* readFrom){
        char intReadingBuffer[sizeof(int32_t) / sizeof(char)];
        readFrom->read((char*)&intReadingBuffer, sizeof(int32_t) / sizeof(char));
        return *((int32_t*)(&intReadingBuffer));
    }

    std::string projectReadString(std::istream* readFrom){
        int length = projectReadInt(readFrom);
        char* buffer = new char[length + 1]; // remember 0-termination
        buffer[length] = 0;
        readFrom->read(buffer, length);
        std::string data = std::string(buffer);
        delete buffer;
        return data;
    }


    void projectWriteLong(std::ostream* writeTo, int64_t data){
        writeTo->write((char*)(&data), sizeof(int64_t) / sizeof(char));
    }

    void projectWriteInt(std::ostream* writeTo, int32_t data){
        writeTo->write((char*)(&data), sizeof(int32_t) / sizeof(char));
    }

    void projectWriteString(std::ostream* writeTo, std::string data){
        boost::erase_all(data, "\r"); // windows handles just \n fine, but on Linux things sometimes break on \r\n
        projectWriteInt(writeTo, data.length());
        writeTo->write(data.c_str(), data.length());
    }






    int32_t nodeReadInt(std::istream* readFrom){
        char intReadingBuffer[sizeof(int32_t) / sizeof(char)];
        readFrom->read((char*)&intReadingBuffer, sizeof(int32_t) / sizeof(char));
        return *((int32_t*)(&intReadingBuffer));
    }

    std::string nodeReadString(std::istream* readFrom){
        int length = nodeReadInt(readFrom);
        char* buffer = new char[length + 1]; // remember 0-termination
        buffer[length] = 0;
        readFrom->read(buffer, length);
        std::string data = std::string(buffer);
        delete buffer;
        return data;
    }


    void nodeWriteInt(std::ostream* writeTo, int32_t data){
        writeTo->write((char*)(&data), sizeof(int32_t) / sizeof(char));
    }

    void nodeWriteString(std::ostream* writeTo, std::string data){
        std::string writing = data;
        boost::erase_all(writing, "\r"); // windows handles just \n fine, but on Linux things sometimes break on \r\n
        nodeWriteInt(writeTo, writing.length());
        writeTo->write(writing.c_str(), writing.length());
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
        nodeWriteString(&optionStream, nodeReadString(readingStream)); // name
        nodeWriteString(&optionStream, nodeReadString(readingStream)); // description
        nodeWriteInt(&optionStream, nodeReadInt(readingStream)); // importance
        int32_t type = nodeReadInt(readingStream);
        if(type == oldSelection) {
            nodeWriteInt(&optionStream, newSelection); // type
            int choiceNumber = nodeReadInt(readingStream);
            nodeWriteInt(&optionStream, choiceNumber);
            for(int a = 0; a < choiceNumber; a++) {
                nodeWriteString(&optionStream, nodeReadString(readingStream)); // a selectable option
            }
            nodeWriteString(&optionStream, nodeReadString(readingStream)); // the current value
        } else if(type == oldTextInput) {
            nodeWriteInt(&optionStream, newTextInput); // type
            nodeWriteString(&optionStream, nodeReadString(readingStream)); // just the standard contents
        } else if(type == oldTextAreaInput) {
            nodeWriteInt(&optionStream, newTextAreaInput); // type
            nodeWriteString(&optionStream, nodeReadString(readingStream)); // just the standard contents
        } else if(type == oldCheckBox) {
            nodeWriteInt(&optionStream, newCheckBox); // type
            nodeWriteInt(&optionStream, nodeReadInt(readingStream)); // standard activation
        }
        return optionStream.str();
    }
}

// Because the original format wasn't well adapted for updating, this will be quite of a mess, as we will have to determine the size of the nodes by reading them normally.
namespace project_file_version_upgrading{
    bool updatePreVersionProjectFile(std::string* toUpdate){
        // This version serves to facilitate easier upgrading for future versions.
        // As such we add a version string and lengths for nodes and project options.
        std::istringstream readingStream = std::istringstream(*toUpdate); // Should cause a copy, as such we can overwrite it directly.
        std::ostringstream writingStream;

        projectWriteString(&writingStream, "HGPV-0"); // Adding version

        projectWriteString(&writingStream, projectReadString(&readingStream)); // name
        projectWriteString(&writingStream, projectReadString(&readingStream)); // description
        projectWriteString(&writingStream, projectReadString(&readingStream)); // answers


        // Now all the project options (will be removed in future versions, but we do not know how long they are a priori, so we have to parse them fully).
        int32_t optionCount = projectReadInt(&readingStream);
        projectWriteInt(&writingStream, optionCount);
        for(int32_t optionNumber = 0; optionNumber < optionCount; optionNumber++){
            projectWriteString(&writingStream, updatedOption(&readingStream)); // This way we get the length of the option before it.
        }


        projectWriteLong(&writingStream, projectReadLong(&readingStream)); // editor offsets
        projectWriteLong(&writingStream, projectReadLong(&readingStream));


        // Now for the contained nodes
        int32_t nodeCount = projectReadInt(&readingStream);
        projectWriteInt(&writingStream, nodeCount);
        for(int32_t nodeNumber = 0; nodeNumber < nodeCount; nodeNumber++){
            std::ostringstream nodeStream;
            nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // name
            nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // description

            {
                int32_t inputCount = nodeReadInt(&readingStream);
                nodeWriteInt(&nodeStream, inputCount);
                for(int32_t inputNumber = 0; inputNumber < inputCount; inputNumber++){
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // name
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // description
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // type
                }
            }

            {
                int32_t outputCount = nodeReadInt(&readingStream);
                nodeWriteInt(&nodeStream, outputCount);
                for(int32_t outputNumber = 0; outputNumber < outputCount; outputNumber++){
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // name
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // description
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // type
                }
            }

            {
                int32_t optionCount = nodeReadInt(&readingStream);
                nodeWriteInt(&nodeStream, optionCount);
                for(int32_t optionNumber = 0; optionNumber < optionCount; optionNumber++){
                    std::string option = updatedOption(&readingStream);
                    nodeStream.write(option.c_str(), option.length()); // We do not write with option length here. That upgrade is done when upgrading the node by itself.
                }
            }

            nodeWriteString(&nodeStream, projectReadString(&readingStream)); // task
            // Following size parts weren't used at this point. They refer to size on the page.
            nodeWriteInt(&nodeStream, nodeReadInt(&readingStream)); // sizeMode
            nodeWriteInt(&nodeStream, nodeReadInt(&readingStream)); // xSize
            nodeWriteInt(&nodeStream, nodeReadInt(&readingStream)); // ySize

            nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // runScript
            nodeWriteString(&nodeStream, nodeReadString(&readingStream)); // initScript

            {
                int32_t importCount = nodeReadInt(&readingStream);
                nodeWriteInt(&nodeStream, importCount);
                for(int32_t importNumber = 0; importNumber < importCount; importNumber++){
                    nodeWriteString(&nodeStream, nodeReadString(&readingStream));
                }
            }
            projectWriteString(&writingStream, nodeStream.str()); // Now nodes have a known length too.
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // node x
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // node y
        }

        // Nodes are through, now to connections.

        int32_t connectionCount = nodeReadInt(&readingStream);
        projectWriteInt(&writingStream, connectionCount);
        for(int32_t connectionNumber = 0; connectionNumber < connectionCount; connectionNumber++){
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // Type of starting port.
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // Number of starting node.
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // Number of starting port on node.
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // Type of ending port.
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // Number of ending node.
            projectWriteInt(&writingStream, projectReadInt(&readingStream)); // Number of ending port on node.
        }





        *toUpdate = writingStream.str();

        // Unless I am mistaken, stringstreams are not closed.
        return true;
    }
};
