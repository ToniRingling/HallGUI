#include "TextFieldCore.h"

#include "../NodeFileHandling.h"
#include "../../customUI/optionCoreEditors/BlankEditorPanel.h"
#include "../../customUI/optionInputs/TextFieldOptionInput.h"

using namespace option_cores;

TextFieldCore::TextFieldCore(node_components::Option* shell) : OptionCore(shell){
    value = "";
}

TextFieldCore::~TextFieldCore(){

}

option_inputs::OptionInputElement* TextFieldCore::makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination){
    return new option_inputs::TextFieldOptionInput(shell, signalDestination, parentWindow);
}

option_editor_panels::OptionEditorPanel* TextFieldCore::makeOptionEditor(wxWindow* parentWindow){
    return new option_editor_panels::BlankEditorPanel(parentWindow);
}

std::string TextFieldCore::getValue(){
    return value;
}

void TextFieldCore::setValue(std::string to){
    value = to;
}

void TextFieldCore::write(std::ostream* where){
    using namespace node_file_handling;
    writeString(where, &value);
}

void TextFieldCore::read(std::istream* from){
    using namespace node_file_handling;
    std::string* temp = readString(from);
    value = *temp;
    delete temp;
}
