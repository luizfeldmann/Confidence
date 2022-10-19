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
    vec_cref_t vSubitems = SubItems();

    if (!vSubitems.empty())
    {
        rExporter.IncrementHeading();

        for (const cref_t& rSubitem : vSubitems)
            bStatus = bStatus && rSubitem.get().Document(rExporter);

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