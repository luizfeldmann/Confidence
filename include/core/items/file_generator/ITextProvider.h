#ifndef _ITEXTPROVIDER_H_
#define _ITEXTPROVIDER_H_

#include <string>
#include "docs/IDocumentable.h"
#include "core/ISerializationNotify.h"

//! @brief Reflection types of ITextProvider implementations
enum  class ETextProviderType
{
    EStored = 0,    //!< The text is stored as a member
    EFile   = 1,    //!< Read/write from a file
};

//! @brief Generic interface for access of text
class ITextProvider : public IDocumentable, public ISerializationNotify
{
public:
    ITextProvider() = default;
    virtual ~ITextProvider() = default;

    //! @brief Gets the reflected type of the object
    virtual ETextProviderType GetType() const = 0;

    //! @brief Reads the text
    virtual std::string GetText() const = 0;

    //! @brif Writes the text
    //! @return True if success
    virtual bool SetText(const std::string&) = 0;
};

#endif