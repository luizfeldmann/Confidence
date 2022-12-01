#ifndef _CREGEXHANDLER_H_
#define _CREGEXHANDLER_H_

#include "IFileOperationHandler.h"
#include <JInterface/JsonSerialization.h>

//! @brief Handles R/W operations on arbitrary files using regular expressions
class CRegexHandler : public IFileOperationHandler
{
public:
    //! @brief Constructs a handler for regular expressions
    CRegexHandler();
    ~CRegexHandler();

    //! @brief Instantiates a new CRegexHandler
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

DECLARE_SERIALIZATION_SCHEME(CRegexHandler);

#endif