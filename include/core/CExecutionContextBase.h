/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CEXECUTIONCONTEXTBASE_H_
#define _CEXECUTIONCONTEXTBASE_H_

#include <memory>
#include "core/CEvaluationContext.h"

class CProject;

//! @brief Base execution context
//! Allows execution of items dependant only on project-wide information
class CExecutionContextBase : public CEvaluationContext
{
private:
    CExecutionContextBase(const CExecutionContextBase&) = delete;
    CExecutionContextBase& operator=(const CExecutionContextBase&) = delete;

protected:
    //! Pointer to the project being executed
    std::weak_ptr<const CProject> m_pProject;

public:
    //! @brief Constructs a context associated to the provided project
    //! @brief pProject The project to execute
    CExecutionContextBase(const std::shared_ptr<const CProject>& pProject);

    //! @brief Gets a pointer to the project being executed
    std::shared_ptr<const CProject> GetProject() const;
};

#endif