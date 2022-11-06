#ifndef _CCONDITIONALEDITOR_
#define _CCONDITIONALEDITOR_

#include "ui/CAcceleratorEntry.h"
#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IConditionalEditor.h"

class CConditional;
class CConditionalTreeModel;

//! @brief Item editor for conditionals
class CConditionalEditorUI : public IConditionalEditor
{
protected:
    //! The underlying conditional item being edited
    CConditional& m_rEdit;

    //! The model used to manage the table of conditions
    CConditionalTreeModel* const m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

    void onBtnNewSelected(wxCommandEvent& event);

    /* OVERRIDES FROM IConditionalEditor */
    void onBtnNew(wxCommandEvent& event) override;
    void onBtnDelete(wxCommandEvent& event) override;
    void onBtnUp(wxCommandEvent& event) override;
    void onBtnDown(wxCommandEvent& event) override;

public:
    CConditionalEditorUI(wxWindow* pParent, CConditional& rEdit);

};

//! @copydoc CConditionalEditorUI
class CConditionalEditor : public CNameDescEditor
{
protected:
    CConditionalEditorUI* const m_pUi;

public:
    CConditionalEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif