#include "ProjectFileHandling.h"

#include <sstream>

#include "NodeFileHandling.h"
#include "../customUI/SmallPreview.h"
#include "../customUI/GraphEditor.h"
#include "../customUI/ProjectProperties.h"

#include "CommonFileHandling.h"
#include "projectFileVersionUpgraders/projectFileVersionUpgrading.h"
#include "../Globals.h"

using namespace common_file_handling;

/* Description of GHP (Grim-Hall-Project) files:
Note that all data will be interpreted as char* when reading and writing;

Sequentially (no additional line breaks between the data):

    Length of the version name - 32 bit int
    version name (as in the version of the project file, changes described at bottom) - chars (content of std::string)

    Length of project name - 32 bit int
    project name - chars (content of std::string)

    Length of project description - 32 bit int
    Project description - chars (content of std::string)

    Length of project answers - 32 bit int
    Project answers - chars (content of std::string)

    xOffset of the editor - 64 bit long
    yOffset of the editor - 64 bit long

    Number of Nodes - 32 bit int
    For each Node - given in the same order as in the task preview:
        Length of the node as a string (i.e. what we read in the next step) in bytes - int
        The node as for GHN files
        The x-position of the node - 32 bit int
        The y-position of the node - 32 bit int

    Number of NodeConnections - 32 bit int
    For each NodeConnection:
        For each NodePort of the connection (first start, then end):
            The type(node_port_type) - 32 bit int
            The node number(numbered in the same order as they are written, starting from 0) - 32 bit int
            The port number(inside the node, of that type of port) - 32 bit int

Changes in the versions:
    HGPV-0
        Initial named version.
        Now with lengths for project options and nodes.
    HGPV-1
        Removed project wide options (bad feature) and project description (unused).
*/

// Reading

void project_file_handling::resetProject(GraphEditor* editor, ProjectProperties* properties){
    editor->reset();
    editor->taskPreview->reset();
    properties->reset();
    editor->Refresh();
}

void project_file_handling::loadProjectFromFile(std::string path, GraphEditor* editor, ProjectProperties* properties){
    std::ifstream* from = new std::ifstream(path, std::istream::binary);
    std::stringstream buffer;
    buffer << from->rdbuf();
    std::string content = buffer.str();
    loadProjectFromString(&content, editor, properties);
    from->close();
    delete from;
}



