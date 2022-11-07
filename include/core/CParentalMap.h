#ifndef _CPARENTALMAP_
#define _CPARENTALMAP_

#include <unordered_map>
#include <memory>

class IProjTreeItem;

//! @brief Utility to keep track of parents of tree items
class CParentalMap
{
protected:
    //! Maps each item (key) to it's parent (value)
    using parent_map_t = std::unordered_map<const IProjTreeItem*, std::weak_ptr<const IProjTreeItem>>;
    parent_map_t m_mParents;

    //! @brief Recursively fills the paternity map for the provided item
    void RemapInternal(const std::shared_ptr<const IProjTreeItem>& pParent);

public:
    CParentalMap();

    //! @brief Recursively fills the paternity map for the provided item
    //! @param[in] pParent The parent item whose subitems will be mapped
    void Remap(const std::shared_ptr<const IProjTreeItem>& pParent);

    //! @brief Gets the parent of the requested child item
    //! @param[in] pItem Reference to the item whose parent must be found
    //! @return The parent of the item or nullptr if not found (the project root has no parent)
    std::shared_ptr<const IProjTreeItem> GetParent(const IProjTreeItem* pItem) const;
};

#endif