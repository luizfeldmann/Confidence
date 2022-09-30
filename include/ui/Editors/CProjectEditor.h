#ifndef _CPROJECTEDITOR_
#define _CPROJECTEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IProjectEditor.h"

//! @brief Item editor for the project
class CProjectEditorUI : public IProjectEditor
{
public:
    CProjectEditorUI(wxWindow* pParent);

};

//! @copydoc CProjectEditorUI
class CProjectEditor : public CNameDescEditor
{
protected:
    CProjectEditorUI* const m_pUiProj;

public:
    CProjectEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);
};

#endif