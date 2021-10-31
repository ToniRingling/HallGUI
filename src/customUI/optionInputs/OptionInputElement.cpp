#include "OptionInputElement.h"

#include "../nodePropertyPages/NodePropertyPage.h"
#include "../../nodes/NodeComponents.h"

using namespace option_inputs;

OptionInputElement::OptionInputElement(node_components::Option* option, node_property_pages::NodePropertyPage* signalDestination){
    this->baseOption = option;
    this->signalDestination = signalDestination;
}

OptionInputElement::~OptionInputElement(){

}

wxControl* OptionInputElement::asControl(){
    return NULL;
}

void OptionInputElement::signalChange(){
    if(baseOption->importance == node_components::important && signalDestination != NULL){
        signalDestination->onImportantChange();
    }
}
