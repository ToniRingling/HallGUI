#ifndef NODEOUTPUTPAGE_H_INCLUDED
#define NODEOUTPUTPAGE_H_INCLUDED

#include <wx/wx.h>
#include <vector>
#include <utility>

#include "NodePropertyPage.h"

////FORWARD DECLARATIONS
class RewrappableText;
////

namespace node_property_pages{
    /** \brief A GUI element used for editing the outputs of nodes.
     */
    class NodeOutputPage : public NodePropertyPage{
    private:
        std::vector<RewrappableText*> lastColumnWrappables;/**< In user mode these are the text elements in the last column, which are wrapped on resize. */
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
        NodeOutputPage(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
        /** \brief Destroys this GUI element without further effects.
         *
         * \return virtual
         *
         */
        virtual ~NodeOutputPage();
        /** \brief Sets a node to be edited. Loads the relevant options.
         *
         * \param node Node* The node to be edited. NULL will clear this UI.
         * \return void
         *
         */
        void setOptions(Node* node);
    private:
        /** \brief Adds a new Output to the node which is being edited. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a new Output to be created.
         * \return void
         *
         */
        void newOutput(wxCommandEvent& event);
        /** \brief Deletes an Output from the node which is being edited. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause the deletion of an Output. Carries the number of the Output (w.r. to the vector in the node).
         * \return void
         *
         */
        void deleteOutput(wxCommandEvent& event);
        /** \brief Opens a dialog to edit the given Output. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a dialog for editing the Output.
         * \return void
         *
         */
        void editOutput(wxCommandEvent& event);
        /** \brief Handles resizing of this element. In Particular, this wraps the texts in the last column in user mode.
         *
         * \param e wxSizeEvent& The corresponding resize event.
         * \return void
         *
         */
        void onResize(wxSizeEvent& e);

        wxDECLARE_EVENT_TABLE();
    };
}

#endif // NODEOUTPUTPAGE_H_INCLUDED
