#ifndef _COPERATIONEDITOR_H_
#define _COPERATIONEDITOR_H_

#include "ui/Editors/CNameDescEditor.h"

class COperationEditorUI;

//! @copydoc COperationEditorUI
class COperationEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    COperationEditorUI* const m_pUi;

public:
    //! @brief Constructs a new #COperationEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CFileOperations
    COperationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif