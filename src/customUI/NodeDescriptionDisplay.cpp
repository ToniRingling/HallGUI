#include "NodeDescriptionDisplay.h"

#define SCROLL_SENSITIVITY 10

wxBEGIN_EVENT_TABLE(NodeDescriptionDisplay, wxPanel)
    EVT_SIZE(NodeDescriptionDisplay::OnResize)
wxEND_EVENT_TABLE()

NodeDescriptionDisplay::NodeDescriptionDisplay(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxPanel(Parent, Id, Position, Size, Style){

    sizer = new wxFlexGridSizer(2, 1, 0, 0);

    SetSizerAndFit(sizer);

    nodeNameText = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    nodeDescriptionText = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    sizer->Add(nodeNameText, wxALL | wxEXPAND);
    sizer->Add(nodeDescriptionText, wxALL | wxEXPAND);

    SetSizerAndFit(sizer);
    Layout();
    SetSizerAndFit(sizer);
}

void NodeDescriptionDisplay::setContent(std::string name, std::string description){
    nodeNameText->SetValue(name);
    nodeDescriptionText->SetValue(description);
}


void NodeDescriptionDisplay::OnResize(wxSizeEvent& e){
    if(e.GetSize().GetX() != 0){ // There is an initial resize with both sizes 0 before everything is properly set up.
        nodeNameText->SetSize(wxSize(e.GetSize().GetX(), nodeNameText->GetSize().GetY()));
        nodeDescriptionText->SetSize(wxSize(e.GetSize().GetX(), e.GetSize().GetY() - nodeNameText->GetSize().GetY()));
    }
}
