#ifndef NODEPROPERTYPAGE_H_INCLUDED
#define NODEPROPERTYPAGE_H_INCLUDED

#include <wx/wx.h>
#include <vector>
#include <utility>

#include "../../Globals.h"

////FORWARD DECLARATIONS
class GraphEditor;
class Node;
namespace node_components{
    class Option;
}
////

/** \brief A namespace for containing the different notebook pages for NodeOptions elements which can be used to edit nodes.
 */
namespace node_property_pages{
    /** \brief A virtual class for notebook pages which are used to edit nodes.
     */
    class NodePropertyPage : public wxScrolledWindow{
    protected:
        wxFlexGridSizer* sizer;/**<The sizer to contain all the options.*/
        Node* editedNode;/**<The node which is currently being edited here.*/
        globals::userMode mode;/**<The current user mode, according to which this page is built.*/
    private:
        GraphEditor* graphEditor;/**<The graph editor to signal important changes to.*/
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
        NodePropertyPage(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
        /** \brief Destroys this GUI element without further effects.
         *
         * \return virtual
         *
         */
        virtual ~NodePropertyPage();
        /** \brief Sets a node to be edited. Loads the relevant options.
         *
         * \param node Node* The node to be edited. NULL will clear this UI.
         * \return void
         *
         */
        virtual void setOptions(Node* node) = 0;
        /** \brief Sets the graph editor to alert on important option changes.
         *
         * \param editor GraphEditor* The graph editor to alert on important option changes.
         * \return void
         *
         */
        void setGraphEditor(GraphEditor* editor);
        /** \brief Called by the contained options to signal an important change happened.
         *
         * \return void
         *
         */
        void onImportantChange();
        /** \brief Sets the user mode for this page.
         *
         * \param mode globals::userMode The user mode to set this page to.
         * \return void
         *
         */
        void setMode(globals::userMode mode);
    protected:
        /** \brief Inserts the UI elements to the given option at the bottom. Note that the number of elements may change depending on the parameters.
         *
         * \param name std::string The name of this input part.
         * \param description std::string The tooltip for this input part.
         * \param NULL std::string* forDirectInput= If this is not NULL a text field for directly inputting this is added.
         * \param true bool withDeletion= Adds a field with a button for deleting this option. Still has to be bound.
         * \param true bool withEdit= Adds a field with a button for editing thid option. Still has to be bound.
         * \return std::pair<wxButton*, wxButton*> The first element is the button for deletion of this option, the second element is the button for editing this option. They will be NULL if these buttons were not added.
         *
         */
        std::pair<wxButton*, wxButton*> showOption(std::string name, std::string description, std::string* forDirectInput = NULL, bool withDeletion = true, bool withEdit = true);
        /** \brief Saves the node which is currently being edited to a file.
         *
         * \param event wxCommandEvent& The button press which should cause the node to be saved.
         * \return void
         *
         */
        void saveNode(wxCommandEvent& event);
    };
}

#endif // NODEPROPERTYPAGE_H_INCLUDED
