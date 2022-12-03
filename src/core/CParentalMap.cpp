/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/CParentalMap.h"
#include "core/IProjTreeItem.h"
#include <cassert>

CParentalMap::CParentalMap()
{

}

void CParentalMap::RemapInternal(const IProjTreeItem::cptr_t& pParent)
{
    assert(pParent && "Mapping children of null parent");

    IProjTreeItem::vec_cptr_t vSubitems = pParent->SubItems();
    for (const IProjTreeItem::cptr_t& pItem : vSubitems)
    {
        m_mParents[pItem.get()] = pParent;
        RemapInternal(pItem);
    }
}

void CParentalMap::Remap(const IProjTreeItem::cptr_t& pParent)
{
    m_mParents.clear();
    RemapInternal(pParent);
}

IProjTreeItem::cptr_t CParentalMap::GetParent(const IProjTreeItem* pItem) const
{
    assert(pItem && "Getting parent of a null item");

    IProjTreeItem::cptr_t pFind;

    parent_map_t::const_iterator itFind = m_mParents.find(pItem);
    if (m_mParents.cend() != itFind)
        pFind = itFind->second.lock();

    return pFind;
}