/***************************************************************
 * Name:      HallGUIMain.h
 * Purpose:   Defines Application Frame
 * Author:    Toni Ringling (t.ringling@gmx.net)
 * Created:   2020-10-20
 * Copyright: Toni Ringling ()
 * License:
 **************************************************************/

#ifndef HALLGUIMAIN_H
#define HALLGUIMAIN_H

//(*Headers(HallGUIFrame)
#include "src/customUI/GraphEditor.h"
#include "src/customUI/LogField.h"
#include "src/customUI/NewNodeSearch.h"
#include "src/customUI/NewNodeSelection.h"
#include "src/customUI/NodeDescriptionDisplay.h"
#include "src/customUI/NodeOptions.h"
#include "src/customUI/ProjectProperties.h"
#include "src/customUI/SmallPreview.h"
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
//*)

#include "src/TaskAssembly.h"
#include "src/ProjectFileHandling.h"
#include "src/Globals.h"


/** \brief The main frame of the program. Initializes all GUI components.
 */
class HallGUIFrame: public wxFrame
{
    public:

        /** \brief Creates the window with all GUI components.
         *
         * \param parent wxWindow* The window which contains this frame.
         * \param id wxWindowIDThe wxWidgets ID for this frame.
         *
         */
        HallGUIFrame(wxWindow* parent,wxWindowID id = -1);
        /** \brief Deletes the frame and all GUI components.
         *
         * \return virtual
         *
         */
        virtual ~HallGUIFrame();

    private:

        //(*Handlers(HallGUIFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        /** \brief Called when the project build command is called. Causes assembly of the output script.
         *
         * \param event wxCommandEvent& The causal wxCommandEvent, comes from clicking the build option in the project menu.
         * \return void
         *
         */
        void OnBuild(wxCommandEvent& event);
        /** \brief Called when the new project command is called. Resets the project to the standard project after confirmation in a dialog.
         *
         * \param event wxCommandEvent& The causal wxCommandEvent, comes from clicking the new option in the file menu.
         * \return void
         *
         */
        void OnNew(wxCommandEvent& event);
        /** \brief Called when the project save command is called. Saves the whole project after a path is given in a dialog.
         *
         * \param event wxCommandEvent& The causal wxCommandEvent, comes from clicking the save option in the file menu.
         * \return void
         *
         */
        void OnSave(wxCommandEvent& event);
        /** \brief Called when the project load command is called. Loads another project after a file is given in a dialog, overwrites old project.
         *
         * \param event wxCommandEvent& The causal wxCommandEvent, comes from clicking the load option in the file menu.
         * \return void
         *
         */
        void OnLoad(wxCommandEvent& event);
        /** \brief Called when the usage mode is selected. Forwards the mode to the relevant UI components.
         *
         * \param event wxCommandEvent& The causal wxCommandEvent, comes from clicking any mode in the mode menu.
         * \return void
         *
         */
        void OnMode(wxCommandEvent& event);

        //(*Identifiers(HallGUIFrame)
        static const long ID_STATICTEXT3;
        static const long GUINewNodeSelection;
        static const long GUINewNodeSearch;
        static const long ID_STATICTEXT2;
        static const long GUINodeDescriptionDisplay;
        static const long ID_STATICTEXT1;
        static const long GUIProjectProperties;
        static const long GUIGraphEditor;
        static const long GUILogField;
        static const long ID_STATICTEXT4;
        static const long GUISmallPreview;
        static const long ID_STATICTEXT5;
        static const long GUINodeOptions;
        static const long idMenuQuit;
        static const long idMenuNew;
        static const long idMenuSave;
        static const long idMenuLoad;
        static const long idMenuBuild;
        static const long idMenuUser;
        static const long idMenuDeveloper;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(HallGUIFrame)
        GraphEditor* graphEd;
        LogField* logField;
        NewNodeSearch* newNodeSearch;
        NewNodeSelection* newNodeSel;
        NodeDescriptionDisplay* nodeDescDisplay;
        NodeOptions* opt;
        ProjectProperties* projProp;
        SmallPreview* preview;
        wxMenu* Menu3;
        wxMenu* Menu4;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuItem5;
        wxMenuItem* MenuItem6;
        wxMenuItem* MenuItem7;
        wxMenuItem* MenuItem8;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // HALLGUIMAIN_H
