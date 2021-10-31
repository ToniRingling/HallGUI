#ifndef SMALLPREVIEW_H
#define SMALLPREVIEW_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <map>
#include <unordered_map>

////FORWARD DECLARATIONS
class Node;
////

/** \brief Used to rearrange the task elements as they appear in the result.
 * This GUI element is used to decide the order in which the task elements of nodes are put into the task.
 * In the future, this should display a preview and allow dragging of elements.
 *
 */
class SmallPreview: public wxPanel{
private:
    wxListCtrl* nodeList;/**<The list-gui-element where all the nodes are displayed.*/
    std::unordered_map<Node*, int> NodeIDs;/**<Maps the managed nodes to positions in the list.*/
public:
    /** \brief Creates the preview with an empty list of nodes.
     *
     * \param parent wxWindow* The window which contains the preview.
     * \param id wxWindowID The id of the preview.
     * \param pos const wxPoint& The default position of the preview.
     * \param size const wxSize& The default size of the preview.
     *
     */
    SmallPreview(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
    /** \brief Destroys the preview along with the UI-elements which belong to it.
     *
     * \return virtual
     *
     */
    virtual ~SmallPreview();

    /** \brief Pushes the currently selected node up in the order. Should be caused by a button press.
     *
     * \param event wxCommandEvent& The causal event for this action.
     * \return void
     *
     */
    void pushUp(wxCommandEvent& event);
    /** \brief Pushes the currently selected node down in the order. Should be caused by a button press.
     *
     * \param event wxCommandEvent& The causal event for this action.
     * \return void
     *
     */
    void pushDown(wxCommandEvent& event);

    /** \brief Selects the list item belonging to the given node.
     *
     * \param toSelect Node* The node for which the list item is to be selected.
     * \return void
     *
     */
    void selectNode(Node* toSelect);
    /** \brief Reloads the name of the given node.
     *
     * \param toRefresh Node* The node which possibly had its name changed, and for which it should be reloaded.
     * \return void
     *
     */
    void refreshNode(Node* toRefresh);
    /** \brief Adds another node to the list.
     *
     * \param toAdd Node* The node to add to the list.
     * \return void
     *
     */
    void addNode(Node* toAdd);
    /** \brief Removes a node from the list.
     *
     * \param toRemove Node* The node to remove from the list.
     * \return void
     *
     */
    void removeNode(Node* toRemove);
    /** \brief Creates and returns a mapping from positions in the task to nodes.
     *
     * \return std::map<int, Node*>* A new mapping, which maps the positions in the task to the contained nodes.
     *
     */
    std::map<int, Node*>* getNodePositions();
    /** \brief Clears this UI element of all inserted nodes.
     *
     * \return void
     *
     */
    void reset();
};

#endif
