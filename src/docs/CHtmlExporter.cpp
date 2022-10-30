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
"   .indent { padding-left: 10px; }\r\n"
"   .item { margin-bottom: 10px; padding-left: 10px; padding-right: 16px; border-radius: 5px; background-color: white; }\r\n"
"   .item:nth-of-type(odd) {  border: 1px solid LightSkyBlue; border-left: 4px solid LightSkyBlue;  }\r\n"
"   .item:nth-of-type(odd):hover { background-color: AliceBlue; }\r\n"
"   .item:nth-of-type(even){  border: 1px solid MediumPurple; border-left: 4px solid MediumPurple;  }\r\n"
"   .item:nth-of-type(even):hover { background-color: Lavender; }\r\n"
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

/* CHtmlExporter */

/* static */ IDocExporter* CHtmlExporter::Create()
{
    return new CHtmlExporter;
}

CHtmlExporter::CHtmlExporter()
    : CStoredOutputFileName(GetDefaultPath() + ".html")
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

bool CHtmlExporter::Container()
{
    bool bStatus = false;

    CMarkCustomBlock cDiv("<div class=\"item\">", "</div>");
    cmark_node* pParent = TopNode();

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