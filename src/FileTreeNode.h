#ifndef FILETREENODE_H_INCLUDED
#define FILETREENODE_H_INCLUDED

#include <vector>
#include <string>

////FORWARD DECLARATIONS
class NodeDescriptor;
////

/** \brief A tree node for representing directory/file structures which allows for filtering. Intended for insertable nodes.
*/
struct FileTreeNode{
    bool hidden;/**<Whether this node is to be shown. Determined by filtering.*/
    bool expanded;/**<Whether this node is expanded or not. Only sensible for directories.*/
    std::vector<FileTreeNode*> children;/**<The children of this node.*/
    std::string name;/**<The display name which will be visible to the user.*/
    NodeDescriptor* description;/**<The description of the contained node, NULL if this is a directory.*/

    /** \brief
     *
     * \param path std::string
     * \param parent FileTreeNode*
     *
     */
    FileTreeNode(std::string path, FileTreeNode* parent);
    /** \brief
     *
     * \param description NodeDescriptor*
     *
     */
    FileTreeNode(NodeDescriptor* description);
    /** \brief Destroys this node and all nodes below it recursively. Also destroys the descriptors in this subtree.
     *
     *
     */
    ~FileTreeNode();
    /** \brief Checks whether this is a file and not a directory.
     *
     * \return bool True exactly if this is a file.
     *
     */
    bool isFile();
    /** \brief Filters this node and all nodes below it according to the given regex pattern. Matching files and all directories which contain a matching file will not be hidden, the rest will be. Both the regex and the searched content will be set to lower case before searching.
     *
     * \param pattern std::string The regex which some substring of the descriptor name plus a line break plus the descriptor description must match for visibility.
     * \return bool True exactly if this node is to be shown.
     *
     */
    bool filter(std::string pattern);
};

#endif // FILETREENODE_H_INCLUDED
