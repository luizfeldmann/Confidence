#include "core/CAssignable.h"
#include "core/CExecutionContext.h"
#include "core/items/variable/CEnvironmentVariable.h"

DEFINE_SERIALIZATION_SCHEME(CAssignable,
    SERIALIZATION_MEMBER(m_bExportToEnvironment)
)

CAssignable::CAssignable()
    : m_bExportToEnvironment(false)
{

}

bool CAssignable::Assign(CExecutionContext& rContext, const std::string& strName, std::string& strExpression) const
{
    bool bStatus = rContext.Evaluate(strExpression);

    if (bStatus)
    {
        rContext.SetVariableLiteral(strName, strExpression);

        std::shared_ptr<CEnvironmentVariable> pEnv =
            std::make_shared<CEnvironmentVariable>(strName);

        bStatus = pEnv && pEnv->Set(strExpression.c_str());
        if (bStatus)
            rContext.Store(pEnv);
    }

    return bStatus;
}

bool CAssignable::GetExportToEnv() const
{
    return m_bExportToEnvironment;
}

void CAssignable::SetExportToEnv(bool bExport)
{
    m_bExportToEnvironment = bExport;
}

