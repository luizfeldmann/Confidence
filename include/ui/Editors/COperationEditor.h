#ifndef _COPERATIONEDITOR_H_
#define _COPERATIONEDITOR_H_

#include "ui/Editors/CNameDescEditor.h"

class COperationEditorUI;

//! @copydoc COperationEditorUI
class COperationEditor : public CNameDescEditor
{
protected:
    COperationEditorUI* const m_pUi;

public:
    COperationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif