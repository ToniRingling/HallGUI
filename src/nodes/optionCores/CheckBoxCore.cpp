#include "CheckBoxCore.h"

#include "../NodeFileHandling.h"
#include "../../customUI/optionCoreEditors/BlankEditorPanel.h"
#include "../../customUI/optionInputs/CheckBoxOptionInput.h"

using namespace option_cores;

CheckBoxCore::CheckBoxCore(node_components::Option* shell) : OptionCore(shell){
    value = false;
}

CheckBoxCore::~CheckBoxCore(){

}

option_inputs::OptionInputElement* CheckBoxCore::makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination){
    return new option_inputs::CheckBoxOptionInput(shell, signalDestination, parentWindow);
}

option_editor_panels::OptionEditorPanel* CheckBoxCore::makeOptionEditor(wxWindow* parentWindow){
    return new option_editor_panels::BlankEditorPanel(parentWindow);
}

std::string CheckBoxCore::getValue(){
    return (value ? "true" : "false");
}

void CheckBoxCore::setValue(bool to){
    value = to;
}

void CheckBoxCore::write(std::ostream* where){
    using namespace node_file_handling;
    writeInt(where, value ? 1 : 0);
}

void CheckBoxCore::read(std::istream* from){
    using namespace node_file_handling;
    value = (readInt(from) == 1);
}
