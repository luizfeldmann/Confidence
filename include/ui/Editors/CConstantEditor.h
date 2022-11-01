#ifndef _CCONSTANTEDITOR_
#define _CCONSTANTEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IConstantEditor.h"

class CConstant;

//! @brief Item editor for constant
class CConstantEditorUI : public IConstantEditor
{
protected:
    CConstant& m_rEdit;

    void onExpressionChanged(wxCommandEvent& event) override;
    void onChkboxExport(wxCommandEvent& event) override;

public:
    CConstantEditorUI(wxWindow* pParent, CConstant& rEdit);

};

//! @copydoc CConstantEditorUI
class CConstantEditor : public CNameDescEditor
{
protected:
    CConstantEditorUI* const m_pUi;

public:
    CConstantEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif