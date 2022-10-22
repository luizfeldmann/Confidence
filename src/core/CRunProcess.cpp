#include "core/CRunProcess.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CRunProcess,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_bRunAsAdmin)
    SERIALIZATION_MEMBER_AS(m_eStartMode, int)
    SERIALIZATION_MEMBER(m_pPolicy)
    SERIALIZATION_MEMBER(m_vArgs)
)

REGISTER_POLYMORPHIC_CLASS(CRunProcess);

CRunProcess::CRunProcess()
    : CStoredNameItem("<new process>")
    , CStoredDescriptionItem("<no process description>")
    , m_bRunAsAdmin(false)
    , m_eStartMode(EProcessStartWindowMode::Normal)
{

}

CRunProcess::~CRunProcess()
{

}

IProjTreeItem* CRunProcess::Create()
{
    return new CRunProcess;
}

ETreeItemType CRunProcess::GetType() const
{
    return ERunProcess;
}

CRunProcess::vec_args_t& CRunProcess::GetArguments()
{
    return m_vArgs;
}

const CRunProcess::vec_args_t& CRunProcess::GetArguments() const
{
    return m_vArgs;
}

IProcessPolicy* CRunProcess::GetPolicy() const
{
    return m_pPolicy.get();
}

void CRunProcess::SetPolicy(IProcessPolicy* pNewPolicy)
{
    m_pPolicy.reset(pNewPolicy);
}

bool CRunProcess::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "PROCESS:");
}

static const std::string& GetWindowModeString(EProcessStartWindowMode eMode)
{
    static const std::unordered_map<EProcessStartWindowMode, std::string> mModes{
        {EProcessStartWindowMode::Minimized,    "Minimized"},
        {EProcessStartWindowMode::Normal,       "Normal"},
        {EProcessStartWindowMode::Maximized,    "Maximized"},
    };

    return mModes.at(eMode);
}

bool CRunProcess::DocumentCustom(IDocExporter& rExporter) const
{
    // Write 'Run As'
    bool bStatus = rExporter.FormField("Run as:", m_bRunAsAdmin ? "Administrator" : "Current user");

    // Write window state
    if (bStatus)
    {
        bStatus = rExporter.FormField("Window mode:", GetWindowModeString(m_eStartMode));
    }

    // Write the policy
    if (bStatus && m_pPolicy)
    {
        bStatus = m_pPolicy->Document(rExporter);
    }

    // Write the arguments
    if (bStatus && !m_vArgs.empty())
    {
        bStatus = rExporter.List();

        for (vec_args_t::const_iterator it = m_vArgs.cbegin(); bStatus && (it != m_vArgs.cend()); ++it)
        {
            bStatus = rExporter.Item()
                && it->Document(rExporter)
                && rExporter.PopStack();
        }

        bStatus = bStatus && rExporter.PopStack();
    }

    return bStatus;
}

bool CRunProcess::Execute(CExecutionContext& rContext) const
{
    LogExecution();
    bool bStatus = true;

    // Evaluate each argument expression to a literal
    SProcessStartInfo sProc;
    sProc.m_bRunAsAdmin = m_bRunAsAdmin;
    sProc.m_eStartMode = m_eStartMode;

    for (const CProcessArgument& rArgument : GetArguments())
    {
        std::string strExpression = rArgument.GetExpression();
        bStatus = rContext.Evaluate(strExpression);

        if (!bStatus)
            break;

        sProc.m_vArguments.push_back(strExpression);
    }

    if (!m_pPolicy)
    {
        const std::string strName = GetName();
        CWARNING("Process '%s' execution policy is undefined", strName.c_str());
        bStatus = false;
    }
    else if (bStatus)
    {
        bStatus = m_pPolicy->Execute(rContext, sProc);
    }

    return bStatus;
}