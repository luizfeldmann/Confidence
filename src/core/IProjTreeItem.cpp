#include "core/IProjTreeItem.h"

DEFINE_SERIALIZATION_SCHEME(IProjTreeItem_Tracker_t,
    SERIALIZATION_MEMBER(m_ID)
)

IProjTreeItem_Tracker_t::set_type IProjTreeItem_Tracker_t::m_trackSet;

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