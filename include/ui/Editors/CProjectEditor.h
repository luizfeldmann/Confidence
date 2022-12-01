#ifndef _CPROJECTEDITOR_
#define _CPROJECTEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CProjectEditorUI;

//! @copydoc CProjectEditorUI
class CProjectEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CProjectEditorUI* const m_pUiProj;

public:
    //! @brief Constructs a new #CProjectEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CProject
    CProjectEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif