#ifndef _CGENERATEDTEXTFILE_H_
#define _CGENERATEDTEXTFILE_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CNoChildren.h"
#include "core/items/file_generator/ITextProvider.h"
#include "vfs/IFileGenerator.h"

//! @brief Text file with dynamically generated content
class CGeneratedTextFile : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
public:
    using provider_ptr_t = std::unique_ptr<ITextProvider>;
    using generator_ptr_t = std::unique_ptr<IFileGenerator>;

protected:
    //! The destination of the generated file
    std::string m_strOutputPath;

    //! Provides the text either from memory or from a file
    provider_ptr_t m_pProvider;

    //! Implements generating output files in the filesystem
    generator_ptr_t m_pGenerator;

    SERIALIZATION_FRIEND(CGeneratedTextFile);

    //! @brief Creates a new, empty text file
    CGeneratedTextFile();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:
    ~CGeneratedTextFile();

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @brief Gets a pointer to the underlying text provider
    ITextProvider* GetProvider() const;

    //! @brief Sets the underlying text provider
    void SetProvider(provider_ptr_t pNewProvider);

    //! @brief Gets a pointer to the underlying file generator
    IFileGenerator* GetGenerator() const;

    //! @brief Sets the underlying file generator
    void SetGenerator(generator_ptr_t pNewGenerator);

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @brief Reads the path where this file is generated
    std::string GetOutputPath() const;

    //! @brief Sets the output path for this file
    void SetOutputPath(const std::string& strNewPath);

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CGeneratedTextFile)

#endif