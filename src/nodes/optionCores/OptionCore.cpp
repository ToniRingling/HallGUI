#include "OptionCore.h"

#include "../../customUI/optionCoreEditors/BlankEditorPanel.h"

using namespace option_cores;

OptionCore::OptionCore(node_components::Option* shell){
    this->shell = shell;
}

OptionCore::~OptionCore(){

}
