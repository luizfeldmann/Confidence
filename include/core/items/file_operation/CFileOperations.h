#ifndef _CFILEOPERATIONS_H_
#define _CFILEOPERATIONS_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CNoChildren.h"
#include "CFileOperation.h"

class IFileOperationHandler;

//! @brief Represents a set of operations to apply to an existing file of supported format
class CFileOperations : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
public:
    //! @brief Type of collection used to pass/store the operations performed on a file by the #CFileOperations
    using vec_oper_t = std::list<CFileOperation>;

protected:
    //! The path to the relevant file
    std::string m_strFilePath;

    //! Implements R/W on a specific file format
    std::unique_ptr<IFileOperationHandler> m_pHandler;

    //! Stores each operation to perform on the file
    vec_oper_t m_vOperations;

    SERIALIZATION_FRIEND(CFileOperations);

    //! @brief Creates a new, empty instance
    CFileOperations();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:

    ~CFileOperations();

    //! @brief Reads the path to the relevant file
    std::string GetFilePath() const;

    //! @brief Sets the path for this file
    void SetFilePath(const std::string& strNewPath);

    //! @brief Gets a pointer to the underlying file operations handler
    IFileOperationHandler* GetHandler() const;

    //! @brief Sets the underlying file operations handler
    //! @details The old handler is destroyed; assumes exclusive ownership of the new pointer
    void SetHandler(std::unique_ptr<IFileOperationHandler> pNewHandler);

    //! @brief Gets a reference to the internal collection of operations
    vec_oper_t& GetItems();

    //! @copydoc GetItems
    const vec_oper_t& GetItems() const;

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CFileOperations)

#endif