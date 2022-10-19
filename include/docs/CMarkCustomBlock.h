#ifndef _CMARKCUSTOMBLOCK_H_
#define _CMARKCUSTOMBLOCK_H_

#include "docs/CMarkNodeWrapper.h"

//! @brief A generic tree item for the cmark interface with custom opening/closing tags
class CMarkCustomBlock : public CMarkNodeWrapper
{
public:
    CMarkCustomBlock(const char* szStart, const char* szEnd);
};

#endif