#include "OptionTypeChoiceDialog.h"

#include "../../nodes/NodeComponents.h"

using namespace custom_dialogs;

namespace {
    class wxOptionTypeObject : public wxObject{
    public:
        node_components::optionType type;
        wxOptionTypeObject(node_components::optionType tp) : wxObject(){type = tp;}
    };

    // The following are the types, names and descriptions for all cores. The order should be the same in all 3.
    static const node_components::optionType types[] = {node_components::textInput, node_components::textAreaInput, node_components::selection, node_components::checkBox};
    static const std::string names[] = {"Text field", "Text Area", "Selection", "Checkbox"};
    static const std::string descriptions[] = {
    "This type of option allows a single line of text input.",
    "This type of option allows multiple lines of text input.",
    "This type of option allows the selection of some number of predefined inputs.",
    "This type of option allows setting it as 'false' or 'true' via a checkbox."
    };
}

OptionTypeChoiceDialog::OptionTypeChoiceDialog(node_components::optionType* toSet) : wxDialog(NULL, -1, "Option type choice", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER){

    editedType = toSet;
    *editedType = node_components::invalid;
    wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

    for(unsigned int typeNumber = 0; typeNumber < (sizeof(types) / sizeof(node_components::optionType)); typeNumber++){
        sizer->Add(new wxStaticText(this, -1, names[typeNumber]));
        sizer->Add(new wxStaticText(this, -1, descriptions[typeNumber]));
        wxButton* selector = new wxButton(this, -1, "Select");
        selector->Bind(wxEVT_BUTTON, &OptionTypeChoiceDialog::chooseType, this, wxID_ANY, wxID_ANY, new wxOptionTypeObject(types[typeNumber]));
        sizer->Add(selector);
    }

    wxButton* ender = new wxButton(this, -1, "Cancel");
    ender->Bind(wxEVT_BUTTON, &OptionTypeChoiceDialog::exitDialog, this);
    sizer->Add(ender);

    SetSizerAndFit(sizer);
}

OptionTypeChoiceDialog::~OptionTypeChoiceDialog(){

}

void OptionTypeChoiceDialog::chooseType(wxCommandEvent& event){
    *editedType = ((wxOptionTypeObject*)(event.GetEventUserData()))->type;
    exitDialog(event);
}

void OptionTypeChoiceDialog::exitDialog(wxCommandEvent& event){
    EndModal(1);
}

