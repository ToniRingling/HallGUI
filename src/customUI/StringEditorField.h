#ifndef STRINGEDITORFIELD_H_INCLUDED
#define STRINGEDITORFIELD_H_INCLUDED

#include <wx/wx.h>
#include <string>

////FORWARD DECLARATIONS
namespace node_property_pages{
    class NodePropertyPage;
}
////

/** \brief A GUI-Element for editing a single line string from a given pointer and signaling any changes.
 */
class StringEditorField : public wxTextCtrl {
private:
    node_property_pages::NodePropertyPage* signalDestination;/**<This element will be notified on change.*/
    std::string* editing;/**<The string which is being edited by this element.*/
public:
    /** \brief
     *
     * \param toEdit std::string* The string to edit with this element.
     * \param Parent wxWindow* The wxWindow which contains this element.
     * \param signalDestination node_property_pages::NodePropertyPage* The location where any changes will be signaled to.
     *
     */
    StringEditorField(std::string* toEdit, wxWindow* Parent, node_property_pages::NodePropertyPage* signalDestination);
    /** \brief Destroys this without further effects.
     *
     *
     */
    ~StringEditorField();
    /** \brief Called when the input changes. Saves the input to the edited string and tries signaling change to the wider GUI.
     *
     * \param e wxCommandEvent& The event which describes the change of the input.
     * \return void
     *
     */
    void onUpdate(wxCommandEvent& e);
    wxDECLARE_EVENT_TABLE();
};

#endif // STRINGEDITORFIELD_H_INCLUDED
