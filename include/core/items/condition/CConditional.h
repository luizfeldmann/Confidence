/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CCONDITIONAL_H_
#define _CCONDITIONAL_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CStoredExpression.h"
#include "core/CStoredItemCollection.h"

class ICondition;

//! @brief Represents a conditional that may or not execute it's subitems
class CConditional : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CStoredExpression
    , public CStoredItemCollection
{
protected:
    //! Stores the underlying condition
    std::shared_ptr<ICondition> m_pCondition;

    SERIALIZATION_FRIEND(CConditional);

    //! @brief Creates a new, empty condition
    CConditional();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:

    ~CConditional();

    //! @brief Gets a pointer to the underlying condition
    std::shared_ptr<ICondition> GetCondition();

    //! @copydoc GetCondition
    std::shared_ptr<const ICondition> GetCondition() const;

    //! @brief Replaces the underlying condition
    void SetCondition(std::shared_ptr<ICondition> pCondition);

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CConditional)

#endif