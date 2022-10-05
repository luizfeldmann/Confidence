#include "docs/CCommonMarkExporter.h"
#include "util/Log.h"
#include <cmark.h>

/* CMarkdownExporterSession */
class CCommonMarkExporterSession
{
public:
    std::ostream& m_rOutput;
    cmark_node* const m_pRootNode;

    CCommonMarkExporterSession(std::ostream& rOutput)
        : m_rOutput(rOutput)
        , m_pRootNode(cmark_node_new(CMARK_NODE_DOCUMENT))
    {

    }

    ~CCommonMarkExporterSession()
    {
        if (m_pRootNode)
            cmark_node_free(m_pRootNode);
    }
};


/* CMarkdownExporter */
CCommonMarkExporter::CCommonMarkExporter()
{

}

CCommonMarkExporter::~CCommonMarkExporter()
{

}

std::ostream& CCommonMarkExporter::GetStream() const
{
    return m_pSession->m_rOutput;
}

struct cmark_node* CCommonMarkExporter::GetRootNode()
{
    return m_pSession->m_pRootNode;
}

//! @copydoc IDocExporter::Start
bool CCommonMarkExporter::Start(std::ostream& rOutput)
{
    bool bStatus = false;

    m_pSession = std::move(std::make_unique<CCommonMarkExporterSession>(rOutput));

    if (m_pSession && m_pSession->m_pRootNode)
        bStatus = true;
    else
        CERROR("Failed to create the root 'cmark_node' of the document");

    return bStatus;
}