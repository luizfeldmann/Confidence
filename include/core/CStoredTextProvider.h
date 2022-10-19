#ifndef _CSTOREDTEXTPROVIDER_H_
#define _CSTOREDTEXTPROVIDER_H_

#include "ITextProvider.h"
#include <JInterface/JsonSerialization.h>

//! @brief Stores text as a serialized member
class CStoredTextProvider : public ITextProvider
{
protected:
    //! Stores the text
    std::string m_strText;
    SERIALIZATION_FRIEND(CStoredTextProvider);

public:
    CStoredTextProvider() = default;

    //! @brief Factory function
    static ITextProvider* Create();

    //! @copydoc ITextProvider::GetType
    ETextProviderType GetType() const;

    //! @copydoc ITextProvider::GetText
    std::string GetText() const override;

    //! @copydoc ITextProvider::SetText
    bool SetText(const std::string&) override;

    //! @copydoc IDocumentable:Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredTextProvider);

#endif 