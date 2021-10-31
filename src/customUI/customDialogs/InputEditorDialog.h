#ifndef INPUTEDITORDIALOG_H_INCLUDED
#define INPUTEDITORDIALOG_H_INCLUDED

#include <wx/wx.h>
#include <wx/stc/stc.h>

////FORWARD DECLARATIONS
namespace node_components{
    class Input;
}
////

namespace custom_dialogs{
    /**
    A modal dialog used to edit a given Node-Input.
    */
    class InputEditorDialog : public wxDialog{
    private:
        node_components::Input* editedInput;/**<The Input which is being edited here.*/
        wxTextCtrl* nameInput;/**<An input field for entering the name of the Input.*/
        wxTextCtrl* typeInput;/**<An input field for entering the expected data type of the Input.*/
        wxStyledTextCtrl* descriptionInput;/**<An input area for entering the description of the Input.*/
    public:
        /** \brief Creates the dialog for editing the given Input.
         *
         * \param toEdit node_components::Input* The Input to edit.
         *
         */
        InputEditorDialog(node_components::Input* toEdit);
        /** \brief Destroys the InputEditorDialog and contained GUI elements. The inputs will always be retained.
         *
         *
         */
        ~InputEditorDialog();
        /** \brief Stop modal execution.
         *
         * \param event wxCommandEvent& The event caused by clicking the button to end the dialog.
         * \return void
         *
         */
        void exitDialog(wxCommandEvent& event);
    };
}

#endif // INPUTEDITORDIALOG_H_INCLUDED
