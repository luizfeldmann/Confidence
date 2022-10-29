#ifndef _COPERATIONEDITOR_H_
#define _COPERATIONEDITOR_H_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IOperationEditor.h"
#include "core/items/CGroup.h"

class CFileOperations;
class CFileOperationsModel;

//! @brief Item editor for file operations
class COperationEditorUI : public IOperationEditor
{
protected:
    //! The underlying item being editted
    CFileOperations& m_rEdit;

    //! Pointer to the model managing the data view of operations
    CFileOperationsModel* m_pModel;

    /* OVERRIDES FROM IOperationEditor */
    void onFilePath(wxCommandEvent& event) override;
    void onHandlerChoice(wxCommandEvent& event) override;
    void onToolNew(wxCommandEvent& event) override;
    void onToolDelete(wxCommandEvent& event) override;
    void onToolUp(wxCommandEvent& event) override;
    void onToolDown(wxCommandEvent& event) override;

public:
    COperationEditorUI(wxWindow* pParent, CFileOperations& rEdit);
};

//! @copydoc COperationEditorUI
class COperationEditor : public CNameDescEditor
{
protected:
    COperationEditorUI* const m_pUi;

public:
    COperationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif