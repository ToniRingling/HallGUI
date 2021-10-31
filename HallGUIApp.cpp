/***************************************************************
 * Name:      HallGUIApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Toni Ringling (t.ringling@gmx.net)
 * Created:   2020-10-20
 * Copyright: Toni Ringling ()
 * License:
 **************************************************************/

#include "HallGUIApp.h"

//(*AppHeaders
#include "HallGUIMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(HallGUIApp);

bool HallGUIApp::OnInit()
{
    globals::initialize();
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	HallGUIFrame* Frame = new HallGUIFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
