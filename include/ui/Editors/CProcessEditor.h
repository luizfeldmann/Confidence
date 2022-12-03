/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CPROCESSEDITOR_
#define _CPROCESSEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CRunProcessEditorUI;

//! @copydoc CGroupEditorUI
class CRunProcessEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CRunProcessEditorUI* const m_pUiProc;

public:
    //! @brief Constructs a new #CRunProcessEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CRunProcess
    CRunProcessEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif