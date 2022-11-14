#ifndef _CPROJECTEDITOR_
#define _CPROJECTEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CProjectEditorUI;

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