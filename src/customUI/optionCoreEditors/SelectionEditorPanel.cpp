#include "SelectionEditorPanel.h"

#include <algorithm>

#include "../StringEditorField.h"

using namespace option_editor_panels;

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}

SelectionEditorPanel::SelectionEditorPanel(option_cores::SelectionCore* core, wxWindow* parent) : OptionEditorPanel(parent){
    editedCore = core;
    choices = core->getChoices();
    sizer = new wxFlexGridSizer(2);
    rebuild();
}

SelectionEditorPanel::~SelectionEditorPanel(){
    std::string oldValue = editedCore->getValue(); // removing and adding choices may change the value
    editedCore->removeAllChoices();
    for(std::string choice : choices){
        editedCore->addChoice(choice);
    }
    if(std::find(choices.begin(), choices.end(), oldValue) == choices.end()){
        if(choices.size() == 0){
            editedCore->setValue("");
        }
        else{
            editedCore->setValue(choices[0]);
        }
    }
    else{
        editedCore->setValue(oldValue);
    }
}

void SelectionEditorPanel::rebuild(){
    sizer->Clear(true);
    wxStaticText* choiceDescriptor = new wxStaticText(this, -1, "Choices:");
    sizer->Add(choiceDescriptor);
    sizer->AddSpacer(0);
    int choiceNumber = 0;
    for(std::string choice : choices) {
        StringEditorField* fieldEditor = new StringEditorField(&choices[choiceNumber], this, NULL);
        sizer->Add(fieldEditor, wxEXPAND);

        wxButton* remover = new wxButton(this, -1, "-");
        remover->Bind(wxEVT_BUTTON, &SelectionEditorPanel::deleteChoice, this, wxID_ANY, wxID_ANY, new wxIntObject(choiceNumber));
        sizer->Add(remover, wxEXPAND);
        choiceNumber++;
    }
    wxButton* choiceMaker = new wxButton(this, -1, "New choice");
    choiceMaker->Bind(wxEVT_BUTTON, &SelectionEditorPanel::newChoice, this);
    sizer->Add(choiceMaker);

    Layout();
    SetSizerAndFit(sizer);
}

void SelectionEditorPanel::newChoice(wxCommandEvent& event){
    choices.push_back("New choice");
    rebuild();
}

void SelectionEditorPanel::deleteChoice(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    choices.erase(choices.begin() + number);
    rebuild();
}
