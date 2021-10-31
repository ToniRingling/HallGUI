#include "NodeOptionPage.h"

#include "../customDialogs/StyledTextDialog.h"
#include "../customDialogs/TextFieldDialog.h"
#include "../customDialogs/OptionEditorDialog.h"
#include "../customDialogs/OptionTypeChoiceDialog.h"
#include "../optionInputs/OptionInputElement.h"
#include "../../nodes/Node.h"

#define SCROLL_SENSITIVITY 10

using namespace option_inputs;
using namespace node_property_pages;

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}


NodeOptionPage::NodeOptionPage(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : NodePropertyPage(Parent, Id, Position, Size, Style){
    sizer = new wxFlexGridSizer(4);
    SetScrollRate(0, SCROLL_SENSITIVITY); // scroll bars do not appear without specifying the rate
    setOptions(NULL);
}

NodeOptionPage::~NodeOptionPage(){

}

void NodeOptionPage::setOptions(Node* node){
    int scrollX = 0; // for restoring position when remaking UI for the same node
    int scrollY = 0;
    if(node == editedNode){
        CalcScrolledPosition(0, 0, &scrollX, &scrollY);
    }
    editedNode = node;
    sizer->Clear(true); // actually deletes all subsizers and their contents
    if(node==NULL){
        return;
    }
    else{
        int optionNumber = 0;
        for(node_components::Option* opt : node->getOptions()){
            if(mode == globals::modeUser){
                showOption(opt, false);
                sizer->AddSpacer(0);
                sizer->AddSpacer(0);
            }
            else if(mode == globals::modeDeveloper){
                std::pair<wxButton*, wxButton*> buttons = showOption(opt, true);
                buttons.first->Bind(wxEVT_BUTTON, &NodeOptionPage::deleteOption, this, wxID_ANY, wxID_ANY, new wxIntObject(optionNumber));
                buttons.second->Bind(wxEVT_BUTTON, &NodeOptionPage::editOption, this, wxID_ANY, wxID_ANY, new wxIntObject(optionNumber));
            }
            optionNumber++;
        }

        if(mode == globals::modeDeveloper){
            wxButton* optionMaker = new wxButton(this, -1, "New option");
            optionMaker->Bind(wxEVT_BUTTON, &NodeOptionPage::newOption, this);
            sizer->Add(optionMaker);
            sizer->AddSpacer(0);
            sizer->AddSpacer(0);
            sizer->AddSpacer(0);
        }

        wxButton* nodeSaver = new wxButton(this, -1, "Save Node");
        nodeSaver->Bind(wxEVT_BUTTON, &NodeOptionPage::saveNode, this);
        sizer->Add(nodeSaver);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);

        SetSizer(sizer);
        FitInside();
    }
    Scroll(scrollX, -scrollY / SCROLL_SENSITIVITY);
}

std::pair<wxButton*, wxButton*> NodeOptionPage::showOption(node_components::Option* opt, bool withButtons){
    wxStaticText* optionName = new wxStaticText(this, -1, opt->name);
    optionName->SetToolTip(opt->description);
    sizer->Add(optionName, wxEXPAND);

    sizer->Add(opt->makeOptionElement(this, this)->asControl(), wxEXPAND);

    wxButton* remover = NULL;
    wxButton* editor = NULL;

    if(withButtons){
        remover = new wxButton(this, -1, "-", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(remover, wxEXPAND);

        editor = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editor, wxEXPAND);
    }

    return std::pair<wxButton*, wxButton*>(remover, editor);
}

void NodeOptionPage::newOption(wxCommandEvent& event){
    node_components::optionType type;
    custom_dialogs::OptionTypeChoiceDialog* typeChoiceDialog = new custom_dialogs::OptionTypeChoiceDialog(&type);
    typeChoiceDialog->ShowModal();
    delete typeChoiceDialog;
    if(type != node_components::invalid){
        editedNode->addOption(new node_components::Option("Option_" + std::to_string(editedNode->options.size()), "An undefined option.", node_components::important, type));
        setOptions(editedNode);
        onImportantChange();
    }
}

void NodeOptionPage::deleteOption(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Option* toDelete = editedNode->options[number];
    editedNode->options.erase(editedNode->options.begin() + number);
    editedNode->forceRefresh();
    delete toDelete;
    setOptions(editedNode);
    onImportantChange();
}

void NodeOptionPage::editOption(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Option* toEdit = editedNode->options[number];
    custom_dialogs::OptionEditorDialog* optionEditor = new custom_dialogs::OptionEditorDialog(toEdit);
    optionEditor->ShowModal();
    delete optionEditor;
    setOptions(editedNode);
    onImportantChange();
}
