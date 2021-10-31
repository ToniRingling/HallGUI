#ifndef PROJECTFILEHANDLING_H_INCLUDED
#define PROJECTFILEHANDLING_H_INCLUDED

#include <fstream>
#include <stdint.h>

////FORWARD DECLARATIONS
class GraphEditor;
class ProjectProperties;
////

/** \brief Contains functionalities for saving a whole project.
 *
 */
namespace project_file_handling{
    /** \brief Reads an long, as written by writeLong, from the given ifstream without returning to the starting position.
     *
     * \param readFrom std::istream* The istream to read the long from.
     * \return int64_t The read long.
     *
     */
    int64_t readLong(std::istream* readFrom); // should probably look into making a general template for primitive data
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
    /** \brief Restes the project to the default project.
     *
     * \param editor GraphEditor* The graph editor which is to be reset.
     * \param properties ProjectProperties* The project properties which are to be reset.
     * \return void
     *
     */
    void resetProject(GraphEditor* editor, ProjectProperties* properties);
    /** \brief Loads a project from the given path into the given editor
     *
     * \param path std::string The path to read the project from.
     * \param editor GraphEditor* The graph editor in the UI, where the project is loaded to.
     * \param properties ProjectProperties* The container for the properties of the whole project, where data concerning it is to be written to.
     * \return void
     *
     */
    void loadProjectFromFile(std::string path, GraphEditor* editor, ProjectProperties* properties);
    /** \brief Loads a project from the given file contents into the given editor
     *
     * \param content std::string The contents of the project file to read the project from.
     * \param editor GraphEditor* The graph editor in the UI, where the project is loaded to.
     * \param properties ProjectProperties* The container for the properties of the whole project, where data concerning it is to be written to.
     * \return void
     *
     */
    void loadProjectFromString(std::string* content, GraphEditor* editor, ProjectProperties* properties);
    /** \brief Writes a long to the given ostream, such that it can be read by readLong.
     *
     * \param writeTo std::ostream* The ostream to write the long to.
     * \param data int64_t The long to write.
     * \return void
     *
     */
    void writeLong(std::ostream* writeTo, int64_t data);
    /** \brief Writes an int to the given ostream, such that it can be read by readLong.
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
    /** \brief Saves the project in the editor to the given path.
     *
     * \param path std::string The path to write the project to.
     * \param editor GraphEditor* The graph editor in the UI, where the project is taken from.
     * \param properties ProjectProperties* The container for the properties of the whole project, where data concerning it is to be taken from.
     * \return void
     *
     */
    void saveProject(std::string path, GraphEditor* editor, ProjectProperties* properties);
}

#endif // PROJECTFILEHANDLING_H_INCLUDED
