/***************************************************************
 * Name:      HallGUIMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Toni Ringling (t.ringling@gmx.net)
 * Created:   2020-10-20
 * Copyright: Toni Ringling ()
 * License:
 **************************************************************/

#include "HallGUIMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(HallGUIFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(HallGUIFrame)
const long HallGUIFrame::ID_STATICTEXT3 = wxNewId();
const long HallGUIFrame::GUINewNodeSelection = wxNewId();
const long HallGUIFrame::GUINewNodeSearch = wxNewId();
const long HallGUIFrame::ID_STATICTEXT2 = wxNewId();
const long HallGUIFrame::GUINodeDescriptionDisplay = wxNewId();
const long HallGUIFrame::ID_STATICTEXT1 = wxNewId();
const long HallGUIFrame::GUIProjectProperties = wxNewId();
const long HallGUIFrame::GUIGraphEditor = wxNewId();
const long HallGUIFrame::GUILogField = wxNewId();
const long HallGUIFrame::ID_STATICTEXT4 = wxNewId();
const long HallGUIFrame::GUISmallPreview = wxNewId();
const long HallGUIFrame::ID_STATICTEXT5 = wxNewId();
const long HallGUIFrame::GUINodeOptions = wxNewId();
const long HallGUIFrame::idMenuQuit = wxNewId();
const long HallGUIFrame::idMenuNew = wxNewId();
const long HallGUIFrame::idMenuSave = wxNewId();
const long HallGUIFrame::idMenuLoad = wxNewId();
const long HallGUIFrame::idMenuBuild = wxNewId();
const long HallGUIFrame::idMenuUser = wxNewId();
const long HallGUIFrame::idMenuDeveloper = wxNewId();
const long HallGUIFrame::idMenuAbout = wxNewId();
const long HallGUIFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(HallGUIFrame,wxFrame)
    //(*EventTable(HallGUIFrame)
    //*)
END_EVENT_TABLE()

