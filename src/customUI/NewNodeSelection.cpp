#include "NewNodeSelection.h"

#include "../nodes/NodeFileHandling.h"
#include "../nodes/NodeDescriptor.h"
#include "GraphEditor.h"
#include "../FileTreeNode.h"
#include "NodeDescriptionDisplay.h"

wxDEFINE_EVENT(EVT_NEW_NODE_SELECTION, wxEvent);

wxBEGIN_EVENT_TABLE(NewNodeSelection, wxTreeCtrl)
    EVT_TREE_ITEM_ACTIVATED(wxID_ANY, NewNodeSelection::onActivation)
    EVT_TREE_SEL_CHANGED(wxID_ANY, NewNodeSelection::onSelection)
    EVT_TREE_ITEM_COLLAPSED(wxID_ANY, NewNodeSelection::onCollapse)
    EVT_TREE_ITEM_EXPANDED(wxID_ANY, NewNodeSelection::onExpansion)
wxEND_EVENT_TABLE()

NewNodeSelection::NewNodeSelection(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) : wxTreeCtrl(Parent, Id, Position, Size, Style){
    rebuildTree();
}

NewNodeSelection::~NewNodeSelection(){
    for(NodeDescriptor* desc : descriptions){
        delete desc;
    }
}

void NewNodeSelection::registerGraphEditor(GraphEditor* editor){
    this->usedEditor = editor;
}

void NewNodeSelection::registerDescriptionDisplay(NodeDescriptionDisplay* display){
    this->usedDescriptionDisplay = display;
}


void NewNodeSelection::refreshGUITree(FileTreeNode* toDisplay, wxTreeItemId attachTo){
    // We display all children of toDisplay here - this way there is no special case for the root of the wxTree and the data tree (the data tree root is not displayed).
    for(FileTreeNode* node : toDisplay->children){
        if(!node->hidden){
            if(node->isFile()){
                AppendItem(attachTo, node->name, -1, -1, new TreeNodeHolder(node));
            }
            else{
                wxTreeItemId newParent = AppendItem(attachTo, node->name, -1, -1, new TreeNodeHolder(node));
                refreshGUITree(node, newParent);
                if(node->expanded){
                    Expand(newParent);
                }
            }
        }
    }
}

void NewNodeSelection::setFilter(std::string pattern){
    filter = pattern;
    realRoot->filter(pattern);
    DeleteAllItems();
    wxRoot = AddRoot("Node Prefabs");
    refreshGUITree(realRoot, wxRoot);
    Expand(wxRoot);
}

void NewNodeSelection::rebuildTree(){
    if(realRoot != NULL){
        delete realRoot;
    }

    realRoot = new FileTreeNode("nodePrefabs", NULL);

    setFilter(filter); // This causes the filter to apply again and will also refresh the GUI tree.
}

void NewNodeSelection::onSelection(wxTreeEvent& e){
    TreeNodeHolder* selected = (TreeNodeHolder*)GetItemData(e.GetItem());
    if(selected != NULL){ // On startup and when clicking the root we get NULL.
        if(selected->treeNode->description != NULL){
            NodeDescriptor* data = selected->treeNode->description;
            usedDescriptionDisplay->setContent(data->getName(), data->getDescription());
        }
    }
}

void NewNodeSelection::onActivation(wxTreeEvent& e){
    TreeNodeHolder* activated = (TreeNodeHolder*)GetItemData(e.GetItem());
    if(activated != NULL){ // We get NULL on double-clicking the root.
        if(activated->treeNode->description != NULL){
            usedEditor->onNewNodeInsertion(activated->treeNode->description);
        }
    }
}

void NewNodeSelection::onExpansion(wxTreeEvent& e){
    TreeNodeHolder* expanded = (TreeNodeHolder*)GetItemData(e.GetItem());
    if(expanded != NULL){ // We get NULL when the root is expanded.
        expanded->treeNode->expanded = true;
    }
}

void NewNodeSelection::onCollapse(wxTreeEvent& e){
    ((TreeNodeHolder*)GetItemData(e.GetItem()))->treeNode->expanded = false;
}
