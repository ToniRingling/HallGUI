#include "NewNodeSearch.h"

#include "NewNodeSelection.h"

wxBEGIN_EVENT_TABLE(NewNodeSearch, wxSearchCtrl)
    EVT_SEARCHCTRL_SEARCH_BTN(wxID_ANY, NewNodeSearch::OnSearch)
wxEND_EVENT_TABLE()

NewNodeSearch::NewNodeSearch(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxSearchCtrl(Parent, Id, wxEmptyString, Position, Size, Style){
    toSearch = NULL;
    SetDescriptiveText("Search node prefabs...");
}

void NewNodeSearch::OnSearch(wxCommandEvent& evt){
    toSearch->setFilter(evt.GetString().mb_str().data());
}

void NewNodeSearch::registerTreeDisplay(NewNodeSelection* toRegister){
    toSearch = toRegister;
}
