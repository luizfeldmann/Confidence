#include "docs/CMarkCustomBlock.h"

CMarkCustomBlock::CMarkCustomBlock(const char* szStart, const char* szEnd)
    : CMarkNodeWrapper(CMARK_NODE_CUSTOM_BLOCK)
{
    bool bStatus = cmark_node_set_on_enter(m_pNode, szStart);
    bStatus = bStatus && cmark_node_set_on_exit(m_pNode, szEnd);

    if (!bStatus)
        reset();
}