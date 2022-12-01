#ifndef _CMARKCUSTOMBLOCK_H_
#define _CMARKCUSTOMBLOCK_H_

#include "docs/CMarkNodeWrapper.h"

//! @brief A generic tree item for the cmark interface with custom opening/closing tags
class CMarkCustomBlock : public CMarkNodeWrapper
{
public:
    //! @brief Constructs a custom with with provided open/close tags
    //! @param[in] szStart Text for "on_enter" event
    //! @param[in] szEnd Text for "on_exit" event
    CMarkCustomBlock(const char* szStart, const char* szEnd);
};

#endif