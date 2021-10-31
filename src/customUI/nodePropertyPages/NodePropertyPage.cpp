#include "NodeBaseDataPage.h"

#include "../GraphEditor.h"
#include "../../nodes/Node.h"
#include "../StringEditorField.h"

using namespace option_inputs;
using namespace node_property_pages;

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}

NodePropertyPage::NodePropertyPage(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxScrolledWindow(Parent, Id, Position, Size, Style){

}

NodePropertyPage::~NodePropertyPage(){
    //delete sizer; //automatically done on window deletion, do not uncomment
}

std::pair<wxButton*, wxButton*> NodePropertyPage::showOption(std::string name, std::string description, std::string* forDirectInput, bool withDeletion, bool withEdit){
    wxStaticText* optionName = new wxStaticText(this, -1, name);
    optionName->SetToolTip(description);
    sizer->Add(optionName, wxEXPAND);

    if(forDirectInput != NULL){
        StringEditorField* fieldEditor = new StringEditorField(forDirectInput, this, this);
        sizer->Add(fieldEditor, wxEXPAND);
    }

    wxButton* remover = NULL;
    if(withDeletion){
        remover = new wxButton(this, -1, "-", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(remover, wxEXPAND);
    }

    wxButton* editor = NULL;
    if(withEdit){
        editor = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editor, wxEXPAND);
    }

    return std::pair<wxButton*, wxButton*>(remover, editor);
}

void NodePropertyPage::setGraphEditor(GraphEditor* editor){
    graphEditor = editor;
}

void NodePropertyPage::onImportantChange(){
    graphEditor->onNodeChange(editedNode);
}

void NodePropertyPage::setMode(globals::userMode mode){
    this->mode = mode;
    setOptions(editedNode);
}

void NodePropertyPage::saveNode(wxCommandEvent& event){
    wxFileDialog* savePathDialog = new wxFileDialog(this, "Save Node", wxEmptyString, wxEmptyString, "*.hgn", wxFD_SAVE |wxFD_OVERWRITE_PROMPT);
    if(savePathDialog->ShowModal() != wxID_CANCEL){
        node_file_handling::writeFull(savePathDialog->GetPath().ToStdString(), editedNode);
    }
    delete savePathDialog;
}
