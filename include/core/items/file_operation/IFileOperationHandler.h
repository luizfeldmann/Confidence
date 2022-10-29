#ifndef _IFILEOPERATIONHANDLER_H_
#define _IFILEOPERATIONHANDLER_H_

#include <memory>
#include <filesystem>
#include "EFileOperationHandlerType.h"
#include "docs/IDocumentable.h"

class IFileDataLocator;
class IFileOperatorContext;

//! @brief Handles operations on a supported file format
class IFileOperationHandler : public IDocumentable
{
public:
    virtual ~IFileOperationHandler() = default;

    //! @brief Reflects the object type
    virtual EFileOperationHandlerType GetType() const = 0;

    using context_ptr_t = std::shared_ptr<IFileOperatorContext>;
    using locator_ptr_t = std::unique_ptr<IFileDataLocator>;
    using handler_ptr_t = std::unique_ptr<IFileOperationHandler>;

    //! @brief Creates a context to handle operations to a provided file path
    //! @param[in] filePath The file which will undergo the operations
    virtual context_ptr_t CreateContext(const std::filesystem::path& filePath) const = 0;

    //! @brief Creates a new locator adequate to the file structure
    virtual locator_ptr_t CreateLocator() const = 0;
};

#endif