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

bool CStoredTextProvider::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.Collapsible()
        && rExporter.Summary()
        && rExporter.Text("Contents:")
        && rExporter.PopStack()
        && rExporter.Code(GetText())
        && rExporter.PopStack();

    return bStatus;
}