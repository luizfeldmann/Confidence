#ifndef _CPROCESSEDITOR_
#define _CPROCESSEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CRunProcessEditorUI;

//! @copydoc CGroupEditorUI
class CRunProcessEditor : public CNameDescEditor
{
protected:
    CRunProcessEditorUI* const m_pUiProc;

public:
    CRunProcessEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif