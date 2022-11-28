#include "core/items/file_generator/CStoredTextProvider.h"
#include "core/CEvaluationContext.h"
#include "docs/IDocExporter.h"
#include <sstream>

DEFINE_SERIALIZATION_SCHEME(CStoredTextProvider,
    SERIALIZATION_MEMBER(m_vText)
)

REGISTER_POLYMORPHIC_CLASS(CStoredTextProvider);

static const char c_cDelimiter = '\n';

std::unique_ptr<ITextProvider> CStoredTextProvider::Create()
{
    return std::make_unique<CStoredTextProvider>();
}

ETextProviderType CStoredTextProvider::GetType() const
{
    return ETextProviderType::EStored;
}

bool CStoredTextProvider::GetText(std::string& strOutText) const
{
    strOutText = m_strText;
    return true;
}

bool CStoredTextProvider::SetText(const std::string& strText)
{
    m_strText = strText;
    return true;
}

bool CStoredTextProvider::PostDeserialize(CProject& rProject)
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

std::vector<std::string> CStoredTextProvider::GetDependencies() const
{
    return CEvaluationContext::ListVariables(m_strText);
}

bool CStoredTextProvider::Document(IDocExporter& rExporter) const
{
    std::string strContents;

    bool bStatus = GetText(strContents);
    bStatus = bStatus && rExporter.Collapsible();
    bStatus = bStatus && rExporter.Summary();
    bStatus = bStatus && rExporter.Text("Contents:");
    bStatus = bStatus && rExporter.PopStack();
    bStatus = bStatus && rExporter.Code(strContents);
    bStatus = bStatus && rExporter.PopStack();

    return bStatus;
}