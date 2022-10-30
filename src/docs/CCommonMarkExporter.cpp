#include "docs/CCommonMarkExporter.h"
#include "util/Log.h"
#include "docs/CMarkCustomBlock.h"
#include <stack>

/* CMarkdownExporterSession */

//! @brief Exporter session state
class CCommonMarkExporterSession : public CMarkNodeWrapper
{
public:
    //! Stream where the exported document must be written to
    std::ostream& m_rOutput;

    //! Stack of nodes in the current tree branch
    std::stack<cmark_node*> m_pStack;

    //! Level of heading (title, subtitle, section, subsection, etc...)
    int m_nHeading = 1;

    CCommonMarkExporterSession(std::ostream& rOutput)
        : CMarkNodeWrapper(CMARK_NODE_DOCUMENT)
        , m_rOutput(rOutput)
    {
        m_pStack.push(GetRootNode());
    }

    cmark_node* GetRootNode()
    {
        return m_pNode;
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
    return m_pSession->GetRootNode();
}

struct cmark_node* CCommonMarkExporter::TopNode()
{
    return m_pSession->m_pStack.top();
}

void CCommonMarkExporter::PushNode(struct cmark_node* pNode)
{
    return m_pSession->m_pStack.push(pNode);
}

struct cmark_node* CCommonMarkExporter::NewNode(int nNodeType, bool bPushStack)
{
    bool bStatus = false;

    CMarkNodeWrapper cNewNode((cmark_node_type)nNodeType);
    cmark_node* pParent = TopNode();

    if (pParent && cNewNode && cmark_node_append_child(pParent, cNewNode))
    {
        if (bPushStack)
            PushNode(cNewNode);

        cNewNode.disown();
        bStatus = true;
    }

    return bStatus ? (cmark_node*)cNewNode : nullptr;
}

bool CCommonMarkExporter::PopStack()
{
    m_pSession->m_pStack.pop();
    return !m_pSession->m_pStack.empty();
}

//! @copydoc IDocExporter::Start
bool CCommonMarkExporter::Start(std::ostream& rOutput)
{
    bool bStatus = false;

    m_pSession = std::move(std::make_unique<CCommonMarkExporterSession>(rOutput));

    if (m_pSession && GetRootNode())
        bStatus = true;
    else
        CERROR("Failed to create the root 'cmark_node' of the document");

    return bStatus;
}

int CCommonMarkExporter::GetHeading() const
{
    return m_pSession->m_nHeading;
}

void CCommonMarkExporter::IncrementHeading()
{
    m_pSession->m_nHeading++;
}

void CCommonMarkExporter::DecrementHeading()
{
    m_pSession->m_nHeading--;
}

bool CCommonMarkExporter::Heading()
{
    cmark_node* pHeading = NewNode(CMARK_NODE_HEADING, true);
    return (pHeading && cmark_node_set_heading_level(pHeading, GetHeading()));
}

bool CCommonMarkExporter::Strong()
{
    cmark_node* pStyle = NewNode(CMARK_NODE_STRONG, true);
    return (nullptr != pStyle);
}

bool CCommonMarkExporter::Emphasis()
{
    cmark_node* pStyle = NewNode(CMARK_NODE_EMPH, true);
    return (nullptr != pStyle);
}

bool CCommonMarkExporter::Paragraph()
{
    cmark_node* pParagraph = NewNode(CMARK_NODE_PARAGRAPH, true);
    return (nullptr != pParagraph);
}

bool CCommonMarkExporter::Text(const std::string& strText)
{
    cmark_node* pText = NewNode(CMARK_NODE_TEXT, false);
    return (pText && cmark_node_set_literal(pText, strText.c_str()));
}

bool CCommonMarkExporter::FormField(const std::string& strLabel, const std::string& strValue, bool bCode = false)
{
    bool bStatus = Paragraph()
        && Emphasis()
        && Text(strLabel)
        && PopStack()
        && Text(" ")
        && (bCode ? Code(strValue) : Text(strValue))
        && PopStack();

    return bStatus;
}

bool CCommonMarkExporter::List()
{
    cmark_node* pList = NewNode(CMARK_NODE_LIST, true);
    return (nullptr != pList);
}

bool CCommonMarkExporter::Item()
{
    cmark_node* pItem = NewNode(CMARK_NODE_ITEM, true);
    return (nullptr != pItem);
}

bool CCommonMarkExporter::Code(const std::string& strText)
{
    cmark_node_type eNodeType = (std::string::npos == strText.find('\n'))
        ? CMARK_NODE_CODE        // Single line code 
        : CMARK_NODE_CODE_BLOCK; // Multi-line code block

    cmark_node* pCode = NewNode(eNodeType, false);
    return (pCode && cmark_node_set_literal(pCode, strText.c_str()));
}

bool CCommonMarkExporter::Collapsible()
{
    bool bStatus = false;

    CMarkCustomBlock cDetails("<details open>", "</details>");
    cmark_node* pParent = TopNode();

    if (pParent && cDetails && cmark_node_append_child(pParent, cDetails))
    {
        PushNode(cDetails);
        cDetails.disown();
        bStatus = true;
    }

    return bStatus;
}

bool CCommonMarkExporter::Summary()
{
    bool bStatus = false;

    CMarkCustomBlock cSummary("<summary>", "</summary>");
    cmark_node* pParent = TopNode();

    if (pParent && cSummary && cmark_node_append_child(pParent, cSummary))
    {
        PushNode(cSummary);
        cSummary.disown();
        bStatus = true;
    }

    return bStatus;
}