void project_file_handling::loadProjectFromString(std::string* content, GraphEditor* editor, ProjectProperties* properties){
    resetProject(editor, properties);

    project_file_version_upgrading::updateProjectFile(content);

    std::istringstream* from = new std::istringstream(*content);

    std::string* version = readString(from);
    delete version;

    std::string* preName = readString(from);
    std::string* preAnswers = readString(from);

    properties->taskName = *preName;
    properties->answers = *preAnswers;

    delete preName;
    delete preAnswers;

    editor->offset[0] = readLong(from);
    editor->offset[1] = readLong(from);

    int nodeAmount = readInt(from);

    std::unordered_map<int, Node*> nodeNumbering;

    for(int nodeNum = 0; nodeNum < nodeAmount; nodeNum++){
        std::string* nodeStr = readString(from);
        std::istringstream nodeStream = std::istringstream(*nodeStr);
        Node* newNode = node_file_handling::loadFull(&nodeStream);
        int xP = readInt(from);
        int yP = readInt(from);
        newNode->setPos(xP, yP);
        editor->nodes.insert(newNode);
        nodeNumbering[nodeNum] = newNode;
        editor->taskPreview->addNode(newNode);
        delete nodeStr;
    }

    // as the number of triggerIn and triggerOut ports changes while adding connections, we have to do a bit of trickery to get them right
    // for this we keep all these connections in memory at first, then we add an imaginary NodePort with imaginary connections, which open up the needed NodePorts
    // in the end we just delete all these imaginary parts again

    struct connectionHolder{
        node_port_type::nodePortType sType;
        int sPar;
        int sPos;
        node_port_type::nodePortType eType;
        int ePar;
        int ePos;
    };

    int connectionAmount = readInt(from);

    std::vector<connectionHolder> triggerConnections;

    for(int conNum = 0; conNum < connectionAmount; conNum++){
        node_port_type::nodePortType sType = (node_port_type::nodePortType)readInt(from);
        int sPar = readInt(from);
        int sPos = readInt(from);
        node_port_type::nodePortType eType = (node_port_type::nodePortType)readInt(from);
        int ePar = readInt(from);
        int ePos = readInt(from);
        if(sType == node_port_type::output){
            NodePort* startPort = nodeNumbering[sPar]->getPort(node_port_type::output, sPos);
            NodePort* endPort = nodeNumbering[ePar]->getPort(node_port_type::input, ePos);
            NodeConnection* newCon = new NodeConnection(startPort, endPort);
            editor->connections.insert(newCon);
        }
        else{ // triggerOut
            triggerConnections.push_back(connectionHolder({sType, sPar, sPos, eType, ePar, ePos}));
        }
    }

    std::string* imagName = new std::string();
    *imagName = "Imaginary node";
    std::string* imagDesc = new std::string();
    *imagDesc = "You should not see this. This is the description of an imaginary node used for loading projects.";
    Node* imagNode = new Node(imagName, imagDesc);
    NodePort* imagTrOPort = imagNode->getPort(node_port_type::triggerOut, 0);
    NodePort* imagTrIPort = imagNode->getPort(node_port_type::triggerIn, 0);

    std::vector<NodeConnection*> fauxConnections;

    std::map<Node*, int> trINums; // used to count opened ports for triggers
    std::map<Node*, int> trONums;

    for(Node* node : editor->nodes){
        trINums[node] = 0;
        trONums[node] = 0;
    }

    for(connectionHolder cH : triggerConnections){ // open all ports
        Node* startNode = nodeNumbering[cH.sPar];
        Node* endNode = nodeNumbering[cH.ePar];
        NodePort* startPort = startNode->getPort(node_port_type::triggerOut, trONums[startNode]); // imaginary connections go to last spot - which is always the open spot - to open new one
        NodePort* endPort = endNode->getPort(node_port_type::triggerIn, trINums[endNode]);
        trONums[startNode]++;
        trINums[endNode]++;
        fauxConnections.push_back(new NodeConnection(imagTrOPort, endPort));
        fauxConnections.push_back(new NodeConnection(startPort, imagTrIPort));
    }

    for(connectionHolder cH : triggerConnections){
        NodePort* startPort = nodeNumbering[cH.sPar]->getPort(node_port_type::triggerOut, cH.sPos);
        NodePort* endPort = nodeNumbering[cH.ePar]->getPort(node_port_type::triggerIn, cH.ePos);
        editor->connections.insert(new NodeConnection(startPort, endPort));
    }

    for(NodeConnection* fauxConnection : fauxConnections){
        delete fauxConnection; // this also tries disconnecting the attached ports from these connections - but that happened anyway when we connected the real connections, so that is just ignored
    }

    fauxConnections.clear();

    delete imagNode;

    delete from;

    properties->rebuild();
    editor->Refresh();
}

// Writing

void project_file_handling::saveProject(std::string path, GraphEditor* editor, ProjectProperties* properties){
    std::ofstream* to = new std::ofstream(path, std::ostream::binary);

    writeString(to, &globals::currentProjectFileVersion); // Version string for project, see file description for rough description of changes.

    writeString(to, &properties->taskName);
    writeString(to, &properties->answers);

    writeLong(to, editor->offset[0]);
    writeLong(to, editor->offset[1]);

    writeInt(to, editor->nodes.size());

    std::map<int, Node*>* nodeOrder = editor->taskPreview->getNodePositions();
    std::unordered_map<Node*, int> nodeNumbering;

    for(std::map<int, Node*>::iterator current = nodeOrder->begin(); current != nodeOrder->end(); current++){
        Node* node = (*current).second;
        std::ostringstream nodeStream;
        node_file_handling::writeFull(&nodeStream, node);
        std::string nodeContent = nodeStream.str();
        writeString(to, &nodeContent); // This way we get the length of the node saved beforehand.*/
        writeInt(to, node->getX());
        writeInt(to, node->getY());
        nodeNumbering[node] = (*current).first;
    }

    delete nodeOrder;

    writeInt(to, editor->connections.size());

    for(NodeConnection* con : editor->connections){
        NodePort* startP = con->start();
        writeInt(to, startP->getType());
        writeInt(to, nodeNumbering[startP->getParent()]);
        writeInt(to, startP->getParent()->getPortPosition(startP));
        NodePort* endP = con->other(startP);
        writeInt(to, endP->getType());
        writeInt(to, nodeNumbering[endP->getParent()]);
        writeInt(to, endP->getParent()->getPortPosition(endP));
    }

    to->close();
    delete to;
}
