#ifndef NODEPREFABDESCRIPTOR_H_INCLUDED
#define NODEPREFABDESCRIPTOR_H_INCLUDED

#include <wx/treebase.h>
#include <string>

/** \brief Holds the base information about a node, including a path if it corresponds to a .ghn file. Used to create nodes from files.
 */
class NodeDescriptor{
private:
    std::string name;/**<The name of the described node.*/
    std::string description;/**<The description of the described node.*/
    std::string path;/**<The path to the file of the described node (only sensible for unedited nodes loaded from a .ghn).*/

public:
    /** \brief Creates a NodeDescriptor from the given parameters.
     *
     * \param name std::string The name of the described node.
     * \param description std::string The description of the described node.
     * \param path std::string The path to the file of the described node.
     *
     */
    NodeDescriptor(std::string name, std::string description, std::string path);
    /** \brief Destroys the NodeDescriptor.
     *
     *
     */
    ~NodeDescriptor();
    /** \brief Returns the name of the described node.
     *
     * \return std::string The name of the described node.
     *
     */
    std::string getName();
    /** \brief Returns the description of the described node.
     *
     * \return std::string The description of the described node.
     *
     */
    std::string getDescription();
    /** \brief Returns the path to the .ghn corresponding to the described node.
     *
     * \return std::string The path to the .ghn corresponding to the described node.
     *
     */
    std::string getPath();
};

#endif // NODEPREFABDESCRIPTOR_H_INCLUDED
