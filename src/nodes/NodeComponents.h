#ifndef NODECOMPONENTS_H_INCLUDED
#define NODECOMPONENTS_H_INCLUDED

#include <string>
#include <wx/wx.h>
#include <iostream>

#include "NodeFileHandling.h"
#include "../customUI/customDialogs/OptionEditorDialog.h"
#include "NodePort.h"

////FORWARD DECLARATIONS
namespace option_inputs{
    class OptionInputElement;
}
namespace option_cores{
    class OptionCore;
}
namespace node_property_pages{
    class NodePropertyPage;
}
class NodePort;
////

/** \brief Namespace for all node components (i.e. inputs, outputs and options) along with some corresponding enums.
 *
 *
 */
namespace node_components{
    /** \brief Ways of interpretation of HTML elements sizes. Either as an absolute number of pixels or as a percentage of available space.
     */
    enum taskSizeType : int{
        absolute = 0, // in pixels
        relative = 1, // in percent (as is standard for css)
    };

    /** \brief The importance of an option determines whether it is displayed directly on the node. This is the case for important options.
     */
    enum optionImportance : int{
        unimportant = 0,
        important = 1,
    };

    /** \brief Describes the type of a node option, i.e. how the option is put in.
     */
    enum optionType : int{
        invalid = -1,
        selection = 0,
        textInput = 1,
        textAreaInput = 2,
        checkBox = 3,
    };

    /** \brief An input for a node, including the port.
     */
    struct Input{
        std::string name;/**<The name of this Input. Used for display and inside the script.*/
        std::string description;/**<The description of the purpose of this Input. Used to aid the user.*/
        std::string type;/**<The expected data type of the Input values. Used to display warnings on mismatch. "any" for unknown.*/
        NodePort* port;/**<The corresponding port to this Input.*/

        /** \brief Constructs an Input, without making a port, from the given parameters.
         *
         * \param name std::string The name for this Input.
         * \param description std::string The description for this Input.
         * \param type std::string The expected type of the values of this Input.
         *
         */
        Input(std::string name, std::string description, std::string type);
        /** \brief Reads and constructs an Input from a stream, beginning at the position it is at, not rewinding the stream. File structure described in NodeFileHandling. Includes construction of a port.
         *
         * \param from std::istream* The stream to read the data for this Input from.
         *
         */
        Input(std::istream* from);
        /** \brief Destroys this Input along with the assigned port, if it exists.
         *
         *
         */
        ~Input();
        /** \brief Writes the Input to the given ostream as described in NodeFileHandling.
         *
         * \param where std::ostream* The ostream to write this Input to.
         * \return void
         *
         */
        void write(std::ostream* where);
    };





    /** \brief An Output for a node, including the port.
     */
    struct Output{
        std::string name;/**<The name of this Output. Used for display and inside the script.*/
        std::string description;/**<The description of the purpose of this Output. Used to aid the user.*/
        std::string type;/**<The expected data type of the Output values. Used to display warnings on mismatch. "any" accepts all.*/
        NodePort* port;/**<The corresponding port to this Output.*/

        /** \brief Constructs an Output, without making a port, from the given parameters.
         *
         * \param name std::string The name for this Output.
         * \param description std::string The description for this Output.
         * \param type std::string The expected type of the values of this Output.
         *
         */
        Output(std::string name, std::string description, std::string type);
        /** \brief Reads and constructs an Output from a file stream, beginning at the position it is at, not rewinding the stream. File structure described in NodeFileHandling. Includes construction of a port.
         *
         * \param from std::istream* The stream to read the data for this Output from.
         *
         */
        Output(std::istream* from);
        /** \brief Destroys this Output along with the assigned port, if it exists.
         *
         *
         */
        ~Output();
        /** \brief Writes the Output to the given ostream as described in NodeFileHandling.
         *
         * \param where std::ostream* The stream to write this Output to.
         * \return void
         *
         */
        void write(std::ostream* where);
    };



    /** \brief An option for a node. Can be used to create GUI elements to edit it.
     */
    struct Option{
        std::string name;/**<The name of this Option. Used for display, filling out the task and inside the script.*/
        std::string description;/**<The description of the purpose of this Option. Used to aid the user.*/
        node_components::optionImportance importance;/**<Whether this Option is important, i.e. displayed directly on the node.*/
        node_components::optionType type;/**<The type of this Option, i.e. how the user can input the value.*/
        option_cores::OptionCore* core;/**The type-dependent parts of this option.*/

        /** \brief Creates an option with the given parameters.
         *
         * \param name std::string* The name for this option.
         * \param description std::string* The description for this option.
         * \param importance node_components::optionImportance The importance of this option (i.e. whether it will be displayed on the node).
         * \param type node_components::optionType The type of this option.
         *
         */
        Option(std::string name, std::string description, node_components::optionImportance importance, node_components::optionType type);
        /** \brief Reads and constructs an Option from a stream, beginning at the position it is at, not rewinding the stream. File structure described in NodeFileHandling.
         *
         * \param from std::istream* The stream to read the data for this Option from.
         *
         */
        Option(std::istream* from);
        /** \brief Destroys the option including the core and the value.
         *
         *
         */
        ~Option();
        /** \brief Generates a core appropriate to the type. Will delete any old core that may exist.
         *
         * \return void
         *
         */
        void generateCore();
        /** \brief Writes the Option to the given ostream as described in NodeFileHandling.
         *
         * \param where std::oftream* The ostring to write this Option to.
         * \return void
         *
         */
        void write(std::ostream* where);
        /** \brief Returns the value of the option.
         *
         * \return std::string The value of the option as set by the user with the string representation described in the option core.
         *
         */
        std::string getValue();
        /** \brief Creates a GUI element to input the option values with.
         *
         * \param parentWindow wxWindow* The window which should contain this GUI element.
         * \param NULL signalDestination node_property_pages::NodePropertyPage* The element to notify on changes if this option is important.
         * \return option_inputs::OptionInputElement* A GUI element with which values for this option can be inserted.
         *
         */
        option_inputs::OptionInputElement* makeOptionElement(wxWindow* parentWindow, node_property_pages::NodePropertyPage* signalDestination = NULL);
        /** \brief Creates a dialog to configure this option with.
         *
         * \return custom_dialogs::OptionEditorDialog* A dialog which allows editing this option.
         *
         */
        custom_dialogs::OptionEditorDialog* makeOptionEditor();
    };
}

#endif // NODECOMPONENTS_H_INCLUDED
