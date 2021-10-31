#ifndef TEXTFIELDCORE_H_INCLUDED
#define TEXTFIELDCORE_H_INCLUDED


#include "OptionCore.h"

//documentation in OptionCore.h
namespace option_cores{
    class TextFieldCore : public OptionCore{
    private:
        std::string value;/**< The current value of this option. */
    public:
        /** \brief Creates the core for the given option.
         *
         * \param shell node_components::Option* The option which is supposed to contain this core.
         *
         */
        TextFieldCore(node_components::Option* shell);
        /** \brief Trivial destructor.
         *
         *
         */
        ~TextFieldCore();
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
        /** \brief Returns the boolean value of this option as a string.
         *
         * \return virtual std::string The value property as a string, i.e. "false" or "true".
         *
         */
        std::string getValue();
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
        /** \brief Reads the configuration of this option core from the given file. This should include the value and as such update the option this belongs to. Details are in TextFieldCore.cpp.
         *
         * \param from std::istream* The istream to read the details to this option core from.
         * \return void
         *
         */
        void read(std::istream* from);
    };
}

#endif // TEXTFIELDCORE_H_INCLUDED
