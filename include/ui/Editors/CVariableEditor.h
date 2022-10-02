#ifndef _CVARIABLEEDITOR_
#define _CVARIABLEEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IVariableEditor.h"
#include "ui/Models/CVariableTableModel.h"
#include "core/CVariable.h"

//! @brief Item editor for variable types
class CVariableEditorUI : public IVariableEditor
{
protected:
    //! Reference to the variable being edited
    CVariable& m_rVar;

    //! Pointer to the model managing the data view table for the variable
    CVariableTableModel* m_pModel;

public:
    CVariableEditorUI(wxWindow* pParent, CVariable& rVar);

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