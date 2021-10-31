#include "StyledTextDialog.h"

using namespace custom_dialogs;

StyledTextDialog::StyledTextDialog(std::string* toEdit, std::string title, textStyles::textStyles textStyle) : wxDialog(NULL, -1, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxRESIZE_BORDER){
    editing = toEdit;
    sizer = new wxBoxSizer(wxVERTICAL);
    input = new wxStyledTextCtrl(this, -1, wxDefaultPosition, wxSize(640, 480), 0, "");
    switch(textStyle){
        case (textStyles::javascript):
            input->SetLexer(wxSTC_LEX_ESCRIPT);
            input->StyleSetForeground(wxSTC_ESCRIPT_BRACE,          wxColor(255, 100, 100));
            input->StyleSetForeground(wxSTC_ESCRIPT_COMMENT,        wxColor(100, 100, 100));
            input->StyleSetForeground(wxSTC_ESCRIPT_COMMENTDOC,     wxColor(100, 100, 100));
            input->StyleSetForeground(wxSTC_ESCRIPT_COMMENTLINE,    wxColor(100, 100, 100));
            input->StyleSetForeground(wxSTC_ESCRIPT_DEFAULT,        wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_ESCRIPT_IDENTIFIER,     wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_ESCRIPT_NUMBER,         wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_ESCRIPT_OPERATOR,       wxColor(130, 0, 130));
            input->StyleSetForeground(wxSTC_ESCRIPT_STRING,         wxColor(0, 0, 255));
            input->StyleSetForeground(wxSTC_ESCRIPT_WORD,           wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_ESCRIPT_WORD2,          wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_ESCRIPT_WORD3,          wxColor(0, 0, 0));
            break;
        case (textStyles::markdown):
            input->SetLexer(wxSTC_LEX_MARKDOWN);
            input->StyleSetForeground(wxSTC_MARKDOWN_BLOCKQUOTE,        wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_CODE,              wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_CODE2,             wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_CODEBK,            wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_DEFAULT,           wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_EM1,               wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_EM2,               wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HEADER1,           wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HEADER2,           wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HEADER3,           wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HEADER4,           wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HEADER5,           wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HEADER6,           wxColor(255, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_HRULE,             wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_LINE_BEGIN,        wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_LINK,              wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_OLIST_ITEM,        wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_PRECHAR,           wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_STRIKEOUT,         wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_STRONG1,           wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_STRONG2,           wxColor(0, 0, 0));
            input->StyleSetForeground(wxSTC_MARKDOWN_ULIST_ITEM,        wxColor(0, 0, 0));
            break;
        default:
            break;
    }

    input->SetText(*toEdit);
    ender = new wxButton(this, -1, "OK");
    ender->Bind(wxEVT_BUTTON, &StyledTextDialog::exitDialog, this);

    sizer->Add(input, 1, wxEXPAND, 0);
    sizer->Add(ender);

    SetSizerAndFit(sizer);
}

StyledTextDialog::~StyledTextDialog(){
    // delete sizer(true); // just a reminder this is not supposed to be here - the deletion is automatic when deleting the dialog due to base destructor
}

void StyledTextDialog::exitDialog(wxCommandEvent& event){
    (*editing) = input->GetText();
    EndModal(1);
}
