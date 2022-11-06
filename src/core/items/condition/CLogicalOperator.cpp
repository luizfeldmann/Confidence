#include "core/items/condition/CLogicalOperator.h"
#include <JInterface/JsonSerialization.h>

/* CBaseOperator */

class CBaseOperator : public CLogicalOperator
{
protected:
    //! Stores the input conditions
    vec_cond_t m_vInputs;

    SERIALIZATION_FRIEND(CBaseOperator);

public:
    CBaseOperator() = default;
    virtual ~CBaseOperator() = default;

    vec_cond_t SubItems() override
    {
        return m_vInputs;
    }

    vec_ccond_t SubItems() const override
    {
        return vec_ccond_t(m_vInputs.cbegin(), m_vInputs.cend());
    }

    bool AddItem(condition_ptr_t pNewItem) override
    {
        m_vInputs.push_back( std::move(pNewItem) );
        return true;
    }

    bool EraseItem(const_condition_ptr_t pItem) override
    {
        bool bStatus = false;

        vec_cond_t::const_iterator it = std::find(m_vInputs.cbegin(), m_vInputs.cend(), pItem);

        if (m_vInputs.cend() != it)
        {
            m_vInputs.erase(it);
            bStatus = true;
        }

        return bStatus;
    }

    bool SwapItems(const_condition_ptr_t pA, const_condition_ptr_t pB)
    {
        bool bStatus = false;

        vec_cond_t::iterator itEnd = m_vInputs.end();
        vec_cond_t::iterator itA = std::find(m_vInputs.begin(), itEnd, pA);
        vec_cond_t::iterator itB = std::find(m_vInputs.begin(), itEnd, pB);

        if (itEnd != itA && itEnd != itB)
        {
            std::iter_swap(itA, itB);
            bStatus = true;
        }

        return bStatus;
    }
};

DECLARE_SERIALIZATION_SCHEME(CBaseOperator);

DEFINE_SERIALIZATION_SCHEME(CBaseOperator, 
    SERIALIZATION_MEMBER(m_vInputs)
);

/* CNot */
class CNot : public CLogicalOperator
{
protected:
    //! Stores the input condition
    condition_ptr_t m_pInput;

    SERIALIZATION_FRIEND(CNot);

public:
    CNot() = default;
    virtual ~CNot() = default;

    vec_cond_t SubItems() override
    {
        vec_cond_t vSub;

        if (m_pInput)
            vSub.push_back(m_pInput);

        return vSub;
    }

    vec_ccond_t SubItems() const override
    {
        vec_ccond_t vSub;

        if (m_pInput)
            vSub.push_back(m_pInput);

        return vSub;
    }

    bool AddItem(condition_ptr_t pNewItem) override
    {
        bool bStatus = false;

        if (!m_pInput)
        {
            m_pInput = pNewItem;
            bStatus = true;
        }

        return bStatus;
    }

    bool EraseItem(const_condition_ptr_t pItem) override
    {
        bool bStatus = false;

        if (pItem == m_pInput)
        {
            m_pInput.reset();
            bStatus = true;
        }

        return bStatus;
    }

    bool SwapItems(const_condition_ptr_t pA, const_condition_ptr_t pB)
    {
        return false;
    }

    ELogicalOperator GetType() const override
    {
        return ELogicalOperator::Not;
    }

    std::string GetInfix() const override
    {
        return "NOT";
    }

    bool Document(IDocExporter& rExporter) const override
    {
        bool bStatus = rExporter.FormField(GetInfix(), "The following condition must evaluate to FALSE");
        bStatus = bStatus && CLogicalOperator::Document(rExporter);

        return bStatus;
    }

    bool Evaluate(CEvaluationContext& rContext, bool& bResult) const override
    {
        bool bStatus = false;

        if (m_pInput)
        {
            bStatus = m_pInput->Evaluate(rContext, bResult);

            if (bStatus)
                bResult = !bResult;
        }

        return bStatus;
    }
};

