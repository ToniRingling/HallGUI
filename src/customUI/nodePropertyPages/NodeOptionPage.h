#ifndef NODEOPTIONPAGE_H_INCLUDED
#define NODEOPTIONPAGE_H_INCLUDED

#include <wx/wx.h>
#include <vector>
#include <utility>

#include "NodePropertyPage.h"

namespace node_property_pages{
    /** \brief A GUI element used for editing the options of nodes.
     */
    class NodeOptionPage : public NodePropertyPage{
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
        NodeOptionPage(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
        /** \brief Destroys this GUI element without further effects.
         *
         * \return virtual
         *
         */
        virtual ~NodeOptionPage();
        /** \brief Sets a node to be edited. Loads the relevant options.
         *
         * \param node Node* The node to be edited. NULL will clear this UI.
         * \return void
         *
         */
        void setOptions(Node* node);
    private:
        /** \brief Displays the GUI elements for the given option at the end of the sizer. This will be 4 elements.
         *
         * \param opt node_components::Option* The option to display the GUI elements for.
         * \param false bool withButtons= Whether to generate buttons for deleting and editing.
         * \return std::pair<wxButton*, wxButton*> The first element is the button for deletion of this option, the second element is the button for editing this option - as long buttons were wished for.
         *
         */
        std::pair<wxButton*, wxButton*> showOption(node_components::Option* opt, bool withButtons);
        /** \brief Adds a new Option to the node which is being edited. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a new Option to be created.
         * \return void
         *
         */
        void newOption(wxCommandEvent& event);
        /** \brief Deletes an Option from the node which is being edited. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause the deletion of an Option. Carries the number of the option (w.r. to the vector in the node).
         * \return void
         *
         */
        void deleteOption(wxCommandEvent& event);
        /** \brief Opens a dialog to edit the given option. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a dialog for editing the option.
         * \return void
         *
         */
        void editOption(wxCommandEvent& event);
    };
}

#endif // NODEOPTIONPAGE_H_INCLUDED
