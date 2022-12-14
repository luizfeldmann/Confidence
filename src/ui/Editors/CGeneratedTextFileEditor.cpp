/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "ui/Editors/CGeneratedTextFileEditor.h"
#include "ui/CTextStyler.h"
#include "wxExport/ITextFileEditor.h"
#include "core/items/file_generator/CFileTextProvider.h"
#include "core/items/file_generator/CStoredTextProvider.h"
#include "core/items/file_generator/CGeneratedTextFile.h"
#include "vfs/CPersistentFileGenerator.h"
#include "vfs/CTempSymlinkGenerator.h"
#include "util/Log.h"
#include "util/CTempFile.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <unordered_map>
#include <functional>
#include <filesystem>
#include <Shlobj.h>

/* CGeneratedTextFileEditorUI */

//! @brief Item editor for generated text files
class CGeneratedTextFileEditorUI : public ITextFileEditor
{
public:
    //! @brief Constructs a new editor UI associated with the specified #CGeneratedTextFile
    //! @param[in] pParent Pointer to the window where this editor will be created
    //! @param[in] rEdit @copybrief m_rEdit
    CGeneratedTextFileEditorUI(wxWindow* pParent, CGeneratedTextFile& rEdit);
    ~CGeneratedTextFileEditorUI();

protected:
    //! Manages the syntax highlighting in the textbox
    CTextStyler m_cStyler;

    //! Underlying file generator item being edited
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

    /** @name ITextFileEditor overrides */
    //!@{
    
    //! @brief Creates and associates a new provider of the type selected by the user
    void onChangeProvider(wxCommandEvent& event) override;
    
    //! @brief Updates the saved path to the template file and reloads the text
    void onInputFilePathChanged(wxCommandEvent& event) override;
    
    //! @brief Opens a dialog to select a new template file
    void onSelectFile(wxCommandEvent& event) override;
    
    //! @brief Saves the changes to the template file
    void onSaveFile(wxCommandEvent& event) override;
    
    //! @brief Creates and associates a new generator of the type selected by the user
    void onChangeGenerator(wxCommandEvent& event) override;
    
    //! @brief Updates the saved path to the output file
    void onOutputFilePathChanged(wxCommandEvent& event) override;
    
    //! @brief Opens a dialog to select and external editor for the current text
    void onOpenEditor(wxCommandEvent& event) override;

    //!@}
};

CGeneratedTextFileEditorUI::CGeneratedTextFileEditorUI(wxWindow* pParent, CGeneratedTextFile& rEdit)
    : ITextFileEditor(pParent)
    , m_cStyler(*m_textEditor)
    , m_rEdit(rEdit)
{
    // Configure styling & events for the text editor
    m_cStyler.SetLexer(wxSTC_LEX_CONTAINER);
    m_textEditor->Bind(wxEVT_STC_CHANGE, &CGeneratedTextFileEditorUI::OnTextChanged, this);
    
    // Load data from the underlying object
    ReloadText();
    ReloadProviderType();
    ReloadGeneratorType();

    m_textCtrlOutputPath->ChangeValue( m_rEdit.GetOutputPath() );
}

CGeneratedTextFileEditorUI::~CGeneratedTextFileEditorUI()
{
    m_textEditor->Unbind(wxEVT_STC_CHANGE, &CGeneratedTextFileEditorUI::OnTextChanged, this);
}

bool CGeneratedTextFileEditorUI::ReloadText()
{
    bool bStatus = false;

    const ITextProvider* const pProvider = m_rEdit.GetProvider();
    if (pProvider)
    {
        std::string strText;
        bStatus = pProvider->GetText(strText);

        if (bStatus)
            m_textEditor->ChangeValue(strText);
    }

    m_textEditor->Show(bStatus);
    return bStatus;
}

void CGeneratedTextFileEditorUI::ReloadProviderType()
{
    bool bIsFile = false;
    int iSelectedIndex = wxNOT_FOUND;
    
    ITextProvider* pProvider = m_rEdit.GetProvider();
    if (nullptr != pProvider)
    {
        const ETextProviderType eType = pProvider->GetType();
        iSelectedIndex = (int)eType;

        bIsFile = (ETextProviderType::EFile == eType);
        if (bIsFile)
        {
            CFileTextProvider* pFile = dynamic_cast<CFileTextProvider*>(pProvider);
            assert(nullptr != pFile);

            m_textCtrlInputFilePath->ChangeValue(pFile->GetFilePath());
        }
    }

    m_textCtrlInputFilePath->Show(bIsFile);
    m_buttonOpenFile->Show(bIsFile);
    m_buttonSave->Show(bIsFile);
    m_choiceProvider->SetSelection(iSelectedIndex);
}