DECLARE_SERIALIZATION_SCHEME(CNot);

DEFINE_SERIALIZATION_SCHEME(CNot,
    SERIALIZATION_MEMBER(m_pInput)
);

REGISTER_POLYMORPHIC_CLASS(CNot);

/* CAnd */
class CAnd : public CBaseOperator
{
public:
    CAnd() = default;
    virtual ~CAnd() = default;

    ELogicalOperator GetType() const override
    {
        return ELogicalOperator::And;
    }

    std::string GetInfix() const override
    {
        return "AND";
    }

    bool Document(IDocExporter& rExporter) const override
    {
        bool bStatus = rExporter.FormField(GetInfix(), "ALL of the below conditions must be true simultaneously");
        bStatus = bStatus && CLogicalOperator::Document(rExporter);

        return bStatus;
    }

    bool Evaluate(CEvaluationContext& rContext, bool& bResult) const override
    {
        bool bStatus = true;
        bResult = true;
        
        for (vec_cond_t::const_iterator it = m_vInputs.cbegin();
            (it != m_vInputs.cend()) && bStatus && bResult; ++it)
        {
            bStatus = it->get()->Evaluate(rContext, bResult);
        }

        return bStatus;
    }
};

DECLARE_SERIALIZATION_SCHEME(CAnd);

DEFINE_SERIALIZATION_SCHEME(CAnd,
    SERIALIZATION_INHERIT(CBaseOperator)
);

REGISTER_POLYMORPHIC_CLASS(CAnd);

/* COr */
class COr : public CBaseOperator
{
public:
    COr() = default;
    virtual ~COr() = default;

    ELogicalOperator GetType() const override
    {
        return ELogicalOperator::Or;
    }

    std::string GetInfix() const override
    {
        return "OR";
    }

    bool Document(IDocExporter& rExporter) const override
    {
        bool bStatus = rExporter.FormField(GetInfix(), "ANY of the below conditions must be true");
        bStatus = bStatus && CLogicalOperator::Document(rExporter);

        return bStatus;
    }

    bool Evaluate(CEvaluationContext& rContext, bool& bResult) const override
    {
        bool bStatus = true;
        bResult = false;

        for (vec_cond_t::const_iterator it = m_vInputs.cbegin();
            (it != m_vInputs.cend()) && bStatus && (!bResult); ++it)
        {
            bStatus = it->get()->Evaluate(rContext, bResult);
        }

        return bStatus;
    }
};

DECLARE_SERIALIZATION_SCHEME(COr);

DEFINE_SERIALIZATION_SCHEME(COr,
    SERIALIZATION_INHERIT(CBaseOperator)
);

REGISTER_POLYMORPHIC_CLASS(COr);

/* CLogicalOperator */

CLogicalOperator::CLogicalOperator()
{

}

CLogicalOperator::~CLogicalOperator()
{

}

CLogicalOperator::ECategory CLogicalOperator::GetCategory() const
{
    return ECategory::Operator;
}

bool CLogicalOperator::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.List();

    const vec_ccond_t vConds = SubItems();
    for (vec_ccond_t::const_iterator it = vConds.cbegin(); (it != vConds.cend()) && bStatus; ++it)
    {
        bStatus = rExporter.Item();
        bStatus = bStatus && it->get()->Document(rExporter);
        bStatus = bStatus && rExporter.PopStack();
    }

    bStatus = bStatus && rExporter.PopStack();

    return bStatus;
}

/* static */ std::shared_ptr<CLogicalOperator> CLogicalOperator::Create(ELogicalOperator eType)
{
    std::shared_ptr<CLogicalOperator> ptr;

    switch (eType)
    {
    case ELogicalOperator::And: ptr = std::make_shared<CAnd>(); break;
    case ELogicalOperator::Or:  ptr = std::make_shared<COr>();  break;
    case ELogicalOperator::Not: ptr = std::make_shared<CNot>(); break;
    default: break;
    }

    return ptr;
}