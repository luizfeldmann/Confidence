#ifndef _IOUTPUTFILENAME_H_
#define _IOUTPUTFILENAME_H_

#include <string>

//! @brief Common interface for classes having an output file name
class IOutputFileName
{
public:
    IOutputFileName() = default;
    virtual ~IOutputFileName() = default;

    //! @brief Reads the output file name
    virtual std::string GetOutputFileName() const = 0;

    //! @brief Sets the new output file name
    virtual void SetOutputFileName(const std::string&) = 0;
};

#endif