#include "core/items/condition/CLogicalComparator.h"
#include "core/CEvaluationContext.h"
#include "util/Log.h"
#include "docs/IDocExporter.h"
#include <cassert>

/* CLogicalComparator */

DEFINE_SERIALIZATION_SCHEME(CLogicalComparator,
    SERIALIZATION_MEMBER(m_arrExpr)
)

CLogicalComparator::CLogicalComparator()
{

}

CLogicalComparator::~CLogicalComparator()
{

}

CLogicalComparator::CLogicalComparator(const std::string& lhs, const std::string& rhs)
    : m_arrExpr {lhs, rhs}
{

}

std::string CLogicalComparator::GetLHS() const
{
    return m_arrExpr[0];
}

std::string CLogicalComparator::GetRHS() const
{
    return m_arrExpr[1];
}

bool CLogicalComparator::SetLHS(const std::string& strExpr)
{
    m_arrExpr[0] = strExpr;
    return true;
}

bool CLogicalComparator::SetRHS(const std::string& strExpr)
{
    m_arrExpr[1] = strExpr;
    return true;
}

CLogicalComparator::ECategory CLogicalComparator::GetCategory() const
{
    return ECategory::Comparator;
}

bool CLogicalComparator::Evaluate(CEvaluationContext& rContext, bool& bResult) const
{
    std::string lhs = GetLHS();
    std::string rhs = GetRHS();
    
    bool bStatus = rContext.Evaluate(lhs);
    bStatus = bStatus && rContext.Evaluate(rhs);
    bStatus = bStatus && Evaluate(lhs, rhs, bResult);

    return bStatus;
}

std::vector<std::string> CLogicalComparator::GetDependencies() const
{
    auto vDeps = CEvaluationContext::ListVariables(GetLHS());
    const auto vDepsR = CEvaluationContext::ListVariables(GetRHS());

    vDeps.insert(vDeps.cend(), vDepsR.cbegin(), vDepsR.cend());

    return vDeps;
}

bool CLogicalComparator::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.Paragraph();
    bStatus = bStatus && rExporter.Text(GetLHS());
    bStatus = bStatus && rExporter.Text(" ");
    bStatus = bStatus && rExporter.Strong();
    bStatus = bStatus && rExporter.Text(GetInfix());
    bStatus = bStatus && rExporter.PopStack();
    bStatus = bStatus && rExporter.Text(" ");
    bStatus = bStatus && rExporter.Text(GetRHS());
    bStatus = bStatus && rExporter.PopStack();

    return bStatus;
}

/* CNumericCapableComparator */

//! @brief Comparison can happen in numeric mode
class CNumericCapableComparator : public CLogicalComparator
{
protected:
    //! Type used to represent the number converted from the expression
    using numeric_t = double;

    //! @brief Evaluates the comparison in numeric mode
    virtual bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const = 0;

    //! @brief Parses the strings into numbers and then evaluates the comparison in numeric mode
    bool EvaluateNumeric(const std::string& lhs, const std::string& rhs, bool& bResult) const
    {
        bool bStatus;

        try {
            const double dL = std::stod(lhs);
            const double dR = std::stod(rhs);
            bStatus = EvaluateNumeric(dL, dR, bResult);
        }
        catch (const std::exception& e)
        {
            CERROR("Failed parsing expression for numeric-mode comparator: %s", e.what());
            bStatus = false;
        }

        return bStatus;
    }
};

/* CStringCapableComparator */

//! @brief Comparison can happen between strings
class CStringCapableComparator : public CNumericCapableComparator
{
protected:
    //! Compare in string mode or numeric mode ?
    bool m_bStringMode;

    SERIALIZATION_FRIEND(CStringCapableComparator);

    //! @brief Performs the comparison in string mode
    virtual bool EvaluateString(const std::string& lhs, const std::string& rhs, bool& bResult) const = 0;

public:
    //! @copydoc CLogicalComparator::AllowStringMode
    bool AllowStringMode() const override
    {
        return true;
    }

