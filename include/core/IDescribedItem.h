#ifndef _IDESCRIBEDITEM_H_
#define _IDESCRIBEDITEM_H_

#include <string>

//! @brief Interface for tree items with a description
class IDescribedItem
{
public:
    IDescribedItem() = default;
    virtual ~IDescribedItem() = default;

    //! @brief Reads the description of this item
    virtual const std::string& GetDescription() const = 0;

    //! @brief Sets the description of this item
    //! @param[in] strNewDesc Assigned description
    virtual void SetDescription(const std::string& strNewDesc) = 0;
};

#endif