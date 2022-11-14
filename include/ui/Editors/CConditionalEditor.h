#ifndef _CCONDITIONALEDITOR_
#define _CCONDITIONALEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CConditionalEditorUI;

//! @copydoc CConditionalEditorUI
class CConditionalEditor : public CNameDescEditor
{
protected:
    CConditionalEditorUI* const m_pUi;

public:
    CConditionalEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif