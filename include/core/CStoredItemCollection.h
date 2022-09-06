#ifndef _CSTOREDITEMCOLLECTION_H_
#define _CSTOREDITEMCOLLECTION_H_

#include "ITreeItemCollection.h"
#include <JInterface/JsonSerialization.h>

//! @brief Implementation of ITreeItemCollection with member storage of the contained items
class CStoredItemCollection : public virtual ITreeItemCollection
{
private:
    CStoredItemCollection(const CStoredItemCollection&) = delete;
    CStoredItemCollection& operator=(const CStoredItemCollection&) = delete;

protected:
    //! Stores the subitems of this tree item
    VecPtrItem m_vItems;

    //! The types of children that may be added to this object
    ETreeItemType m_eSupportedItemTypes;

    SERIALIZATION_FRIEND(CStoredItemCollection);

public:
    //! @brief Initializes a new instance with support for the given types of subitems
    CStoredItemCollection(ETreeItemType eSupportedSubitems);

    CStoredItemCollection(CStoredItemCollection&&) = default;
    CStoredItemCollection& operator=(CStoredItemCollection&&) = default;

    //! @copydoc ITreeItemCollection::GetItems
    OptVecPtrItem GetItems() override;

    //! @copydoc ITreeItemCollection::GetItems
    OptCVecPtrItem GetItems() const override;

    //! @copydoc ITreeItemCollection::GetSupportedChildTypes
    ETreeItemType GetSupportedChildTypes() const override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredItemCollection);

#endif