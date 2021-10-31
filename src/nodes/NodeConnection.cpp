#include "NodeConnection.h"

#include "../Globals.h"

NodeConnection::NodeConnection(NodePort* first, NodePort* second){
    drawPoints = new wxPoint[4];
    if(first->getType() == node_port_type::input || first->getType() == node_port_type::triggerIn){ // TODO: think about checking whether ports fit here - but throwing an exception in constructors is not so nice
        startPort = second;
        endPort = first;
    }
    else{
        startPort = first;
        endPort = second;
    }
    startPort->connect(this);
    endPort->connect(this);
    refresh();
}

NodeConnection::~NodeConnection(){
    startPort->disconnect(this);
    endPort->disconnect(this);
    delete drawPoints;
}

void NodeConnection::refresh(){
    double sX = startPort->getX();
    double sY = startPort->getY();
    double eX = endPort->getX();
    double eY = endPort->getY();
    double dX = eX - sX;
    double dY = eY - sY;
    double dAbs = std::sqrt((dX * dX) + (dY * dY));
    double perpX = -(globals::connectionThickness / 2) * dY / dAbs;
    double perpY = (globals::connectionThickness / 2) * dX / dAbs;
    drawPoints[0] = wxPoint(sX - perpX, sY - perpY);
    drawPoints[1] = wxPoint(sX + perpX, sY + perpY);
    drawPoints[2] = wxPoint(eX + perpX, eY + perpY);
    drawPoints[3] = wxPoint(eX - perpX, eY - perpY);
}

wxPoint* NodeConnection::getPolygonPoints(){
    return drawPoints;
}

NodePort* NodeConnection::other(NodePort* relativeTo)
{
    if(relativeTo == startPort)
    {
        return endPort;
    }
    else if (relativeTo == endPort)
    {
        return startPort;
    }
    else{
        return NULL;
    }
}

NodePort* NodeConnection::start(){
    return startPort;
}
