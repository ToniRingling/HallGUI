#include "NodeOptions.h"

#include "customDialogs/StyledTextDialog.h"
#include "customDialogs/TextFieldDialog.h"
#include "NodeDescriptionDisplay.h"
#include "GraphEditor.h"
#include "../nodes/Node.h"
#include "nodePropertyPages/NodeBaseDataPage.h"
#include "nodePropertyPages/NodeOptionPage.h"
#include "nodePropertyPages/NodeInputPage.h"
#include "nodePropertyPages/NodeOutputPage.h"

#define SCROLL_SENSITIVITY 10

using namespace option_inputs;

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}


NodeOptions::NodeOptions(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxNotebook(Parent, Id, Position, Size, Style){
    NodeBaseDataPage* baseData = new NodeBaseDataPage(this, -1, wxDefaultPosition, wxDefaultSize, 0);
    pages.push_back(baseData);
    InsertPage(0, baseData, "Base", true);
    NodeOptionPage* options = new NodeOptionPage(this, -1, wxDefaultPosition, wxDefaultSize, 0);
    pages.push_back(options);
    InsertPage(1, options, "Options", false);
    NodeInputPage* inputs = new NodeInputPage(this, -1, wxDefaultPosition, wxDefaultSize, 0);
    pages.push_back(inputs);
    InsertPage(2, inputs, "Inputs", false);
    NodeOutputPage* outputs = new NodeOutputPage(this, -1, wxDefaultPosition, wxDefaultSize, 0);
    pages.push_back(outputs);
    InsertPage(3, outputs, "Outputs", false);
}

NodeOptions::~NodeOptions(){
    //delete sizer; //automatically done on window deletion, do not uncomment
}

void NodeOptions::setOptions(Node* node){
    editedNode = node;
    for(NodePropertyPage* page : pages){
        page->setOptions(node);
    }
}

void NodeOptions::setGraphEditor(GraphEditor* editor){
    graphEditor = editor;
    for(NodePropertyPage* page : pages){
        page->setGraphEditor(editor);
    }
}

void NodeOptions::setMode(globals::userMode mode){
    for(NodePropertyPage* page : pages){
        page->setMode(mode);
    }
}
