#ifndef _CPROCESSEDITOR_
#define _CPROCESSEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IRunProcessEditor.h"
#include "core/CRunProcess.h"
#include "ui/CAcceleratorEntry.h"

class CProcessArgsModel;

//! @brief Item editor for "run process"
class CRunProcessEditorUI : public IRunProcessEditor
{
protected:
    //! The underlying process item being edited
    CRunProcess& m_cEdit;

    //! The model used to manage the list of arguments
    CProcessArgsModel* const m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

    //! Loads the combo box of "policy" type
    EProcessPolicy GetCurrentPolicy() const;

    //! Loads the "wait completion" exclusive UI elements
    void LoadWaitCompletion();

    /* OVERRIDES FROM IRunProcessEditor */
    void onToolNewArg(wxCommandEvent& event) override;
    void onToolDelArg(wxCommandEvent& event) override;
    void onToolMoveUp(wxCommandEvent& event) override;
    void onToolMoveDown(wxCommandEvent& event) override;
    void onPolicyChanged(wxCommandEvent& event) override;
    void onWindowChanged(wxCommandEvent& event) override;
    void onAdminChanged(wxCommandEvent& event) override;
    void onReturnCodeCombo(wxCommandEvent& event) override;
    void onReturnCodeText(wxCommandEvent& event) override;

public:
    CRunProcessEditorUI(wxWindow* pParent, CRunProcess& cEdit);
};

//! @copydoc CGroupEditorUI
class CRunProcessEditor : public CNameDescEditor
{
protected:
    CRunProcessEditorUI* const m_pUiProc;

public:
    CRunProcessEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);
};

#endif