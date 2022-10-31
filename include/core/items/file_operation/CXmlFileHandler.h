#ifndef _CXMLFILEHANDLER_H_
#define _CXMLFILEHANDLER_H_

#include "IFileOperationHandler.h"
#include <JInterface/JsonSerialization.h>

//! @brief Handles R/W operations on XML files
class CXmlFileHandler : public IFileOperationHandler
{
public:
    CXmlFileHandler();
    ~CXmlFileHandler();

    //! @brief Instantiates a new CXmlFileHandler
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

DECLARE_SERIALIZATION_SCHEME(CXmlFileHandler);

#endif