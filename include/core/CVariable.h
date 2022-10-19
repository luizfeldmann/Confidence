#ifndef _CVARIABLE_H_
#define _CVARIABLE_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"
#include "CStoredExpression.h"

//! @brief Stores a constant or an expression
class CVariable : public IProjTreeItem
                , public CStoredNameItem
                , public CStoredDescriptionItem
                , public CNoChildren
{
protected:
    using vec_rules_t = std::vector<CStoredExpression>;

    //! @brief Collection of expressions keyed by configurations and instances
    vec_rules_t m_vRules;

    SERIALIZATION_FRIEND(CVariable);

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

public:
    //! @brief Creates a new, empty variable
    CVariable();

    ~CVariable();

    //! If true, the variable is exported to the system environment when evaluated
    bool m_bExportToEnvironment;

    //! @brief Creates a new rule associated to a provided configuration and instance, and returns a reference to it
    IExpression& AddRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance);

    //! @brief Erases the specified rule from the collection
    bool EraseRule(const IExpression* pExpr);

    //! @brief Returns an expression associated to a provided configuration and instance, if such exists
    IExpression* GetRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance);

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize() override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;
};

DECLARE_SERIALIZATION_SCHEME(CVariable)

#endif