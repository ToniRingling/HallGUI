#include "Node.h"

#define MAXOPTIONVALUELENGTH 30

#include <iostream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <wx/graphics.h>

#include "../Globals.h"
#include "optionCores/OptionCore.h"

namespace{
    const double Pi = 3.141592654;

    std::string stringShortener(std::string input){
        std::istringstream lineReader(input);
        std::string line;
        std::getline(lineReader, line);
        line = line.substr(0, MAXOPTIONVALUELENGTH);
        if(line != input){
            line += "...";
        }
        return line;
    }

    // contains total width, total height, overhang (i.e. the x-difference between two points on one end of the text)
    std::vector<int> textSizeDeterminer(std::string text, double angle, wxMemoryDC* DC){
        std::vector<int> out;
        wxCoord xC;
        wxCoord yC;
        DC->GetTextExtent(text, &xC, &yC);
        if(angle == 0){
            out.push_back(xC);
            out.push_back(yC);
            out.push_back(0);
        }
        else{
            double xP = xC;
            double yP = yC;
            double radAngle = Pi * angle / 180;
            out.push_back(std::ceil(std::abs(std::cos(radAngle) * xP) + std::abs(std::sin(radAngle) * yP)));
            out.push_back(std::ceil(std::abs(std::cos(radAngle) * yP) + std::abs(std::sin(radAngle) * xP)));
            out.push_back(std::ceil(std::abs(std::sin(radAngle) * yP) / 2));
        }
        return out;
    }
}

NodeClickEvent::NodeClickEvent(node_click_type::nodeClickType type, NodePort* port){
    this->type = type;
    this->port = port;
}

NodeClickEvent::~NodeClickEvent(){

}

Node::Node(){
    name = new std::string();
    *name = "Default Node";
    description = new std::string();
    *description = "Default Description";

    triggerIn.push_back(new NodePort(node_port_type::triggerIn));
    triggerIn[0]->setParent(this);
    allPorts.insert(triggerIn.back());
    triggerOut.push_back(new NodePort(node_port_type::triggerOut));
    triggerOut[0]->setParent(this);
    allPorts.insert(triggerOut.back());

    mustRefresh = true;
    innerBitmap = NULL;

    task = new std::string();
    *task = "Default task";
    runScript = new std::string();
    *runScript = "'Default run script';";
    initScript = new std::string();
    *initScript = "'Default init script';";

    width = 0;
    height = 0;
}

Node::Node(std::string* name, std::string* description){
    this->name = name;
    this->description = description;

    triggerIn.push_back(new NodePort(node_port_type::triggerIn));
    triggerIn[0]->setParent(this);
    allPorts.insert(triggerIn.back());
    triggerOut.push_back(new NodePort(node_port_type::triggerOut));
    triggerOut[0]->setParent(this);
    allPorts.insert(triggerOut.back());

    mustRefresh = true;
    innerBitmap = NULL;

    task = NULL;
    runScript = NULL;
    initScript = NULL;

    width = 0;
    height = 0;
}

Node::~Node(){
    while(!inputs.empty()){
        delete inputs.back();
        inputs.pop_back();
    }
    while(!outputs.empty()){
        delete outputs.back();
        outputs.pop_back();
    }
    while(!triggerIn.empty()){
        delete triggerIn.back();
        triggerIn.pop_back();
    }
    while(!triggerOut.empty()){
        delete triggerOut.back();
        triggerOut.pop_back();
    }
    while(!options.empty()){
        delete options.back();
        options.pop_back();
    }
    if(innerBitmap != NULL){
        delete innerBitmap;
    }
    if(task != NULL){
        delete task;
    }
    if(runScript != NULL){
        delete runScript;
    }
    if(initScript != NULL){
        delete initScript;
    }
    if(name != NULL){
        delete name;
    }
    if(description != NULL){
        delete description;
    }
}


void Node::addInput(node_components::Input* input){
    if(input->port == NULL){
        input->port = new NodePort(node_port_type::input);
    }
    input->port->setParent(this);
    inputs.push_back(input);
    allPorts.insert(input->port);
}

void Node::addOutput(node_components::Output* output){
    if(output->port == NULL){
        output->port = new NodePort(output);
    }
    output->port->setParent(this);
    outputs.push_back(output);
    allPorts.insert(output->port);
}

void Node::removeInput(node_components::Input* input){
    std::vector<node_components::Input*>::iterator pos = std::find(inputs.begin(), inputs.end(), input);
    if(pos != inputs.end()){
        inputs.erase(pos);
        allPorts.erase(std::find(allPorts.begin(), allPorts.end(), input->port));
        delete input;
    }
}

