#include "core/CStoredTextProvider.h"

DEFINE_SERIALIZATION_SCHEME(CStoredTextProvider,
    SERIALIZATION_MEMBER(m_strText)
)

REGISTER_POLYMORPHIC_CLASS(CStoredTextProvider);

ITextProvider* CStoredTextProvider::Create()
{
    return new CStoredTextProvider;
}

ETextProviderType CStoredTextProvider::GetType() const
{
    return ETextProviderType::EStored;
}

std::string CStoredTextProvider::GetText() const
{
    return m_strText;
}

bool CStoredTextProvider::SetText(const std::string& strText)
{
    m_strText = strText;
    return true;
}