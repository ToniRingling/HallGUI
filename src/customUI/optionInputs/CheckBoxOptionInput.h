#ifndef CHECKBOXOPTION_H_INCLUDED
#define CHECKBOXOPTION_H_INCLUDED

#include "OptionInputElement.h"

namespace option_cores{
    class CheckBoxCore;
}

// documentation in OptionInputElement.h
namespace option_inputs{
    /** \brief An GUI-Element for changing a node option by unmarking/marking a checkbox. The output is "false" or "true" - strings as to be directly insertible into javaScript.
     */
    class CheckBoxOptionInput : public OptionInputElement, public wxCheckBox{
    private:
        option_cores::CheckBoxCore* core;/**<The core belonging to this option.*/
    public:
        /** \brief
         *
         * \param option node_components::Option* The Option which this GUI element influences.
         * \param signalDestination node_property_pages::NodePropertyPage* The element to notify on changes if the considered option is important.
         * \param Parent wxWindow* The wxWindow which contains this element.
         * \param Id wxWindowID The wxWindowID to assign to this element.
         *
         */
        CheckBoxOptionInput(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination = NULL, wxWindow* Parent = NULL, wxWindowID Id = -1);
        /** \brief Destroys this without further effects.
         *
         *
         */
        ~CheckBoxOptionInput();
        /** \brief Returns the base wxCheckBox.
         *
         * \return wxControl* The base wxCheckBox.
         *
         */
        wxControl* asControl();
        /** \brief Called when the input changes. Saves the input to the core and tries signaling change to the wider GUI.
         *
         * \param e wxCommandEvent& The event which describes the change of the input.
         * \return void
         *
         */
        void onUpdate(wxCommandEvent& e);
    private:
        /** \brief Returns the current value of the checkbox.
         *
         * \return std::string the current value of the checkbox ("true" or "false").
         *
         */
        std::string getValue();
        wxDECLARE_EVENT_TABLE();
    };
}

#endif // CHECKBOXOPTION_H_INCLUDED
