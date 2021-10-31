#include "NodeBaseDataPage.h"

#include "../customDialogs/StyledTextDialog.h"
#include "../customDialogs/TextFieldDialog.h"
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


NodeBaseDataPage::NodeBaseDataPage(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : NodePropertyPage(Parent, Id, Position, Size, Style){
    sizer = new wxFlexGridSizer(3);
    SetScrollRate(0, SCROLL_SENSITIVITY); // scroll bars do not appear without specifying the rate
    setOptions(NULL);
}

NodeBaseDataPage::~NodeBaseDataPage(){

}

void NodeBaseDataPage::setOptions(Node* node){
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
        // name and description
        showOption("Name:", "The name of the node. Does not affect output task.", node->getName(), false, false);
        sizer->AddSpacer(0);
        showOption("Description:", "The description of the node. Does not affect output task.", NULL, false, true).second->Bind(wxEVT_BUTTON, &NodeBaseDataPage::editDescription, this, wxID_ANY, wxID_ANY);
        sizer->AddSpacer(0);
        // node contents
        if(mode == globals::modeDeveloper){
            showOption("Task:", "The non-script HTML elements of the node, i.e. the parts which will be displayed.", NULL, false, true).second->Bind(wxEVT_BUTTON, &NodeBaseDataPage::editTask, this, wxID_ANY, wxID_ANY);
            sizer->AddSpacer(0);
            showOption("Run-Script:", "The script parts which will be executed, whenever this node is triggered.", NULL, false, true).second->Bind(wxEVT_BUTTON, &NodeBaseDataPage::editRunScript, this, wxID_ANY, wxID_ANY);
            sizer->AddSpacer(0);
            showOption("Init-Script:", "The script parts which will be executed, whenever the task is started.", NULL, false, true).second->Bind(wxEVT_BUTTON, &NodeBaseDataPage::editInitScript, this, wxID_ANY, wxID_ANY);
            sizer->AddSpacer(0);
        }

        wxButton* nodeSaver = new wxButton(this, -1, "Save Node");
        nodeSaver->Bind(wxEVT_BUTTON, &NodeBaseDataPage::saveNode, this);
        sizer->Add(nodeSaver);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);

        SetSizer(sizer);
        FitInside();
    }
    Scroll(scrollX, -scrollY / SCROLL_SENSITIVITY);
}

void NodeBaseDataPage::editDescription(wxCommandEvent& event){
    custom_dialogs::StyledTextDialog* descriptionEditor = new custom_dialogs::StyledTextDialog(editedNode->description, "Set description", custom_dialogs::textStyles::none);
    descriptionEditor->ShowModal();
    onImportantChange();
    delete descriptionEditor;
}

void NodeBaseDataPage::editTask(wxCommandEvent& event){
    custom_dialogs::StyledTextDialog* taskEditor = new custom_dialogs::StyledTextDialog(editedNode->task, "Set task contents", custom_dialogs::textStyles::markdown);
    taskEditor->ShowModal();
    delete taskEditor;
}

void NodeBaseDataPage::editRunScript(wxCommandEvent& event){
    custom_dialogs::StyledTextDialog* scriptEditor = new custom_dialogs::StyledTextDialog(editedNode->runScript, "Set running contents", custom_dialogs::textStyles::javascript);
    scriptEditor->ShowModal();
    delete scriptEditor;
}

void NodeBaseDataPage::editInitScript(wxCommandEvent& event){
    custom_dialogs::StyledTextDialog* scriptEditor = new custom_dialogs::StyledTextDialog(editedNode->initScript, "Set init contents", custom_dialogs::textStyles::javascript);
    scriptEditor->ShowModal();
    delete scriptEditor;
}
