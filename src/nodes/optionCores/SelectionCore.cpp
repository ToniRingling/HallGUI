#include "SelectionCore.h"

#include "../NodeFileHandling.h"
#include "../../customUI/optionCoreEditors/SelectionEditorPanel.h"
#include "../../customUI/optionInputs/SelectionOptionInput.h"

using namespace option_cores;

SelectionCore::SelectionCore(node_components::Option* shell) : OptionCore(shell){
    value = "";
}

SelectionCore::~SelectionCore(){

}

option_inputs::OptionInputElement* SelectionCore::makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination){
    return new option_inputs::SelectionOptionInput(shell, signalDestination, parentWindow);
}

option_editor_panels::OptionEditorPanel* SelectionCore::makeOptionEditor(wxWindow* parentWindow){
    return new option_editor_panels::SelectionEditorPanel(this, parentWindow);
}

void SelectionCore::addChoice(std::string choice){
    choices.push_back(choice);
    if(choices.size()==1){
        value = choice;
    }
}

void SelectionCore::removeAllChoices(){
    choices.clear();
}

std::string SelectionCore::getValue(){
    return value;
}

std::vector<std::string> SelectionCore::getChoices(){
    return choices;
}

void SelectionCore::setValue(std::string to){
    value = to;
}

void SelectionCore::write(std::ostream* where){
    using namespace node_file_handling;
    writeInt(where, choices.size());
    for(std::string choice : choices){
        writeString(where, &choice);
    }
    writeString(where, &value);
}

void SelectionCore::read(std::istream* from){
    using namespace node_file_handling;
    int choiceNumber = readInt(from);
    for(int a = 0; a < choiceNumber; a++){
        std::string* temp = readString(from);
        choices.push_back(*temp);
        delete temp;
    }
    std::string* temp = readString(from);
    value = *temp;
    delete temp;
}