    //! @copydoc CLogicalComparator::GetStringMode
    bool GetStringMode() const
    {
        return m_bStringMode;
    }

    //! @copydoc CLogicalComparator::SetStringMode
    bool SetStringMode(bool bStringMode)
    {
        m_bStringMode = bStringMode;
        return true;
    }

    //! @copydoc CLogicalComparator::Evaluate(const std::string&,const std::string&, bool&)
    bool Evaluate(const std::string& lhs, const std::string& rhs, bool& bResult) const override final
    {
        bool bStatus;

        if (m_bStringMode)
            bStatus = EvaluateString(lhs, rhs, bResult);
        else
            bStatus = EvaluateNumeric(lhs, rhs, bResult);

        return bStatus;
    }
};

DEFINE_SERIALIZATION_SCHEME(CStringCapableComparator,
    SERIALIZATION_MEMBER(m_bStringMode)
    SERIALIZATION_INHERIT(CLogicalComparator)
);

#define DEFINE_STRING_CAPABLE(CLASSNAME) \
    DECLARE_SERIALIZATION_SCHEME(CLASSNAME); \
    DEFINE_SERIALIZATION_SCHEME(CLASSNAME, SERIALIZATION_INHERIT(CStringCapableComparator)); \
    REGISTER_POLYMORPHIC_CLASS(CLASSNAME);

/* CNumericOnlyComparator */

//! @brief Comparison can only happen in numeric mode
class CNumericOnlyComparator : public CNumericCapableComparator
{
public:
    //! @copydoc CLogicalComparator::AllowStringMode
    bool AllowStringMode() const override
    {
        return false;
    }

    //! @copydoc CLogicalComparator::GetStringMode
    bool GetStringMode() const
    {
        return false;
    }

    //! @copydoc CLogicalComparator::SetStringMode
    bool SetStringMode(bool bStringMode)
    {
        return false;
    }

    //! @copydoc CLogicalComparator::Evaluate
    bool Evaluate(const std::string& lhs, const std::string& rhs, bool& bResult) const override final
    {
        return EvaluateNumeric(lhs, rhs, bResult);
    }
};

#define DEFINE_NUMERIC_ONLY(CLASSNAME) \
    DECLARE_SERIALIZATION_SCHEME(CLASSNAME); \
    DEFINE_SERIALIZATION_SCHEME(CLASSNAME, SERIALIZATION_INHERIT(CLogicalComparator)); \
    REGISTER_POLYMORPHIC_CLASS(CLASSNAME);

/* CEqual */

//! @brief Checks if the left- and right-hand sides are equal
class CEqual : public CStringCapableComparator
{
protected:
    //! @copydoc CStringCapableComparator::EvaluateString
    bool EvaluateString(const std::string& lhs, const std::string& rhs, bool& bResult) const override
    {
        bResult = (lhs == rhs);
        return true;
    }

    //! @copydoc CNumericCapableComparator::EvaluateNumeric
    bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const override
    {
        bResult = (lhs == rhs);
        return true;
    }

public:
    //! @copydoc CLogicalComparator::GetType
    ELogicalComparator GetType() const
    {
        return ELogicalComparator::Equal;
    }

    //! @copydoc ICondition::GetInfix
    std::string GetInfix() const override
    {
        return "is EQUAL to";
    }
};

DEFINE_STRING_CAPABLE(CEqual);

/* CNotEqual */

//! @brief Checks if the left- and right-hand sides are different
class CNotEqual : public CStringCapableComparator
{
protected:
    //! @copydoc CStringCapableComparator::EvaluateString
    bool EvaluateString(const std::string& lhs, const std::string& rhs, bool& bResult) const override
    {
        bResult = (lhs != rhs);
        return true;
    }

    //! @copydoc CNumericCapableComparator::EvaluateNumeric
    bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const override
    {
        bResult = (lhs != rhs);
        return true;
    }

public:
    //! @copydoc CLogicalComparator::GetType
    ELogicalComparator GetType() const
    {
        return ELogicalComparator::NotEqual;
    }