void CGeneratedTextFileEditorUI::ReloadGeneratorType()
{
    int iSelectedIndex = wxNOT_FOUND;

    IFileGenerator* const pGenerator = m_rEdit.GetGenerator();
    if (pGenerator)
        iSelectedIndex = (int)pGenerator->GetType();

    m_choiceGenerator->SetSelection(iSelectedIndex);
}

bool CGeneratedTextFileEditorUI::SaveText()
{
    bool bSuccess = false;

    ITextProvider* pProvider = m_rEdit.GetProvider();
    if (nullptr != pProvider)
    {
        const wxString strText = m_textEditor->GetValue();
        bSuccess = pProvider->SetText(strText.ToStdString());
    }

    if (!bSuccess)
    {
        CERROR("Failed to save contents");
    }

    return bSuccess;
}

void CGeneratedTextFileEditorUI::onSaveFile(wxCommandEvent& event)
{
    // When provider is a file, we save if clicked
    SaveText();
}

void CGeneratedTextFileEditorUI::OnTextChanged(wxStyledTextEvent& event)
{
    // When provider is in memory, we autosave
    ITextProvider* pProvider = m_rEdit.GetProvider();
    if (pProvider && pProvider->GetType() == ETextProviderType::EStored)
        SaveText();
}

void CGeneratedTextFileEditorUI::onChangeProvider(wxCommandEvent& event)
{
    using fnNewProvider_t = std::function<CGeneratedTextFile::provider_ptr_t()>;
    static const std::unordered_map<ETextProviderType, fnNewProvider_t> c_mProviderFactory {
        {ETextProviderType::EStored,    CStoredTextProvider::Create },
        {ETextProviderType::EFile,      CFileTextProvider::Create },
    };

    const int iSelected = m_choiceProvider->GetSelection();

    // Skip changing if invalid selection
    if (wxNOT_FOUND == iSelected)
        return;

    // Skip changing if new type is same as old type
    ITextProvider* const pProvider = m_rEdit.GetProvider();
    if (pProvider && iSelected == (int)pProvider->GetType())
        return;

    // Create a new provider and assign it
    {
        CGeneratedTextFile::provider_ptr_t pNewProvider = c_mProviderFactory.at((ETextProviderType)iSelected)();
        m_rEdit.SetProvider(std::move(pNewProvider));
    }

    ReloadProviderType();
    ReloadText();
}

void CGeneratedTextFileEditorUI::onChangeGenerator(wxCommandEvent& event)
{
    using fnNewGenerator_t = std::function<CGeneratedTextFile::generator_ptr_t()>;
    static const std::unordered_map<EFileGeneratorType, fnNewGenerator_t> c_mGeneratorFactory {
        {EFileGeneratorType::Persistent,   CPersistentFileGenerator::Create },
        {EFileGeneratorType::Symlink,      CTempSymlinkGenerator::Create },
    };

    const int iSelected = m_choiceGenerator->GetSelection();

    // Skip changing if invalid selection
    if (wxNOT_FOUND == iSelected)
        return;

    // Skip changing if new type is same as old type
    IFileGenerator* const pGenerator = m_rEdit.GetGenerator();
    if (pGenerator && iSelected == (int)pGenerator->GetType())
        return;

    // Create a new generator and assign it
    {
        CGeneratedTextFile::generator_ptr_t pNewProvider = c_mGeneratorFactory.at((EFileGeneratorType)iSelected)();
        m_rEdit.SetGenerator( std::move(pNewProvider) );
    }
}

void CGeneratedTextFileEditorUI::onInputFilePathChanged(wxCommandEvent& event)
{
    CFileTextProvider* pFile = dynamic_cast<CFileTextProvider*>(m_rEdit.GetProvider());
    if (nullptr != pFile)
    {
        pFile->SetFilePath(m_textCtrlInputFilePath->GetValue().ToStdString());
        m_textCtrlInputFilePath->ChangeValue(pFile->GetFilePath());
        ReloadText();
    }
}

void CGeneratedTextFileEditorUI::onSelectFile(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
        "Any file (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (wxID_CANCEL == openFileDialog.ShowModal())
        return;

    m_textCtrlInputFilePath->SetValue(openFileDialog.GetPath());
}

