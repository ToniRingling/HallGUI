#include "nodeFileVersionUpgrading.h"

#include <sstream>

#include "../NodeFileHandling.h"

namespace node_file_version_upgrading{
    bool updateNodeFile(std::string* toUpdate){
        std::istringstream stringStream = std::istringstream(*toUpdate);
        std::string* version = node_file_handling::readString(&stringStream);

        bool wasIdentified = false;

        if(*version == "HGNV-0"){ // Current version, nothing to do.
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
