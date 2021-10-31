#include "InputEditorDialog.h"

#include "../../nodes/NodeComponents.h"

using namespace custom_dialogs;

InputEditorDialog::InputEditorDialog(node_components::Input* toEdit) : wxDialog(NULL, -1, "Input configuration", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER){
    editedInput = toEdit;
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);

    sizer->Add(new wxStaticText(this, -1, "Name:"));
    nameInput = new wxTextCtrl(this, -1, editedInput->name);
    sizer->Add(nameInput);

    sizer->Add(new wxStaticText(this, -1, "Expected data type:"));
    typeInput = new wxTextCtrl(this, -1, editedInput->type);
    sizer->Add(typeInput);

    sizer->Add(new wxStaticText(this, -1, "Input description:"));
    descriptionInput = new wxStyledTextCtrl(this, -1, wxDefaultPosition, wxSize(320, 320), 0, "");
    descriptionInput->SetText(editedInput->description);
    sizer->Add(descriptionInput);

    wxButton* ender = new wxButton(this, -1, "OK");
    ender->Bind(wxEVT_BUTTON, &InputEditorDialog::exitDialog, this);
    sizer->Add(ender);

    SetSizerAndFit(sizer);
}

InputEditorDialog::~InputEditorDialog(){

}

void InputEditorDialog::exitDialog(wxCommandEvent& event){
    editedInput->name = nameInput->GetValue();
    editedInput->type = typeInput->GetValue();
    editedInput->description = descriptionInput->GetValue();
    EndModal(1);
}
