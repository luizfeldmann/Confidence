#ifndef _CVARIABLEEDITOR_
#define _CVARIABLEEDITOR_

#include "ui/CNameDescEditor.h"
#include "wxExport/IVariableEditor.h"
#include "core/CVariable.h"

//! @brief Item editor for variable types
class CVariableEditorUI : public IVariableEditor
{
public:
    CVariableEditorUI(wxWindow* pParent);

};

//! @copydoc CVariableEditorUI
class CVariableEditor : public CNameDescEditor
{
protected:
    CVariableEditorUI* const m_pUiVar;

public:
    CVariableEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);
};

#endif