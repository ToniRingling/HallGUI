#ifndef NODECONNECTION_H_INCLUDED
#define NODECONNECTION_H_INCLUDED

#include <wx/wx.h>

#include "NodePort.h"

////FORWARD DECLARATIONS
class NodePort;
////

/** \brief A connection between two NodePort objects.
 */
class NodeConnection{
private:
    NodePort* startPort;/**<The start port of this node. This should be an output or a trigger-out.*/
    NodePort* endPort;/**<The end port of this node. This should be an input or a trigger-in.*/
    wxPoint* drawPoints;/**<The points of the polygon which represents this connection. Given in global coordinates. Should be exactly 4.*/
public:
    /** \brief Creates a connection between the given two nodes, also registering itself in them. The start and end are determined from the ports, not the order of arguments.
     *
     * \param first NodePort* One end of the connection.
     * \param second NodePort* The other end of the connection.
     *
     */
    NodeConnection(NodePort* first, NodePort* second);
    /** \brief Destroys this connection, disconnecting itself from both attached ports and deleting the polygon points.
     *
     *
     */
    ~NodeConnection();
    /** \brief Reconstructs the polygon points.
     *
     * \return void
     *
     */
    void refresh();
    /** \brief Returns the polygon for this connection.
     *
     * \return wxPoint* A pointer to 4 wxPoints which are the vertices of the polygon for this connection.
     *
     */
    wxPoint* getPolygonPoints();
    /** \brief Given a port it is connected to, it will return the other port.
     *
     * \param relativeTo NodePort* The port of this connection we want the other end to.
     * \return NodePort* The other port it is connected to, or NULL if relativeTo is not an end point of this connection.
     *
     */
    NodePort* other(NodePort* relativeTo);
    /** \brief Returns the starting Port of this connection.
     *
     * \return NodePort* The starting Port of this connection.
     *
     */
    NodePort* start();
};

#endif // NODECONNECTION_H_INCLUDED