void CGeneratedTextFileEditorUI::onOutputFilePathChanged(wxCommandEvent& event)
{
    const wxString wxPath = m_textCtrlOutputPath->GetValue();
    m_rEdit.SetOutputPath(wxPath.ToStdString());
}

static std::wstring utf8_decode(const std::string& strNarrow)
{
    // Calculate size needed to store converted string
    const int iSize = MultiByteToWideChar(CP_UTF8, 0, strNarrow.data(), (int)strNarrow.size(), NULL, 0);

    // Store converted data into a vector
    std::wstring strWide(iSize, 0x00);
    MultiByteToWideChar(CP_UTF8, 0, strNarrow.data(), (int)strNarrow.size(), strWide.data(), (int)strWide.size());

    return strWide;
}

static HRESULT SHOpenWithDialogSTR(const std::string& strFile)
{
    const std::wstring wstrFile = utf8_decode(strFile);

    OPENASINFO sInfo;
    sInfo.oaifInFlags = OAIF_EXEC;
    sInfo.pcszClass = nullptr;
    sInfo.pcszFile = wstrFile.c_str();

    return SHOpenWithDialog(nullptr, &sInfo);
}

void CGeneratedTextFileEditorUI::onOpenEditor(wxCommandEvent& event)
{
    std::string strEditFile;
    std::unique_ptr<CTempFile> pTmpFile;

    // Get the path to the file being editted
    // It may be a real file (if using template) or a temporary file if using in-memory
    ITextProvider* const pOrigProvider = m_rEdit.GetProvider();
    if (nullptr == pOrigProvider)
        CERROR("Cannot edit contents because no provider is defined");
    else
    {
        if (ETextProviderType::EFile == pOrigProvider->GetType())
        {
            const CFileTextProvider* const pFileProvider = dynamic_cast<const CFileTextProvider*>(pOrigProvider);
            
            // Use the path to the template file
            if (pFileProvider)
                strEditFile = pFileProvider->GetFilePathAbsolute();
        }
        else
        {
            std::string strCurrentText;
            if (pOrigProvider->GetText(strCurrentText))
            {
                // Create a temp file to edit
                pTmpFile = std::make_unique<CTempFile>();
                strEditFile = pTmpFile->path();

                // Initial data - store current text inside temporary file
                CFileTextProvider cIntermediateProvider(strEditFile);
                if (!cIntermediateProvider.SetText(strCurrentText))
                {
                    CWARNING("Unable to update contents of temporary file '%s'", strEditFile.c_str());
                    pTmpFile.reset();
                    strEditFile.clear();
                }
            }
        }
    }

    // Open the file in editor if its valid
    if (strEditFile.empty())
        CERROR("Cannot edit '%s': unable to get a path to the file", GetName().c_str());
    else if (!std::filesystem::exists(strEditFile))
        CERROR("Cannot edit '%s': file does not exist", strEditFile.c_str());
    else
    {
        const HRESULT hr = SHOpenWithDialogSTR(strEditFile);
        if (SUCCEEDED(hr))
        {
            // Prompt the user to reload the contents after edit
            wxMessageDialog msgDiag(GetParent(),
                "The file was editted outside this application.\r\nDo you wish to reload the contents ?",
                "Reload file ?",
                wxYES_NO | wxICON_QUESTION);

            if (wxID_YES == msgDiag.ShowModal())
            {
                // If temp file, bring the disk content back to memory
                if (pTmpFile)
                {
                    std::string strModifiedText;
                    const CFileTextProvider cIntermediateProvider(pTmpFile->path());

                    if (!cIntermediateProvider.GetText(strModifiedText))
                        CERROR("Unable to read contents of temporary '%s'", cIntermediateProvider.GetFilePathAbsolute().c_str());
                    else if (!pOrigProvider->SetText(strModifiedText))
                        CERROR("Unable to update file contents");
                }

                // Bring new content to GUI
                ReloadText();
            }
        }
    }
}

/* CGeneratedTextFileEditor */

CGeneratedTextFileEditor::CGeneratedTextFileEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUiTextFile(new CGeneratedTextFileEditorUI(m_pPanel, dynamic_cast<CGeneratedTextFile&>(rItem)))
{
    m_pSizer->Add(m_pUiTextFile, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

/* static */ std::unique_ptr<ITreeItemEditor> CGeneratedTextFileEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return std::make_unique<CGeneratedTextFileEditor>(rMainWindow, rItem);
}