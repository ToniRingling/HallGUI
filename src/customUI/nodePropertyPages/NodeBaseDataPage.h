#ifndef NODEBASEDATAPAGE_H_INCLUDED
#define NODEBASEDATAPAGE_H_INCLUDED

#include <wx/wx.h>
#include <vector>
#include <utility>

#include "NodePropertyPage.h"

namespace node_property_pages{
    /** \brief A GUI element used for editing the basic data of nodes, i.e. name, description, task and scripts.
     */
    class NodeBaseDataPage : public NodePropertyPage{
    public :
        /** \brief
         *
         * \param Parent wxWindow* The window which contains this element.
         * \param Id wxWindowID The wxWidgets ID for this element.
         * \param Position const wxPoint& The default position of this element.
         * \param Size const wxSize& The default size of this element.
         * \param Style long The style of this element, used as in wxScrolledWindow.
         *
         */
        NodeBaseDataPage(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
        /** \brief Destroys this GUI element without further effects.
         *
         * \return virtual
         *
         */
        virtual ~NodeBaseDataPage();
        /** \brief Sets a node to be edited. Loads the relevant options.
         *
         * \param node Node* The node to be edited. NULL will clear this UI.
         * \return void
         *
         */
        void setOptions(Node* node);
    private:
        /** \brief Opens a dialog to edit the description of the given node. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a dialog for editing the description.
         * \return void
         *
         */
        void editDescription(wxCommandEvent& event);
        /** \brief Opens a dialog to edit the task contents of the given node. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a dialog for editing the task contents.
         * \return void
         *
         */
        void editTask(wxCommandEvent& event);
        /** \brief Opens a dialog to edit the script contents for running the given node. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a dialog for editing the run-script contents.
         * \return void
         *
         */
        void editRunScript(wxCommandEvent& event);
        /** \brief Opens a dialog to edit the script contents for initializing the given node. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a dialog for editing the initialization-script contents.
         * \return void
         *
         */
        void editInitScript(wxCommandEvent& event);
    };
}

#endif // NODEBASEDATAPAGE_H_INCLUDED
