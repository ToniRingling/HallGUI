#include "LogField.h"

wxBEGIN_EVENT_TABLE(LogField, wxListCtrl)
#ifdef MSWHALLGUI
    EVT_SIZE(LogField::OnResize)
#endif
wxEND_EVENT_TABLE()

LogField::LogField(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxListCtrl(Parent, Id, Position, Size, wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES){
    reset();
}

void LogField::reset(){
    ClearAll();
    contentCount = 0;
    InsertColumn(0, wxT("Type"), wxLIST_FORMAT_LEFT);
    InsertColumn(1, wxT("Message"), wxLIST_FORMAT_LEFT);
    wxSizeEvent fakeResize(this->GetSize()); // needed to appropriately size columns on reset
    OnResize(fakeResize);
}

void LogField::log(LogMessageTypes::LogMessageTypes type, std::string message){
    switch(type){
    case(LogMessageTypes::regular):
        InsertItem(contentCount, "Message");
        SetItemTextColour(contentCount, wxColor(0, 0, 0));
        break;
    case(LogMessageTypes::warning):
        InsertItem(contentCount, "Warning");
        SetItemTextColour(contentCount, wxColor(255, 100, 0));
        break;
    case(LogMessageTypes::error):
        InsertItem(contentCount, "Error");
        SetItemTextColour(contentCount, wxColor(255, 0, 0));
        break;
    case(LogMessageTypes::success):
        InsertItem(contentCount, "Success");
        SetItemTextColour(contentCount, wxColor(0, 200, 0));
        break;
    default:
        InsertItem(contentCount, "Defective Type(Error in HallGUI)");
    }
    SetItem(contentCount, 1, message);
    contentCount++;
}

void LogField::OnResize(wxSizeEvent& e){
    int totalX = e.GetSize().GetX();
    int typeX = (totalX * 15) / 100;
    int messageX = totalX - typeX;
    SetColumnWidth(0, typeX);
    SetColumnWidth(1, messageX);
}
