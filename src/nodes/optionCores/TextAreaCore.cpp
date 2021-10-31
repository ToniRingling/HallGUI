#include "TextAreaCore.h"

#include "../NodeFileHandling.h"
#include "../../customUI/optionCoreEditors/BlankEditorPanel.h"
#include "../../customUI/optionInputs/TextAreaOptionInput.h"

using namespace option_cores;

TextAreaCore::TextAreaCore(node_components::Option* shell) : OptionCore(shell){
    value = "";
}

TextAreaCore::~TextAreaCore(){

}

option_inputs::OptionInputElement* TextAreaCore::makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination){
    return new option_inputs::TextAreaOptionInput(shell, signalDestination, parentWindow);
}

option_editor_panels::OptionEditorPanel* TextAreaCore::makeOptionEditor(wxWindow* parentWindow){
    return new option_editor_panels::BlankEditorPanel(parentWindow);
}

std::string TextAreaCore::getValue(){
    return value;
}

void TextAreaCore::setValue(std::string to){
    value = to;
}

void TextAreaCore::write(std::ostream* where){
    using namespace node_file_handling;
    writeString(where, &value);
}

void TextAreaCore::read(std::istream* from){
    using namespace node_file_handling;
    std::string* temp = readString(from);
    value = *temp;
    delete temp;
}
