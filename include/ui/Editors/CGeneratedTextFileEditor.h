/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CGENERATEDTEXTFILEEDITOR_
#define _CGENERATEDTEXTFILEEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CGeneratedTextFileEditorUI;

//! @copydoc CGeneratedTextFileEditorUI
class CGeneratedTextFileEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CGeneratedTextFileEditorUI* const m_pUiTextFile;

public:
    //! @brief Constructs a new #CGeneratedTextFileEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CGeneratedTextFile
    CGeneratedTextFileEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif