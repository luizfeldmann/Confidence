#ifndef _CVARIABLEEDITOR_
#define _CVARIABLEEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IVariableEditor.h"
#include "ui/Models/CVariableTableModel.h"
#include "ui/CAcceleratorEntry.h"
#include "core/CVariable.h"

//! @brief Item editor for variable types
class CVariableEditorUI : public IVariableEditor, public INotifyItemOperation
{
protected:
    //! Reference to the variable being edited
    CVariable& m_rVar;

    //! Pointer to the model managing the data view table for the variable
    CVariableTableModel* m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

public:
    CVariableEditorUI(wxWindow* pParent, std::shared_ptr<const CProject> pProject, CVariable& rVar);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemChanged
    void OnAnyItemRenamed(const IProjTreeItem& pItem) override;

    /* OVERRIDES FROM IVariableEditor */
    void onToolExportEnv(wxCommandEvent& event) override;
    void onDeleteCell(wxCommandEvent& event) override;
};

//! @copydoc CVariableEditorUI
class CVariableEditor : public CNameDescEditor
{
protected:
    CVariableEditorUI* const m_pUiVar;

public:
    CVariableEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif