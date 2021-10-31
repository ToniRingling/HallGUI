#ifndef SELECTIONCORE_H_INCLUDED
#define SELECTIONCORE_H_INCLUDED

#include <vector>

#include "OptionCore.h"

//documentation in OptionCore.h
namespace option_cores{
    class SelectionCore : public OptionCore{
    private:
        std::string value;/**<The current value of this option.*/
        std::vector<std::string> choices;/**<The possible values for this option.*/
    public:
        /** \brief Creates the core for the given option.
         *
         * \param shell node_components::Option* The option which is supposed to contain this core.
         *
         */
        SelectionCore(node_components::Option* shell);
        /** \brief Trivial destructor.
         *
         *
         */
        ~SelectionCore();
        /** \brief Creates a GUI element to input the option values with.
         *
         * \param parentWindow wxWindow* The window which should contain this GUI element.
         * \param NULL signalDestination node_property_pages::NodePropertyPage* The element to notify on changes if the considered option is important.
         * \return option_inputs::OptionInputElement* A GUI element, containing a text field, with which values for the corresponding option can be given.
         *
         */
        option_inputs::OptionInputElement* makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination = NULL);
        /** \brief Creates a dialog to configure the this option core with.
         *
         * \param parentWindow wxWindow* The window which should contain the editing panel.
         * \return option_editor_panels::OptionEditorPanel* A panel which allows editing the details of the this option core.
         *
         */
        option_editor_panels::OptionEditorPanel* makeOptionEditor(wxWindow* parentWindow);
        /** \brief Adds the given choice to the core.
         *
         * \param choice std::string The choice to add to the core.
         * \return void
         *
         */
        void addChoice(std::string choice);
        /** \brief Removes all choices from this core.
         *
         * \return void
         *
         */
        void removeAllChoices();
        /** \brief Returns the boolean value of this option as a string.
         *
         * \return virtual std::string The value property as a string, i.e. "false" or "true".
         *
         */
        std::string getValue();
        /** \brief Returns all choices open for selection.
         *
         * \return std::vector<std::string> A vector containing all choices in this selection.
         *
         */
        std::vector<std::string> getChoices();
        /** \brief Sets the value to the given string.
         *
         * \param to std::string The new value for this core. Should be one line only.
         * \return void
         *
         */
        void setValue(std::string to);
        /** \brief Writes the OptionCore to the given ostream as described in NodeFileHandling and at the top of TextFieldCore.cpp.
         *
         * \param where std::ostream* The ostream to write this OptionCore to.
         * \return void
         *
         */
        void write(std::ostream* where);
        /** \brief Reads the configuration of this option core from the given stream. This should include the value and as such update the option this belongs to. Details are in TextFieldCore.cpp.
         *
         * \param from std::istream* The istream to read the details to this option core from.
         * \return void
         *
         */
        void read(std::istream* from);
    };
}

#endif // SELECTIONCORE_H_INCLUDED
