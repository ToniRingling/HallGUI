#ifndef OUTPUTEDITORDIALOG_H_INCLUDED
#define OUTPUTEDITORDIALOG_H_INCLUDED

#include <wx/wx.h>
#include <wx/stc/stc.h>

////FORWARD DECLARATIONS
namespace node_components{
    class Output;
}
////

namespace custom_dialogs{
    /**
    A modal dialog used to edit a given Node-Output.
    */
    class OutputEditorDialog : public wxDialog{
    private:
        node_components::Output* editedOutput;/**<The Output which is being edited here.*/
        wxTextCtrl* nameInput;/**<An input field for entering the name of the Output.*/
        wxTextCtrl* typeInput;/**<An input field for entering the expected data type of the Output.*/
        wxStyledTextCtrl* descriptionInput;/**<An input area for entering the description of the Output.*/
    public:
        /** \brief Creates the dialog for editing the given Ouput.
         *
         * \param toEdit node_components::Ouput* The Ouput to edit.
         *
         */
        OutputEditorDialog(node_components::Output* toEdit);
        /** \brief Destroys the OutputEditorDialog and contained GUI elements. The inputs will always be retained.
         *
         *
         */
        ~OutputEditorDialog();
        /** \brief Stop modal execution.
         *
         * \param event wxCommandEvent& The event caused by clicking the button to end the dialog.
         * \return void
         *
         */
        void exitDialog(wxCommandEvent& event);
    };
}

#endif // OUTPUTEDITORDIALOG_H_INCLUDED
