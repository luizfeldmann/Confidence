#ifndef _CSTOREDEXPRESSION_H_
#define _CSTOREDEXPRESSION_H_

#include "IExpression.h"
#include <JInterface/JsonSerialization.h>

//! @brief Implementation of IExpression that stores the raw string in a serializable member
class CStoredExpression : public IExpression
{
protected:
    //! Holds the raw expression string
    std::string m_strExpression;

    SERIALIZATION_FRIEND(CStoredExpression);

public:
    //! Constructs an empty expression
    CStoredExpression() = default;

    //! @copydoc IExpression::GetExpression
    std::string GetExpression() const override;

    //! @copydoc IExpression::SetExpression
    bool SetExpression(const std::string&) override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredExpression);

#endif