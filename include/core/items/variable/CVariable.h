#ifndef _CVARIABLE_H_
#define _CVARIABLE_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CNoChildren.h"
#include "CVariableExpressionKey.h"

//! @brief Stores a constant or an expression
class CVariable : public IProjTreeItem
                , public CStoredNameItem
                , public CStoredDescriptionItem
                , public CNoChildren
{
protected:
    using vec_rules_t = std::vector<CVariableExpressionKey>;

    //! @brief Collection of expressions keyed by configurations and instances
    vec_rules_t m_vRules;

    SERIALIZATION_FRIEND(CVariable);

    //! @brief Creates a new, empty variable
    CVariable();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:
    ~CVariable();

    //! If true, the variable is exported to the system environment when evaluated
    bool m_bExportToEnvironment;

    //! @brief Creates a new rule associated to a provided configuration and instance, and returns a reference to it
    IExpression& AddRule(std::weak_ptr<const CConfiguration> pKeyConfig, std::weak_ptr<const CInstance> pKeyInstance);

    //! @brief Erases the specified rule from the collection
    bool EraseRule(const IExpression* pExpr);

    //! @brief Returns an expression associated to a provided configuration and instance, if such exists
    IExpression* GetRule(const CConfiguration* pKeyConfig, const CInstance* pKeyInstance);

    //! @copydoc GetRule
    const IExpression* GetRule(const CConfiguration* pKeyConfig, const CInstance* pKeyInstance) const;

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CVariable)

#endif