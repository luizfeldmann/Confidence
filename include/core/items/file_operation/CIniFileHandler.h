#ifndef _CINIFILEHANDLER_H_
#define _CINIFILEHANDLER_H_

#include "IFileOperationHandler.h"
#include <JInterface/JsonSerialization.h>

//! @brief Handles R/W operations on INI files
class CIniFileHandler : public IFileOperationHandler
{
public:
    CIniFileHandler();
    ~CIniFileHandler();

    //! @brief Instantiates a new CIniFileHandler
    static handler_ptr_t Create();

    //! @copydoc IFileOperationHandler::GetType
    EFileOperationHandlerType GetType() const override;

    //! @copydoc IFileOperationHandler::CreateContext
    context_ptr_t CreateContext(const std::filesystem::path& filePath) const override;

    //! @copydoc IFileOperationHandler::CreateLocator
    locator_ptr_t CreateLocator() const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CIniFileHandler);

#endif