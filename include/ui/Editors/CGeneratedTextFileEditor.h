#ifndef _CGENERATEDTEXTFILEEDITOR_
#define _CGENERATEDTEXTFILEEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/ITextFileEditor.h"
#include "core/items/file_generator/CGeneratedTextFile.h"
#include "ui/CTextStyler.h"

//! @brief Item editor for generated text files
class CGeneratedTextFileEditorUI : public ITextFileEditor
{
public:
    CGeneratedTextFileEditorUI(wxWindow* pParent, CGeneratedTextFile& rEdit);
    ~CGeneratedTextFileEditorUI();

protected:
    CTextStyler m_cStyler;

    //! Underlying project item being edited
    CGeneratedTextFile& m_rEdit;

    //! @brief Reloads the text from the underlying object
    bool ReloadText();

    //! @brief Saves text in editor to the provider
    bool SaveText();

    //! @brief Sets the combo box & UI according to the type of ITextProvider
    void ReloadProviderType();

    //! @brief Sets the combo box & UI according to the type of IFileGenerator
    void ReloadGeneratorType();

    //! @brief Called by the text editor when the text is changed
    void OnTextChanged(wxStyledTextEvent& event);

    /* OVERRIDES FROM ITextFileEditor */
    void onChangeProvider(wxCommandEvent& event) override;
    void onInputFilePathChanged(wxCommandEvent& event) override;
    void onSelectFile(wxCommandEvent& event) override;
    void onSaveFile(wxCommandEvent& event) override;
    void onChangeGenerator(wxCommandEvent& event) override;
    void onOutputFilePathChanged(wxCommandEvent& event) override;
    void onOpenEditor(wxCommandEvent& event) override;
};

//! @copydoc CGeneratedTextFileEditorUI
class CGeneratedTextFileEditor : public CNameDescEditor
{
protected:
    CGeneratedTextFileEditorUI* const m_pUiTextFile;

public:
    CGeneratedTextFileEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif