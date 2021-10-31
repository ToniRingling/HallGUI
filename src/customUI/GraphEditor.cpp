#include "GraphEditor.h"

#include <math.h>
#include <wx/dcbuffer.h>

#include "NodeOptions.h"
#include "SmallPreview.h"
#include "NodeDescriptionDisplay.h"
#include "../nodes/NodeDescriptor.h"
#include "../nodes/NodePort.h"
#include "../nodes/Node.h"
#include "../Globals.h"

#define ID_CONTEXT_DELETE_NODE 0

wxBEGIN_EVENT_TABLE(GraphEditor, wxControl)
    EVT_PAINT(GraphEditor::OnPaint)
    EVT_MOUSE_EVENTS(GraphEditor::OnMouseControl)
    EVT_SIZE(GraphEditor::OnResize)
wxEND_EVENT_TABLE()

GraphEditor::GraphEditor(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxControl(Parent, Id, Position, Size, Style){

    // needed for SFML in an element - otherwise there will be flickering
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // set up the pairings between node ports we start dragging from and node ports we end on
    portTypePairing.insert({node_port_type::input, node_click_type::output});
    portTypePairing.insert({node_port_type::output, node_click_type::input});
    portTypePairing.insert({node_port_type::triggerIn, node_click_type::triggerOut});
    portTypePairing.insert({node_port_type::triggerOut, node_click_type::triggerIn});

    clickStartNode = NULL;
    clickStartPort = NULL;
    oldConnection = NULL;

    SetBackgroundStyle(wxBG_STYLE_PAINT); // to avoid flickering in conjunction with buffered paint context

    wxSizeEvent fakeResize(this->GetSize()); // needed to shift the view (see VIEW_Y_SHIFT at top)
    OnResize(fakeResize);

}

GraphEditor::~GraphEditor(){
    reset();
}

void GraphEditor::reset(){
    optionGUI->setOptions(NULL);
    clickStartNode = NULL;
    clickStartPort = NULL;
    oldConnection = NULL;
    for(Node* node : nodes){
        delete node;
    }
    taskPreview->reset();
    nodes.clear();
    connections.clear(); // connections get deleted when nodes are deleted
}

void GraphEditor::OnUpdate(){

}

void GraphEditor::OnPaint(wxPaintEvent& e){
    wxBufferedPaintDC context(this); // these have to be created in OnPaint, wxWidgets does not allow otherwise

    context.SetUserScale(scale, scale);

    int xSize = GetSize().GetX();
    int ySize = GetSize().GetY();

    context.SetBackground(wxBrush(wxColor(255, 255, 255)));

    context.Clear();

    // draw the vertical background grid lines
    context.SetPen(wxPen(wxColor(200, 200, 200), 1));
    context.SetBrush(wxBrush(wxColor(200, 200, 200)));
    int gridX = (offset[0] % globals::gridSize) - globals::gridSize;
    while(gridX <= xSize / scale){
        context.DrawRectangle(gridX, 0, 2, ySize / scale);
        gridX += globals::gridSize;
    }

    // draw the horizontal background grid lines
    context.SetPen(wxPen(wxColor(200, 200, 200), 1));
    context.SetBrush(wxBrush(wxColor(200, 200, 200)));
    int gridY = (offset[1] % globals::gridSize) - globals::gridSize;
    while(gridY <= ySize / scale){
        context.DrawRectangle(0, gridY, xSize / scale, 2);
        gridY += globals::gridSize;
    }
    // draw all nodes, done in reverse order, so front-most drawn node is the first one checked for clicks
    std::set<Node*>::reverse_iterator nodeIter = nodes.rbegin();
    context.SetPen(wxPen(wxColor(0, 0, 0), 1));
    context.SetBrush(wxBrush(wxColor(0, 0, 0)));
    while(nodeIter != nodes.rend()){
        Node* node = (*nodeIter);
        context.SetBrush(wxBrush(wxColor(0, 0, 0)));
        for(NodePort* port : node->getPorts()){ // draw ports first
            if(port != highlightedPort){
                context.DrawCircle(port->getX() + offset[0], port->getY() + offset[1], globals::portRadius);
            }
            else{
                context.SetBrush(wxBrush(wxColor(200, 50, 50)));
                context.DrawCircle(port->getX() + offset[0], port->getY() + offset[1], globals::portRadius);
                context.SetBrush(wxBrush(wxColor(0, 0, 0)));
            }
        }
        context.DrawBitmap(*node->getBitmap(), wxCoord(node->getX() + offset[0]), wxCoord(node->getY() + offset[1]), false);
        nodeIter++;
    }
    // draw the connections between nodes
    context.SetPen(wxPen(wxColor(0, 0, 0), 1));
    context.SetBrush(wxBrush(wxColor(0, 0, 0)));
    for(NodeConnection* connection : connections){
        context.DrawPolygon(4, connection->getPolygonPoints(), offset[0], offset[1]);
    }
    // draw the node connection, which is currently being drawn (if it exists)
    context.SetPen(wxPen(wxColor(200, 50, 50), 1));
    context.SetBrush(wxBrush(wxColor(200, 50, 50)));
    if(clickStartPort != NULL){
        // construct a rectangular line between the end points:
        // start at port we started drawing from
        double sX = clickStartPort->getX() + offset[0];
        double sY = clickStartPort->getY() + offset[1];
        // end at current mouse position
        double eX = mouseX;
        double eY = mouseY;
        double dX = eX - sX;
        double dY = eY - sY;
        double dAbs = std::sqrt((dX * dX) + (dY * dY));
        // perpendicular to connection from start to end and of length globals::connectionThickness/2, as we go both ways from start and end points
        double perpX = -(globals::connectionThickness / 2) * dY / dAbs;
        double perpY = (globals::connectionThickness / 2) * dX / dAbs;
        wxPoint polyPoints[4] = {wxPoint(sX - perpX, sY - perpY), wxPoint(sX + perpX, sY + perpY), wxPoint(eX + perpX, eY + perpY), wxPoint(eX - perpX, eY - perpY)};
        context.DrawPolygon(4, polyPoints);
    }
}

void GraphEditor::OnMouseControl(wxMouseEvent& e){
    bool forceRepaint = false;
    e.GetPosition(&mouseX, &mouseY);
    mouseX /= scale;
    mouseY /= scale;
    NodePort* oldHighlightedPort = highlightedPort;
    highlightedPort = NULL;
    for(Node* node : nodes){ // Should probably revisit the whole highlighting thing to make it at least marginally more efficient
        NodeClickEvent click = node->click(mouseX - (node->getX() + offset[0]), mouseY - (node->getY() + offset[1]));
        if(!((click.type == node_click_type::none) || (click.type == node_click_type::middle))){
            highlightedPort = click.port;
            break;
        }
    }
    if(oldHighlightedPort != highlightedPort){
        forceRepaint = true;
    }
    if(e.IsButton()){
        if(e.Button(wxMOUSE_BTN_LEFT)){
            if(e.ButtonDown()){ // starting left click
                leftMouseHeld = true;
                clickStartPos[0] = mouseX;
                clickStartPos[1] = mouseY;
                dragStartOffset[0] = offset[0];
                dragStartOffset[1] = offset[1];
                for(Node* node : nodes){ // checking, whether we clicked on any node
                    NodeClickEvent click = node->click(mouseX - (node->getX() + offset[0]), mouseY - (node->getY() + offset[1]));
                    if(click.type == node_click_type::middle){ // we hit a node but none of the ports: prepare dragging or selecting the node
                        clickStartNode = node;
                        dragStartOffset[0] = node->getX();
                        dragStartOffset[1] = node->getY();
                        break;
                    }
                    else if(click.type != node_click_type::none){ // we hit a port of the node, prepare drawing a connection
                        clickStartPort = click.port;
                        if((clickStartPort->getType() != node_port_type::output) && (!clickStartPort->isVacant())){ // there already is a connection to this port and it is not an output (so it can only have one connection)
                            NodeConnection* conn = clickStartPort->getConnection();
                            clickStartPort = conn->other(clickStartPort);
                            oldConnection = conn; // we remove the old connection after inserting the new one to not trigger an update of the node (i.e. reduce/increase number of trigger ports, which would move them)
                            connections.erase(conn);
                        }
                        break;
                    }
                }
            }
            else if(e.ButtonUp()){ // ending left click
                if(!dragging && clickStartNode != NULL){ // we clicked on the middle of a node and didn't drag - this means selection
                    descriptionDisplay->setContent(*clickStartNode->getName(), *clickStartNode->getDescription());
                    optionGUI->setOptions(clickStartNode);
                    taskPreview->selectNode(clickStartNode);
                }
                dragging = false;
                leftMouseHeld = false;
                clickStartNode = NULL;
                if(clickStartPort != NULL){ // we started on a port and now stopped
                    for(Node* node : nodes){ // so we see, whether we hit another port
                        node_click_type::nodeClickType wanted = portTypePairing[clickStartPort->getType()]; // which would have to be of this type for a valid connection
                        NodeClickEvent click = node->click(mouseX - (node->getX() + offset[0]), mouseY - (node->getY() + offset[1]));
                        // We have to check the following:
                        //  1. the type of the ports we want to connect has to fit
                        //  2. the port we connect to has to be able to accept a connection:
                        //   2.1. it could be vacant
                        //   2.2. it could only have the connection we are currently redrawing as an (invisible/virtual) connection
                        //   2.3. it is an output and as such allows unlimited connections
                        if(click.type == wanted && (click.port->isVacant() || ((oldConnection != NULL) && (click.port == oldConnection->other(clickStartPort))) || (click.port->getType() == node_port_type::output))){ // TODO: allow replacement of existing connections without erasing them first
                            connections.insert(new NodeConnection(clickStartPort, click.port));
                            break;
                        }
                    }
                    // the oldConnection can now be deleted, as it has been redrawn (drawing the same connection again makes a new one)
                    if(oldConnection != NULL){
                        delete oldConnection;
                        oldConnection = NULL;
                    }
                    clickStartPort = NULL;
                    Refresh();
                }
            }
        }
        else if(e.Button(wxMOUSE_BTN_RIGHT)){
            if(e.ButtonUp()){
                for(Node* node : nodes){
                    if(node->click(mouseX - (node->getX() + offset[0]), mouseY - (node->getY() + offset[1])).type != node_click_type::none){
                        clickStartNode = node;
                        wxMenu* nodeContextMenu = new wxMenu("Node context");
                        nodeContextMenu->Append(ID_CONTEXT_DELETE_NODE, "Delete", "Deletes this node", false);
                        nodeContextMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &GraphEditor::OnNodeContext, this);
                        PopupMenu(nodeContextMenu);
                        break;
                    }
                }
            }
        }
    }
    else if(e.GetWheelRotation() != 0){ // we are scrolling, i.e. zooming
        double newScale = scale + (e.GetWheelRotation() > 0 ? 0.1 : -0.1);
        if(newScale < 0.2){
            newScale = 0.2;
        }
        else if (newScale > 2){
            newScale = 2;
        }
        if(scale != newScale){
            int middleX = e.GetX();
            int middleY = e.GetY();
            // here we want to keep the point at the mouse position fixed when zooming, following these thoughts:
            // old size (in pixels) = baseSize / scale;
            // new size (in pixels) = baseSize / newscale;
            // where base size is for zoom factor 1 (i.e. just the pixel size of this control)
            // change in size (in pixels) = baseSize * ((1/newscale) - 1/scale)
            // we take the mouse position instead of the baseSize to keep the place where it is static
            double scaleFactor = (1 / newScale) - (1 / scale);
            offset[0] += middleX * scaleFactor;
            offset[1] += middleY * scaleFactor;
            scale = newScale;
            forceRepaint = true;
        }
    }
    else{ // we are currently just moving the mouse and have no button changes
        if(leftMouseHeld){
            if(!dragging){ // check whether we have moved far enough to mark this action as dragging
                long totalManDist = std::abs(mouseX - clickStartPos[0]) + std::abs(mouseY - clickStartPos[1]);
                if(totalManDist > DRAGGING_THRESHOLD){
                    dragging = true;
                }
            }
            if(dragging){ // if we are already dragging
                if(clickStartNode != NULL){ // we are dragging a node, so we move it
                    clickStartNode->setPos(dragStartOffset[0] - (clickStartPos[0] - mouseX), dragStartOffset[1] - (clickStartPos[1] - mouseY));
                }
                else if(clickStartPort != NULL){ // we are currently drawing a port, so we have to do nothing special

                }
                else{ // we are not dragging a node, so we are dragging the whole view
                    offset[0] = dragStartOffset[0] - (clickStartPos[0] - mouseX);
                    offset[1] = dragStartOffset[1] - (clickStartPos[1] - mouseY);
                }
                Refresh();
            }
        }
    }
    if(forceRepaint){
        Refresh();
    }
}