    //! @copydoc ICondition::GetInfix
    std::string GetInfix() const override
    {
        return "is NOT EQUAL to";
    }
};

DEFINE_STRING_CAPABLE(CNotEqual);

/* Less */
//! @brief Comparator evaluates to true when LHS < RHS
class CLess : public CNumericOnlyComparator
{
protected:
    //! @copydoc CNumericCapableComparator::EvaluateNumeric
    bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const override
    {
        bResult = (lhs < rhs);
        return true;
    }

public:
    //! @copydoc CLogicalComparator::GetType
    ELogicalComparator GetType() const
    {
        return ELogicalComparator::Less;
    }

    //! @copydoc ICondition::GetInfix
    std::string GetInfix() const override
    {
        return "is LESS than";
    }
};

DEFINE_NUMERIC_ONLY(CLess);

/* LessOrEqual */
//! @brief Comparator evaluates to true when LHS <= RHS
class CLessOrEqual : public CNumericOnlyComparator
{
protected:
    //! @copydoc CNumericCapableComparator::EvaluateNumeric
    bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const override
    {
        bResult = (lhs <= rhs);
        return true;
    }

public:
    //! @copydoc CLogicalComparator::GetType
    ELogicalComparator GetType() const
    {
        return ELogicalComparator::LessOrEqual;
    }

    //! @copydoc ICondition::GetInfix
    std::string GetInfix() const override
    {
        return "is LESS than or EQUAL to";
    }
};

DEFINE_NUMERIC_ONLY(CLessOrEqual);

/* Greater */
//! @brief Comparator evaluates to true when LHS > RHS
class CGreater : public CNumericOnlyComparator
{
protected:
    //! @copydoc CNumericCapableComparator::EvaluateNumeric
    bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const override
    {
        bResult = (lhs > rhs);
        return true;
    }

public:
    //! @copydoc CLogicalComparator::GetType
    ELogicalComparator GetType() const
    {
        return ELogicalComparator::Greater;
    }

    //! @copydoc ICondition::GetInfix
    std::string GetInfix() const override
    {
        return "is GREATER than";
    }
};

DEFINE_NUMERIC_ONLY(CGreater);

/* GreaterOrEqual */
//! @brief Comparator evaluates to true when LHS >= RHS
class CGreaterOrEqual : public CNumericOnlyComparator
{
protected:
    //! @copydoc CNumericCapableComparator::EvaluateNumeric
    bool EvaluateNumeric(numeric_t lhs, numeric_t rhs, bool& bResult) const override
    {
        bResult = (lhs >= rhs);
        return true;
    }

public:
    //! @copydoc CLogicalComparator::GetType
    ELogicalComparator GetType() const
    {
        return ELogicalComparator::GreaterOrEqual;
    }
    
    //! @copydoc ICondition::GetInfix
    std::string GetInfix() const override
    {
        return "is GREATER than or EQUAL to";
    }
};

DEFINE_NUMERIC_ONLY(CGreaterOrEqual);

/* CLogicalComparator */

/* static */ std::shared_ptr<CLogicalComparator> CLogicalComparator::Create(ELogicalComparator eType)
{
    std::shared_ptr<CLogicalComparator> ptr;

    switch (eType)
    {
    case ELogicalComparator::Equal:         ptr = std::make_shared<CEqual>();           break;
    case ELogicalComparator::NotEqual:      ptr = std::make_shared<CNotEqual>();        break;
    case ELogicalComparator::Less:          ptr = std::make_shared<CLess>();            break;
    case ELogicalComparator::LessOrEqual:   ptr = std::make_shared<CLessOrEqual>();     break;
    case ELogicalComparator::Greater:       ptr = std::make_shared<CGreater>();         break;
    case ELogicalComparator::GreaterOrEqual:ptr = std::make_shared<CGreaterOrEqual>();  break;
    default: break;
    }

    return ptr;
}