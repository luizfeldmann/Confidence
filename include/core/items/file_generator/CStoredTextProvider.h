/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CSTOREDTEXTPROVIDER_H_
#define _CSTOREDTEXTPROVIDER_H_

#include "ITextProvider.h"
#include <JInterface/JsonSerialization.h>

//! @brief Stores text as a serialized member
class CStoredTextProvider : public ITextProvider
{
protected:
    //! Stores the text for runtime use
    std::string m_strText;

    //! Stores the text split in lines for (de)-serialization
    std::vector<std::string> m_vText;

    SERIALIZATION_FRIEND(CStoredTextProvider);

public:
    CStoredTextProvider() = default;

    //! @brief Factory function
    static std::unique_ptr<ITextProvider> Create();

    //! @copydoc ITextProvider::GetType
    ETextProviderType GetType() const;

    //! @copydoc ITextProvider::GetText
    bool GetText(std::string&) const override;

    //! @copydoc ITextProvider::SetText
    bool SetText(const std::string&) override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @copydoc IDependsOnVariables::GetDependencies
    std::vector<std::string> GetDependencies() const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CStoredTextProvider);

#endif 