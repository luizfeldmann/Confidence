#include "ui/CProcessEditor.h"
#include "ui/Models/CProcessArgsModel.h"
#include "core/CProcessFireForget.h"
#include "core/CProcessWaitCompletion.h"

/* Utils */
static constexpr int c_nChoiceIndexToEnumOffset = 1;

//! @brief Converts the index in the wxChoice box in the UI to a value of EProcessStartWindowMode
static EProcessStartWindowMode WindowModeFromChoiceIndex(int nChoice)
{
    return static_cast<EProcessStartWindowMode>(nChoice - c_nChoiceIndexToEnumOffset);
}

//! @brief Converts the value of EProcessStartWindowMode to the index in the wxChoice box in the UI
static int ChoiceIndexFromWindowMode(EProcessStartWindowMode eMode)
{
    return static_cast<int>(eMode) + c_nChoiceIndexToEnumOffset;
}

/* CRunProcessEditorUI */
CRunProcessEditorUI::CRunProcessEditorUI(wxWindow* pParent, CRunProcess& cEdit)
    : IRunProcessEditor(pParent)
    , m_cEdit(cEdit)
    , m_pModel(new CProcessArgsModel(m_cEdit))
{
    // Create model
    m_dataViewCtrlArgs->AssociateModel(m_pModel);
    m_pModel->DecRef();

    // Draw all initial items
    wxDataViewItem wxRoot(nullptr);
    wxDataViewItemArray arrItems;

    m_pModel->GetChildren(wxRoot, arrItems);
    m_pModel->ItemsAdded(wxRoot, arrItems);

    // Initial state of the UI
    m_checkBoxRunAs->SetValue(m_cEdit.m_bRunAsAdmin);
    m_choiceWindowMode->SetSelection(ChoiceIndexFromWindowMode(m_cEdit.m_eStartMode));
    m_choicePolicy->SetSelection((int)GetCurrentPolicy());
    LoadWaitCompletion();
}

CProcessArgument* CRunProcessEditorUI::GetSelectedArgument() const
{
    return static_cast<CProcessArgument*>(m_dataViewCtrlArgs->GetSelection().GetID());
}

void CRunProcessEditorUI::onToolNewArg(wxCommandEvent& event)
{
    CProcessArgument* pNewItem = m_pModel->NewItem();
    m_dataViewCtrlArgs->Select(wxDataViewItem(pNewItem));
}

void CRunProcessEditorUI::onToolDelArg(wxCommandEvent& event)
{
    CProcessArgument* pSelected = GetSelectedArgument();

    if (nullptr != pSelected)
    {
        m_pModel->DeleteItem(pSelected);
    }
}

void CRunProcessEditorUI::onToolMoveUp(wxCommandEvent& event)
{
    CProcessArgument* pSelected = GetSelectedArgument();

    if (nullptr != pSelected)
    {
        CProcessArgument* pSwap = m_pModel->MoveItem(pSelected, true);

        if (pSwap)
            m_dataViewCtrlArgs->Select(wxDataViewItem(pSwap));
    }
}

void CRunProcessEditorUI::onToolMoveDown(wxCommandEvent& event)
{
    CProcessArgument* pSelected = GetSelectedArgument();

    if (nullptr != pSelected)
    {
        CProcessArgument* pSwap = m_pModel->MoveItem(pSelected, false);

        if (pSwap)
            m_dataViewCtrlArgs->Select(wxDataViewItem(pSwap));
    }
}

void CRunProcessEditorUI::onPolicyChanged(wxCommandEvent& event)
{
    const EProcessPolicy eCurrentPolicyType = GetCurrentPolicy();
    const EProcessPolicy eNewPolicyType = (EProcessPolicy)m_choicePolicy->GetSelection();

    if (eNewPolicyType == eCurrentPolicyType)
        return;

    // Apply the new desired policy
    IProcessPolicy* pNewPolicy;

    switch (eNewPolicyType)
    {
    default:
    case EProcessPolicy::Undefined:
        pNewPolicy = nullptr;
        break;
    case EProcessPolicy::FireForget:
        pNewPolicy = new CProcessFireAndForget;
        break;
    case EProcessPolicy::WaitCompletion:
        pNewPolicy = new CProcessWaitCompletion;
        break;
    }

    m_cEdit.SetPolicy(pNewPolicy);
    LoadWaitCompletion();
}

void CRunProcessEditorUI::onWindowChanged(wxCommandEvent& event)
{
    m_cEdit.m_eStartMode = WindowModeFromChoiceIndex(m_choiceWindowMode->GetSelection());
}

void CRunProcessEditorUI::onAdminChanged(wxCommandEvent& event)
{
    m_cEdit.m_bRunAsAdmin = m_checkBoxRunAs->GetValue();
}

EProcessPolicy CRunProcessEditorUI::GetCurrentPolicy() const
{
    static_assert((int)EProcessPolicy::Undefined == wxNOT_FOUND);

    EProcessPolicy eCurrentPolicyType = EProcessPolicy::Undefined;

    const IProcessPolicy* pCurrentPolicy = m_cEdit.GetPolicy();
    if (nullptr != pCurrentPolicy)
        eCurrentPolicyType = pCurrentPolicy->GetType();
    
    return eCurrentPolicyType;
}

void CRunProcessEditorUI::LoadWaitCompletion()
{
    const CProcessWaitCompletion* const pPolicy = dynamic_cast<CProcessWaitCompletion*>(m_cEdit.GetPolicy());
    const bool bWaitCompletion = (nullptr != pPolicy);

    m_panelWaitCompletion->Show(bWaitCompletion);

    if (bWaitCompletion)
    {
        CProcessWaitCompletion::opt_returncode_t optCode = pPolicy->GetExpectedReturnCode();

        if (optCode.has_value())
            m_comboBoxValidateReturn->ChangeValue(std::to_string(optCode.value()));
        else
            m_comboBoxValidateReturn->SetSelection(0); // select option "<Any>"
    }
}

void CRunProcessEditorUI::onReturnCodeCombo(wxCommandEvent& event)
{
    onReturnCodeText(event);
}

void CRunProcessEditorUI::onReturnCodeText(wxCommandEvent& event)
{
    CProcessWaitCompletion* const pPolicy = dynamic_cast<CProcessWaitCompletion*>(m_cEdit.GetPolicy());

    if (nullptr != pPolicy)
    {
        const int nSelection = m_comboBoxValidateReturn->GetSelection();

        CProcessWaitCompletion::opt_returncode_t optCode;

        if (wxNOT_FOUND == nSelection)
        {
            int nCode = 0;
            const std::string strCode = m_comboBoxValidateReturn->GetValue().ToStdString();

            if (1 == sscanf(strCode.c_str(), "%d", &nCode))
                optCode = nCode;
            else
                m_comboBoxValidateReturn->SetSelection(0); // select option "<Any>"
        }

        pPolicy->SetExpectedReturnCode(optCode);
    }
}

/* CRunProcessEditor */
CRunProcessEditor::CRunProcessEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiProc(new CRunProcessEditorUI(m_pPanel, dynamic_cast<CRunProcess&>(rItem)))
{
    m_pSizer->Add(m_pUiProc, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CRunProcessEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CRunProcessEditor(rNotebook, rItem);
}