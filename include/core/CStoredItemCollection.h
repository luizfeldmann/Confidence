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
    using vstore_t = std::vector<ptr_t>;

    //! Stores the subitems of this tree item
    vstore_t m_vItems;

    //! The types of children that may be added to this object
    ETreeItemType m_eSupportedItemTypes;

    SERIALIZATION_FRIEND(CStoredItemCollection);

    vstore_t::iterator find(const IProjTreeItem&);

public:
    //! @brief Initializes a new instance with support for the given types of subitems
    CStoredItemCollection(ETreeItemType eSupportedSubitems);

    CStoredItemCollection(CStoredItemCollection&&) = default;
    CStoredItemCollection& operator=(CStoredItemCollection&&) = default;

    //! @copydoc ITreeItemCollection::SubItems
    vec_ref_t SubItems() override;

    //! @copydoc ITreeItemCollection::SubItems
    vec_cref_t SubItems() const override;

    //! @copydoc ITreeItemCollection::TakeItem
    ptr_t TakeItem(const IProjTreeItem& rItem) override;

    //! @copydoc ITreeItemCollection::AddItem
    bool AddItem(IProjTreeItem* pNewItem) override;

    //! @copydoc ITreeItemCollection::SwapItems
    bool SwapItems(const IProjTreeItem& rA, const IProjTreeItem& rB) override;

    //! @copydoc ITreeItemCollection::GetSupportedChildTypes
    ETreeItemType GetSupportedChildTypes() const override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredItemCollection);

#endif