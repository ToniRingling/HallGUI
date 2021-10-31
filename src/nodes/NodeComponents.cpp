#include "NodeComponents.h"

#include "optionCores/TextFieldCore.h"
#include "optionCores/CheckBoxCore.h"
#include "optionCores/SelectionCore.h"
#include "optionCores/TextAreaCore.h"
#include "../customUI/optionInputs/OptionInputElement.h"

using namespace node_file_handling;
using namespace node_components;

Input::Input(std::string name, std::string description, std::string type){
    this->name = name;
    this->description = description;
    this->type = type;

    port = NULL;
}

Input::Input(std::istream* from){
    std::string* nameR = readString(from);
    std::string* descriptionR = readString(from);
    std::string* typeR = readString(from);

    name = *nameR;
    description = *descriptionR;
    type = *typeR;

    delete nameR;
    delete descriptionR;
    delete typeR;

    port = new NodePort(node_port_type::input);
}

Input::~Input(){
    if(port != NULL){
        delete port;
    }
}

void Input::write(std::ostream* where){
    writeString(where, &name);
    writeString(where, &description);
    writeString(where, &type);
}





Output::Output(std::string name, std::string description, std::string type){
    this->name = name;
    this->description = description;
    this->type = type;

    port = NULL;
}

Output::Output(std::istream* from){
    std::string* nameR = readString(from);
    std::string* descriptionR = readString(from);
    std::string* typeR = readString(from);

    name = *nameR;
    description = *descriptionR;
    type = *typeR;

    delete nameR;
    delete descriptionR;
    delete typeR;

    port = new NodePort(this);
}

Output::~Output(){
    if(port != NULL){
        delete port;
    }
}

void Output::write(std::ostream* where){
    writeString(where, &name);
    writeString(where, &description);
    writeString(where, &type);
}





Option::Option(std::string name, std::string description, optionImportance importance, optionType type){
    this->name = name;
    this->description = description;
    this->importance = importance;
    this->type = type;
    core = NULL;
    generateCore();
}

Option::Option(std::istream* from){
    std::string* nameR = readString(from);
    std::string* descriptionR = readString(from);

    name = *nameR;
    description = *descriptionR;

    delete nameR;
    delete descriptionR;

    importance = optionImportance(readInt(from));
    type = optionType(readInt(from));

    core = NULL;
    generateCore();
    core->read(from);
}

Option::~Option(){
    delete core;
}

void Option::generateCore(){
    if(core != NULL){
        delete core;
    }
    core = NULL;
    switch(type){
        case optionType::textInput:
            core = new option_cores::TextFieldCore(this);
            break;
        case optionType::textAreaInput:
            core = new option_cores::TextAreaCore(this);
            break;
        case optionType::selection:
            core = new option_cores::SelectionCore(this);
            break;
        case optionType::checkBox:
            core = new option_cores::CheckBoxCore(this);
            break;
        default:
            break;
    }
}

void Option::write(std::ostream* where){
    writeString(where, &name);
    writeString(where, &description);
    writeInt(where, importance);
    writeInt(where, type);
    core->write(where);
}

option_inputs::OptionInputElement* Option::makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination){
    return core->makeOptionElement(parentWindow, signalDestination);
}

custom_dialogs::OptionEditorDialog* Option::makeOptionEditor(){
    return new custom_dialogs::OptionEditorDialog(this);
}