HallGUIFrame::HallGUIFrame(wxWindow* parent,wxWindowID id)
{

    //(*Initialize(HallGUIFrame)
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, _("HallGUI"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableCol(1);
    FlexGridSizer1->AddGrowableCol(2);
    FlexGridSizer1->AddGrowableRow(0);
    FlexGridSizer2 = new wxFlexGridSizer(7, 1, 0, 0);
    FlexGridSizer2->AddGrowableCol(0);
    FlexGridSizer2->AddGrowableRow(1);
    FlexGridSizer2->AddGrowableRow(4);
    FlexGridSizer2->AddGrowableRow(6);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Node prefabs"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    newNodeSel = new NewNodeSelection(this,GUINewNodeSelection,wxDefaultPosition,wxSize(160,139));
    FlexGridSizer2->Add(newNodeSel, 1, wxALL|wxEXPAND, 5);
    newNodeSearch = new NewNodeSearch(this,GUINewNodeSearch,wxDefaultPosition,wxDefaultSize);
    FlexGridSizer2->Add(newNodeSearch, 1, wxALL|wxEXPAND, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Node description"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    nodeDescDisplay = new NodeDescriptionDisplay(this,GUINodeDescriptionDisplay,wxDefaultPosition,wxSize(160,109));
    FlexGridSizer2->Add(nodeDescDisplay, 1, wxALL|wxEXPAND, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Global options"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    projProp = new ProjectProperties(this,GUIProjectProperties,wxDefaultPosition,wxSize(160,99));
    FlexGridSizer2->Add(projProp, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer3 = new wxFlexGridSizer(2, 1, 0, 0);
    FlexGridSizer3->AddGrowableCol(0);
    FlexGridSizer3->AddGrowableRow(0);
    FlexGridSizer3->AddGrowableRow(1);
    graphEd = new GraphEditor(this,GUIGraphEditor,wxDefaultPosition,wxSize(422,317));
    FlexGridSizer3->Add(graphEd, 1, wxALL|wxEXPAND, 5);
    logField = new LogField(this,GUILogField,wxDefaultPosition,wxSize(422,49));
    FlexGridSizer3->Add(logField, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer4 = new wxFlexGridSizer(4, 1, 0, 0);
    FlexGridSizer4->AddGrowableCol(0);
    FlexGridSizer4->AddGrowableRow(1);
    FlexGridSizer4->AddGrowableRow(3);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Node display order"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    FlexGridSizer4->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    preview = new SmallPreview(this,GUISmallPreview,wxDefaultPosition,wxSize(185,158));
    FlexGridSizer4->Add(preview, 1, wxALL|wxEXPAND, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Node options"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    FlexGridSizer4->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    opt = new NodeOptions(this,GUINodeOptions,wxDefaultPosition,wxSize(185,245));
    FlexGridSizer4->Add(opt, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND, 5);
    SetSizer(FlexGridSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuItem6 = new wxMenuItem(Menu1, idMenuNew, _("New Project"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem6);
    MenuItem4 = new wxMenuItem(Menu1, idMenuSave, _("Save Project"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem4);
    MenuItem5 = new wxMenuItem(Menu1, idMenuLoad, _("Load Project"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItem5);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu3, idMenuBuild, _("Build"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem3);
    MenuBar1->Append(Menu3, _("Project"));
    Menu4 = new wxMenu();
    MenuItem7 = new wxMenuItem(Menu4, idMenuUser, _("User"), wxEmptyString, wxITEM_RADIO);
    Menu4->Append(MenuItem7);
    MenuItem8 = new wxMenuItem(Menu4, idMenuDeveloper, _("Developer"), wxEmptyString, wxITEM_RADIO);
    Menu4->Append(MenuItem8);
    MenuBar1->Append(Menu4, _("Mode"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);

    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&HallGUIFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&HallGUIFrame::OnAbout);
    //*)

    TaskAssembly::setGraphEditor(graphEd);
    TaskAssembly::setProjectProperties(projProp);
    TaskAssembly::setTaskPreview(preview);
    TaskAssembly::setLogField(logField);
    Connect(idMenuBuild, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&HallGUIFrame::OnBuild);

    newNodeSel->registerGraphEditor(graphEd);
    newNodeSel->registerDescriptionDisplay(nodeDescDisplay);

    newNodeSearch->registerTreeDisplay(newNodeSel);

    graphEd->registerOptionGUI(opt);
    graphEd->registerDescriptionDisplay(nodeDescDisplay);
    graphEd->registerPreview(preview);

    opt->setGraphEditor(graphEd);

    GetMenuBar()->FindItem(idMenuUser)->Check(true); // User mode is default

    Connect(idMenuNew, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&HallGUIFrame::OnNew);
    Connect(idMenuSave, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&HallGUIFrame::OnSave);
    Connect(idMenuLoad, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&HallGUIFrame::OnLoad);
    Connect(idMenuUser, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&HallGUIFrame::OnMode);
    Connect(idMenuDeveloper, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&HallGUIFrame::OnMode);
}

HallGUIFrame::~HallGUIFrame()
{
    //(*Destroy(HallGUIFrame)
    //*)
}

void HallGUIFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void HallGUIFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = "HallGUI - a graphical user interface for hallgrimJS - © Toni Ringling 2020-2021\nVersion 1";
    wxMessageBox(msg, _("Welcome to..."));
}

void HallGUIFrame::OnBuild(wxCommandEvent& event){
    TaskAssembly::assemble();
}

void HallGUIFrame::OnNew(wxCommandEvent& event){
    wxMessageDialog* resetConfirmationDialog = new wxMessageDialog(this, "This will delete all project contents. Continue?", "Reset Project",wxYES_NO | wxNO_DEFAULT);
    if(resetConfirmationDialog->ShowModal() == wxID_YES){
        project_file_handling::resetProject(graphEd, projProp);
    }
    delete resetConfirmationDialog;
}

void HallGUIFrame::OnSave(wxCommandEvent& event){
    wxFileDialog* savePathDialog = new wxFileDialog(this, "Save Project", wxEmptyString, wxEmptyString, "*.hgp", wxFD_SAVE |wxFD_OVERWRITE_PROMPT);
    if(savePathDialog->ShowModal() != wxID_CANCEL){
        project_file_handling::saveProject(savePathDialog->GetPath().ToStdString(), graphEd, projProp);
    }
    delete savePathDialog;
}

void HallGUIFrame::OnLoad(wxCommandEvent& event){
    wxFileDialog* loadPathDialog = new wxFileDialog(this, "Load Project", wxEmptyString, wxEmptyString, "*.hgp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if(loadPathDialog->ShowModal() != wxID_CANCEL){
        project_file_handling::loadProjectFromFile(loadPathDialog->GetPath().ToStdString(), graphEd, projProp);
    }
    delete loadPathDialog;
}

void HallGUIFrame::OnMode(wxCommandEvent& event){
    if(event.GetId() == idMenuUser){
        opt->setMode(globals::modeUser);
        projProp->setMode(globals::modeUser);
    }
    else if(event.GetId() == idMenuDeveloper){
        opt->setMode(globals::modeDeveloper);
        projProp->setMode(globals::modeDeveloper);
    }
}
