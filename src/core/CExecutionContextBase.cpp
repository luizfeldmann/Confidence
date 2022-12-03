/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/CExecutionContextBase.h"
#include "core/items/variable/EBuiltInVariables.h"
#include "core/items/CProject.h"
#include <filesystem>
#include <cassert>

CExecutionContextBase::CExecutionContextBase(const std::shared_ptr<const CProject>& pProject)
    : m_pProject(pProject)
{
    assert(pProject && "Invalid project for construction of an execution context");

    m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectName)] = pProject->GetName();

    const std::string strProjectFile = pProject->GetCurrentPath();
    if (!strProjectFile.empty())
    {
        m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectFile)] = strProjectFile;
        m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectDir)] = std::filesystem::path(strProjectFile).parent_path().string();
    }
}

std::shared_ptr<const CProject> CExecutionContextBase::GetProject() const
{
    std::shared_ptr<const CProject> pProject = m_pProject.lock();
    assert(pProject && "Execution context has no associated project");

    return pProject;
}