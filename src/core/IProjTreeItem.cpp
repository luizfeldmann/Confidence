#include "core/IProjTreeItem.h"

bool IProjTreeItem::PostDeserialize()
{
    // TODO: invoke on all children
    return true;
}

bool IProjTreeItem::PreSerialize()
{
    // TODO: invoke in all children
    return true;
}