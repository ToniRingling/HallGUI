#include "CheckBoxOptionInput.h"

#include "../../nodes/NodeComponents.h"
#include "../../nodes/optionCores/CheckBoxCore.h"

wxBEGIN_EVENT_TABLE(option_inputs::CheckBoxOptionInput, wxCheckBox)
    EVT_CHECKBOX(wxID_ANY, option_inputs::CheckBoxOptionInput::onUpdate)
wxEND_EVENT_TABLE()

using namespace option_inputs;

CheckBoxOptionInput::CheckBoxOptionInput(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination, wxWindow* Parent, wxWindowID Id) :  OptionInputElement(option, signalDestination), wxCheckBox(Parent, Id, ""){
    core = (option_cores::CheckBoxCore*)option->core;
    SetValue(core->getValue() == "true");
}

CheckBoxOptionInput::~CheckBoxOptionInput(){

}

wxControl* CheckBoxOptionInput::asControl(){
    return this;
}

void CheckBoxOptionInput::onUpdate(wxCommandEvent& e){
    core->setValue(GetValue());
    signalChange();
}

