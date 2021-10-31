#ifndef NODEPREFABFILEHANDLING_H_INCLUDED
#define NODEPREFABFILEHANDLING_H_INCLUDED

#include <fstream>
#include <string>

#include "../nodes/Node.h"
#include "../nodes/NodeDescriptor.h"

////FORWARD DECLARATIONS
class Node;
////

/** \brief A namespace for everything concerned with handling .ghn files, i.e. reading and writing nodes.
 *
 *
 */
namespace node_file_handling{
    NodeDescriptor* loadDescriptor(std::string path);
    /** \brief Reads a descriptor from the given istream without returning to the starting position.
     *
     * \param from std::istream* The istream to read the descriptor from.
     * \param path std::string The path the file for this istream was from (needed to fill out descriptor).
     * \return NodeDescriptor* The descriptor read from the istream.
     *
     */
    NodeDescriptor* loadDescriptor(std::istream* from, std::string path = "unkownPath");
    /** \brief Loads a full node given a descriptor.
     *
     * \param from std::istream* The istream to read the node from.
     * \return Node* A new Node read from the given istream.
     *
     */
    Node* loadFull(std::istream* from);
    /** \brief Loads a full node given a descriptor.
     *
     * \param desc NodeDescriptor* The descriptor, from the path of which the new node is loaded (which will get a new descriptor).
     * \return Node* A new Node read from the path of the descriptor.
     *
     */
    Node* loadFull(NodeDescriptor* desc);

    // I can't think of a reason to just write a descriptor, so there isn't a function for that.
    /** \brief Writes the given node to a given ostream.
     *
     * \param to std::ostream* The ostream to write the node to.
     * \param toWrite Node* The node to write into the file.
     * \return void
     *
     */
    void writeFull(std::ostream* to, Node* toWrite);
    /** \brief Writes the given node to a file on the given path.
     *
     * \param path std::string The path to write the node to.
     * \param toWrite Node* The node to write into the file.
     * \return void
     *
     */
    void writeFull(std::string path, Node* toWrite);
}

#endif // NODEPREFABFILEHANDLING_H_INCLUDED
