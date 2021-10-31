#ifndef OPTIONINPUTELEMENT_H_INCLUDED
#define OPTIONINPUTELEMENT_H_INCLUDED

#include <wx/wx.h>
#include <string>

////FORWARD DECLARATIONS
namespace node_property_pages{
    class NodePropertyPage;
}
namespace node_components{
    class Option;
}
////

/** \brief Namespace for all classes which deal with GUI parts designed to deal with specific node options.
 *
 */
namespace option_inputs{
    /** \brief An abstract base class for GUI parts which manage options of nodes.
     *
     */
    class OptionInputElement{
    private:
        node_components::Option* baseOption;/**<The option of which this edits the value.*/
    protected:
        node_property_pages::NodePropertyPage* signalDestination;/**<If the considered option is important, this element will be notified on change.*/
    public:
        /** \brief Generates a base OptionInputElement
         *
         * \param baseOption node_components::Option* The option this core is supposed to edit.
         * \param signalDestination node_property_pages::NodePropertyPage* The element to notify on changes if the considered option is important.
         *
         */
        OptionInputElement(node_components::Option* baseOption, node_property_pages::NodePropertyPage* signalDestination = NULL);
        /** \brief Destroys this without further effects.
         *
         * \return virtual
         *
         */
        virtual ~OptionInputElement();
        /** \brief Returns the base wxControl object, which any child class should posess.
         *
         * \return virtual wxControl* The base wxControl object.
         *
         */
        virtual wxControl* asControl();
    protected:
        /** \brief Called by child classes on change of option. Will check for importance and signal if appropriate.
         *
         * \return void
         *
         */
        void signalChange();
    };
}

#endif // OPTIONINPUTELEMENT_H_INCLUDED
