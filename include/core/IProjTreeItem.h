/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file IProjTreeItem.h
 * @brief Defines interface #IProjTreeItem
 */

#ifndef _IPROJTREEITEM_H_
#define _IPROJTREEITEM_H_

#include "INamedItem.h"
#include "IDescribedItem.h"
#include "ITreeItemCollection.h"
#include "ISerializationNotify.h"
#include "docs/IDocumentable.h"
#include "core/IExecutable.h"

#pragma warning(disable: 4250)

//! @brief Common base for all items placed on the project tree structure
class IProjTreeItem : public virtual INamedItem
                    , public virtual IDescribedItem
                    , public virtual ITreeItemCollection
                    , public ISerializationNotify
                    , public IDocumentable
                    , public IExecutable
                    , public std::enable_shared_from_this<IProjTreeItem>
{
private:
    //! @brief Deleted copy-constructor; class is non-copyable
    IProjTreeItem(const IProjTreeItem&) = delete;

    //! @brief Deleted copy-assignment operator; class is non-copyable
    IProjTreeItem& operator=(const IProjTreeItem&) = delete;

protected:
    //! @brief Returns a pointer to a subitem with a given name, if it exists
    static cptr_t FindSubitemByName(const std::string strFindName, const cptr_t& pParent);

    //! @brief Documents the name of the item using the given category prefix
    virtual bool DocumentName(IDocExporter& rExporter, const std::string& strPrefix) const;

    //! @brief Documents the name of the item
    virtual bool DocumentName(IDocExporter& rExporter) const;

    //! @brief Documents the description of the item
    virtual bool DocumentDescription(IDocExporter& rExporter) const;

    //! @brief Documents any class-specific information
    virtual bool DocumentCustom(IDocExporter& rExporter) const;

    //! @brief Documents the children of this item
    virtual bool DocumentChildren(IDocExporter& rExporter) const;

    //! @brief Logs the execution of this item
    void LogExecution() const;

    //! @brief Executes each subitem
    bool ExecuteChildren(CExecutionContext&) const;

public:
    //! @brief Constructs an empty item
    IProjTreeItem() = default;
    virtual ~IProjTreeItem() = default;

    //! @brief Default move-constructor
    IProjTreeItem(IProjTreeItem&&) = default;

    //! @brief Default move-assignment operator
    IProjTreeItem& operator=(IProjTreeItem&&) = default;

    //! @brief Reflects the type of this item
    virtual ETreeItemType GetType() const = 0;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @copydoc IDocumentable::Document
    virtual bool Document(IDocExporter& rExporter) const override;
};

#endif