#include "docs/CMarkdownExporter.h"
#include "util/Log.h"
#include <cmark.h>

DEFINE_SERIALIZATION_SCHEME(CMarkdownExporter,
    SERIALIZATION_INHERIT(CStoredOutputFileName)
)

REGISTER_POLYMORPHIC_CLASS(CMarkdownExporter);

/* static */ IDocExporter* CMarkdownExporter::Create()
{
    return new CMarkdownExporter;
}

CMarkdownExporter::CMarkdownExporter()
    : CStoredOutputFileName(strDefaultPath + ".md")
{

}

bool CMarkdownExporter::Finish()
{
    bool bStatus = false;

    static const int c_RenderWidth = 80;
    char* const pBuffer = cmark_render_commonmark(GetRootNode(), CMARK_OPT_DEFAULT, c_RenderWidth);
    
    if (!pBuffer)
        CERROR("Failed to 'cmark_render_commonmark' to a buffer");
    else
    {
        GetStream() << pBuffer;
        free(pBuffer);

        bStatus = true;
    }

    return bStatus;
}

EDocExporter CMarkdownExporter::GetType() const
{
    return EDocExporter::Markdown;
}