#ifndef _CLOGICALCOMPARATOR_H_
#define _CLOGICALCOMPARATOR_H_

#include "ICondition.h"
#include "JInterface/JsonSerialization.h"

//! @brief The type of comparison
enum class ELogicalComparator
{
    Equal,          //!< A == B
    NotEqual,       //!< A != B
    Less,           //!< A <  B
    LessOrEqual,    //!< A <= B
    Greater,        //!< A >  B
    GreaterOrEqual, //!< A >= B
};

//! @brief Compares two expressions
class CLogicalComparator : public ICondition
{
public:
    virtual ~CLogicalComparator();

    //! @brief Reads the left side expression
    std::string GetLHS() const;

    //! @brief Reads the left side expression
    std::string GetRHS() const;

    //! @brief Sets the left expression
    bool SetLHS(const std::string& strExpr);

    //! @brief Sets the right expression
    bool SetRHS(const std::string& strExpr);

    //! @copydoc ICondition::Evaluate
    bool Evaluate(CEvaluationContext& rContext, bool& bResult) const override;

    //! @brief Checks if the comparison can be performed in string mode
    virtual bool AllowStringMode() const = 0;

    //! @brief Checks if the comparator is currently set to operate in string mode
    virtual bool GetStringMode() const = 0;

    //! @brief Sets the behaviour from string/numeric modes
    virtual bool SetStringMode(bool bStringMode) = 0;

    //! @brief Reflects the comparator type
    virtual ELogicalComparator GetType() const = 0;

    //! @brief ICondition::GetCategory
    ECategory GetCategory() const override;

    //! @brief Instantiates a new comparator of the given type
    static std::shared_ptr<CLogicalComparator> Create(ELogicalComparator eType);

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;

protected:
    //! Stores the expressions to compare
    std::string m_arrExpr[2];

    SERIALIZATION_FRIEND(CLogicalComparator);

    //! Performs the boolean evaluation of the string-evaluated expressions
    virtual bool Evaluate(const std::string& lhs, const std::string& rhs, bool& bResult) const = 0;

    //! Constructs an empty comparator
    CLogicalComparator();

    //! Constructs a comparator with the provided left and right expressions
    CLogicalComparator(const std::string& lhs, const std::string& rhs);

private:
    CLogicalComparator(const CLogicalComparator&) = delete;
    CLogicalComparator& operator=(const CLogicalComparator&) = delete;
};

DECLARE_SERIALIZATION_SCHEME(CLogicalComparator);

#endif