#ifndef _CCONDITIONALEDITOR_
#define _CCONDITIONALEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IConditionalEditor.h"

class CConditional;
class CConditionalTreeModel;

//! @brief Item editor for conditionals
class CConditionalEditorUI : public IConditionalEditor
{
protected:
    CConditional& m_rEdit;
    CConditionalTreeModel* const m_pModel;

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