void Node::removeOutput(node_components::Output* output){
    std::vector<node_components::Output*>::iterator pos = std::find(outputs.begin(), outputs.end(), output);
    if(pos != outputs.end()){
        outputs.erase(pos);
        allPorts.erase(std::find(allPorts.begin(), allPorts.end(), output->port));
        delete output;
    }
}

void Node::addOption(node_components::Option* option){
    options.push_back(option);
}

void Node::setTask(std::string* task){
    if(this->task != NULL){
        delete this->task;
    }
    this->task = task;
}

void Node::setScript(std::string* run, std::string* init){
    if(this->runScript != NULL){
        delete this->runScript;
    }
    this->runScript = run;
    if(this->initScript != NULL){
        delete this->initScript;
    }
    this->initScript = init;
}

void Node::refreshPorts(){
    using namespace node_components;
    for(NodePort* port : allPorts){
        port->refresh();
    }
}

void Node::refreshGraphics(){
    using namespace node_components;

    // Declared right upfront with font so we can use it to correctly determine text sizes.
    wxMemoryDC context;
    context.SetFont(*wxSWISS_FONT);

    mustRefresh = false;
    // determine dimensions
    int topHeight = 0;
    int middleHeight = globals::fontHeight; // base height is 1 (name) + number of important options times font height assuming enough space for triggerIn and triggerOut ports
    for(Option* opt : options){
        if(opt->importance){
            middleHeight += globals::fontHeight;
        }
    }
    int bottomHeight = 0;
    int topMaxX = 0;
    int topMinX = 0;
    int bottomMinX = 0;
    int bottomMaxX = 0;
    int middleWidth = 0;
    // We first calculate the input and output positions and extents, as if the first input and first output start at 0. We later readjust the starting position.
    // We will also be readjusting them by the "overhang" without which they would be off-centre. We will however use the average overhang for that so the line distances don't wary wildly.
    double overhangSum;
    int inputCounter = 0;
    for(Input* input : inputs){
        std::vector<int> dims = textSizeDeterminer(input->name, globals::diagonalTextAngle, &context);
        topHeight = std::max(topHeight, dims[1]);
        topMinX = std::min(topMinX, int(std::ceil(globals::diagonalTextDistance * inputCounter)));
        topMaxX = std::max(topMaxX, int(std::ceil(globals::diagonalTextDistance * inputCounter + dims[0])));
        overhangSum += dims[2];
        inputCounter++;
    }
    int outputCounter = 0;
    for(Output* output : outputs){
        std::vector<int> dims = textSizeDeterminer(output->name, -globals::diagonalTextAngle, &context);
        bottomHeight = std::max(bottomHeight, dims[1]);
        bottomMinX = std::min(bottomMinX, int(std::ceil(globals::diagonalTextDistance * outputCounter)));
        bottomMaxX = std::max(bottomMaxX, int(std::ceil(globals::diagonalTextDistance * outputCounter + dims[0])));
        overhangSum += dims[2];
        outputCounter++;
    }
    int averageOverhang = overhangSum / (outputs.size() + inputs.size());
    for(Option* option : options){
        if(option->importance){
            std::vector<int> dims = textSizeDeterminer(option->name + ":" + stringShortener(option->core->getValue()), 0, &context);
            middleWidth = std::max(middleWidth, dims[0]);
        }
    }
    if(topHeight != 0){
        topHeight += globals::nodeBorderPadding;
    }
    if(bottomHeight != 0){
        bottomHeight += globals::nodeBorderPadding;
    }
    middleWidth = std::max(middleWidth, textSizeDeterminer(*name, 0, &context)[0]);

    //for the input/output ports we want to spread them equidistant and symmetrically around the center of the top/bottom edge of the node
    //for this we calculate how far they will deviate from this center at most, to determine the needed width - we assume this deviation is needed to both sides of the center

    int topCenter = (int)std::round(globals::diagonalTextDistance * (double(inputs.size() - 1) / 2.0));
    int topMaxDeviation = std::max(topCenter - topMinX, topMaxX - topCenter);

    int bottomCenter = (int)std::round(globals::diagonalTextDistance * (double(outputs.size() - 1) / 2.0));
    int bottomMaxDeviation = std::max(bottomCenter - bottomMinX, bottomMaxX - bottomCenter);

    int totalWidth = std::max(2 * (topMaxDeviation), 2 * (bottomMaxDeviation)) + averageOverhang; // max of top and bottom first
    totalWidth = std::max(totalWidth, middleWidth) + (2 * globals::nodeBorderThickness) + (2 * globals::nodeBorderPadding);

    middleHeight = std::max(middleHeight, (int(std::max(triggerIn.size(), triggerOut.size())) * 2 * globals::portRadius) - (topHeight + bottomHeight));

    int totalHeight = bottomHeight + middleHeight + topHeight + (4 * globals::nodeBorderThickness); // two at top and bottom, two between sections

    width = totalWidth;
    height = totalHeight;

    // calculate the positions of all the ports of the nodes and refresh them
    // I wish I could draw the ports into one bitmap with the rest of the node, but wxWidgets crossplatform transparency is a real hassle. So the ports will be drawn outside instead.
    int topStart = (totalWidth / 2) - (int(globals::diagonalTextDistance * (inputs.size() - 1) / 2.0));
    int bottomStart = (totalWidth / 2) - (int(globals::diagonalTextDistance * (outputs.size() - 1) / 2.0));
    for(int a = 0; a < int(inputs.size()); a++) {
        inputs[a]->port->x = topStart + (a * globals::diagonalTextDistance);
        inputs[a]->port->y = 0;
    }
    for(int a = 0; a < int(outputs.size()); a++) {
        outputs[a]->port->x = bottomStart + (a * globals::diagonalTextDistance);
        outputs[a]->port->y = totalHeight;
    }
    for(int a = 1; a <= int(triggerIn.size()); a++) {
        triggerIn[a - 1]->x = 0;
        triggerIn[a - 1]->y = ((totalHeight * a) / (triggerIn.size() + 1));
    }
    for(int a = 1; a <= int(triggerOut.size()); a++) {
        triggerOut[a - 1]->x = totalWidth;
        triggerOut[a - 1]->y = ((totalHeight * a) / (triggerOut.size() + 1));
    }
    refreshPorts();

    //draw inner part of the node, i.e. everything but the ports
    // TODO: reuse bitmap if possible
    if(innerBitmap != NULL){
        delete innerBitmap;
    }
    innerBitmap = new wxBitmap(totalWidth, totalHeight);

    context.SelectObject(*innerBitmap);

    context.SetBackground(wxBrush(wxColor(100, 100, 100)));
    context.Clear();

    context.SetPen(wxPen(wxColor(230, 230, 230), 1));
    context.SetBrush(wxBrush(wxColor(230, 230, 230)));
    context.DrawRectangle(globals::nodeBorderThickness, globals::nodeBorderThickness, totalWidth - (globals::nodeBorderThickness * 2), totalHeight - (globals::nodeBorderThickness * 2));

    context.SetPen(wxPen(wxColor(100, 100, 100), 1));
    context.SetBrush(wxBrush(wxColor(100, 100, 100)));
    context.DrawRectangle(0, topHeight + globals::nodeBorderThickness, totalWidth, globals::nodeBorderThickness);
    context.DrawRectangle(0, topHeight + middleHeight + (globals::nodeBorderThickness * 2), totalWidth, globals::nodeBorderThickness);

    context.DrawText(*name, wxCoord(globals::nodeBorderThickness + globals::nodeBorderPadding), wxCoord(topHeight + (globals::nodeBorderThickness * 2)));

    int optionCounter = 0;
    for(Option* opt : options) {
        if(opt->importance) {
            optionCounter++;
            context.DrawText(opt->name + ":" + stringShortener(opt->core->getValue()), wxCoord(globals::nodeBorderThickness + globals::nodeBorderPadding), wxCoord(topHeight + (optionCounter * globals::fontHeight) + (globals::nodeBorderThickness * 2)));
        }
    }

    for(int a = 0; a < int(inputs.size()); a++) {
        context.DrawRotatedText(inputs[a]->name, wxCoord(topStart + (a * globals::diagonalTextDistance) + (globals::nodeBorderThickness * 1) + averageOverhang), wxCoord(0), -globals::diagonalTextAngle);
    }

    for(int a = 0; a < int(outputs.size()); a++) {
        context.DrawRotatedText(outputs[a]->name, wxCoord(bottomStart + (a * globals::diagonalTextDistance) - averageOverhang), wxCoord(totalHeight - (globals::diagonalTextBottomDistance + globals::nodeBorderThickness)), globals::diagonalTextAngle);
    }
}

