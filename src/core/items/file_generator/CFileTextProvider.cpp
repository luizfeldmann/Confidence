#include <fstream>
#include <sstream>
#include "core/items/file_generator/CFileTextProvider.h"
#include "docs/IDocExporter.h"

DEFINE_SERIALIZATION_SCHEME(CFileTextProvider,
    SERIALIZATION_MEMBER(m_strFilePath)
)

REGISTER_POLYMORPHIC_CLASS(CFileTextProvider);

#undef str //! Defined by JInterface, interferes with std::stringstream

CFileTextProvider::CFileTextProvider(const std::string& strPath)
    : m_strFilePath(strPath)
{

}

std::unique_ptr<ITextProvider> CFileTextProvider::Create()
{
    return std::make_unique<CFileTextProvider>();
}

ETextProviderType CFileTextProvider::GetType() const
{
    return ETextProviderType::EFile;
}

std::string CFileTextProvider::GetText() const
{
    std::stringstream str;

    std::ifstream ifFile(m_strFilePath, std::ios_base::binary);
    if (ifFile.is_open())
        str << ifFile.rdbuf();

    return str.str();
}

bool CFileTextProvider::SetText(const std::string& strText)
{
    bool bSuccess = false;

    std::ofstream ofFile(m_strFilePath, std::ios_base::binary);
    if (ofFile.is_open())
    {
        ofFile << strText;
        bSuccess = true;
    }

    return bSuccess;
}

bool CFileTextProvider::PostDeserialize(CProject& rProject)
{
    return true;
}

bool CFileTextProvider::PreSerialize()
{
    return true;
}

std::string CFileTextProvider::GetFilePathAbsolute() const
{
    return m_strFilePath;
}

std::string CFileTextProvider::GetFilePath() const
{
    return m_strFilePath;
}

bool CFileTextProvider::SetFilePath(const std::string& strPath)
{
    m_strFilePath = strPath;
    return true;
}

bool CFileTextProvider::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Template File:", GetFilePath(), true);
    return bStatus;
}