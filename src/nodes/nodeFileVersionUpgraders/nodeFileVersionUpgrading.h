#ifndef NODEFILEVERSIONUPGRADING_H_INCLUDED
#define NODEFILEVERSIONUPGRADING_H_INCLUDED

#include <string>

/** \brief This namespace contains various functions which can be used to update old project file contents to newer versions.
 */
namespace node_file_version_upgrading{
    /** \brief Updates the given node file contents if possible.
     *
     * \param toUpdate std::string* The complete contents of the old node file. The contents of this string will be updated by this function.
     * \return bool True exactly if the conversion was successful.
     *
     */
    bool updateNodeFile(std::string* toUpdate);

    /** \brief Updates a node file of the type right before the introduction of named versions. May fail, leaving the input damaged.
     *
     * \param toUpdate std::string* The complete contents of the old node file. The contents of this string will be updated by this function.
     * \return bool True exactly if the conversion was successful.
     *
     */
    bool updatePreVersionNodeFile(std::string* toUpdate);
};

#endif // NODEFILEVERSIONUPGRADING_H_INCLUDED
