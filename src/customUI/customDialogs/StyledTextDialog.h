#ifndef STYLEDTEXTDIALOG_H_INCLUDED
#define STYLEDTEXTDIALOG_H_INCLUDED

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <string>

/** \brief Namespace to contain the various dialogs made for Grimhall.
 *
 *
 */
namespace custom_dialogs{
    namespace textStyles{
        enum textStyles : int{
            none = 0,
            javascript = 1,
            markdown = 2,
        };
    }

    /**
    A modal dialog used to edit a given multi-line string with highlighting.
    */
    class StyledTextDialog : public wxDialog{
    private:
        wxBoxSizer* sizer; /**<The sizer to contain the text field and the closing button.*/
        wxStyledTextCtrl* input; /**<The text input for the edited string, initially filled with contents of the edited string.*/
        wxButton* ender; /**<A button which is to be used for closing the dialog - any other closing method will result in the edits being discarded.*/
        std::string* editing; /**The string which we are currently editing.*/
    public:
        /** \brief Creates a StyledTextDialog which edits toEdit and has the given title.
         *
         * \param toEdit std::string* A pointer to the string which is to be edited.
         * \param title std::string The title of the dialog.
         * \param textStyles::textStyles textStyle The style for the highlighting of the input text.
         *
         */
        StyledTextDialog(std::string* toEdit, std::string title, textStyles::textStyles textStyle = textStyles::none);
        /** \brief Destroys the StyledTextDialog and contained GUI elements without saving the inputs.
         *
         *
         */
        ~StyledTextDialog();
        /** \brief Saves the inputs and stop modal execution.
         *
         * \param event wxCommandEvent& The event caused by the click of the confirmation button, signaling the input is to be saved.
         * \return void
         *
         */
        void exitDialog(wxCommandEvent& event);
    };
}

#endif // STYLEDTEXTDIALOG_H_INCLUDED
