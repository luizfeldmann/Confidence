#ifndef _CASSIGNABLE_H_
#define _CASSIGNABLE_H_

#include <JInterface/JsonSerialization.h>

class CExecutionContext;

class CAssignable
{
protected:
    //! If true, the constant is exported to the system environment when evaluated
    bool m_bExportToEnvironment;

    SERIALIZATION_FRIEND(CAssignable);

    //! @brief Assigns a name-value pair to the execution context, optionally also exports to the environment
    //! @param[in] strName The name of the pair to assign
    //! @param[in] strExpression Un-evaluated expression to assign
    //! @param[out] strExpression Is updated with the evaluated expression
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