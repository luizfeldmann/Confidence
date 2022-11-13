#ifndef _CPROJECTEDITOR_
#define _CPROJECTEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IProjectEditor.h"
#include "ui/CAcceleratorEntry.h"

class CProject;
class CDocExporterTableModel;

//! @brief Item editor for the project
class CProjectEditorUI : public IProjectEditor
{
protected:
    //! The underlying project being edited
    CProject& m_rEdit;

    //! The model used to manage the list of exporters
    CDocExporterTableModel* const m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

    //! Event fired when the user choses the type of exporter to add, from the popup menu
    void onBtnNewItemMenu(wxCommandEvent& evt);

public:
    CProjectEditorUI(wxWindow* pParent, CProject& rEdit);

    /* OVERRIDES FROM IProjectEditor */
    void onToolNewDoc(wxCommandEvent& event) override;
    void onToolDeleteDoc(wxCommandEvent& event) override;
};

//! @copydoc CProjectEditorUI
class CProjectEditor : public CNameDescEditor
{
protected:
    CProjectEditorUI* const m_pUiProj;

public:
    CProjectEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif