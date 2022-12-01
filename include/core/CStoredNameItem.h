#ifndef _CSTOREDNAMEITEM_H_
#define _CSTOREDNAMEITEM_H_

#include "INamedItem.h"
#include <JInterface/JsonSerialization.h>

//! @brief Implementation of INamedItem which stores the name as a serializable member
class CStoredNameItem : virtual public INamedItem
{
protected:
    //! The display name of the item
    std::string m_strName;

    SERIALIZATION_FRIEND(CStoredNameItem);

public:
    //! @brief Constructs an item without a name
    CStoredNameItem() = default;

    //! @brief Initializes the item with the provided name
    //! @param[in] strName @copybrief m_strName
    CStoredNameItem(const std::string& strName);

    virtual ~CStoredNameItem() = default;

    //! @copydoc INamedItem::GetName
    virtual std::string GetName() const override;

    //! @copydoc INamedItem::SetName
    virtual bool SetName(const std::string& strNewName) override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredNameItem);

#endif