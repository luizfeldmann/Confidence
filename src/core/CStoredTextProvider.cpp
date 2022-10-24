#include "core/CStoredTextProvider.h"
#include <sstream>

DEFINE_SERIALIZATION_SCHEME(CStoredTextProvider,
    SERIALIZATION_MEMBER(m_vText)
)

REGISTER_POLYMORPHIC_CLASS(CStoredTextProvider);

static const char c_cDelimiter = '\n';

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

bool CStoredTextProvider::PostDeserialize()
{
    m_strText.clear();

    size_t uLineCount = 0;
    for (const std::string& strLine : m_vText)
    {
        if (0 != uLineCount++)
            m_strText += c_cDelimiter;

        m_strText += strLine;
    }

    return true;
}

bool CStoredTextProvider::PreSerialize()
{
    m_vText.clear();

    // Split the current text into lines for serialization
    std::string strLine;
    std::stringstream streamText(m_strText);

    while (std::getline(streamText, strLine, c_cDelimiter))
        m_vText.emplace_back( std::move(strLine) );

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