#ifndef _IFILEOPERATIONCONTEXT_H_
#define _IFILEOPERATIONCONTEXT_H_

#include "EFileOperationHandlerType.h"
#include "IFileDataLocator.h"

//! @brief Performs structured R/W operations on supported file formats
class IFileOperatorContext
{
public:
    virtual ~IFileOperatorContext() = default;

    //! @brief Writes data to the file on the provided resource locator
    //! @param[in] rLocator Describes where in the file structure the value will be written
    //! @param[in] strValue The data to write
    //! @returns True if success
    virtual bool Write(const IFileDataLocator& rLocator, const std::string& strValue) = 0;

    //! @brief Read data from the file on the requested resource locator
    //! @param[in] rLocator Describes where in the file structure the value will be read
    //! @param[out] strValue Receives the read data
    //! @returns True if success
    virtual bool Read(const IFileDataLocator& rLocator, std::string& strValue) = 0;

    //! @brief Reflects the object type
    virtual EFileOperationHandlerType GetType() const = 0;
};

#endif