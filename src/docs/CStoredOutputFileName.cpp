/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "docs/CStoredOutputFileName.h"
#include "core/items/variable/EBuiltInVariables.h"
#include <filesystem>

DEFINE_SERIALIZATION_SCHEME(CStoredOutputFileName,
    SERIALIZATION_MEMBER(m_strOutputPath)
)

static std::filesystem::path GetDefaultPath()
{
    std::filesystem::path path = MakeExpression(GetBuiltinName(EBuiltInVariable::ProjectDir));
    path /= MakeExpression(GetBuiltinName(EBuiltInVariable::ProjectName));

    return path;
}

/* static */ const std::string& CStoredOutputFileName::GetDefaultPath()
{
    static const std::string strDefaultPath = ::GetDefaultPath().string();
    return strDefaultPath;
}

CStoredOutputFileName::CStoredOutputFileName(const std::string& strInitial)
    : m_strOutputPath(strInitial)
{

}

std::string CStoredOutputFileName::GetOutputFileName() const
{
    return m_strOutputPath;
}

void CStoredOutputFileName::SetOutputFileName(const std::string& str)
{
    m_strOutputPath = str;
}