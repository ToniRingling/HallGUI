#ifndef OPTIONEDITORDIALOG_H_INCLUDED
#define OPTIONEDITORDIALOG_H_INCLUDED

#include <wx/wx.h>
#include <wx/stc/stc.h>

////FORWARD DECLARATIONS
namespace node_components{
    class Option;
}
////

namespace custom_dialogs{
    /**
    A modal dialog used to edit a given Node-Option.
    */
    class OptionEditorDialog : public wxDialog{
    private:
        node_components::Option* editedOption;/**<The Option which is being edited here.*/
        wxTextCtrl* nameInput;/**<An input field for entering the name of the Option.*/
        wxStyledTextCtrl* descriptionInput;/**<An input area for entering the description of the Option.*/
        wxCheckBox* importanceInput;/**<A checkbox for entering the importance of this option.*/
    public:
        /** \brief Creates the dialog for editing the given Option including changing the type.
         *
         * \param toEdit node_components::Option* The Option to edit.
         *
         */
        OptionEditorDialog(node_components::Option* toEdit);
        /** \brief Destroys the OptionEditorDialog and contained GUI elements. The inputs will always be retained.
         *
         *
         *
         */
        ~OptionEditorDialog();
        /** \brief A resizing handler which is called whenever the panel for type-specific option resizes. That panel is the only one which may change, so we only have to watch it.
         *
         * \param event wxSizeEvent& The resizing event which caused this handler to be called.
         * \return void
         *
         */
        void onSpecOptSize(wxSizeEvent& event);
        /** \brief Stop modal execution.
         *
         * \param event wxCommandEvent& The event caused by clicking the button to end the dialog.
         * \return void
         *
         */
        void exitDialog(wxCommandEvent& event);
    };
}

#endif // OPTIONEDITORDIALOG_H_INCLUDED
