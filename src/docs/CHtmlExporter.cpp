#include "docs/CHtmlExporter.h"

#include "util/Log.h"
#include <cmark.h>

DEFINE_SERIALIZATION_SCHEME(CHtmlExporter,
    SERIALIZATION_INHERIT(CStoredOutputFileName)
)

REGISTER_POLYMORPHIC_CLASS(CHtmlExporter);

/* static */ IDocExporter* CHtmlExporter::Create()
{
    return new CHtmlExporter;
}

CHtmlExporter::CHtmlExporter()
    : CStoredOutputFileName(strDefaultPath + ".html")
{

}

bool CHtmlExporter::Finish()
{
    bool bStatus = false;

    static const int c_RenderWidth = 80;
    char* const pBuffer = cmark_render_html(GetRootNode(), CMARK_OPT_DEFAULT);

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

EDocExporter CHtmlExporter::GetType() const
{
    return EDocExporter::HTML;
}