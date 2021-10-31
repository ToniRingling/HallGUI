#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <vector>
#include <unordered_set>

#include "NodePort.h"
#include "NodeDescriptor.h"
#include "NodeComponents.h"

////FORWARD DECLARATIONS
namespace node_port_type{
    enum nodePortType : int;
}

namespace node_property_pages{
    class NodeBaseDataPage;
    class NodeOptionPage;
    class NodeInputPage;
    class NodeOutputPage;
}

class NodePort;
class TaskAssembly;

namespace node_components{
    enum sizeType : int;

    struct Input;
    struct Output;
    struct Option;
}

namespace node_file_handling{
    void writeFull(std::ostream* to, Node* toWrite);
}
////


/** \brief Namespace to contain the nodeClickType enum.
 *
 */
namespace node_click_type{
    /** \brief Describes the different ways one can click on a node, i.e. not at all, in the middle or on one of the ports.
     */
    enum nodeClickType : int{
        none = 0,
        middle = 1,
        input = 2,
        output = 3,
        triggerIn = 4,
        triggerOut = 5,
    };
}

/** \brief An event which describes a click on a node, i.e. in which area one clicked and which port one clicked (if one did so).
 */
struct NodeClickEvent{
    /** \brief Creates an event of the given click type with the given port.
     *
     * \param type node_click_type::nodeClickType The type of click this event corresponds to.
     * \param port NodePort* To port which was clicked on (NULL if none)
     *
     */
    NodeClickEvent(node_click_type::nodeClickType type, NodePort* port);
    /** \brief Destroys this event without further effects.
     *
     *
     */
    ~NodeClickEvent();

    node_click_type::nodeClickType type;/**<The type of click this event corresponds to.*/
    NodePort* port;/**<If one clicked on a port, this will be it. Otherwise this is NULL.*/
};

/** \brief A node in the graph. Allows connections and contains HTML-content as well as javaScript-content.
 */
class Node{
    friend class TaskAssembly;
    friend class node_property_pages::NodeBaseDataPage;
    friend class node_property_pages::NodeOptionPage;
    friend class node_property_pages::NodeInputPage;
    friend class node_property_pages::NodeOutputPage;
    friend void node_file_handling::writeFull(std::ostream*, Node*);
private:
    std::string* name;
    std::string* description;

    std::vector<node_components::Input*> inputs;/**<The inputs this node has.*/
    std::vector<node_components::Output*> outputs;/**<The outputs this node has.*/
    std::vector<NodePort*> triggerIn;/**<The ports of this node which cause it to be triggered.*/
    std::vector<NodePort*> triggerOut;/**<The ports of this node with which it triggers other nodes.*/
    std::unordered_set<NodePort*> allPorts;/**<Contains all ports attached to this node.*/

    std::vector<node_components::Option*> options;/**<The options to this node.*/

    std::string* task;/**<The HTML-task content of this node.*/

    std::string* runScript;/**<The javaScript content of this node, which is executed when the node is triggered.*/
    std::string* initScript;/**<The javaScript content of this node, which is executed when the task is started.*/

    wxBitmap* innerBitmap;/**<Bitmap for rendering the inner parts of this node, i.e. without ports.*/
    bool mustRefresh;/**<If true graphics will be refreshed when requested the next time.*/
    int xPos;/**<The left bound of this node, including ports.*/
    int yPos;/**<The lower bound of this node, including (potential input/output-) ports.*/
    int width;/**<The width of this node including (potential) ports.*/
    int height;/**<The height of this node including (potential) ports.*/
    NodePort* toHighlight;/**<The node port to be highlighted in the graphics (for hovering over), NULL if none.*/
public:

    /** \brief Creates a completely empty node with some standard parameters.
     *
     *  Useful when changing the specifications of nodes (or their files), as it can be used to generate some first node from which to make the others in the program.
     *
     */
    Node();
    /** \brief Creates an otherwise empty node (only a triggerIn and a triggerOut port) with the given name an description.
     *
     * \param name std::string* The name of this node.
     * \param description std::string* The description of this node.
     *
     */
    Node(std::string* name, std::string* description);
    /** \brief Destroys the node with all inputs, outputs, options and trigger-ports and therefore also with all connections.
     *
     *
     */
    ~Node();
    /** \brief Adds a new input to the node. Constructs the corresponding port if needed and configures it to this node.
     *
     * \param input node_components::Input* The input to add.
     * \return void
     *
     */
    void addInput(node_components::Input* input);
    /** \brief Adds a new output to the node. Constructs the corresponding port if needed and configures it to this node.
     *
     * \param output node_components::Output* The output to add.
     * \return void
     *
     */
    void addOutput(node_components::Output* output);
    /** \brief Removes an input and the corresponding port from the node.
     *
     * \param input node_components::Input* The input to remove.
     * \return void
     *
     */
    void removeInput(node_components::Input* input);
    /** \brief Removes an output and the corresponding port from the node.
     *
     * \param output node_components::Output* The output to remove.
     * \return void
     *
     */
    void removeOutput(node_components::Output* output);
    /** \brief Adds a new option to the node.
     *
     * \param option node_components::Option* The option to add.
     * \return void
     *
     */
    void addOption(node_components::Option* option);
    /** \brief Sets the HTML task contents. Will delete the old task.
     *
     * \param task std::string* The new HTML contents to this node.
     * \return void
     *
     */
    void setTask(std::string* task);
    /** \brief Sets the javaScript contents. Will delete the old script.
     *
     * \param run std::string* The new javaScript contents to this node, which should run when it is triggered.
     * \param init std::string* The new javaScript contents to this node, which should run when the task starts.
     * \return void
     *
     */
    void setScript(std::string* run, std::string* init);
    /** \brief Adds an import to this node.
     *
     * \param import std::string The import to be added.
     * \return void
     *
     */
    void addImport(std::string import);
    /** \brief Causes all ports to refresh, i.e. signal their connections to redraw themselves. Used when ports (or even the whole node) move.
     *
     * \return void
     *
     */
    void refreshPorts();
    /** \brief Returns a displayable bitmap for the node. May redraw the node before that. Does not include the ports of the node.
     *
     * \return wxBitmap* A bitmap containing the representation of this node.
     *
     */
    wxBitmap* getBitmap();
    /** \brief Forces the node to be redrawn when getSprite is called next.
     *
     * \return void
     *
     */
    void forceRefresh();
    /** \brief Returns the current left bound of the node with ports.
     *
     * \return int The current left bound of the node with ports.
     *
     */
    int getX();
    /** \brief Returns the current lower bound of the node with (potential input/output-) ports.
     *
     * \return int The current lower bound of the node with (potential input/output-) ports.
     *
     */
    int getY();
    /** \brief Sets the x and y position as described in xPos and yPos.
     *
     * \param x int The new x position.
     * \param y int The new y position.
     * \return void
     *
     */
    void setPos(int x, int y);
    /** \brief Called by the trigger ports when their connection state changes (the number of connections, that is). Used to readjust the number of trigger ports.
     *
     * \param changedPort NodePort* The port which had their connection state changed.
     * \return void
     *
     */
    void portChanged(NodePort* changedPort);
    /** \brief Returns the name of this node.
     *
     * \return std::string& The name of this node.
     *
     */
    std::string* getName();
    /** \brief Returns the description of this node.
     *
     * \return std::string& The description of this node.
     *
     */
    std::string* getDescription();
    /** \brief Returns a click event corresponding to the given relative coordinates.
     *
     * \param x int The relative coordinates of the click compared to xPos.
     * \param y int The relative coordinates of the click compared to yPos.
     * \return NodeClickEvent The event corresponding to this click.
     *
     */
    NodeClickEvent click(int x, int y);
    /** \brief Returns all options to this node.
     *
     * \return std::vector<node_components::Option*> The options of this node.
     *
     */
    std::vector<node_components::Option*> getOptions();
    /** \brief Gets the position of a port in this node among ports of the same type.
     *
     * \param of NodePort* The port to get the position of.
     * \return int The number of this port among ports of the same type in this node, starting from 0. -1 if not contained.
     *
     */
    int getPortPosition(NodePort* of);
    /** \brief Returns the port of the given type and position.
     *
     * \param type node_port_type The type of the sought port.
     * \param number int The number of the sought port amongst the ports of the same type.
     * \return NodePort* The port of the given type at the given position relative to ports of the same type. NULL if it does not exist.
     *
     */
    NodePort* getPort(node_port_type::nodePortType type, int number);
    /** \brief Returns all ports of this node.
     *
     * \return std::unordered_set<NodePort*>* A set containing all node ports of this node.
     *
     */
    std::unordered_set<NodePort*> getPorts();
private:
    /** \brief Redraws the node.
     *
     * \return void
     *
     */
    void refreshGraphics();
};

#endif // NODE_H_INCLUDED
