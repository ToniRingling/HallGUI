#ifndef NODEPORT_H_INCLUDED
#define NODEPORT_H_INCLUDED

#include <unordered_set>

#include "Node.h"
#include "NodeComponents.h"
#include "NodeConnection.h"

////FORWARD DECLARATIONS
class Node;
class NodeConnection;
class TaskAssembly;
namespace node_components{
    struct Output;
}
////

/** \brief Namespace to contain the enum for the types of node ports.
 *
 *
 */
namespace node_port_type{
    /** \brief The different type of node ports, i.e. inputs, outputs and both type of trigger ports. Corresponds to sides of the nodes.
     */
    enum nodePortType : int{
        input = 0,
        output = 1,
        triggerIn = 2,
        triggerOut = 3,
    };
}

/** \brief The points at which nodes may be attached for information transfer during tasks, i.e. sending/receiving outputs or triggering.
 */
class NodePort{
    friend class Node;
    friend class TaskAssembly;
private:
    std::unordered_set<NodeConnection*> connected;/**<All connections which have this port as one end point. For ports which do not represent outputs, this may at most be one.*/
    node_port_type::nodePortType type;/**<The type of this port, i.e. input, output, trigger-in or trigger-out.*/
    node_components::Output* output;/**<The corresponding output, if this is an output port. Needed to go input->port->connection->port->output when assembling tasks.*/
    Node* parent;/**<The node this port belongs to*/
    int x;/**<Relative x coordinate of this port to the parent node.*/
    int y;/**<Relative y coordinate of this port to the parent node.*/

public:
    /** \brief Creates a new port of the given type. For use mainly if type is not output.
     *
     * \param type node_port_type::nodePortType the type for the port.
     *
     */
    NodePort(node_port_type::nodePortType type);
    /** \brief Creates a new port of output type from the given Output.
     *
     * \param output node_components::Output* The Output this port stands for.
     *
     */
    NodePort(node_components::Output* output);
    /** \brief Destroys this port without further effects.
     *
     *
     */
    ~NodePort();

    /** \brief Sets a Node to be the parent of this port, i.e. the Node this is attached to.
     *
     * \param parent Node* The node this port is supposed to be attached to.
     * \return void
     *
     */
    void setParent(Node* parent);
    /** \brief Returns the node this port is attached to.
     *
     * \return Node* The node this port is attached to.
     *
     */
    Node* getParent();
    /** \brief Returns the type of the port.
     *
     * \return node_port_type::nodePortType The type of this port.
     *
     */
    node_port_type::nodePortType getType();
    /** \brief Returns whether this port has no connections.
     *
     * \return bool True if and only if this port is not part of a connection.
     *
     */
    bool isVacant();

    /** \brief Disconnects the given connection from this port (if it is connected).
     *
     * \param connection NodeConnection* The connection to disconnect form this port.
     * \return void
     *
     */
    void disconnect(NodeConnection* connection);
    /** \brief Connects a given connection to this port.
     *
     * \param connection NodeConnection* The connection to connect this port with.
     * \return void
     *
     */
    void connect(NodeConnection* connection);

    /** \brief Returns one of the connections this port has.
     *
     * We only need any connection, as this is used for backwards tracing inputs->outputs and triggerIn->triggerOut. So in these cases we only have at most one connection anyways.
     *
     * \return NodeConnection* A connection this port is part of.
     *
     */
    NodeConnection* getConnection();

    /** \brief Causes the graphics of all attached connections to be refreshed.
     *
     * \return void
     *
     */
    void refresh();

    /** \brief Returns the absolute x position of this port.
     *
     * \return int The absolute x position of this port, which takes the parent node position into account.
     *
     */
    int getX();
    /** \brief Returns the absolute y position of this port.
     *
     * \return int The absolute y position of this port, which takes the parent node position into account.
     *
     */
    int getY();
};

#endif // NODEPORT_H_INCLUDED
