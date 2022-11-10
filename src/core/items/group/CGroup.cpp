#include "core/items/group/CGroup.h"
#include "core/items/group/IGroupFilter.h"

DEFINE_SERIALIZATION_SCHEME(CGroup,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_pFilter)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CGroup);

CGroup::CGroup()
    : CStoredNameItem("<new group>")
    , CStoredDescriptionItem("<no group description>")
    , CStoredItemCollection(EGroupable)
{

}

IProjTreeItem::ptr_t CGroup::Create()
{
    return ptr_t(new CGroup());
}

CGroup::~CGroup()
{

}

ETreeItemType CGroup::GetType() const
{
    return EGroup;
}

std::shared_ptr<IGroupFilter> CGroup::GetFilter()
{
    return m_pFilter;
}

std::shared_ptr<const IGroupFilter> CGroup::GetFilter() const
{
    return m_pFilter;
}

void CGroup::SetFilter(std::shared_ptr<IGroupFilter> pNewFilter)
{
    m_pFilter.swap(pNewFilter);
}

bool CGroup::Execute(CExecutionContext& rContext) const
{
    bool bStatus = true;

    // Decide if this group must execute
    bool bCondition = true;
    if (m_pFilter)
        bCondition = m_pFilter->Filter(*this, rContext);

    // (Maybe) execute children
    if (bCondition)
    {
        LogExecution();
        bStatus = ExecuteChildren(rContext);
    }

    return bStatus;
}

bool CGroup::PostDeserialize(CProject& rProject)
{
    bool bStatus = true;

    if (m_pFilter)
        bStatus = m_pFilter->PostDeserialize(rProject);

    return bStatus;
}

bool CGroup::PreSerialize()
{
    bool bStatus = true;

    if (m_pFilter)
        bStatus = m_pFilter->PreSerialize();

    return bStatus;
}

bool CGroup::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "GROUP:");
}

bool CGroup::DocumentCustom(IDocExporter& rExporter) const
{
    bool bStatus = true;

    if (m_pFilter)
        bStatus = m_pFilter->Document(rExporter);

    return bStatus;
}