#ifndef _CPROCESSEDITOR_
#define _CPROCESSEDITOR_

#include "ui/CNameDescEditor.h"
#include "wxExport/IRunProcessEditor.h"
#include "core/CRunProcess.h"

class CProcessArgsModel;

//! @brief Item editor for "run process"
class CRunProcessEditorUI : public IRunProcessEditor
{
protected:
    //! The underlying process item being edited
    CRunProcess& m_cEdit;

    //! The model used to manage the list of arguments
    CProcessArgsModel* const m_pModel;

    //! Gets the pointer to the currently selected argument
    CProcessArgument* GetSelectedArgument() const;

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