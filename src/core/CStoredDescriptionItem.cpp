#include "core/CStoredDescriptionItem.h"

DEFINE_SERIALIZATION_SCHEME(CStoredDescriptionItem,
    SERIALIZATION_MEMBER(m_strDescription)
)

CStoredDescriptionItem::CStoredDescriptionItem(const std::string& strDesc)
    : m_strDescription(strDesc)
{

}

std::string CStoredDescriptionItem::GetDescription() const
{
    return m_strDescription;
}

bool CStoredDescriptionItem::SetDescription(const std::string& strNewDesc)
{
    m_strDescription = strNewDesc;
    return true;
}