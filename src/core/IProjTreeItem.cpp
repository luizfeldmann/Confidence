#include "core/IProjTreeItem.h"
#include "util/Log.h"
#include <cassert>

bool IProjTreeItem::PostDeserialize(CProject& rProject)
{
    bool bStatus = true;

    const vec_ptr_t& vSubItems = SubItems();
    for (const ptr_t& pItem : vSubItems)
        bStatus = bStatus && pItem && pItem->PostDeserialize(rProject);

    return bStatus;
}

bool IProjTreeItem::PreSerialize()
{
    bool bStatus = true;

    const vec_ptr_t& vSubItems = SubItems();
    for (const ptr_t& pItem : vSubItems)
        bStatus = bStatus && pItem && pItem->PreSerialize();

    return bStatus;
}

/*static*/ IProjTreeItem::cptr_t IProjTreeItem::FindSubitemByName(const std::string strFindName, const cptr_t& pParent)
{
    assert(pParent);

    cptr_t pFound;

    if (strFindName == pParent->GetName())
        pFound = pParent;
    else
    {
        vec_cptr_t vSubitems = pParent->SubItems();

        for (const cptr_t& pItem : vSubitems)
        {
            pFound = FindSubitemByName(strFindName, pItem);
            if (pFound)
                break;
        }
    }

    return pFound;
}

bool IProjTreeItem::DocumentName(IDocExporter& rExporter) const
{
    return DocumentName(rExporter, "");
}

bool IProjTreeItem::DocumentName(IDocExporter& rExporter, const std::string& strPrefix) const
{
    bool bStatus = rExporter.Heading();

    if (bStatus && !strPrefix.empty())
    {
        bStatus = rExporter.Strong();
        bStatus = bStatus && rExporter.Text(strPrefix);
        bStatus = bStatus && rExporter.PopStack();
        bStatus = bStatus && rExporter.Text(" ");
    }

    bStatus = bStatus && rExporter.Text(GetName());
    bStatus = bStatus && rExporter.PopStack();

    return bStatus;
}

bool IProjTreeItem::DocumentDescription(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.Paragraph();
    bStatus = bStatus && rExporter.Text(GetDescription());
    bStatus = bStatus && rExporter.PopStack();

    return bStatus;
}

bool IProjTreeItem::DocumentCustom(IDocExporter& rExporter) const
{
    return true;
}

bool IProjTreeItem::DocumentChildren(IDocExporter& rExporter) const
{
    bool bStatus = true;
    vec_cptr_t vSubitems = SubItems();

    if (!vSubitems.empty())
    {
        rExporter.IncrementHeading();

        for (const cptr_t& pSubitem : vSubitems)
            bStatus = bStatus && pSubitem && pSubitem->Document(rExporter);

        rExporter.DecrementHeading();
    }

    return bStatus;
}

bool IProjTreeItem::Document(IDocExporter& rExporter) const
{
    bool bStatus = DocumentName(rExporter);
    bStatus = bStatus && DocumentDescription(rExporter);
    bStatus = bStatus && DocumentCustom(rExporter);
    bStatus = bStatus && DocumentChildren(rExporter);

    return bStatus;
}

void IProjTreeItem::LogExecution() const
{
    const std::string strName = GetName();
    CLOG("EXEC: %s", strName.c_str());
}

bool IProjTreeItem::ExecuteChildren(CExecutionContext& rContext) const
{
    vec_cptr_t vSubitems = SubItems();

    bool bStatus = true;
    for (const cptr_t& pItem : vSubitems)
        bStatus = bStatus && pItem && pItem->Execute(rContext);

    return bStatus;
}