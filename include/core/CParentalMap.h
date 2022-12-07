/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CPARENTALMAP_
#define _CPARENTALMAP_

#include <unordered_map>
#include <memory>

class IProjTreeItem;

//! @brief Utility to keep track of parents of tree items
class CParentalMap
{
protected:
    //! Type used to hold the association pairs between tree items and theirs parents
    using parent_map_t = std::unordered_map<const IProjTreeItem*, std::weak_ptr<const IProjTreeItem>>;
    
    //! Maps each item (key) to it's parent (value)
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

    //! @brief Fills a collection with the sequence of parents of a child item
    //! @param[in] pChild Pointer to the child whose parents must be found
    //! @param[out] it Iterator used the output the parent items to the collection
    template<typename OutputIterator>
    void GetParentBranch(const IProjTreeItem* pChild, OutputIterator& it)
    {
        for (const IProjTreeItem* pCurrent = pChild; nullptr != pCurrent; pCurrent = GetParent(pCurrent).get())
            ++it = pCurrent;
    }

    //! @copydoc GetParentBranch
    //! @param[in] pParent The parent item whose subitems will be mapped
    template<typename OutputIterator>
    static void GetParentBranch(const std::shared_ptr<const IProjTreeItem>& pParent, const IProjTreeItem* pChild, OutputIterator& it)
    {
        CParentalMap cMap;
        cMap.Remap(pParent);
        cMap.GetParentBranch(pChild, it);
    }
};

#endif