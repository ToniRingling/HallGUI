#ifndef COMMONFILEHANDLING_H_INCLUDED
#define COMMONFILEHANDLING_H_INCLUDED

#include <iostream>
#include <stdint.h>

/** \brief A namespace for basic functions used in for reading and writing different kinds of files this programme works with.
 *
 *
 */
namespace common_file_handling{

    /** \brief Reads an int, as written by writeInt, from the given istream without returning to the starting position.
     *
     * \param readFrom std::istream* The istream to read the int from.
     * \return int32_t The read int.
     *
     */
    int32_t readInt(std::istream* readFrom);
    /** \brief Reads an long, as written by writeLong, from the given ifstream without returning to the starting position.
     *
     * \param readFrom std::istream* The istream to read the long from.
     * \return int64_t The read long.
     *
     */
    int64_t readLong(std::istream* readFrom);
    /** \brief Reads a string, as written by writeString, from the given istream without returning to the starting position.
     *
     * \param readFrom std::istream*
     * \return std::string* The read string.
     *
     */
    std::string* readString(std::istream* readFrom);


    /** \brief Writes an int to the given ostream, such that it can be read by readInt.
     *
     * \param writeTo std::ostream* The ostream to write the int to.
     * \param data int32_t The int to write.
     * \return void
     *
     */
    void writeInt(std::ostream* writeTo, int32_t data);
    /** \brief Writes a long to the given ostream, such that it can be read by readLong.
     *
     * \param writeTo std::ostream* The ostream to write the long to.
     * \param data int64_t The long to write.
     * \return void
     *
     */
    void writeLong(std::ostream* writeTo, int64_t data);
    /** \brief Writes a string to the given ostream as length + string data, such that it can be read by readString.
     *
     * \param writeTo std::ostream* The ostream to write the string to.
     * \param data std::string* The string to write.
     * \return void
     *
     */
    void writeString(std::ostream* writeTo, std::string* data);
}

#endif // COMMONFILEHANDLING_H_INCLUDED
