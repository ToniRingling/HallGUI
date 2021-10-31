#include "SmallPreview.h"

#include <string>

#include "../nodes/Node.h"

/**
 * As far as I know the wxListCtrl we use here can only hold either an int or a pointer.
 * As we want to order these nodes, the value has been chosen to be the position of the respective element.
 * Which node belongs to which element is kept separately.
 * Whenever we reorder the nodes using the built in sort, their numbering changes and so we must update the mapping from nodes to positions as well.
 */

SmallPreview::SmallPreview(wxWindow* parent,wxWindowID id,const wxPoint& pos, const wxSize& size) : wxPanel(parent, id, pos, size){
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    nodeList = new wxListCtrl(this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL);
    reset();
    sizer->Add(nodeList, 1, wxEXPAND); // the 1 is the proportion and means that it can expand in both directions - specific to box sizers
    wxButton* upButton = new wxButton(this, -1, "Up");
    upButton->Bind(wxEVT_BUTTON, &SmallPreview::pushUp, this);
    wxButton* downButton = new wxButton(this, -1, "Down");
    downButton->Bind(wxEVT_BUTTON, &SmallPreview::pushDown, this);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(upButton);
    buttonSizer->Add(downButton);
    sizer->Add(buttonSizer);
    SetSizerAndFit(sizer);
}

SmallPreview::~SmallPreview(){

}

namespace{
    int wxCALLBACK wxListCompareFunction(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData){
        return item1 - item2;
    }
}

void SmallPreview::pushUp(wxCommandEvent& event){
    if(nodeList->GetSelectedItemCount() == 1){
        int selection =  nodeList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(selection != 0){ // i.e. not uppermost element
            Node* firstMoved = NULL;
            Node* secondMoved = NULL;
            for(std::pair<Node*, int> it : NodeIDs){
                if(it.second == selection){
                    firstMoved = it.first;
                }
                else if(it.second == selection - 1){
                    secondMoved = it.first;
                }
            }
            NodeIDs[firstMoved] = selection - 1;
            NodeIDs[secondMoved] = selection;
            nodeList->SetItemData(selection, selection - 1);
            nodeList->SetItemData(selection - 1, selection);
            nodeList->SortItems(wxListCompareFunction, 0);
        }
    }
}

void SmallPreview::pushDown(wxCommandEvent& event){
    if(nodeList->GetSelectedItemCount() == 1){
        int selection =  nodeList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(nodeList->GetNextItem(selection) != -1){ // i.e. there is no element after/below this
            Node* firstMoved = NULL;
            Node* secondMoved = NULL;
            for(std::pair<Node*, int> it : NodeIDs){
                if(it.second == selection){
                    firstMoved = it.first;
                }
                else if(it.second == selection + 1){
                    secondMoved = it.first;
                }
            }
            NodeIDs[firstMoved] = selection + 1;
            NodeIDs[secondMoved] = selection;
            nodeList->SetItemData(selection, selection + 1);
            nodeList->SetItemData(selection + 1, selection);
            nodeList->SortItems(wxListCompareFunction, 0);
        }
    }
}

void SmallPreview::selectNode(Node* toSelect){
    std::unordered_map<Node*, int>::iterator searcher = NodeIDs.find(toSelect);
    if(searcher != NodeIDs.end()){
        nodeList->SetItemState(searcher->second, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    }
}

void SmallPreview::refreshNode(Node* toRefresh){
    std::unordered_map<Node*, int>::iterator searcher = NodeIDs.find(toRefresh);
    if(searcher != NodeIDs.end()){
        nodeList->SetItemText(searcher->second, *toRefresh->getName());
    }
}

void SmallPreview::addNode(Node* toAdd){
    if(NodeIDs.find(toAdd) == NodeIDs.end()){
        int newPos = NodeIDs.size();
        nodeList->InsertItem(newPos, *toAdd->getName());
        nodeList->SetItemData(newPos, newPos);
        NodeIDs[toAdd] = newPos;
    }
}

void SmallPreview::removeNode(Node* toRemove){
    std::unordered_map<Node*, int>::iterator searcher = NodeIDs.find(toRemove);
    int pos = searcher->second;
    for(std::pair<Node*, int> it : NodeIDs){
        if(nodeList->GetItemData(it.second) > nodeList->GetItemData(pos)){ // set the item data down by one for all nodes which came after in the ordering, so the numbers are contiguous again
            nodeList->SetItemData(it.second, nodeList->GetItemData(it.second) - 1);
        }
        if(it.second > pos){ // also lower position for all nodes which were later in the list
            NodeIDs[it.first] = it.second - 1;
        }
    }
    NodeIDs.erase(toRemove);
    nodeList->DeleteItem(pos);
}

std::map<int, Node*>* SmallPreview::getNodePositions(){
    std::map<int, Node*>* mapping = new std::map<int, Node*>;
    for(std::pair<Node*, int> it : NodeIDs){
        mapping->insert(std::pair<int, Node*>((int)nodeList->GetItemData(it.second), it.first));
    }
    return mapping;
}

void SmallPreview::reset(){
    nodeList->ClearAll();
    nodeList->InsertColumn(0, wxT("Nodes"), wxLIST_FORMAT_LEFT);
    NodeIDs.clear();
}