void GraphEditor::OnResize(wxSizeEvent& e){

}

void GraphEditor::onNewNodeInsertion(NodeDescriptor* desc){
    Node* newNode = node_file_handling::loadFull(desc);
    nodes.insert(newNode);
    newNode->setPos(-offset[0] + (this->GetSize().GetX() / 2), -offset[1] + (this->GetSize().GetY() / 2));
    taskPreview->addNode(newNode);
    Refresh();
}

void GraphEditor::onNodeChange(Node* changed){
    changed->forceRefresh();
    descriptionDisplay->setContent(*changed->getName(), *changed->getDescription());
    taskPreview->refreshNode(changed);
    Refresh();
}

void GraphEditor::registerOptionGUI(NodeOptions* options){
    optionGUI = options;
}

void GraphEditor::registerDescriptionDisplay(NodeDescriptionDisplay* display){
    descriptionDisplay = display;
}

void GraphEditor::registerPreview(SmallPreview* preview){
    taskPreview = preview;
}

void GraphEditor::OnNodeContext(wxCommandEvent& evt){
    if(evt.GetId() == ID_CONTEXT_DELETE_NODE){
       std::unordered_set<NodeConnection*>::iterator it = connections.begin();
        while(it != connections.end()){ // we should probably do this more cleverly, than checking all connections
            NodeConnection* conn = *it;
            if((conn->start()->getParent() == clickStartNode) ||(conn->other(conn->start())->getParent() == clickStartNode)){
                it = connections.erase(it);
            }
            else{
                it++;
            }
        }
        nodes.erase(clickStartNode);
        delete clickStartNode;
        descriptionDisplay->setContent("", "");
        optionGUI->setOptions(NULL);
        taskPreview->removeNode(clickStartNode);
        clickStartNode = NULL;
        Refresh();
    }
}
