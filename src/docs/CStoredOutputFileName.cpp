#include "docs/CStoredOutputFileName.h"

DEFINE_SERIALIZATION_SCHEME(CStoredOutputFileName,
    SERIALIZATION_MEMBER(m_strOutputPath)
)

/* static */ const std::string CStoredOutputFileName::strDefaultPath = "$(ProjectDir)/docs/$(ProjectName)";

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