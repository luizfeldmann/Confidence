#ifndef _CGENERATEDTEXTFILE_H_
#define _CGENERATEDTEXTFILE_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"
#include "ITextProvider.h"

//! @brief Text file with dynamically generated content
class CGeneratedTextFile : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
protected:
    //! The destination of the generated file
    std::string m_strOutputPath;

    //! Provides the text either from memory or from a file
    std::unique_ptr<ITextProvider> m_cProvider;
    SERIALIZATION_FRIEND(CGeneratedTextFile);

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:
    //! @brief Creates a new, empty group
    CGeneratedTextFile();
    ~CGeneratedTextFile();

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @brief Gets a pointer to the underlying text provider
    ITextProvider* GetProvider() const;

    //! @brief Sets the underlying text provider
    //! @details The old provider is destroyed; assumes exclusive ownership of the new pointer
    void SetProvider(ITextProvider* pNewProvider);

    //! @brief Reads the path where this file is generated
    std::string GetOutputPath() const;

    //! @brief Sets the output path for this file
    void SetOutputPath(const std::string& strNewPath);
};

DECLARE_SERIALIZATION_SCHEME(CGeneratedTextFile)

#endif