#include "ProjectProperties.h"

#include "customDialogs/StyledTextDialog.h"
#include "customDialogs/TextFieldDialog.h"
#include "../nodes/NodeComponents.h"
#include "optionInputs/OptionInputElement.h"
#include "customDialogs/OptionTypeChoiceDialog.h"

#define SCROLL_SENSITIVITY 10

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}

ProjectProperties::ProjectProperties(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxScrolledWindow(Parent, Id, Position, Size, Style){
    sizer = new wxFlexGridSizer(4);
    SetScrollRate(0, SCROLL_SENSITIVITY); // scroll bars do not appear without specifying the rate
    reset();
}

ProjectProperties::~ProjectProperties(){
    //do not delete the sizer or its' contents, this is done automatically by wxWidgets, as they are contained in this
}

void ProjectProperties::rebuild(){
    int scrollX = 0; // for restoring position when remaking UI
    int scrollY = 0;
    CalcScrolledPosition(0, 0, &scrollX, &scrollY);
    sizer->Clear(true);
    // for the name
    {
        sizer->Add(new wxStaticText(this, -1, "Name"));
        wxButton* editingButton = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editingButton);
        editingButton->Bind(wxEVT_BUTTON, &ProjectProperties::changeName, this);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
    }
    // for answers
    {
        sizer->Add(new wxStaticText(this, -1, "Answers"));
        wxButton* editingButton = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editingButton);
        editingButton->Bind(wxEVT_BUTTON, &ProjectProperties::changeAnswers, this);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
    }
    int optionNumber = 0;
    for(node_components::Option* opt : extraOptions) {
        wxStaticText* optionName = new wxStaticText(this, -1, opt->name);
        optionName->SetToolTip(opt->description);
        sizer->Add(optionName, wxEXPAND);

        sizer->Add(opt->makeOptionElement(this)->asControl(), wxEXPAND);

        wxButton* remover = new wxButton(this, -1, "-", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(remover, wxEXPAND);
        remover->Bind(wxEVT_BUTTON, &ProjectProperties::deleteOption, this, wxID_ANY, wxID_ANY, new wxIntObject(optionNumber));

        wxButton* editor = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editor, wxEXPAND);
        editor->Bind(wxEVT_BUTTON, &ProjectProperties::editOption, this, wxID_ANY, wxID_ANY, new wxIntObject(optionNumber));

        optionNumber++;
    }
    if(mode == globals::modeDeveloper){
        wxButton* optionMaker = new wxButton(this, -1, "New option", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        optionMaker->Bind(wxEVT_BUTTON, &ProjectProperties::newOption, this);
        sizer->Add(optionMaker);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
    }

    FitInside();
    SetSizer(sizer);
    Scroll(scrollX, -scrollY / SCROLL_SENSITIVITY);
}

void ProjectProperties::reset(){
    taskName = "Task";
    answers = "Correct : 5.0P";
    for(node_components::Option* opt : extraOptions){
        delete opt;
    }
    extraOptions.clear();
    rebuild();
}

void ProjectProperties::changeName(wxCommandEvent& event){
    custom_dialogs::TextFieldDialog* dialog = new custom_dialogs::TextFieldDialog(&taskName, "Name");
    dialog->ShowModal();
    dialog->Destroy();
}

void ProjectProperties::changeAnswers(wxCommandEvent& event){
    custom_dialogs::StyledTextDialog* dialog = new custom_dialogs::StyledTextDialog(&answers, "Answers");
    dialog->ShowModal();
    dialog->Destroy();
}

void ProjectProperties::newOption(wxCommandEvent& event){
    node_components::optionType type;
    custom_dialogs::OptionTypeChoiceDialog* typeChoiceDialog = new custom_dialogs::OptionTypeChoiceDialog(&type);
    typeChoiceDialog->ShowModal();
    delete typeChoiceDialog;
    if(type != node_components::invalid){
        extraOptions.push_back(new node_components::Option("Option_" + std::to_string(extraOptions.size()), "An undefined option.", node_components::important, type));
        rebuild();
    }
}

void ProjectProperties::deleteOption(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Option* toDelete = extraOptions[number];
    extraOptions.erase(extraOptions.begin() + number);
    delete toDelete;
    rebuild();
}

void ProjectProperties::editOption(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Option* toEdit = extraOptions[number];
    custom_dialogs::OptionEditorDialog* optionEditor = new custom_dialogs::OptionEditorDialog(toEdit);
    optionEditor->ShowModal();
    delete optionEditor;
    rebuild();
}

void ProjectProperties::setMode(globals::userMode mode){
    this->mode = mode;
    rebuild();
}
