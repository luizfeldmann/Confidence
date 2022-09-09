#ifndef _CGENERATEDTEXTFILEEDITOR_
#define _CGENERATEDTEXTFILEEDITOR_

#include "ui/CNameDescEditor.h"
#include "wxExport/ITextFileEditor.h"
#include "core/CGeneratedTextFile.h"

//! @brief Item editor for generated text files
class CGeneratedTextFileEditorUI : public ITextFileEditor
{
public:
    CGeneratedTextFileEditorUI(wxWindow* pParent);

};

//! @copydoc CGeneratedTextFileEditorUI
class CGeneratedTextFileEditor : public CNameDescEditor
{
protected:
    CGeneratedTextFileEditorUI* const m_pUiTextFile;

public:
    CGeneratedTextFileEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);
};

#endif