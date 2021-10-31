#include "NodePort.h"

#include <algorithm>

NodePort::NodePort(node_port_type::nodePortType type){
    this->type = type;
    output = NULL;
    parent = NULL;
}

NodePort::NodePort(node_components::Output* output){
    this->type = node_port_type::output;
    this->output = output;
    parent = NULL;
}

NodePort::~NodePort(){
    parent = NULL;
    while(!connected.empty()){ // iterating through the set normally while removing the contents (as is done by deleting the connections) causes errors with some environments (it continues iterating even after the list is empty - crashes result)
        delete (*connected.begin());
    }
}

void NodePort::setParent(Node* parent){
    this->parent = parent;
};

Node* NodePort::getParent(){
    return parent;
}

node_port_type::nodePortType NodePort::getType(){
    return type;
}

bool NodePort::isVacant(){
    return (connected.size() == 0);
}

void NodePort::disconnect(NodeConnection* connection){
    if(std::find(connected.begin(), connected.end(), connection) != connected.end()){
        connected.erase(connection);
        if((type == node_port_type::triggerIn || type == node_port_type::triggerOut) && parent != NULL){
            parent->portChanged(this);
        }
    }
}

void NodePort::connect(NodeConnection* connection){
    bool replaced = false;
    if((type != node_port_type::output) && (connected.size() != 0)){
        connected.clear();
        replaced = true;
    }
    connected.insert(connection);
    if((type == node_port_type::triggerIn || type == node_port_type::triggerOut) && (!replaced) && (parent != NULL)){
        parent->portChanged(this);
    }
}

NodeConnection* NodePort::getConnection(){
    if(!isVacant()){
        return *connected.begin();
    }
    else{
        return NULL;
    }
}

void NodePort::refresh(){
    for(NodeConnection* connection : connected){
        connection->refresh();
    }
}

int NodePort::getX(){
    return x + parent->getX();
}

int NodePort::getY(){
    return y  + parent->getY();
}
