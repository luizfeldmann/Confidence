/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/CExecutionContext.h"
#include "core/items/CProject.h"
#include "core/items/CInstance.h"
#include "core/items/CConfiguration.h"
#include "core/items/variable/EBuiltInVariables.h"
#include <cassert>

CExecutionContext::CExecutionContext(const std::shared_ptr<const CProject>& pProject, const std::shared_ptr<const CInstance>& pInstance, const std::shared_ptr<const CConfiguration>& pConfiguration)
    : CExecutionContextBase(pProject)
    , m_pInstance(pInstance)
    , m_pConfiguration(pConfiguration)
{
    Remap(pProject);

    // Set the built-in variables
    assert(pConfiguration && "Creating execution context from null configuration");
    m_mVariables[GetBuiltinName(EBuiltInVariable::ConfigurationName)] = pConfiguration->GetName();

    assert(pInstance && "Creating execution context from null instance");
    m_mVariables[GetBuiltinName(EBuiltInVariable::InstanceName)] = pInstance->GetName();
}

std::shared_ptr<const CInstance> CExecutionContext::GetInstance() const
{
    std::shared_ptr<const CInstance> pInstance = m_pInstance.lock();
    assert(pInstance && "Execution context instance is null");

    return pInstance;
}

std::shared_ptr<const CConfiguration> CExecutionContext::GetConfiguration() const
{
    std::shared_ptr<const CConfiguration> pConfiguration = m_pConfiguration.lock();
    assert(pConfiguration && "Execution context configuration is null");

    return pConfiguration;
}

void CExecutionContext::Store(std::shared_ptr<IContextStorage> pStore)
{
    m_lStorage.push_back(pStore);
}