#include "docs/CHtmlExporter.h"
#include "docs/CMarkCustomBlock.h"
#include "util/Log.h"
#include <cmark.h>

DEFINE_SERIALIZATION_SCHEME(CHtmlExporter,
    SERIALIZATION_INHERIT(CStoredOutputFileName)
)

REGISTER_POLYMORPHIC_CLASS(CHtmlExporter);

/* HTML CONTENT OF GENERATED PAGES*/
static const char* g_szCssStyles = 
"<style type=\"text/css\">\r\n"
"   .indent { margin-left: 5%; }\r\n"
"   strong { color: darkgrey; }\r\n"
"   em { font-weight: bold; }\r\n"
"   summary { font-weight: bold; }\r\n"
"   code { background: darkgrey; display: inline-block; width: 100%; }\r\n"
"</style>\r\n";

/* CHtmlExporter */

/* static */ IDocExporter* CHtmlExporter::Create()
{
    return new CHtmlExporter;
}

CHtmlExporter::CHtmlExporter()
    : CStoredOutputFileName(strDefaultPath + ".html")
{
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
    bool bStatus = CCommonMarkExporter::Start(rOutput);
    bStatus = bStatus && CreateHtml();

    return bStatus;
}

bool CHtmlExporter::Finish()
{
    bool bStatus = false;

    static const int c_RenderWidth = 80;
    char* const pBuffer = cmark_render_html(GetRootNode(), CMARK_OPT_UNSAFE);

    if (!pBuffer)
        CERROR("Failed to 'cmark_render_html' to a buffer");
    else
    {
        GetStream() << pBuffer;
        free(pBuffer);

        bStatus = true;
    }

    return bStatus;
}

void CHtmlExporter::IncrementHeading()
{
    CCommonMarkExporter::IncrementHeading();

    CMarkCustomBlock cDiv("<div class=\"indent\">", "</div><hr/>");
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

EDocExporter CHtmlExporter::GetType() const
{
    return EDocExporter::HTML;
}