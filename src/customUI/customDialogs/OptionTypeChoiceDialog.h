#ifndef OPTIONTYPECHOICEDIALOG_H_INCLUDED
#define OPTIONTYPECHOICEDIALOG_H_INCLUDED

#include <wx/wx.h>
#include <vector>

////FORWARD DECLARATIONS
namespace node_components{
    enum optionType : int;
}
////

namespace custom_dialogs{
    /**
    A modal dialog used to choose the type for a new option core.
    */
    class OptionTypeChoiceDialog : public wxDialog{
    private:
        node_components::optionType* editedType;/**<The option type to be set by this dialog.*/
    public:
        /** \brief Creates the dialog for choosing a type of option core.
         *
         * \param toSet node_components::optionType* The optionType to set according to the users choice. node_components::invalid will be returned if no choice is made.
         *
         */
        OptionTypeChoiceDialog(node_components::optionType* toSet);
        /** \brief Destroys the OptionTypeChoiceDialog and contained GUI elements.
         *
         *
         *
         */
       ~OptionTypeChoiceDialog();
        /** \brief Caused by selecting a type. Sets the type and ends this dialog.
         *
         * \param event wxCommandEvent& The event caused by the press of a selection button.
         * \return void
         *
         */
        void chooseType(wxCommandEvent& event);
        /** \brief Stop modal execution.
         *
         * \param event wxCommandEvent& The event caused by clicking the button to end the dialog.
         * \return void
         *
         */
        void exitDialog(wxCommandEvent& event);
    };
};

#endif // OPTIONTYPECHOICEDIALOG_H_INCLUDED
