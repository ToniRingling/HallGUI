#ifndef PROJECTFILEHANDLING_H_INCLUDED
#define PROJECTFILEHANDLING_H_INCLUDED

#include <fstream>

////FORWARD DECLARATIONS
class GraphEditor;
class ProjectProperties;
////

/** \brief Contains functionalities for saving a whole project.
 *
 */
namespace project_file_handling{
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
