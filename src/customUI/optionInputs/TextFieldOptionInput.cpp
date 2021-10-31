#include "TextFieldOptionInput.h"

#include "../../nodes/NodeComponents.h"
#include "../../nodes/optionCores/TextFieldCore.h"

wxBEGIN_EVENT_TABLE(option_inputs::TextFieldOptionInput, wxTextCtrl)
    EVT_TEXT(wxID_ANY, option_inputs::TextFieldOptionInput::onUpdate)
wxEND_EVENT_TABLE()

using namespace option_inputs;

TextFieldOptionInput::TextFieldOptionInput(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination, wxWindow* Parent, wxWindowID Id) :  OptionInputElement(option, signalDestination), wxTextCtrl(Parent, Id){
    core = (option_cores::TextFieldCore*)option->core;
    ChangeValue(option->core->getValue());
}

TextFieldOptionInput::~TextFieldOptionInput(){

}

wxControl* TextFieldOptionInput::asControl(){
    return this;
}

void TextFieldOptionInput::onUpdate(wxCommandEvent& e){
    core->setValue(GetLineText(0).ToStdString());
    signalChange();
}
