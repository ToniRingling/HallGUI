#ifndef NODEPREFABFILEHANDLING_H_INCLUDED
#define NODEPREFABFILEHANDLING_H_INCLUDED

#include <fstream>
#include <string>
#include <stdint.h>

#include "Node.h"
#include "NodeDescriptor.h"

////FORWARD DECLARATIONS
class Node;
////

/** \brief A namespace for everything concerned with handling .ghn files, i.e. reading and writing nodes.
 *
 *
 */
namespace node_file_handling{

    /** \brief Reads an int, as written by writeInt, from the given istream without returning to the starting position.
     *
     * \param readFrom std::istream* The istream to read the int from.
     * \return int32_t The read int.
     *
     */
    int32_t readInt(std::istream* readFrom);
    /** \brief Reads a string, as written by writeString, from the given istream without returning to the starting position.
     *
     * \param readFrom std::istream*
     * \return std::string* The read string.
     *
     */
    std::string* readString(std::istream* readFrom);
    /** \brief Reads the node descriptor from the .ghn on the given path.
     *
     * \param path std::string The path of the .ghn to get the descriptor from.
     * \return NodeDescriptor* The descriptor to the node in the given file.
     *
     */
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


    /** \brief Writes an int to the given ostream, such that it can be read by readInt.
     *
     * \param writeTo std::ostream* The ostream to write the int to.
     * \param data int32_t The int to write.
     * \return void
     *
     */
    void writeInt(std::ostream* writeTo, int32_t data);
    /** \brief Writes a string to the given ostream as length + string data, such that it can be read by readString.
     *
     * \param writeTo std::ostream* The ostream to write the string to.
     * \param data std::string* The string to write.
     * \return void
     *
     */
    void writeString(std::ostream* writeTo, std::string* data);
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
