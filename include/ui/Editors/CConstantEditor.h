#ifndef _CCONSTANTEDITOR_
#define _CCONSTANTEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CConstantEditorUI;

//! @copydoc CConstantEditorUI
class CConstantEditor : public CNameDescEditor
{
protected:
    CConstantEditorUI* const m_pUi;

public:
    CConstantEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif