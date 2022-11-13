#include "core/items/group/CGroupFilterBase.h"
#include "core/items/CProject.h"
#include "core/CExecutionContext.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"
#include <algorithm>

DEFINE_SERIALIZATION_SCHEME(CGroupFilterBase,
    SERIALIZATION_MEMBER(m_vItemNames)
)

CGroupFilterBase::CGroupFilterBase()
{

}

CGroupFilterBase::~CGroupFilterBase()
{

}

static bool compare(CGroupFilterBase::sptr_t S, CGroupFilterBase::wptr_t W)
{
    return S == W.lock();
}

void CGroupFilterBase::push_back(wptr_t pInstance)
{
    m_vItemPtrs.emplace_back(pInstance);
}

void CGroupFilterBase::erase(sptr_t pInstance)
{
    m_vItemPtrs.erase(
        std::remove_if(m_vItemPtrs.begin(), m_vItemPtrs.end(),
            std::bind(compare, pInstance, std::placeholders::_1)
        ));
}

bool CGroupFilterBase::Filter(sptr_t pItem) const
{
    return std::any_of(m_vItemPtrs.cbegin(), m_vItemPtrs.cend(),
        std::bind(compare, pItem, std::placeholders::_1));
}

CGroupFilterBase::ptrvec_t CGroupFilterBase::GetItems() const
{
    return m_vItemPtrs;
}

bool CGroupFilterBase::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.List();

    for (ptrvec_t::const_iterator it = m_vItemPtrs.cbegin();
        (bStatus) && (it != m_vItemPtrs.cend()); ++it)
    {
        sptr_t sPtr = it->lock();

        if (!sPtr)
            continue; // Instance no longer exists on the project, but still existes on the filter

        bStatus = rExporter.Item();
        {
            bStatus = bStatus && rExporter.Paragraph();
            bStatus = bStatus && rExporter.Text(sPtr->GetName());
            bStatus = bStatus && rExporter.PopStack();
        }
        bStatus = bStatus && rExporter.PopStack();
    }

    bStatus = bStatus && rExporter.PopStack();
    return bStatus;
}

bool CGroupFilterBase::PreSerialize()
{
    m_vItemNames.clear();

    for (const wptr_t& wPtr : m_vItemPtrs)
    {
        sptr_t sPtr = wPtr.lock();

        if (!sPtr)
            continue; // Instance no longer exists on the project, but still existes on the filter

        m_vItemNames.emplace_back(sPtr->GetName());
    }

    return true;
}