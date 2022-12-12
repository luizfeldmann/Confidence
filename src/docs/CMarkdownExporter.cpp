/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "docs/CMarkdownExporter.h"
#include "util/Log.h"
#include <cmark.h>

DEFINE_SERIALIZATION_SCHEME(CMarkdownExporter,
    SERIALIZATION_INHERIT(CStoredOutputFileName)
)

REGISTER_POLYMORPHIC_CLASS(CMarkdownExporter);

/* CMarkdownExporter */

/* static */ IDocExporter* CMarkdownExporter::Create()
{
    return new CMarkdownExporter;
}

CMarkdownExporter::CMarkdownExporter()
    : CStoredOutputFileName(GetDefaultPath() + ".md")
{

}

void CMarkdownExporter::Dependency(const std::string& strSrc, const std::string& strDst)
{

}

bool CMarkdownExporter::Container(const std::string& strClass, const std::string& strId)
{
    // In MD, just a fake block that does nothing
    cmark_node* pBlock = NewNode(CMARK_NODE_CUSTOM_BLOCK, true);
    return (nullptr != pBlock);
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

        #ifdef free
        #undef free
        #endif

        cmark_get_default_mem_allocator()->free(pBuffer);

        bStatus = true;
    }

    return bStatus;
}

EDocExporter CMarkdownExporter::GetType() const
{
    return EDocExporter::Markdown;
}