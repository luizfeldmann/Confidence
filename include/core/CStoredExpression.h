#ifndef _CSTOREDEXPRESSION_H_
#define _CSTOREDEXPRESSION_H_

#include "IExpression.h"
#include <JInterface/JsonSerialization.h>

//! @brief Implementation of IExpression that stores the raw string in a serializable member
class CStoredExpression : public IExpression
{
protected:
    std::string m_strExpression;

    SERIALIZATION_FRIEND(CStoredExpression);

public:
    CStoredExpression() = default;

    //! @copydoc IExpression::GetExpression
    std::string GetExpression() const override;

    //! @copydoc IExpression::SetExpression
    bool SetExpression(const std::string&) override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredExpression);

#endif