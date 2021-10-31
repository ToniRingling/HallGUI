#include "TextFieldDialog.h"

using namespace custom_dialogs;

TextFieldDialog::TextFieldDialog(std::string* toEdit, std::string title) : wxDialog(NULL, -1, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER){
    editing = toEdit;
    sizer = new wxBoxSizer(wxVERTICAL);
    input = new wxTextCtrl(this, -1, *toEdit, wxDefaultPosition, wxSize(640, -1), 0);
    ender = new wxButton(this, -1, "OK");
    ender->Bind(wxEVT_BUTTON, &TextFieldDialog::exitDialog, this);

    sizer->Add(input, 1, wxEXPAND, 0);
    sizer->Add(ender);

    SetSizerAndFit(sizer);
}

TextFieldDialog::~TextFieldDialog(){
    // delete sizer(true); // just a reminder this is not supposed to be here - the deletion is automatic when deleting the dialog due to base destructor
}

void TextFieldDialog::exitDialog(wxCommandEvent& event){
    (*editing) = input->GetValue();
    EndModal(1);
}
