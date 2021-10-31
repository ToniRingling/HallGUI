#include "NodeInputPage.h"

#include "../customDialogs/StyledTextDialog.h"
#include "../customDialogs/TextFieldDialog.h"
#include "../customDialogs/InputEditorDialog.h"
#include "../optionInputs/OptionInputElement.h"
#include "../RewrappableText.h"
#include "../../nodes/Node.h"

#define SCROLL_SENSITIVITY 10

using namespace option_inputs;
using namespace node_property_pages;

wxBEGIN_EVENT_TABLE(NodeInputPage, wxScrolledWindow)
    EVT_SIZE(NodeInputPage::onResize)
wxEND_EVENT_TABLE()

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}


NodeInputPage::NodeInputPage(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : NodePropertyPage(Parent, Id, Position, Size, Style){
    sizer = new wxFlexGridSizer(3);
    SetScrollRate(0, SCROLL_SENSITIVITY); // scroll bars do not appear without specifying the rate
    setOptions(NULL);
}

NodeInputPage::~NodeInputPage(){

}

void NodeInputPage::setOptions(Node* node){
    int scrollX = 0; // for restoring position when remaking UI for the same node
    int scrollY = 0;
    lastColumnWrappables.clear();
    if(node == editedNode){
        CalcScrolledPosition(0, 0, &scrollX, &scrollY);
    }
    editedNode = node;
    sizer->Clear(true); // actually deletes all subsizers and their contents
    if(node==NULL){
        return;
    }
    else{
        int inputNumber = 0;
        for(node_components::Input* inp : node->inputs){
            if(mode == globals::modeUser){
                showOption(inp->name, inp->description, NULL, false, false);
                sizer->Add(new wxStaticText(this, -1, " Type:"));
                {
                    wxStaticText* typeText = new wxStaticText(this, -1, inp->type);
                    sizer->Add(typeText);
                }
                sizer->AddSpacer(0);
                sizer->Add(new wxStaticText(this, -1, " Description:"));
                RewrappableText* descText = new RewrappableText(this, -1, inp->description);
                lastColumnWrappables.push_back(descText);
                sizer->Add(descText);
            }
            else if(mode == globals::modeDeveloper){
                std::pair<wxButton*, wxButton*> buttons = showOption(inp->name, inp->description);
                buttons.first->Bind(wxEVT_BUTTON, &NodeInputPage::deleteInput, this, wxID_ANY, wxID_ANY, new wxIntObject(inputNumber));
                buttons.second->Bind(wxEVT_BUTTON, &NodeInputPage::editInput, this, wxID_ANY, wxID_ANY, new wxIntObject(inputNumber));
            }
            inputNumber++;
        }
        if(mode == globals::modeDeveloper){
            wxButton* inputMaker = new wxButton(this, -1, "New input");
            inputMaker->Bind(wxEVT_BUTTON, &NodeInputPage::newInput, this);
            sizer->Add(inputMaker);
            sizer->AddSpacer(0);
            sizer->AddSpacer(0);
        }

        wxButton* nodeSaver = new wxButton(this, -1, "Save Node");
        nodeSaver->Bind(wxEVT_BUTTON, &NodeInputPage::saveNode, this);
        sizer->Add(nodeSaver);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);

        SetSizer(sizer);
        FitInside();
        wxSizeEvent fakeResize(this->GetSize());
        onResize(fakeResize);
    }
    Scroll(scrollX, -scrollY / SCROLL_SENSITIVITY);
}

void NodeInputPage::newInput(wxCommandEvent& event){
    editedNode->addInput(new node_components::Input("Input_" + std::to_string(editedNode->inputs.size()), "An undefined input.", "any"));
    setOptions(editedNode);
    onImportantChange();
}

void NodeInputPage::deleteInput(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Input* toDelete = editedNode->inputs[number];
    editedNode->removeInput(toDelete);
    editedNode->forceRefresh();
    setOptions(editedNode);
    onImportantChange();
}

void NodeInputPage::editInput(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Input* toEdit = editedNode->inputs[number];
    custom_dialogs::InputEditorDialog* inputEditor = new custom_dialogs::InputEditorDialog(toEdit);
    inputEditor->ShowModal();
    setOptions(editedNode);
    onImportantChange();
}

void NodeInputPage::onResize(wxSizeEvent& e){
    int lastColSize = this->GetSize().GetWidth();
    for(unsigned int col = 0; col + 1 < sizer->GetColWidths().size(); col++){ // +1 on the left because right size is unsigned will underflow
        lastColSize -= sizer->GetColWidths()[col];
    }
    for(RewrappableText* text : lastColumnWrappables){
            text->Wrap(lastColSize);
    }
}
