#ifndef OPTIONCORE_H_INCLUDED
#define OPTIONCORE_H_INCLUDED

#include <wx/wx.h>

////FORWARD DECLARATIONS
namespace node_property_pages{
    class NodePropertyPage;
}
namespace option_inputs{
    class OptionInputElement;
}
namespace option_editor_panels{
    class OptionEditorPanel;
}
namespace node_components{
    class Option;
}
////

/** \brief Namespace for all option cores.
 *
 *
 */
namespace option_cores{
    /** \brief An abstract class used to represent the different types of inputs and configurations which different option types allow.
     */
    class OptionCore{
    protected:
        node_components::Option* shell;/**<The option which is supposed to contain this core.*/
    public:
        /** \brief Creates an empty OptionCore.
         *
         * \param shell node_components::Option* The option which is supposed to contain this core.
         *
         */
        OptionCore(node_components::Option* shell);
        /** \brief Trivial destructor.
         *
         * \return virtual
         *
         */
        virtual ~OptionCore();
        /** \brief Creates a GUI element to input the option values with.
         *
         * \param parentWindow wxWindow* The window which should contain this GUI element.
         * \param NULL signalDestination node_property_pages::NodePropertyPage* The element to notify on changes if the considered option is important.
         * \return option_inputs::OptionInputElement* A GUI element with which values for the corresponding option can be inserted.
         *
         */
        virtual option_inputs::OptionInputElement* makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination = NULL) = 0;
        /** \brief Creates a dialog to configure the this option core with.
         *
         * \param parentWindow wxWindow* The window which should contain the editing panel.
         * \return option_editor_panels::OptionEditorPanel* A panel which allows editing the details of the this option core.
         *
         */
        virtual option_editor_panels::OptionEditorPanel* makeOptionEditor(wxWindow* parentWindow) = 0;
        /** \brief Returns the value of this option.
         *
         * \return virtual std::string The value of this option, i.e. exactly what should be inserted into the task.
         *
         */
        virtual std::string getValue() = 0;
        /** \brief Writes the OptionCore to the given ostream as described in NodeFileHandling and the implementations of the cores.
         *
         * \param where std::ostream* The ostream to write this OptionCore to.
         * \return void
         *
         */
        virtual void write(std::ostream* where) = 0;
        /** \brief Reads the configuration of this option core from the given stream. This should include the value and as such update the option this belongs to.
         *
         * \param from std::istream* The istream to read the details to this option core from.
         * \return void
         *
         */
        virtual void read(std::istream* from) = 0;
    };
}

#endif // OPTIONCORE_H_INCLUDED
