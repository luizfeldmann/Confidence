#ifndef _CGENERATEDTEXTFILEEDITOR_
#define _CGENERATEDTEXTFILEEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CGeneratedTextFileEditorUI;

//! @copydoc CGeneratedTextFileEditorUI
class CGeneratedTextFileEditor : public CNameDescEditor
{
protected:
    CGeneratedTextFileEditorUI* const m_pUiTextFile;

public:
    CGeneratedTextFileEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif