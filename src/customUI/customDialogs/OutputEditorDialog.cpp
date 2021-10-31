#include "OutputEditorDialog.h"

#include "../../nodes/NodeComponents.h"

using namespace custom_dialogs;

OutputEditorDialog::OutputEditorDialog(node_components::Output* toEdit) : wxDialog(NULL, -1, "Output configuration", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER){
    editedOutput = toEdit;
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);

    sizer->Add(new wxStaticText(this, -1, "Name:"));
    nameInput = new wxTextCtrl(this, -1, editedOutput->name);
    sizer->Add(nameInput);

    sizer->Add(new wxStaticText(this, -1, "Expected data type:"));
    typeInput = new wxTextCtrl(this, -1, editedOutput->type);
    sizer->Add(typeInput);

    sizer->Add(new wxStaticText(this, -1, "Output description:"));
    descriptionInput = new wxStyledTextCtrl(this, -1, wxDefaultPosition, wxSize(320, 320), 0, "");
    descriptionInput->SetText(editedOutput->description);
    sizer->Add(descriptionInput);

    wxButton* ender = new wxButton(this, -1, "OK");
    ender->Bind(wxEVT_BUTTON, &OutputEditorDialog::exitDialog, this);
    sizer->Add(ender);

    SetSizerAndFit(sizer);
}

OutputEditorDialog::~OutputEditorDialog(){

}

void OutputEditorDialog::exitDialog(wxCommandEvent& event){
    editedOutput->name = nameInput->GetValue();
    editedOutput->type = typeInput->GetValue();
    editedOutput->description = descriptionInput->GetValue();
    EndModal(1);
}

