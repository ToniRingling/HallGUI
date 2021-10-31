#include "NodeOutputPage.h"

#include "../customDialogs/StyledTextDialog.h"
#include "../customDialogs/TextFieldDialog.h"
#include "../customDialogs/OutputEditorDialog.h"
#include "../optionInputs/OptionInputElement.h"
#include "../RewrappableText.h"
#include "../../nodes/Node.h"

#define SCROLL_SENSITIVITY 10

using namespace option_inputs;
using namespace node_property_pages;

wxBEGIN_EVENT_TABLE(NodeOutputPage, wxScrolledWindow)
    EVT_SIZE(NodeOutputPage::onResize)
wxEND_EVENT_TABLE()

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}


NodeOutputPage::NodeOutputPage(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : NodePropertyPage(Parent, Id, Position, Size, Style){
    sizer = new wxFlexGridSizer(3);
    SetScrollRate(0, SCROLL_SENSITIVITY); // scroll bars do not appear without specifying the rate
    setOptions(NULL);
}

NodeOutputPage::~NodeOutputPage(){

}

void NodeOutputPage::setOptions(Node* node){
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
        int outputNumber = 0;
        for(node_components::Output* out : node->outputs){
            if(mode == globals::modeUser){
                showOption(out->name, out->description, NULL, false, false);
                sizer->Add(new wxStaticText(this, -1, " Type:"));
                {
                    wxStaticText* typeText = new wxStaticText(this, -1, out->type);
                    sizer->Add(typeText);
                }
                sizer->AddSpacer(0);
                sizer->Add(new wxStaticText(this, -1, " Description:"));
                RewrappableText* descText = new RewrappableText(this, -1, out->description);
                lastColumnWrappables.push_back(descText);
                sizer->Add(descText);
            }
            else if(mode == globals::modeDeveloper){
                std::pair<wxButton*, wxButton*> buttons = showOption(out->name, out->description);
                buttons.first->Bind(wxEVT_BUTTON, &NodeOutputPage::deleteOutput, this, wxID_ANY, wxID_ANY, new wxIntObject(outputNumber));
                buttons.second->Bind(wxEVT_BUTTON, &NodeOutputPage::editOutput, this, wxID_ANY, wxID_ANY, new wxIntObject(outputNumber));
                outputNumber++;
            }
        }
        if(mode == globals::modeDeveloper){
            wxButton* outputMaker = new wxButton(this, -1, "New Output");
            outputMaker->Bind(wxEVT_BUTTON, &NodeOutputPage::newOutput, this);
            sizer->Add(outputMaker);
            sizer->AddSpacer(0);
            sizer->AddSpacer(0);
        }

        wxButton* nodeSaver = new wxButton(this, -1, "Save Node");
        nodeSaver->Bind(wxEVT_BUTTON, &NodeOutputPage::saveNode, this);
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

void NodeOutputPage::newOutput(wxCommandEvent& event){
    editedNode->addOutput(new node_components::Output("Output_" + std::to_string(editedNode->outputs.size()), "An undefined output.", "any"));
    setOptions(editedNode);
    onImportantChange();
}

void NodeOutputPage::deleteOutput(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Output* toDelete = editedNode->outputs[number];
    editedNode->removeOutput(toDelete);
    editedNode->forceRefresh();
    setOptions(editedNode);
    onImportantChange();
}

void NodeOutputPage::editOutput(wxCommandEvent& event){
    int number = ((wxIntObject*)(event.GetEventUserData()))->number;
    node_components::Output* toEdit = editedNode->outputs[number];
    custom_dialogs::OutputEditorDialog* outputEditor = new custom_dialogs::OutputEditorDialog(toEdit);
    outputEditor->ShowModal();
    setOptions(editedNode);
    onImportantChange();
}

void NodeOutputPage::onResize(wxSizeEvent& e){
    int lastColSize = this->GetSize().GetWidth();
    for(unsigned int col = 0; col + 1 < sizer->GetColWidths().size(); col++){ // +1 on the left because right size is unsigned will underflow
        lastColSize -= sizer->GetColWidths()[col];
    }
    for(RewrappableText* text : lastColumnWrappables){
            text->Wrap(lastColSize);
    }
}
