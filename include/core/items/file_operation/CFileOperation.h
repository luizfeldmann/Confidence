#ifndef _CFILEOPERATION_H_
#define _CFILEOPERATION_H_

#include "core/CStoredExpression.h"
#include "core/CStoredDescriptionItem.h"
#include "docs/IDocumentable.h"

class CExecutionContext;
class IFileDataLocator;
class IFileOperatorContext;

//! @brief The type of operation
enum class EFileOperationType : int
{
    Read,   //!< A configuration is read from a file
    Write   //!< A configuration is set to a file
};

//! @brief Describes a single R/W operation to a file
class CFileOperation : public CStoredExpression, public CStoredDescriptionItem, public IDocumentable
{
protected:
    //! Informs if this operation is a read or a write
    EFileOperationType m_eDirection;

    //! Stores information on where in the file structure this operation will happen
    std::unique_ptr<IFileDataLocator> m_pLocator;

    SERIALIZATION_FRIEND(CFileOperation);

public:
    CFileOperation();
    ~CFileOperation();

    //! @brief Default move-constructor
    CFileOperation(CFileOperation&&) noexcept;

    //! @brief Default move-assignment operator
    CFileOperation& operator=(CFileOperation&&) noexcept;

    //! Gets the direction of data operation
    EFileOperationType GetType() const;

    //! Sets the new direction of data operation
    void SetType(EFileOperationType eNewType);

    //! @brief Gets a pointer to the data locator associated to this operation
    IFileDataLocator* GetLocator();

    //! @copydoc GetLocator
    const IFileDataLocator* GetLocator() const;

    //! @brief Sets a new data locator for this operation
    //! @param[in] pNewLocator The new data locator to assume
    void SetLocator(std::unique_ptr<IFileDataLocator> pNewLocator);

    //! @brief Executes the requested operation on the file context provided
    bool Execute(CExecutionContext& rContext, IFileOperatorContext& rFile) const;

    //! @copydoc IDependsOnVariables::GetDependencies
    std::vector<std::string> GetDependencies() const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CFileOperation);

#endif