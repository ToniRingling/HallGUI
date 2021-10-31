#include "StringEditorField.h"

#include "nodePropertyPages/NodePropertyPage.h"

wxBEGIN_EVENT_TABLE(StringEditorField, wxTextCtrl)
    EVT_TEXT(wxID_ANY, StringEditorField::onUpdate)
wxEND_EVENT_TABLE()

StringEditorField::StringEditorField(std::string* toEdit, wxWindow* Parent, node_property_pages::NodePropertyPage* signalDestination) :  wxTextCtrl(Parent, -1){
    editing = toEdit;
    this->signalDestination = signalDestination;
    ChangeValue(*editing);
}

StringEditorField::~StringEditorField(){

}

void StringEditorField::onUpdate(wxCommandEvent& e){
    *editing = GetLineText(0).ToStdString();
    if(signalDestination != NULL){
        signalDestination->onImportantChange();
    }
}
