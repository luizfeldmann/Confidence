#ifndef _CPROCESSWAITCOMPLETION_H_
#define _CPROCESSWAITCOMPLETION_H_

#include "IProcessPolicy.h"
#include <optional>

//! @copydoc EProcessPolicy::EProcessWaitCompletion
class CProcessWaitCompletion : public IProcessPolicy
{
public:
    CProcessWaitCompletion();

    //! @copydoc IProcessPolicy::GetType
    EProcessPolicy GetType() const override;

    using opt_returncode_t = std::optional<int>;

    //! @brief Sets the (optional) checked return code
    void SetExpectedReturnCode(opt_returncode_t);

    //! @brief Reads the (optional) checked return code
    const opt_returncode_t GetExpectedReturnCode() const;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;

    //! @copydoc IProcessPolicy::Execute
    bool Execute(CExecutionContext& rContext, const SProcessStartInfo& rInfo) const override;

protected:
    //! @brief If true, the return code must be compared to m_nExpectedReturnCode when the process returns
    bool m_bCheckReturnCode;

    //! @brief Stores the expected return code for process validation (if set)
    int m_nExpectedReturnCode;

    SERIALIZATION_FRIEND(CProcessWaitCompletion);
};

DECLARE_SERIALIZATION_SCHEME(CProcessWaitCompletion);

#endif