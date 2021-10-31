#ifndef TEXTFIELDDIALOG_H_INCLUDED
#define TEXTFIELDDIALOG_H_INCLUDED

#include <wx/wx.h>
#include <string>

namespace custom_dialogs{
    /**
    A modal dialog used to edit a given single line string.
    */
    class TextFieldDialog : public wxDialog{
    private:
        wxBoxSizer* sizer; /**<The sizer to contain the text field and the closing button.*/
        wxTextCtrl* input; /**<The text input for the edited string, initially filled with contents of the edited string.*/
        wxButton* ender; /**<A button which is to be used for closing the dialog - any other closing method will result in the edits being discarded.*/
        std::string* editing; /**The string which we are currently editing.*/
    public:
        /** \brief Creates a TextFieldDialog which edits toEdit and has the given title.
         *
         * \param toEdit std::string* A pointer to the string which is to be edited.
         * \param title std::string The title of the dialog.
         *
         */
        TextFieldDialog(std::string* toEdit, std::string title);
        /** \brief Destroys the TextFieldDialog and contained GUI elements without saving the inputs.
         *
         *
         */
        ~TextFieldDialog();
        /** \brief Saves the inputs and stop modal execution.
         *
         * \param event wxCommandEvent& The event caused by the click of the confirmation button, signaling the input is to be saved.
         * \return void
         *
         */
        void exitDialog(wxCommandEvent& event);
    };
}

#endif // TEXTFIELDDIALOG_H_INCLUDED
