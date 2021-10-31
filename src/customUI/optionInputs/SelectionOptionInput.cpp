#include "SelectionOptionInput.h"

#include "../../nodes/NodeComponents.h"
#include "../../nodes/optionCores/SelectionCore.h"

wxBEGIN_EVENT_TABLE(option_inputs::SelectionOptionInput, wxChoice)
    EVT_CHOICE(wxID_ANY, option_inputs::SelectionOptionInput::onUpdate)
wxEND_EVENT_TABLE()

using namespace option_inputs;

SelectionOptionInput::SelectionOptionInput(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination, wxWindow* Parent, wxWindowID Id) :  OptionInputElement(option, signalDestination), wxChoice(Parent, Id){
    core = (option_cores::SelectionCore*)option->core;
    std::vector<std::string> choices = core->getChoices();
    for(std::string choice : choices){
        Append(choice);
    }
    SetSelection(FindString(core->getValue(), true));
}

SelectionOptionInput::~SelectionOptionInput(){

}

wxControl* SelectionOptionInput::asControl(){
    return this;
}

void SelectionOptionInput::onUpdate(wxCommandEvent& e){
    core->setValue(std::string(GetString(GetSelection()).mb_str()));
    signalChange();
}
