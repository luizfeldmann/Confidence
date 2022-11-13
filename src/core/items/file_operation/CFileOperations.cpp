#include "core/items/file_operation/CFileOperations.h"
#include "core/items/file_operation/IFileOperationHandler.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CFileOperations,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_strFilePath)
    SERIALIZATION_MEMBER(m_pHandler)
    SERIALIZATION_MEMBER(m_vOperations)
)

REGISTER_POLYMORPHIC_CLASS(CFileOperations);

CFileOperations::CFileOperations()
    : CStoredNameItem("<new file operation>")
    , CStoredDescriptionItem("<no operation description>")
{

}

IProjTreeItem::ptr_t CFileOperations::Create()
{
    return ptr_t(new CFileOperations);
}

CFileOperations::~CFileOperations()
{

}

ETreeItemType CFileOperations::GetType() const
{
    return EOperations;
}

std::string CFileOperations::GetFilePath() const
{
    return m_strFilePath;
}

void CFileOperations::SetFilePath(const std::string& strNewPath)
{
    m_strFilePath = strNewPath;
}

IFileOperationHandler* CFileOperations::GetHandler() const
{
    return m_pHandler.get();
}

void CFileOperations::SetHandler(std::unique_ptr<IFileOperationHandler> pNewHandler)
{
    m_pHandler.swap(pNewHandler);
}

CFileOperations::vec_oper_t& CFileOperations::GetItems()
{
    return m_vOperations;
}

const CFileOperations::vec_oper_t& CFileOperations::GetItems() const
{
    return m_vOperations;
}

bool CFileOperations::Execute(CExecutionContext& rContext) const
{
    bool bStatus = false;

    const std::string strName = GetName();

    if (!m_pHandler)
        CERROR("No file handler is associated to operator '%s'", strName.c_str());
    else
    {
        std::string strFile = GetFilePath();
        std::shared_ptr<IFileOperatorContext> m_pFile;

        if (rContext.Evaluate(strFile))
            m_pFile = m_pHandler->CreateContext(strFile);

        if (!m_pFile)
            CERROR("Failed to create a file handler context for operator '%s' on file '%s'", strName.c_str(), strFile.c_str());
        else
        {
            for (const CFileOperation& rOper : m_vOperations)
            {
                bStatus = rOper.Execute(rContext, *m_pFile);

                if (!bStatus)
                    break;
            }
        }
    }

    return bStatus;
}

bool CFileOperations::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "FILE OPERATIONS:");
}

bool CFileOperations::DocumentCustom(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Destination Path:", GetFilePath(), true);

    if (bStatus && m_pHandler)
        bStatus = m_pHandler->Document(rExporter);

    if (bStatus && !m_vOperations.empty())
    {
        bStatus = rExporter.List();

        for (vec_oper_t::const_iterator it = m_vOperations.cbegin(); bStatus && (it != m_vOperations.cend()); ++it)
        {
            bStatus = rExporter.Item()
                && it->Document(rExporter)
                && rExporter.PopStack();
        }

        bStatus = bStatus && rExporter.PopStack();
    }

    return bStatus;
}