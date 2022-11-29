#ifndef _CASSIGNABLE_H_
#define _CASSIGNABLE_H_

#include <JInterface/JsonSerialization.h>

class CExecutionContext;

//! @brief Common logic for assigning a name/value pair to a #CExecutionContext and optionally exporting the same pair to the environment
class CAssignable
{
protected:
    //! If true, the value is exported to the system environment when evaluated
    bool m_bExportToEnvironment;

    SERIALIZATION_FRIEND(CAssignable);

    //! @brief Assigns a name-value pair to the execution context, optionally also exports to the environment
    //! @param[in,out] rContext The current execution context: used to evaluate the expression and store the result
    //! @param[in] strName The name of the pair to assign
    //! @param[in,out] strExpression Un-evaluated expression to assign; Gets updated with the evaluated expression
    //! @return True if success
    bool Assign(CExecutionContext& rContext, const std::string& strName, std::string& strExpression) const;

public:
    CAssignable();
    virtual ~CAssignable() = default;

    //! @brief Checks if this object is configured to export it's value to the environment
    bool GetExportToEnv() const;

    //! @brief Sets if this object should export it's value to the environment
    void SetExportToEnv(bool bExport);
};

DECLARE_SERIALIZATION_SCHEME(CAssignable);

#endif