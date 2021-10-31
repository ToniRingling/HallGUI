#include "nodeFileVersionUpgrading.h"

#include <sstream>

#include "../CommonFileHandling.h"

namespace node_file_version_upgrading{
    bool updateNodeFile(std::string* toUpdate){
        std::istringstream stringStream = std::istringstream(*toUpdate);
        std::string* version = common_file_handling::readString(&stringStream);

        bool wasIdentified = false;
        if(*version == "HGNV-0"){
            updateV0NodeFile(toUpdate);
            *version = "HGNV-1";
        }
        if(*version == "HGNV-1"){ // Current version, nothing to do.
            wasIdentified = true;
        }

        delete version;

        if(!wasIdentified){ // Corresponds to no known version string, we assume it is older than the version naming.
            // As such we make it to a HGNV-0 and send it through the version checking again.
            updatePreVersionNodeFile(toUpdate);
            return updateNodeFile(toUpdate);
        }
        return true;
    }
};
