#ifndef _IEXPRESSION_H_
#define _IEXPRESSION_H_

#include "IDependsOnVariables.h"

//! @brief A string containing variables that can be evaluated according to a context
class IExpression : public IDependsOnVariables
{
public:
    IExpression() = default;
    virtual ~IExpression() = default;

    //! @brief Reads the raw expression text
    virtual std::string GetExpression() const = 0;

    //! @brief Sets the raw expression text
    virtual bool SetExpression(const std::string&) = 0;

    //! @copydoc IDependsOnVariables::GetDependencies
    virtual std::vector<std::string> GetDependencies() const override;
};

#endif