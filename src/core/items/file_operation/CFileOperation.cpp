#include "core/items/file_operation/CFileOperation.h"
#include "core/items/file_operation/IFileDataLocator.h"
#include "core/items/file_operation/IFileOperationContext.h"
#include "core/CExecutionContext.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"
#include <cassert>

DEFINE_SERIALIZATION_SCHEME(CFileOperation,
    SERIALIZATION_MEMBER_AS(m_eDirection, int)
    SERIALIZATION_MEMBER(m_pLocator)
    SERIALIZATION_INHERIT(CStoredExpression)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
)

CFileOperation::CFileOperation()
    : CStoredDescriptionItem("<no description>")
    , m_eDirection(EFileOperationType::Read)
{

}

CFileOperation::~CFileOperation()
{

}

CFileOperation::CFileOperation(CFileOperation&&) noexcept = default;
CFileOperation& CFileOperation::operator=(CFileOperation&&) noexcept = default;

EFileOperationType CFileOperation::GetType() const
{
    return m_eDirection;
}

void CFileOperation::SetType(EFileOperationType eNewType)
{
    m_eDirection = eNewType;
}

IFileDataLocator* CFileOperation::GetLocator()
{
    return m_pLocator.get();
}

const IFileDataLocator* CFileOperation::GetLocator() const
{
    return m_pLocator.get();
}

void CFileOperation::SetLocator(std::unique_ptr<IFileDataLocator> pNewLocator)
{
    m_pLocator.swap(pNewLocator);
}

bool CFileOperation::Execute(CExecutionContext& rContext, IFileOperatorContext& rFile) const
{
    bool bStatus = false;

    if (!m_pLocator)
        CERROR("No locator specified for operation");
    else if (GetType() == EFileOperationType::Read)
    {
        std::string strValue;

        bStatus = rFile.Read(*m_pLocator, strValue);
        
        if (bStatus)
            rContext.SetVariableLiteral(GetExpression(), strValue);
    }
    else
    {
        bStatus = rFile.Write(*m_pLocator, GetExpression());
    }

    return bStatus;
}

std::vector<std::string> CFileOperation::GetDependencies() const
{
    std::vector<std::string> vDepends;

    // Get dependencies on read variable or write expression
    if (GetType() == EFileOperationType::Read)
        vDepends.assign({ GetExpression() }); // In read mode, the expression itself is the name of an output variable
    else
        vDepends = CStoredExpression::GetDependencies(); // In write mode, the expression is evaluated

    // TODO: get dependencies in the locator

    return vDepends;
}

bool CFileOperation::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.Paragraph()
        && rExporter.Text(GetDescription())
        && rExporter.PopStack();

    if (bStatus && m_pLocator)
        bStatus = m_pLocator->Document(rExporter);

    if (bStatus)
    {
        const bool bIsRead = (GetType() == EFileOperationType::Read);

        const std::string strLabel = bIsRead
            ? "(READ) Store in variable:"
            : "(WRITE) Update value to:";

        bStatus = rExporter.FormField(strLabel, GetExpression(), !bIsRead);
    }

    return bStatus;
}