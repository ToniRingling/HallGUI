#include "RewrappableText.h"

#include <iostream>

RewrappableText::RewrappableText(wxWindow* Parent, wxWindowID Id, std::string Label, const wxPoint& Position, const wxSize& Size, long Style) : wxStaticText(Parent, Id, Label, Position, Size, Style){
    fullLabel = this->GetLabel();
}

void RewrappableText::Wrap(int width){
    this->SetLabel(fullLabel);
    ((wxStaticText*)this)->Wrap(width);
}
