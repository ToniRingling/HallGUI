#ifndef NODEOPTIONS_H_INCLUDED
#define NODEOPTIONS_H_INCLUDED

#include <wx/wx.h>
#include <wx/notebook.h>
#include <vector>
#include <utility>

#include "../Globals.h"

////FORWARD DECLARATIONS
class GraphEditor;
class OptionElement;
class Node;
namespace node_property_pages{
    class NodePropertyPage;
}
////

using namespace node_property_pages;

/** \brief A GUI element used for editing nodes, especially entering the option values.
 */
class NodeOptions : public wxNotebook{
private:
    GraphEditor* graphEditor;/**<The graph editor to signal important changes to.*/
    wxFlexGridSizer* sizer;/**<The sizer to contain all the options.*/

    Node* editedNode;/**<The node which is currently being edited here.*/

    std::vector<NodePropertyPage*> pages;

public :

    /** \brief
     *
     * \param Parent wxWindow* The window which contains this element.
     * \param Id wxWindowID The wxWidgets ID for this element.
     * \param Position const wxPoint& The default position of this element.
     * \param Size const wxSize& The default size of this element.
     * \param Style long The style of this element, used as in wxNotebook.
     *
     */
    NodeOptions(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
    /** \brief Destroys this GUI element without further effects.
     *
     * \return virtual
     *
     */
    virtual ~NodeOptions();
    /** \brief Sets a node to be edited. Loads the relevant options.
     *
     * \param node Node* The node to be edited. NULL will clear this UI.
     * \return void
     *
     */
    void setOptions(Node* node);
    /** \brief Sets the graph editor to alert on important option changes.
     *
     * \param editor GraphEditor* The graph editor to alert on important option changes.
     * \return void
     *
     */
    void setGraphEditor(GraphEditor* editor);
    /** \brief Sets the mode of all pages to the given one.
     *
     * \param mode globals::userMode The mode to set all pages to.
     * \return void
     *
     */
    void setMode(globals::userMode mode);
};

#endif // NODEOPTIONS_H_INCLUDED
