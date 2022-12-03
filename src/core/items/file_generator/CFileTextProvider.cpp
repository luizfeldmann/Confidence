/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include <fstream>
#include <sstream>
#include "core/items/file_generator/CFileTextProvider.h"
#include "core/CEvaluationContext.h"
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

bool CFileTextProvider::GetText(std::string& strOutText) const
{
    bool bStatus = false;

    std::ifstream ifFile(m_strFilePath, std::ios_base::binary);
    if (ifFile.is_open())
    {
        std::stringstream str;
        str << ifFile.rdbuf();

        strOutText = str.str();
        bStatus = true;
    }

    return bStatus;
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

std::vector<std::string> CFileTextProvider::GetDependencies() const
{
    // TODO: list variables from the read text file
    return CEvaluationContext::ListVariables(m_strFilePath);
}

bool CFileTextProvider::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Template File:", GetFilePath(), true);
    return bStatus;
}