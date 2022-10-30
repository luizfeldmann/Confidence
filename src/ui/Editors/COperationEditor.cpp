#include "ui/Editors/COperationEditor.h"
#include "ui/Models/CFileOperationsModel.h"
#include "core/items/file_operation/CFileOperations.h"
#include "core/items/file_operation/IFileOperationHandler.h"
#include "core/items/file_operation/CIniFileHandler.h"

/* COperationEditorUI */

COperationEditorUI::COperationEditorUI(wxWindow* pParent, CFileOperations& rEdit)
    : IOperationEditor(pParent)
    , m_rEdit(rEdit)
    , m_pModel( new CFileOperationsModel(m_rEdit) )
{
    // Manually add "Direction" column
    // "Choice" columns not supported by the designer
    wxArrayString arrDirection;
    arrDirection.Add("Read");
    arrDirection.Add("Write");

    m_dataViewCtrlOperations->InsertColumn(0, new wxDataViewColumn("R/W", new wxDataViewChoiceByIndexRenderer(arrDirection), 0, -1, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE));

    //! Associate the model to the data view
    m_dataViewCtrlOperations->AssociateModel(m_pModel);
    m_pModel->DecRef();

    // Draw all initial items
    wxDataViewItem wxRoot = CFileOperationsModel::GetViewItem(nullptr);
    wxDataViewItemArray arrItems;

    m_pModel->GetChildren(wxRoot, arrItems);
    m_pModel->ItemsAdded(wxRoot, arrItems);
    
    //! Load initial file path
    m_textCtrlFilePath->ChangeValue( m_rEdit.GetFilePath() );

    //! Load initial handler
    int iSelectedIndex = wxNOT_FOUND;

    IFileOperationHandler* pHandler = m_rEdit.GetHandler();
    if (pHandler)
        iSelectedIndex = (int)pHandler->GetType();

    m_choiceHandler->SetSelection(iSelectedIndex);
}

void COperationEditorUI::onFilePath(wxCommandEvent& event)
{
    const std::string strPath = m_textCtrlFilePath->GetValue().ToStdString();
    m_rEdit.SetFilePath(strPath);
}

void COperationEditorUI::onHandlerChoice(wxCommandEvent& event)
{
    using fnNewHandler_t = std::function<IFileOperationHandler::handler_ptr_t()>;
    static const std::unordered_map<EFileOperationHandlerType, fnNewHandler_t> c_mHandlerFactory {
        {EFileOperationHandlerType::IniFile,    CIniFileHandler::Create },
        //{EFileOperationHandlerType::JsonFile,   nullptr },
        //{EFileOperationHandlerType::XmlFile,    nullptr },
    };

    const int iSelected = m_choiceHandler->GetSelection();

    // Skip changing if invalid selection
    if (wxNOT_FOUND == iSelected)
        return;

    // Skip changing if new type is same as old type
    {
        IFileOperationHandler* const pHandler = m_rEdit.GetHandler();
        if (pHandler && iSelected == (int)pHandler->GetType())
            return;
    }

    // Create a new handler and assign it
    IFileOperationHandler::handler_ptr_t pNewHandler = c_mHandlerFactory.at((EFileOperationHandlerType)iSelected)();
    m_rEdit.SetHandler( std::move(pNewHandler) );

    // Remove all operation because the locators have different types now
    m_rEdit.GetItems().clear();
}

void COperationEditorUI::onToolNew(wxCommandEvent& event)
{
    const wxDataViewItem cNewItem = m_pModel->NewItem();
    m_dataViewCtrlOperations->Select(cNewItem);
}

void COperationEditorUI::onToolDelete(wxCommandEvent& event)
{
    if (m_dataViewCtrlOperations->HasSelection())
    {
        const wxDataViewItem cSelection = m_dataViewCtrlOperations->GetSelection();
        m_pModel->DeleteItem(cSelection);
    }
}

void COperationEditorUI::onToolUp(wxCommandEvent& event)
{
    if (m_dataViewCtrlOperations->HasSelection())
    {
        const wxDataViewItem cSelection = m_dataViewCtrlOperations->GetSelection();
        const wxDataViewItem cSwapItem = m_pModel->MoveItem(cSelection, true);

        m_dataViewCtrlOperations->Select(cSwapItem);
    }
}

void COperationEditorUI::onToolDown(wxCommandEvent& event)
{
    if (m_dataViewCtrlOperations->HasSelection())
    {
        const wxDataViewItem cSelection = m_dataViewCtrlOperations->GetSelection();
        const wxDataViewItem cSwapItem = m_pModel->MoveItem(cSelection, false);

        m_dataViewCtrlOperations->Select(cSwapItem);
    }
}

/* COperationEditor */

COperationEditor::COperationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUi(new COperationEditorUI(m_pPanel, dynamic_cast<CFileOperations&>(rItem) ))
{
    m_pSizer->Add(m_pUi, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* COperationEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new COperationEditor(rMainWindow, rItem);
}