wxBitmap* Node::getBitmap(){
    if(mustRefresh){
        refreshGraphics();
    }
    return innerBitmap;
}

void Node:: forceRefresh(){
    mustRefresh = true;
}

int Node::getX(){
    return xPos;
}

int Node::getY(){
    return yPos;
}

void Node::setPos(int x, int y){
    xPos = x;
    yPos = y;
    refreshPorts();
}

void Node::portChanged(NodePort* changedPort){
    if(changedPort->type == node_port_type::triggerIn){
        if(changedPort->connected.size() == 0){
            if(triggerIn.size() != 1){
                triggerIn.erase(std::find(triggerIn.cbegin(), triggerIn.cend(), changedPort));
                allPorts.erase(changedPort);
                delete changedPort;
                mustRefresh = true;
            }
        }
        else{
            triggerIn.push_back(new NodePort(node_port_type::triggerIn));
            triggerIn.back()->setParent(this);
            allPorts.insert(triggerIn.back());
            mustRefresh = true;
        }
    }
    else if(changedPort->type == node_port_type::triggerOut){
        if(changedPort->connected.size() == 0){
            if(triggerOut.size() != 1){
                triggerOut.erase(std::find(triggerOut.cbegin(), triggerOut.cend(), changedPort));
                allPorts.erase(changedPort);
                delete changedPort;
                mustRefresh = true;
            }
        }
        else{
            triggerOut.push_back(new NodePort(node_port_type::triggerOut));
            triggerOut.back()->setParent(this);
            allPorts.insert(triggerOut.back());
            mustRefresh = true;
        }
    }
}

