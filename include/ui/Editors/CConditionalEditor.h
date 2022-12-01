#ifndef _CCONDITIONALEDITOR_
#define _CCONDITIONALEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CConditionalEditorUI;

//! @copydoc CConditionalEditorUI
class CConditionalEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CConditionalEditorUI* const m_pUi;

public:
    //! @brief Constructs a new #CConditionalEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CConditional
    CConditionalEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif