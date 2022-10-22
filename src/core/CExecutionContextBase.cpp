#include "core/CExecutionContextBase.h"
#include "core/EBuiltInVariables.h"
#include "core/CProject.h"
#include <filesystem>

CExecutionContextBase::CExecutionContextBase(const CProject& rProj)
    : m_rProject(rProj)
{
    m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectName)] = m_rProject.GetName();

    const std::string strProjectFile = m_rProject.GetCurrentPath();
    if (!strProjectFile.empty())
    {
        m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectFile)] = strProjectFile;
        m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectDir)] = std::filesystem::path(strProjectFile).parent_path().string();
    }
}