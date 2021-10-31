#include "TextAreaOptionInput.h"

#include "../../nodes/NodeComponents.h"
#include "../../nodes/optionCores/TextAreaCore.h"

wxBEGIN_EVENT_TABLE(option_inputs::TextAreaOptionInput, wxStyledTextCtrl)
    EVT_STC_CHANGE(wxID_ANY, option_inputs::TextAreaOptionInput::onUpdate)
wxEND_EVENT_TABLE()

using namespace option_inputs;

TextAreaOptionInput::TextAreaOptionInput(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination, wxWindow* Parent, wxWindowID Id) :  OptionInputElement(option, signalDestination), wxStyledTextCtrl(Parent, Id){
    core = (option_cores::TextAreaCore*)option->core;
    SetText(option->core->getValue());
}

TextAreaOptionInput::~TextAreaOptionInput(){

}

wxControl* TextAreaOptionInput::asControl(){
    return this;
}

void TextAreaOptionInput::onUpdate(wxStyledTextEvent& e){
    core->setValue(std::string(GetText().mb_str()));
    signalChange();
}
