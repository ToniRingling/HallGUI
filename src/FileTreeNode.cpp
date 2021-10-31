#include "FileTreeNode.h"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>

#include "nodes/NodeDescriptor.h"
#include "nodes/NodeFileHandling.h"

namespace{
    #ifdef MSWHALLGUI
        const std::string separator = "\\";
    #elif defined(LINUXHALLGUI)
        const std::string separator = "/";
    #endif
}

FileTreeNode::FileTreeNode(std::string path, FileTreeNode* parent){
    using namespace boost::filesystem;
    description = NULL;
    hidden = false;
    expanded = false;
    name = boost::filesystem::path(path).filename().string();
    for (directory_iterator itr(path); itr!=directory_iterator(); ++itr)
    {
        if(is_regular_file(itr->status())){
            NodeDescriptor* description = node_file_handling::loadDescriptor(itr->path().string());
            children.push_back(new FileTreeNode(description));
        }
        else if(is_directory(itr->status())){
            children.push_back(new FileTreeNode(((wxString)(path + separator + itr->path().filename().string())).mb_str().data(), this));
            // Do not remove the wxString conversion. It is not needed under windows but under unix.
        }
    }
}

FileTreeNode::FileTreeNode(NodeDescriptor* description){
    this->description = description;
    name = description->getName();
    hidden = false;
}

FileTreeNode::~FileTreeNode(){
    for(FileTreeNode* child : children){
        delete child;
    }
    if(description != NULL){
        delete description;
    }
}

bool FileTreeNode::isFile(){
    return description != NULL;
}

bool FileTreeNode::filter(std::string pattern){
    boost::algorithm::to_lower(pattern);
    std::regex reg(pattern);
    if(isFile()){
        std::string toSearch = description->getName() + "\n" + description->getDescription(); // regex_match does not allow usage of temporary strings, so we have to assemble this beforehand.
        boost::algorithm::to_lower(toSearch);
        if(std::regex_search(toSearch, reg)){
            hidden = false;
        }
        else{
            hidden = true;
        }
    }
    else{ // directories are shown exactly if there is at least one visible file in the subtree
        hidden = true;
        for(FileTreeNode* child : children){
            if(child->filter(pattern)){
                hidden = false;
            }
        }
    }
    return !hidden;
}
