#ifndef _CSTOREDDESCRIPTIONITEM_H_
#define _CSTOREDDESCRIPTIONITEM_H_

#include "IDescribedItem.h"
#include <JInterface/JsonSerialization.h>

//! @brief Implementation of IDescribedItem which stores the description as a serializable member
class CStoredDescriptionItem : public virtual IDescribedItem
{
protected:
    //! The display name of the item
    std::string m_strDescription;

    SERIALIZATION_FRIEND(CStoredDescriptionItem);

public:
    CStoredDescriptionItem() = default;
    CStoredDescriptionItem(const std::string& strDesc);
    virtual ~CStoredDescriptionItem() = default;

    //! @copydoc IDescribedItem::GetDescription
    virtual std::string GetDescription() const override;

    //! @copydoc IDescribedItem::SetDescription
    virtual bool SetDescription(const std::string& strNewDesc) override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredDescriptionItem);

#endif