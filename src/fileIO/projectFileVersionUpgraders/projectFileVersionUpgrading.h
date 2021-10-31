#ifndef PROJECTFILEVERSIONUPGRADING_H_INCLUDED
#define PROJECTFILEVERSIONUPGRADING_H_INCLUDED

#include <string>

/** \brief This namespace contains various functions which can be used to update old project file contents to newer versions.
 */
namespace project_file_version_upgrading{
    /** \brief Updates the given project file contents if possible.
     *
     * \param toUpdate std::string* The complete contents of the old project file. The contents of this string will be updated by this function.
     * \return bool True exactly if the conversion was successful.
     *
     */
    bool updateProjectFile(std::string* toUpdate);

    /** \brief Updates a project file of the type right before the introduction of named versions. May fail, leaving the input damaged.
     *
     * \param toUpdate std::string* The complete contents of the old project file. The contents of this string will be updated by this function.
     * \return bool True exactly if the conversion was successful.
     *
     */
    bool updatePreVersionProjectFile(std::string* toUpdate);

    /** \brief Updates a project file of version HGPV-0 to HGPV-1. May fail, leaving the input damaged.
     *
     * \param toUpdate std::string* The complete contents of the old project file. The contents of this string will be updated by this function.
     * \return bool True exactly if the conversion was successful.
     *
     */
    bool updateV0ProjectFile(std::string* toUpdate);
};

#endif // PROJECTFILEVERSIONUPGRADING_H_INCLUDED
