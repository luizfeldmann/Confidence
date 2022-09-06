#ifndef _INAMEDITEM_H_
#define _INAMEDITEM_H_

#include <string>

//! @brief Interface for tree items with a display name
class INamedItem
{
public:
    INamedItem() = default;
    virtual ~INamedItem() = default;

    //! @brief Reads the name of this item
    virtual const std::string& GetName() const = 0;

    //! @brief Sets the name of this item
    //! @param[in] strNewName Assigned name
    virtual void SetName(const std::string& strNewName) = 0;
};

#endif