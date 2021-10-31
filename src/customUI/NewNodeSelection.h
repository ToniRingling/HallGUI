#ifndef NEWNODESELECTION_H_INCLUDED
#define NEWNODESELECTION_H_INCLUDED

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/treectrl.h>
#include <string>
#include <vector>

////FORWARD DECLARATIONS
class NodeDescriptor;
class GraphEditor;
class FileTreeNode;
class NodeDescriptionDisplay;
////

/** \brief This struct is used so the wxTree leaves only gets a holder for the FileTreeNodes. This way they aren't deleted when the tree is, which is done when changing visibility. As such the destructor is trivial and does not affect the held description.
 */
struct TreeNodeHolder : public wxTreeItemData{
    FileTreeNode* treeNode;/**<The FileTreeNode to be held.*/
    /** \brief Makes a holder for the given description.
     *
     * \return TreeNodeHolder(FileTreeNode* description) The description which is to be held.
     *
     */
    TreeNodeHolder(FileTreeNode* treeNode) : wxTreeItemData(){
        this->treeNode = treeNode;
    }
};

/** \brief A GUI-Element used to select new nodes for insertion. The available nodes are given in the nodePrefabs directory as .ghn files.
 */
class NewNodeSelection : public wxTreeCtrl{
private:
    GraphEditor* usedEditor;/**<The graph editor to sent NodeDescriptions for insertion to.*/
    NodeDescriptionDisplay* usedDescriptionDisplay;/**<The display to show node descriptions before insertion.*/

    wxTreeItemId wxRoot = NULL;/**<The root node of the tree actually shown, i.e. the node for the nodePrefabs folder.*/
    FileTreeNode* realRoot = NULL;/**<The root node of the tree which actually holds all the data.*/

    std::vector<NodeDescriptor*> descriptions;/**<All NodeDescriptions which have been saved in the tree. Used for destruction.*/

    std::string filter;/**<The regex which must generate a match with files in the tree for them to be shown.*/

public :
    /** \brief Creates this element and initializes the tree, reading the wanted data into it.
     *
     * \param Parent wxWindow* The window which contains this element.
     * \param Id wxWindowID The wxWidgets id for this element.
     * \param Position const wxPoint& The default position of this element.
     * \param Size const wxSize& The default size of this element.
     * \param Style long The style of this element, as used in wxTreeCtrl.
     *
     */
    NewNodeSelection(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = wxTR_HAS_BUTTONS);
    /** \brief Destroys this element and all NodeDescriptions in the tree.
     *
     * \return virtual
     *
     */
    virtual ~NewNodeSelection();
    /** \brief Registers the graph editor to send requests for the creation of new nodes to.
     *
     * \param editor GraphEditor*
     * \return void
     *
     */
    void registerGraphEditor(GraphEditor* editor);
    /** \brief Registers the display to show the description of selected nodes on.
     *
     * \param display NodeDescriptionDisplay* The display to show the descriptions on.
     * \return void
     *
     */
    void registerDescriptionDisplay(NodeDescriptionDisplay* display);
    /** \brief Applies the given filter to the tree and then displays it.
     *
     * \param pattern std::string The filtering regex, for which only matching nodes will be shown. The matching is done over the name plus a line break plus the description.
     * \return void
     *
     */
    void setFilter(std::string pattern);
    /** \brief Rereads the file system and rebuild the tree accordingly. The filter will still be applied.
     *
     * \return void
     *
     */
    void rebuildTree();
private :
    /** \brief Recursive function to display all the nodes under realRoot inside the tree.
     *
     * \param toDisplay FileTreeNode The node of which the children are to be displayed.
     * \param attachTo wxTreeItemId The id of the root item that the children of toDisplay will be attached to.
     * \return void
     *
     */
    void refreshGUITree(FileTreeNode* toDisplay, wxTreeItemId attachTo);
    /** \brief Called on selection of tree content, used for displaying descriptions.
     *
     * \param e wxTreeEvent& The causal selection event.
     * \return void
     *
     */
    void onSelection(wxTreeEvent& e);
    /** \brief Called on double click on tree content, used for insertion.
     *
     * \param e wxTreeEvent& The causal activation event.
     * \return void
     *
     */
    void onActivation(wxTreeEvent& e);
    /** \brief Called when expanding tree content, used for keeping collapsed/expanded state when parts of tree not shown due to filtering.
     *
     * \param e wxTreeEvent& The causal expansion event.
     * \return void
     *
     */
    void onExpansion(wxTreeEvent& e);
    /** \brief Called when collapsing tree content, used for keeping collapsed/expanded state when parts of tree not shown due to filtering.
     *
     * \param e wxTreeEvent& The causal collapsing event.
     * \return void
     *
     */
    void onCollapse(wxTreeEvent& e);

    DECLARE_EVENT_TABLE();

};

#endif // NEWNODESELECTION_H_INCLUDED
