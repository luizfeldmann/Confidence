#ifndef _ICHILDRENCOLLECTION_H_
#define _ICHILDRENCOLLECTION_H_

#include <memory>
#include <vector>

#include "ETreeItemType.h"

class IProjTreeItem;

//! @brief Interface for collections of tree items
class ITreeItemCollection
{
public:
    ITreeItemCollection() = default;
    virtual ~ITreeItemCollection() = default;

    using ptr_t = std::shared_ptr<IProjTreeItem>;
    using cptr_t = std::shared_ptr<const IProjTreeItem>;
    using vec_ptr_t = std::vector<ptr_t>;
    using vec_cptr_t = std::vector<cptr_t>;

    //! @brief Gets a vector of references to the subitems
    virtual vec_ptr_t SubItems() = 0;

    //! @brief Gets a vector of references to the subitems
    virtual vec_cptr_t SubItems() const = 0;

    //! @brief Removes the item from the subitems collection and gives the caller the item's ownership
    //! @param[in] rItem Reference to the item to take
    virtual ptr_t TakeItem(const IProjTreeItem& rItem) = 0;

    //! @brief Appends a new item to the collection;
    //!        The ownership of the pointer is transfered to this object
    //! @param[in] pNewItem Pointer to the item to add
    //! @return True if success
    virtual bool AddItem(ptr_t pNewItem) = 0;

    //! @brief Swaps the positions of two items in the collection
    //! @param[in] rA First item to swap
    //! @param[in] rB Second item to swap
    //! @return True if success
    virtual bool SwapItems(const IProjTreeItem& rA, const IProjTreeItem& rB) = 0;

    //! @brief OR'd enums of item types whose nesting is supported inside the this type
    virtual ETreeItemType GetSupportedChildTypes() const = 0;
};

#endif