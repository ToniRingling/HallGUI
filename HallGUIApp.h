/***************************************************************
 * Name:      HallGUI.h
 * Purpose:   Defines Application Class
 * Author:    Toni Ringling (t.ringling@gmx.net)
 * Created:   2020-10-20
 * Copyright: Toni Ringling ()
 * License:
 **************************************************************/

#ifndef HALLGUIAPP_H
#define HALLGUIAPP_H

#include <wx/app.h>
#include "src/Globals.h"

/** \brief Contains the whole application. Initialization of this is the first thing that happens.
 */
class HallGUIApp : public wxApp{
    public:
        /** \brief Initializes the globals and the main frame.
         *
         * \return virtual bool True if main frame started successfully (I think - it's auto-generated wxWidgets stuff).
         *
         */
        virtual bool OnInit();
};

#endif // HALLGUIAPP_H
