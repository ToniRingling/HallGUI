#include "ProjectProperties.h"

#include "customDialogs/StyledTextDialog.h"
#include "customDialogs/TextFieldDialog.h"
#include "../nodes/NodeComponents.h"
#include "optionInputs/OptionInputElement.h"
#include "customDialogs/OptionTypeChoiceDialog.h"

#define SCROLL_SENSITIVITY 10

namespace {
    class wxIntObject : public wxObject{
    public:
        wxIntObject(int num) : wxObject(){number = num;}
        int number;
    };
}

ProjectProperties::ProjectProperties(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxScrolledWindow(Parent, Id, Position, Size, Style){
    sizer = new wxFlexGridSizer(4);
    SetScrollRate(0, SCROLL_SENSITIVITY); // scroll bars do not appear without specifying the rate
    reset();
}

ProjectProperties::~ProjectProperties(){
    //do not delete the sizer or its' contents, this is done automatically by wxWidgets, as they are contained in this
}

void ProjectProperties::rebuild(){
    int scrollX = 0; // for restoring position when remaking UI
    int scrollY = 0;
    CalcScrolledPosition(0, 0, &scrollX, &scrollY);
    sizer->Clear(true);
    // for the name
    {
        sizer->Add(new wxStaticText(this, -1, "Name"));
        wxButton* editingButton = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editingButton);
        editingButton->Bind(wxEVT_BUTTON, &ProjectProperties::changeName, this);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
    }
    // for answers
    {
        sizer->Add(new wxStaticText(this, -1, "Answers"));
        wxButton* editingButton = new wxButton(this, -1, "Edit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
        sizer->Add(editingButton);
        editingButton->Bind(wxEVT_BUTTON, &ProjectProperties::changeAnswers, this);
        sizer->AddSpacer(0);
        sizer->AddSpacer(0);
    }

    FitInside();
    SetSizer(sizer);
    Scroll(scrollX, -scrollY / SCROLL_SENSITIVITY);
}

void ProjectProperties::reset(){
    taskName = "Task";
    answers = "Correct : 5.0P";
    rebuild();
}

void ProjectProperties::changeName(wxCommandEvent& event){
    custom_dialogs::TextFieldDialog* dialog = new custom_dialogs::TextFieldDialog(&taskName, "Name");
    dialog->ShowModal();
    dialog->Destroy();
}

void ProjectProperties::changeAnswers(wxCommandEvent& event){
    custom_dialogs::StyledTextDialog* dialog = new custom_dialogs::StyledTextDialog(&answers, "Answers");
    dialog->ShowModal();
    dialog->Destroy();
}

void ProjectProperties::setMode(globals::userMode mode){
    this->mode = mode;
    rebuild();
}
