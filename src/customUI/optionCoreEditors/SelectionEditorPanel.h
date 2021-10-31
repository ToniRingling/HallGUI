#ifndef SELECTIONEDITORPANEL_H_INCLUDED
#define SELECTIONEDITORPANEL_H_INCLUDED

#include "../../nodes/optionCores/SelectionCore.h"
#include "OptionEditorPanel.h"

namespace option_editor_panels{
    /** \brief A panel for editing the type-specific options of a SelectionCore, i.e. the possible choices.
     */
    class SelectionEditorPanel : public OptionEditorPanel{
    private:
        wxFlexGridSizer* sizer;/**<The sizer for this panel.*/
        option_cores::SelectionCore* editedCore;/**<The core which we are editing with this panel.*/
        std::vector<std::string> choices;/**<  */
    public:
        /** \brief Creates a panel for editing the choices of a SelectionCore.
         *
         * \param core option_cores::SelectionCore* The SeletionCore for which the choices are to be edited.
         * \param parent wxWindow* The window which contains this panel.
         *
         */
        SelectionEditorPanel(option_cores::SelectionCore* core, wxWindow* parent);
        /** \brief Saves the new choices to the core and adjusts the value to the first element or the empty string if no matching choice exists anymore and more than 0/no elements exist.
         *
         *
         */
        ~SelectionEditorPanel();
        /** \brief
         *
         * \return void
         *
         */
        void rebuild();
        /** \brief Adds a new Input to the node which is being edited. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause a new choice to be created.
         * \return void
         *
         */
        void newChoice(wxCommandEvent& event);
        /** \brief Deletes an Input from the node which is being edited. Caused by button press.
         *
         * \param event wxCommandEvent& The button press which should cause the deletion of an choice. Carries the number of the Input (w.r. to the vector in the core).
         * \return void
         *
         */
        void deleteChoice(wxCommandEvent& event);
    };
}

#endif // SELECTIONEDITORPANEL_H_INCLUDED
