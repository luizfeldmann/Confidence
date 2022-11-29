#include "docs/CHtmlExporter.h"
#include "docs/CMarkCustomBlock.h"
#include "docs/CDiagram.h"
#include "util/Log.h"
#include <cmark.h>
#include <assert.h>

DEFINE_SERIALIZATION_SCHEME(CHtmlExporter,
    SERIALIZATION_INHERIT(CStoredOutputFileName)
)

REGISTER_POLYMORPHIC_CLASS(CHtmlExporter);

/* HTML CONTENT OF GENERATED PAGES*/
static const char* g_szCssStyles = 
"<style type=\"text/css\">\r\n"
"   .indent { padding-left: 10px; }\r\n"
"   .item { margin-bottom: 10px; padding-left: 10px; padding-right: 16px; border-radius: 5px; background-color: white; }\r\n"
"   .item:nth-of-type(odd) {  border: 1px solid LightSkyBlue; border-left: 4px solid LightSkyBlue;  }\r\n"
"   .item:nth-of-type(odd):hover { background-color: AliceBlue; }\r\n"
"   .item:nth-of-type(even){  border: 1px solid MediumPurple; border-left: 4px solid MediumPurple;  }\r\n"
"   .item:nth-of-type(even):hover { background-color: Lavender; }\r\n"
"   .depends { margin: auto; text-align: center; }\r\n"
"   strong { color: darkgrey; }\r\n"
"   em { font-weight: bold; }\r\n"
"   summary { font-weight: bold; }\r\n"
"   code { margin: 5px; padding: 5px; border-radius: 3px;  border: 1px solid black; background: lightgrey; display: inline-block; width: 98%; box-sizing: border-box; }\r\n"
"   li { border-radius: 4px; padding-left: 10px; margin-bottom: 10px; margin-top: 10px; background-color: white; }\r\n"
"   li:nth-child(odd)  { border: 1px solid PaleGoldenRod; border-left: 4px solid PaleGoldenRod; }\r\n"
"   li:nth-child(odd):hover  { background-color: LightYellow; }\r\n"
"   li:nth-child(even) { border: 1px solid SeaGreen;      border-left: 4px solid SeaGreen; }\r\n"
"   li:nth-child(even):hover { background-color: HoneyDew;  }\r\n"
"</style>\r\n";

/* CHtmlExporterPrivate */

//! @brief Holds internal information required by the #CHtmlExporter for the duration of an exporting session
//! @internal
class CHtmlExporterPrivate : public IDocumentationContext
{
private:
    static std::string c_depdiagTitle;

    //! Stores the dependency diagram
    CDiagram m_diagDeps;

    //! @brief Creates a node with the provided name on the dependency diagram (or gets a pointer if it already exists)
    Agnode_s* Node(const std::string& strNode)
    {
        Agnode_s* pNode = m_diagDeps.Node(strNode);

        char szHash[18];
        {
            const size_t uHash = std::hash<std::string>{}(strNode);
            std::snprintf(szHash, sizeof(szHash), "#%016zX", uHash);
        }
        CDiagram::URL(pNode, szHash);

        return pNode;
    }

public:
    CHtmlExporterPrivate()
        : m_diagDeps(c_depdiagTitle)
    {
    }

    //! @copydoc IDocumentationContext::Dependency
    void Dependency(const std::string& strSrc, const std::string& strDst) override
    {
        auto nodeSrc = Node(strSrc);
        auto nodeDst = Node(strDst);

        std::size_t hEdge = std::hash<std::string>{}(strSrc) ^ (std::hash<std::string>{}(strDst) << 1);
        m_diagDeps.Edge(std::to_string(hEdge), nodeSrc, nodeDst);
    }

    //! @brief Appends a section containing the dependency diagram to the project documentation
    //! @return True if success
    bool GenerateDependencyDiagram(IDocExporter& rExporter)
    {
        bool bStatus = rExporter.Container("item", "");
        bStatus = bStatus && rExporter.Heading();
        bStatus = bStatus && rExporter.Text(c_depdiagTitle);
        bStatus = bStatus && rExporter.PopStack();
        
        if (bStatus)
        {
            std::string strSVGdata;
            bStatus = m_diagDeps.ExportSVG(strSVGdata);
            bStatus = rExporter.Html(
                "<div class=\"depends\">" 
                + strSVGdata 
                + "</div>");
        }

        bStatus = bStatus && rExporter.PopStack();

        return bStatus;
    }
};

