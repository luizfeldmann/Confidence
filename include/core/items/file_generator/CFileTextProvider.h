#ifndef _CFILETEXTPROVIDER_H_
#define _CFILETEXTPROVIDER_H_

#include "ITextProvider.h"
#include <JInterface/JsonSerialization.h>

//! @brief Accesses text from a file in disk
class CFileTextProvider : public ITextProvider
{
protected:
    //! Path to file where the text is accessed
    std::string m_strFilePath;
    SERIALIZATION_FRIEND(CFileTextProvider);

public:
    CFileTextProvider() = default;
    CFileTextProvider(const std::string& strPath);

    //! @copydoc ITextProvider::GetType
    ETextProviderType GetType() const override;

    //! @copydoc ITextProvider::GetText
    std::string GetText() const override;

    //! @copydoc ITextProvider::SetText
    bool SetText(const std::string&) override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @brief Factory function
    static std::unique_ptr<ITextProvider> Create();

    //! Reads the path to the underlying file
    std::string GetFilePath() const;

    //! @brief Reads the absolute path to the underlying file
    std::string GetFilePathAbsolute() const;

    //! @brief Sets the path to the underlying file
    //! @param[in] strPath New path to set
    //! @return True if success
    bool SetFilePath(const std::string& strPath);

    //! @copydoc IDocumentable:Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CFileTextProvider);

#endif