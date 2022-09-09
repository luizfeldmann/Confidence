#ifndef _CGROUPEDITOR_
#define _CGROUPEDITOR_

#include "ui/CNameDescEditor.h"
#include "wxExport/IGroupEditor.h"
#include "core/CGroup.h"

//! @brief Item editor for groups
class CGroupEditorUI : public IGroupEditor
{
public:
    CGroupEditorUI(wxWindow* pParent);

};

//! @copydoc CGroupEditorUI
class CGroupEditor : public CNameDescEditor
{
protected:
    CGroupEditorUI* const m_pUiGroup;

public:
    CGroupEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);
};

#endif