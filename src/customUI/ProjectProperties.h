#ifndef PROJECTPROPERTIES_H_INCLUDED
#define PROJECTPROPERTIES_H_INCLUDED

#include <wx/wx.h>
#include <string>
#include <utility>
#include <vector>
#include "../Globals.h"

////FORWARD DECLARATIONS
class GraphEditor;
class ProjectProperties;
namespace project_file_handling{
    void loadProjectFromString(std::string*, GraphEditor*, ProjectProperties*);
    void saveProject(std::string path, GraphEditor* editor, ProjectProperties*);
}
namespace node_components{
    class Option;
}
////

/** \brief A GUI element which contains controls to edit all project wide properties.
 */
class ProjectProperties : public wxScrolledWindow{
    friend class TaskAssembly;
    friend void project_file_handling::loadProjectFromString(std::string*, GraphEditor*, ProjectProperties*);
    friend void project_file_handling::saveProject(std::string, GraphEditor*, ProjectProperties*);
private:
    std::string taskName;/**<The name of the current task.*/
    std::string answers;/**<The string containing all answers with amounts of points. Currently the answers for a gap are separated with a newline and for different gaps we use double newlines. Otherwise same syntax as in HallgrimJS, for example "Correct : 5.0P".*/
    globals::userMode mode;/**<The current user mode, according to which this page is built.*/

    wxFlexGridSizer* sizer;/**<The sizer to contain all the editable properties.*/

    std::vector<node_components::Option*> extraOptions;/**<All non-mandatory options for the whole project.*/

public :
    /** \brief Creates this element and all GUI elements needed for editing the project properties.
     *
     * \param Parent wxWindow* The window which contains this gui element.
     * \param Id wxWindowID The id for this gui element.
     * \param Position const wxPoint& The default position of this element.
     * \param Size const wxSize& The default size of this element.
     * \param Style long The style of this element, as in wxScrolledWindow.
     *
     */
    ProjectProperties(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
    /** \brief Destroys this element and all contained parts of the GUI, i.e. the editing controls for the project properties.
     *
     * \return virtual
     *
     */
    virtual ~ProjectProperties();
    /** \brief Rebuild this element, i.e. displays all options and buttons.
     *
     * \return void
     *
     */
    void rebuild();
    /** \brief Resets the project properties to the standard values and removes all non-mandatory options. As such it triggers rebuilding.
     *
     * \return void
     *
     */
    void reset();
    /** \brief Opens a modal dialog to change the name of the task.
     *
     * \param event wxCommandEvent& The event caused by pressing the button for editing the name.
     * \return void
     *
     */
    void changeName(wxCommandEvent& event);
    /** \brief Opens a modal dialog to change the answers to the task.
     *
     * \param event wxCommandEvent& The event caused by pressing the button for editing the answers.
     * \return void
     *
     */
    void changeAnswers(wxCommandEvent& event);
    /** \brief Adds a new Option to the project. Caused by button press.
    *
    * \param event wxCommandEvent& The button press which should cause a new Option to be created.
    * \return void
    *
    */
    void newOption(wxCommandEvent& event);
    /** \brief Deletes an Option from the project. Caused by button press.
     *
     * \param event wxCommandEvent& The button press which should cause the deletion of an Option. Carries the number of the option (w.r. to the vector of all non-mandatory options).
     * \return void
     *
     */
    void deleteOption(wxCommandEvent& event);
    /** \brief Opens a dialog to edit the given option. Caused by button press.
     *
     * \param event wxCommandEvent& The button press which should cause a dialog for editing the option.
     * \return void
     *
     */
    void editOption(wxCommandEvent& event);
    /** \brief Sets the user mode for this UI element.
    *
    * \param mode globals::userMode The user mode to set this element to.
    * \return void
    *
    */
    void setMode(globals::userMode mode);
};

#endif // PROJECTPROPERTIES_H_INCLUDED
