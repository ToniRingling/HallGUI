#ifndef GRAPHEDITOR_H_INCLUDED
#define GRAPHEDITOR_H_INCLUDED

#include <set>
#include <wx/wx.h>
#include <boost/variant.hpp>
#include <unordered_set>
#include <unordered_map>

////FORWARD DECLARATIONS
class NodeOptions;
class GraphEditor;
class ProjectProperties;
class SmallPreview;
class NodeDescriptionDisplay;
class NodeDescriptor;
class NodePort;
class Node;
class NodeConnection;
namespace node_click_type{
    enum nodeClickType : int;
}
namespace node_port_type{
    enum nodePortType : int;
}
namespace project_file_handling{
    void resetProject(GraphEditor*, ProjectProperties* properties);
    void loadProjectFromString(std::string*, GraphEditor*, ProjectProperties*);
    void saveProject(std::string, GraphEditor*, ProjectProperties*);
}
////


/** \brief The GUI element which serves to edit and contain the graph of the task.
 */
class GraphEditor : public wxControl{
    friend class TaskAssembly;
    friend void project_file_handling::resetProject(GraphEditor*, ProjectProperties*);
    friend void project_file_handling::loadProjectFromString(std::string*, GraphEditor*, ProjectProperties*);
    friend void project_file_handling::saveProject(std::string, GraphEditor*, ProjectProperties*);
private:

    const long DRAGGING_THRESHOLD = 10;/**<The manhattan distance to move with held left mouse button before dragging commences.*/

    long offset[2] = {0, 0};/**<The internal offset of all elements in pixels. Change is caused by dragging outside of any nodes.*/

    double scale = 1;/**<The current scale of the display, i.e. the zoom factor. Goes from 0.2, 5-fold zoomed out, to 2, 2-fold zoomed in*/

    long clickStartPos[2] = {0, 0};/**< Where the user started holding the mouse last. Used to determine whether we have entered a dragging state.*/
    long dragStartOffset[2] = {0, 0};/**<The internal offset at which the user started dragging. Used to determine amount of offset change.*/

    bool leftMouseHeld = false;/**<Used to check for possible dragging action on events of pure mouse movement.*/
    bool dragging = false;/**<Marks mouse as currently being used for dragging, used as to not act like a left click when dragging back to the original spot.*/
    long mouseX = 0;/**<The currrent x-position of the mouse over this GUI element.*/
    long mouseY = 0;/**<The currrent y-position of the mouse over this GUI element.*/
    NodePort* highlightedPort = 0;/**<The port we are currently mousing over, if any.*/

    Node* clickStartNode;/**<If we click and hold the left mouse button and start on a node (but not a port), it will be here. Used for dragging nodes.*/
    NodePort* clickStartPort;/**<If we click and hold the left mouse button and start on the port of a node, it will be here. Used for drawing connections.*/
    NodeConnection* oldConnection;/**<If we reconfigure an existing connection i.e. start drawing a port from one of its end points (which is not an output), it will be here.*/

    NodeOptions* optionGUI;/**<The gui for the node options. The selected nodes are sent here for reconfiguration.*/
    NodeDescriptionDisplay* descriptionDisplay;/**<The display for node descriptions. The descriptions of the selected nodes are displayed here.*/
    SmallPreview* taskPreview;/**<The gui for rearranging the visible parts of the nodes in the task.*/

    std::set<Node*> nodes;/**<All nodes in the graph.*/
    std::unordered_set<NodeConnection*> connections;/**<All connections between nodes of the graph.*/

    std::unordered_map<node_port_type::nodePortType, node_click_type::nodeClickType> portTypePairing;/**<Pairs of acceptable combinations connection start port and end click type (specifically a click on a port).*/


public :

    /** \brief Initializes the new graph editor as is customary for wxControls and prepares combination wxControl/sf::Renderwindow.
     *
     * \param Parent wxWindow* The window which contains this element.
     * \param Id wxWindowID The wxwidgets ID for this element.
     * \param Position const wxPoint& The default position of this element.
     * \param Size const wxSize& The default size of this element.
     * \param Style long parameters as normal for wxControls.
     *
     */
    GraphEditor(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
    /** \brief Destroys the graph editor and all contents of the graph.
     *
     * \return virtual
     *
     */
    virtual ~GraphEditor();
    /** \brief Resets the graph editor, deleting all nodes and connections.
     *
     * \return void
     *
     */
    void reset();
    /** \brief Called to insert a new node from the given NodeDescriptor.
     *
     * \param desc NodeDescriptor* The NodeDescriptor from which to load and insert a full node.
     * \return void
     *
     */
    void onNewNodeInsertion(NodeDescriptor* desc);
    /** \brief Refreshes the given node, this and the rest of the GUI accordingly after a given node was changed.
     *
     * \param changed Node* The node which was changed.
     * \return void
     *
     */
    void onNodeChange(Node* changed);
    /** \brief Registers a NodeOptions GUI element to configure selected nodes in.
     *
     * \param options NodeOptions* The NodeOptions to display the options of selected nodes in.
     * \return void
     *
     */
    void registerOptionGUI(NodeOptions* options);
    /** \brief Registers a description display to be used for displaying descriptions of displayed nodes.
     *
     * \param display NodeDescriptionDisplay* The display to show the descriptions on.
     * \return void
     *
     */
    void registerDescriptionDisplay(NodeDescriptionDisplay* display);
    /** \brief Registers a preview to be used for rearranging the nodes in the task.
     *
     * \param preview SmallPreview* The preview to be used for rearranging the nodes in the task.
     * \return void
     *
     */
    void registerPreview(SmallPreview* preview);
private :
    /** \brief Automatically by OnPaint to refresh the graphics.
     *
     * \return virtual void
     *
     */
    virtual void OnUpdate();
    /** \brief Clears the screen and paints the graph..
     *
     * \param e wxPaintEvent& The causal paint event.
     * \return void
     *
     */
    void OnPaint(wxPaintEvent& e);
    /** \brief Handles mouse inputs inside this element.
     *
     * \param e wxMouseEvent& The causal mouse event.
     * \return void
     *
     */
    void OnMouseControl(wxMouseEvent& e);
    /** \brief Handles resizing of this element. Non-standard due to SFML-combination.
     *
     * \param e wxSizeEvent& The causal resizing event.
     * \return void
     *
     */
    void OnResize(wxSizeEvent& e);
    /** \brief Called on actions in context (right-click) menus for nodes.
     *
     * \param evt wxCommandEvent& The specific context event which was cause by the user selection.
     * \return void
     *
     */
    void OnNodeContext(wxCommandEvent& evt);

    wxDECLARE_EVENT_TABLE();
};

#endif // GRAPHEDITOR_H_INCLUDED
