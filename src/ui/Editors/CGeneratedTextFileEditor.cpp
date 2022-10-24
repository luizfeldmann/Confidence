#include "ui/Editors/CGeneratedTextFileEditor.h"
#include "core/CFileTextProvider.h"
#include "core/CStoredTextProvider.h"
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
    ITextProvider* pProvider = m_rEdit.GetProvider();
    const bool bHasProvider = (nullptr != pProvider);

    m_textEditor->Show(bHasProvider);
    if (bHasProvider)
        m_textEditor->ChangeValue(pProvider->GetText());

    return bHasProvider;
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
    using fnNewProvider_t = std::function<ITextProvider* ()>;
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
    ITextProvider* const pNewProvider = c_mProviderFactory.at((ETextProviderType)iSelected)();
    m_rEdit.SetProvider(pNewProvider);

    ReloadProviderType();
    ReloadText();
}

void CGeneratedTextFileEditorUI::onChangeGenerator(wxCommandEvent& event)
{
    using fnNewGenerator_t = std::function<IFileGenerator* ()>;
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
    IFileGenerator* const pNewProvider = c_mGeneratorFactory.at((EFileGeneratorType)iSelected)();
    m_rEdit.SetGenerator(pNewProvider);
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
    if (nullptr != pOrigProvider)
    {
        if (ETextProviderType::EFile == pOrigProvider->GetType())
        {
            CFileTextProvider* const pFileProvider = dynamic_cast<CFileTextProvider*>(pOrigProvider);
            
            // Use the path to the template file
            if (pFileProvider)
                strEditFile = pFileProvider->GetFilePathAbsolute();
        }
        else
        {
            // Create a temp file to edit
            pTmpFile.reset(new CTempFile);
            strEditFile = pTmpFile->path();

            // Initial data - store current text inside temporary file
            CFileTextProvider cIntermediateProvider(strEditFile);
            cIntermediateProvider.SetText(pOrigProvider->GetText());
        }
    }

    // Open the file in editor if its valid
    if (strEditFile.empty() || !std::filesystem::exists(strEditFile))
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
                    const CFileTextProvider cIntermediateProvider(pTmpFile->path());
                    pOrigProvider->SetText( cIntermediateProvider.GetText() );
                }

                // Bring new content to GUI
                ReloadText();
            }
        }
    }
}

/* CGeneratedTextFileEditor */

CGeneratedTextFileEditor::CGeneratedTextFileEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiTextFile(new CGeneratedTextFileEditorUI(m_pPanel, dynamic_cast<CGeneratedTextFile&>(rItem)))
{
    m_pSizer->Add(m_pUiTextFile, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CGeneratedTextFileEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CGeneratedTextFileEditor(rNotebook, rItem);
}