std::string CHtmlExporterPrivate::c_depdiagTitle = "Dependency Diagram";

/* CHtmlExporter */

/* static */ IDocExporter* CHtmlExporter::Create()
{
    return new CHtmlExporter;
}

CHtmlExporter::CHtmlExporter()
    : CStoredOutputFileName(GetDefaultPath() + ".html")
{
}

void CHtmlExporter::Dependency(const std::string& strSrc, const std::string& strDst)
{
    m_pPrivate->Dependency(strSrc, strDst);
}

bool CHtmlExporter::CreateHtml()
{
    CMarkCustomBlock cHtml("<html>", "</html>");
    bool bStatus = cHtml && cmark_node_append_child(GetRootNode(), cHtml);
    
    if (bStatus)
    {
        cHtml.disown();
        PushNode(cHtml);

        bStatus = CreateHead(cHtml);
        bStatus = bStatus && CreateBody(cHtml);
    }

    return bStatus;
}

bool CHtmlExporter::CreateHead(CMarkCustomBlock& rHtml)
{
    CMarkCustomBlock cHead("<head>", "</head>");
    bool bStatus = cHead && cmark_node_append_child(rHtml, cHead);
   
    if (bStatus)
    {
        cHead.disown();

        bStatus = CreateStyles(cHead);
    }

    return bStatus;
}

bool CHtmlExporter::CreateStyles(CMarkCustomBlock& rHead)
{
    CMarkNodeWrapper cStyles(CMARK_NODE_HTML_INLINE);
    bool bStatus = cStyles && cmark_node_append_child(rHead, cStyles);

    if (bStatus)
    {
        cStyles.disown();
        bStatus = cmark_node_set_literal(cStyles, g_szCssStyles);
    }

    return true;
}

bool CHtmlExporter::CreateBody(CMarkCustomBlock& rHtml)
{
    CMarkCustomBlock cBody("<body>", "</body>");
    bool bStatus = cBody && cmark_node_append_child(rHtml, cBody);
    
    if (bStatus)
    {
        cBody.disown();
        PushNode(cBody);
    }

    return bStatus;
}

bool CHtmlExporter::Start(std::ostream& rOutput)
{
    m_pPrivate = std::make_unique<CHtmlExporterPrivate>();

    bool bStatus = CCommonMarkExporter::Start(rOutput);
    bStatus = bStatus && CreateHtml();

    return bStatus;
}

bool CHtmlExporter::Finish()
{
    bool bStatus = m_pPrivate->GenerateDependencyDiagram(*this);

    if (bStatus)
    {
        char* const pBuffer = cmark_render_html(GetRootNode(), CMARK_OPT_UNSAFE);

        if (!pBuffer)
            CERROR("Failed to 'cmark_render_html' to a buffer");
        else
        {
            GetStream() << pBuffer;
            free(pBuffer);

            bStatus = true;
        }
    }

    m_pPrivate.reset();

    return bStatus;
}

void CHtmlExporter::IncrementHeading()
{
    CCommonMarkExporter::IncrementHeading();

    CMarkCustomBlock cDiv("<div class=\"indent\">", "</div>");
    cmark_node* pParent = TopNode();

    if (pParent && cDiv && cmark_node_append_child(pParent, cDiv))
    {
        PushNode(cDiv);
        cDiv.disown();
    }
}

void CHtmlExporter::DecrementHeading()
{
    CCommonMarkExporter::DecrementHeading();
    CCommonMarkExporter::PopStack();
}

bool CHtmlExporter::Container(const std::string& strClass, const std::string& strId)
{
    // Build open tag
    std::string strDiv = "<div";

    if (!strClass.empty())
        strDiv += " class=\"" + strClass + "\"";

    if (!strId.empty())
        strDiv += " id=\"" + strId + "\"";

    strDiv += ">";

    // Append to AST
    CMarkCustomBlock cDiv(strDiv.c_str(), "</div>");
    cmark_node* pParent = TopNode();

    bool bStatus = false;
    if (pParent && cDiv && cmark_node_append_child(pParent, cDiv))
    {
        PushNode(cDiv);
        cDiv.disown();

        bStatus = true;
    }

    return bStatus;
}

EDocExporter CHtmlExporter::GetType() const
{
    return EDocExporter::HTML;
}