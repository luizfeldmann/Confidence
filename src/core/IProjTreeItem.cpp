#include "core/IProjTreeItem.h"

bool IProjTreeItem::PostDeserialize()
{
    bool bStatus = true;

    const vec_ref_t& vSubItems = SubItems();
    for (const ref_t& rItem : vSubItems)
        bStatus = bStatus && rItem.get().PostDeserialize();

    return bStatus;
}

bool IProjTreeItem::PreSerialize()
{
    bool bStatus = true;

    const vec_ref_t& vSubItems = SubItems();
    for (const ref_t& rItem : vSubItems)
        bStatus = bStatus && rItem.get().PreSerialize();

    return bStatus;
}