std::string* Node::getName(){
    return name;
}

std::string* Node::getDescription(){
    return description;
}

NodeClickEvent Node::click(int x, int y){
    if(mustRefresh){
        refreshGraphics();
    }
    using namespace node_components;
    auto getDist = [](double xa, double xb, double ya, double yb){
        return std::sqrt(std::pow(xa - xb, 2) + std::pow(ya - yb, 2));
    };
    if(x >= -globals::portRadius && y >= -globals::portRadius && x <= width + globals::portRadius && y <= height + globals::portRadius){ // inside the outer node (i.e. with ports)
        if(x >= 0 && y >= 0 && x <= width && y <= height){ // hit in middle
            return NodeClickEvent(node_click_type::middle, NULL);
        }
        else{//otherwise check port hits
            for(Input* input : inputs){
                if(getDist(input->port->x, x, input->port->y, y) <= globals::portRadius){
                    return NodeClickEvent(node_click_type::input, input->port);
                }
            }
            for(Output* output : outputs){
                if(getDist(output->port->x, x, output->port->y, y) <= globals::portRadius){
                    return NodeClickEvent(node_click_type::output, output->port);
                }
            }
            for(NodePort* port : triggerIn){
                if(getDist(port->x, x, port->y, y) <= globals::portRadius){
                    return NodeClickEvent(node_click_type::triggerIn, port);
                }
            }
            for(NodePort* port : triggerOut){
                if(getDist(port->x, x, port->y, y) <= globals::portRadius){
                    return NodeClickEvent(node_click_type::triggerOut, port);
                }
            }
        }
    }
    return NodeClickEvent(node_click_type::none, NULL); // not hitting the inner node or a port
}

std::vector<node_components::Option*> Node::getOptions(){
    return options;
}

int Node::getPortPosition(NodePort* of){
    std::vector<NodePort*>* triggers = NULL;
    int num = 0;
    if(of->getType() == node_port_type::input){
        for(node_components::Input* input : inputs){
            if(input->port == of){
                return num;
            }
            num++;
        }
    }
    else if(of->getType() == node_port_type::output){
        for(node_components::Output* output : outputs){
            if(output->port == of){
                return num;
            }
            num++;
        }
    }
    else if(of->getType() == node_port_type::triggerIn){
        triggers = &triggerIn;
    }
    else{ // type triggerOut assumed
        triggers = &triggerOut;
    }
    for(NodePort* trigger : *triggers){
        if(trigger == of){
            return num;
        }
        num++;
    }
    return -1; // port not part of this node
}

NodePort* Node::getPort(node_port_type::nodePortType type, int number){
    std::vector<NodePort*>* triggers = NULL;
    if(type == node_port_type::input){
        if((int)inputs.size() > number){
            return inputs[number]->port;
        }
    }
    else if(type == node_port_type::output){
        if((int)outputs.size() > number){
            return outputs[number]->port;
        }
    }
    else if(type == node_port_type::triggerIn){
        triggers = &triggerIn;
    }
    else{ // type triggerOut assumed
        triggers = &triggerOut;
    }
    if((int)(*triggers).size() > number){
        return (*triggers)[number];
    }
    return NULL; // port does not exist
}

std::unordered_set<NodePort*> Node::getPorts(){
    if(mustRefresh){ // remember that ports get their positions when the graphics are refreshed
        refreshGraphics();
    }
    return allPorts;
}
