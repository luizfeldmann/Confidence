#include "core/CStoredNameItem.h"

DEFINE_SERIALIZATION_SCHEME(CStoredNameItem,
    SERIALIZATION_MEMBER(m_strName)
)

CStoredNameItem::CStoredNameItem(const std::string& strName)
    : m_strName(strName)
{

}

const std::string& CStoredNameItem::GetName() const
{
    return m_strName;
}

void CStoredNameItem::SetName(const std::string& strNewName)
{
    m_strName = strNewName;
}