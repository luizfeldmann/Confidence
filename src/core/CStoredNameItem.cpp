#include "core/CStoredNameItem.h"

DEFINE_SERIALIZATION_SCHEME(CStoredNameItem,
    SERIALIZATION_MEMBER(m_strName)
)

CStoredNameItem::CStoredNameItem(const std::string& strName)
    : m_strName(strName)
{

}

std::string CStoredNameItem::GetName() const
{
    return m_strName;
}

bool CStoredNameItem::SetName(const std::string& strNewName)
{
    m_strName = strNewName;
    return true;
}