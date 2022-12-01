#ifndef _CCONSTANTEDITOR_
#define _CCONSTANTEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CConstantEditorUI;

//! @copydoc CConstantEditorUI
class CConstantEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CConstantEditorUI* const m_pUi;

public:
    //! @brief Constructs a new #CConstantEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CConstant
    CConstantEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif