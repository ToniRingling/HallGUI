#include "NodeFileHandling.h"

#include <fstream>
#include <sstream>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include "../Globals.h"

#include "nodeFileVersionUpgraders/nodeFileVersionUpgrading.h"

/* Description of GHN (Grim-Hall-Node) files (GHP (Grim-Hall-Project) contain nodes written in the same way):
Note that all data will be interpreted as char* when reading and writing;

ints are all 32 bit

Sequentially (no additional line breaks between the data):

    Length of the version name - 32 bit int
    version name (as in the version of the node file, changes described at bottom) - chars (content of std::string)

    Length of standard name in bytes - int
    Name - chars (content of std::string)

    Length of description - int
    Description - chars (content of std::string)

    Number of Inputs - int
    for each Input:
        Length of all following input contents together as a string in bytes - int

        Length of input name in bytes - int
        Input name (both external and in javaScript) - chars (content of std::string)
        Length of input description in bytes - int
        Input description - chars (content of std::string)
        Length of preferred type name (i.e. 'string' or specially 'any', used for warnings, oriented on output of typeof) in bytes - int
        Preferred type name - chars (content of std::string)

    Number of outputs - int
    for each output:
        Length of all following output contents together as a string in bytes - int

        Length of output name in bytes - int
        Output name (both external and in javaScript) - chars (content of std::string)
        Length of output description in bytes - int
        Output description - chars (content of std::string)
        Length of expected type name (i.e. 'string' or specially 'any', used for warnings, oriented on output of typeof) in bytes - int
        Expected type name - chars (content of std::string)

    Number of options (i.e. settings for node in GUI) - int
    for each option:
        Length of all following option contents together as a string in bytes - int

        Length of option name in bytes - int
        Option name (this will be displayed and this enclosed in curly braces will be replaced with the selected configuration) - chars (content of std::string)
        Length of option description in bytes - int
        Option description - chars (content of std::string)
        Importance (whether displayed on node in editor or only in option widget) - int (see enum at top)
        Option type (determines appearance and following data) - int (see enum at top)
        -option type specific data, including data to reconstruct the value of the option- (see option cores)

    Length of task content in bytes - int
    Task content - chars (content of std::string)
    Type of size given next (absolute/ relative) - int (see enum at top)
    Standard x size of content
    Standard y size of content

    Length of triggered javaScript content in bytes - int
    Triggered javaScript content - chars;
    Length of initializing javaScript content in bytes - int
    Initializing javaScript content - chars;

    Number of javaScript imports - int
    for each javaScript import:
        Length of import in bytes - int
        Import - chars (content of std::string)

Changes in the versions:
    HGNV-0
        Initial named version.
        Now with lengths for inputs, outputs and options.
*/

// Reading

int32_t node_file_handling::readInt(std::istream* readFrom){
    char intReadingBuffer[sizeof(int32_t) / sizeof(char)];
    readFrom->read((char*)&intReadingBuffer, sizeof(int32_t) / sizeof(char));
    return *((int32_t*)(&intReadingBuffer));
}

std::string* node_file_handling::readString(std::istream* readFrom){
    int length = readInt(readFrom);
    char* buffer = new char[length + 1]; // remember 0-termination
    buffer[length] = 0;
    readFrom->read(buffer, length);
    std::string* data = new std::string(buffer, length);
    delete buffer;
    return data;
}

NodeDescriptor* node_file_handling::loadDescriptor(std::string path){
    std::ifstream* from = new std::ifstream(path, std::istream::binary);
    NodeDescriptor* desc = loadDescriptor(from, path);
    delete from;
    return desc;
};
#include <iostream>
NodeDescriptor* node_file_handling::loadDescriptor(std::istream* from, std::string path){
    // TODO: This is far more fragile as far as version changing is concerned. (Especially if we ever want to change reading/writing functions.)
    std::string* version = readString(from);
    std::string* name;
    if(version->rfind("HGNV", 0) == 0){ // This node either has a version string or a name that starts with HGNV. We just hope for the former.
        name = readString(from);
        delete version;
    }
    else{
        name = version; // Pre-version node. This means that the first entry is the name.
    }
    std::string* description = readString(from);
    NodeDescriptor* desc = new NodeDescriptor(*name, *description, path);
    delete name;
    delete description;
    return desc;
};


Node* node_file_handling::loadFull(std::istream* from){
    std::stringstream buffer;
    buffer << from->rdbuf();
    std::string content = buffer.str();
    node_file_version_upgrading::updateNodeFile(&content);
    from = new std::istringstream(content);

    NodeDescriptor* desc = loadDescriptor(from);
    Node* newNode = new Node(new std::string(desc->getName()), new std::string(desc->getDescription()));
    delete desc;

    int inputNumber = readInt(from);
    for(int input = 0; input < inputNumber; input++){
        std::string* inputStr = readString(from);
        std::istringstream inputStream = std::istringstream(*inputStr);
        newNode->addInput(new node_components::Input(&inputStream));
        delete inputStr;
    }
    int outputNumber = readInt(from);
    for(int output = 0; output < outputNumber; output++){
        std::string* outputStr = readString(from);
        std::istringstream outputStream = std::istringstream(*outputStr);
        newNode->addOutput(new node_components::Output(&outputStream));
        delete outputStr;
    }
    int optionNumber = readInt(from);
    for(int option = 0; option < optionNumber; option++){
        std::string* optionStr = readString(from);
        std::istringstream optionStream = std::istringstream(*optionStr);
        newNode->addOption(new node_components::Option(&optionStream));
        delete optionStr;
    }

    std::string* task = readString(from);
    int sizeMode = readInt(from);
    int xSize = readInt(from);
    int ySize = readInt(from);

    newNode->setTask(task, node_components::sizeType(sizeMode), xSize, ySize);

    std::string* runScript = readString(from);
    std::string* initScript = readString(from);

    newNode->setScript(runScript, initScript);

    int importNumber = readInt(from);
    for(int import = 0; import < importNumber; import++){
        std::string* importR = readString(from);
        newNode->addImport(*importR);
        delete importR;
    }
    delete from; // Note that from was replaced at the top, we do not delete the parameter.
    return newNode;
};

Node* node_file_handling::loadFull(NodeDescriptor* desc){
    std::ifstream* from = new std::ifstream(desc->getPath(), std::istream::binary);
    Node* newNode = loadFull(from);
    from->close();
    delete from;
    return newNode;
}

// Writing

void node_file_handling::writeInt(std::ostream* writeTo, int32_t data){
    writeTo->write((char*)(&data), sizeof(int32_t) / sizeof(char));
}

void node_file_handling::writeString(std::ostream* writeTo, std::string* data){
    std::string writing = *data;
    boost::erase_all(writing, "\r"); // windows handles just \n fine, but on Linux things sometimes break on \r\n
    writeInt(writeTo, writing.length());
    writeTo->write(writing.c_str(), writing.length());
}

void node_file_handling::writeFull(std::ostream* to, Node* toWrite){
    writeString(to, &globals::currentNodeFileVersion);

    writeString(to, toWrite->name);
    writeString(to, toWrite->description);

    std::vector<node_components::Input*> inputs = toWrite->inputs;
    writeInt(to, inputs.size());
    for(unsigned int input = 0; input < inputs.size(); input++){
        std::ostringstream inputStream;
        inputs[input]->write(&inputStream);
        std::string inputContent = inputStream.str();
        writeString(to, &inputContent); // This way we get the length of the input saved beforehand.
    }

    std::vector<node_components::Output*> outputs = toWrite->outputs;
    writeInt(to, outputs.size());
    for(unsigned int output = 0; output < outputs.size(); output++){
        std::ostringstream outputStream;
        outputs[output]->write(&outputStream);
        std::string outputContent = outputStream.str();
        writeString(to, &outputContent); // This way we get the length of the output saved beforehand.
    }

    std::vector<node_components::Option*> options = toWrite->options;
    writeInt(to, options.size());
    for(unsigned int option = 0; option < options.size(); option++){
        std::ostringstream optionStream;
        options[option]->write(&optionStream);
        std::string optionContent = optionStream.str();
        writeString(to, &optionContent); // This way we get the length of the option saved beforehand.
    }

    writeString(to, toWrite->task);
    writeInt(to, toWrite->sizeMode);
    writeInt(to, toWrite->xSize);
    writeInt(to, toWrite->ySize);

    writeString(to, toWrite->runScript);
    writeString(to, toWrite->initScript);

    std::vector<std::string> imports = toWrite->imports;
    writeInt(to, imports.size());
    for(unsigned int import = 0; import < imports.size(); import++){
        writeString(to, &imports[import]);
    }
}

void node_file_handling::writeFull(std::string path, Node* toWrite){
    std::ofstream* to = new std::ofstream(path, std::ostream::binary);
    writeFull(to, toWrite);
    to->close();
    delete to;
}
