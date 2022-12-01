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
    //! Type of collection used to store the items
    using vstore_t = vec_ptr_t;

    //! Stores the subitems of this tree item
    vstore_t m_vItems;

    //! The types of children that may be added as children to this object
    ETreeItemType m_eSupportedItemTypes;

    SERIALIZATION_FRIEND(CStoredItemCollection);

    //! @brief Finds an item in this collection and returns it's iterator
    vstore_t::iterator find(const IProjTreeItem&);

public:
    //! @brief Initializes a new instance with support for the given types of subitems
    //! @param[in] eSupportedSubitems @copybrief m_eSupportedItemTypes
    CStoredItemCollection(ETreeItemType eSupportedSubitems);

    //! @brief Default move-constructor
    CStoredItemCollection(CStoredItemCollection&&) = default;

    //! @brief Default move-assignment operator
    CStoredItemCollection& operator=(CStoredItemCollection&&) = default;

    //! @copydoc ITreeItemCollection::SubItems
    vec_ptr_t SubItems() override;

    //! @copydoc ITreeItemCollection::SubItems
    vec_cptr_t SubItems() const override;

    //! @copydoc ITreeItemCollection::TakeItem
    ptr_t TakeItem(const IProjTreeItem& rItem) override;

    //! @copydoc ITreeItemCollection::AddItem
    bool AddItem(ptr_t pNewItem) override;

    //! @copydoc ITreeItemCollection::SwapItems
    bool SwapItems(const IProjTreeItem& rA, const IProjTreeItem& rB) override;

    //! @copydoc ITreeItemCollection::GetSupportedChildTypes
    ETreeItemType GetSupportedChildTypes() const override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredItemCollection);

#endif