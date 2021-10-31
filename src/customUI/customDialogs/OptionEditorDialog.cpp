#include "OptionEditorDialog.h"

#include "../../nodes/optionCores/OptionCore.h"
#include "../../nodes/NodeComponents.h"

using namespace custom_dialogs;

OptionEditorDialog::OptionEditorDialog(node_components::Option* toEdit) : wxDialog(NULL, -1, "Option configuration", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER){
    editedOption = toEdit;
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);

    sizer->Add(new wxStaticText(this, -1, "Name:"));
    nameInput = new wxTextCtrl(this, -1, editedOption->name);
    sizer->Add(nameInput);

    sizer->Add(new wxStaticText(this, -1, "Option description:"));
    descriptionInput = new wxStyledTextCtrl(this, -1, wxDefaultPosition, wxSize(320, 320), 0, "");
    descriptionInput->SetText(editedOption->description);
    sizer->Add(descriptionInput);

    importanceInput = new wxCheckBox(this, -1, "Importance");
    importanceInput->SetValue(editedOption->importance == node_components::important);
    sizer->Add(importanceInput);

    sizer->Add(new wxStaticText(this, -1, "Type-Specific options:"));
    wxPanel* typeSpecPanel = (wxPanel*)toEdit->core->makeOptionEditor(this);
    typeSpecPanel->Connect(wxEVT_SIZE, wxSizeEventHandler(OptionEditorDialog::onSpecOptSize), NULL, this);
    sizer->Add(typeSpecPanel);

    wxButton* ender = new wxButton(this, -1, "OK");
    ender->Bind(wxEVT_BUTTON, &OptionEditorDialog::exitDialog, this);
    sizer->Add(ender);

    SetSizerAndFit(sizer);
}

OptionEditorDialog::~OptionEditorDialog(){

}

void OptionEditorDialog::onSpecOptSize(wxSizeEvent& event){
    Fit();
    Refresh();
}

void OptionEditorDialog::exitDialog(wxCommandEvent& event){
    editedOption->name = nameInput->GetValue();
    editedOption->description = descriptionInput->GetValue();
    editedOption->importance = importanceInput->GetValue() ? node_components::important : node_components::unimportant;
    EndModal(1);
}
