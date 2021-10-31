#include "projectFileVersionUpgrading.h"

#include <sstream>

#include "../ProjectFileHandling.h"

namespace project_file_version_upgrading{
    bool updateProjectFile(std::string* toUpdate){
        std::istringstream stringStream = std::istringstream(*toUpdate);
        std::string* version = project_file_handling::readString(&stringStream);

        bool wasIdentified = false;

        if(*version == "HGPV-0"){ // Current version, nothing to do.
            wasIdentified = true;
        }

        delete version;

        if(!wasIdentified){ // Corresponds to no known version string, we assume it is older than the version naming.
            // As such we make it to a HGNV-0 and send it through the version checking again.
            updatePreVersionProjectFile(toUpdate);
            return updateProjectFile(toUpdate);
        }

        return true;
    }
};
