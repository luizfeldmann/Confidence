#ifndef _CGROUPEDITOR_
#define _CGROUPEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IGroupEditor.h"
#include "core/items/CGroup.h"

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
    CGroupEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif