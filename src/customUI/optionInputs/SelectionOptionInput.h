#ifndef SELECTIONOPTIONINPUT_H_INCLUDED
#define SELECTIONOPTIONINPUT_H_INCLUDED

#include "OptionInputElement.h"

namespace option_cores{
    class SelectionCore;
}

// documentation in OptionInputElement.h
namespace option_inputs{
    /** \brief An GUI-Element for changing a node option choosing one predefined value.
     */
    class SelectionOptionInput : public OptionInputElement, public wxChoice{
    private:
        option_cores::SelectionCore* core;/**<The core belonging to this option.*/
    public:
        /** \brief
         *
         * \param option node_components::Option* The Option which this GUI element influences.
         * \param signalDestination node_property_pages::NodePropertyPage* The element to notify on changes if the considered option is important.
         * \param Parent wxWindow* The wxWindow which contains this element.
         * \param Id wxWindowID The wxWindowID to assign to this element.
         *
         */
        SelectionOptionInput(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination = NULL, wxWindow* Parent = NULL, wxWindowID Id = -1);
        /** \brief Destroys this without further effects.
         *
         *
         */
        ~SelectionOptionInput();
        /** \brief Returns the base wxChoice.
         *
         * \return wxControl* The base wxChoice.
         *
         */
        wxControl* asControl();
        /** \brief Called when the input changes. Saves the input to the option and tries signaling change to the wider GUI.
         *
         * \param e wxCommandEvent& The event which describes the change of the input.
         * \return void
         *
         */
        void onUpdate(wxCommandEvent& e);
    private:
        /** \brief Returns the current value of the text input.
         *
         * \return std::string the current value of the text input.
         *
         */
        std::string getValue();
        wxDECLARE_EVENT_TABLE();
    };
}

#endif // SELECTIONOPTIONINPUT_H_INCLUDED
