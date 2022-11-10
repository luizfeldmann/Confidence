#ifndef _IGROUPFILTER_H_
#define _IGROUPFILTER_H_

#include "core/ISerializationNotify.h"
#include "docs/IDocumentable.h"

class CGroup;
class CExecutionContext;

//! @brief Filters under which cases a group should execute
class IGroupFilter : public IDocumentable, public ISerializationNotify
{
public:
    virtual ~IGroupFilter() = default;

    //! @brief Checks if the group should execute
    //! @param[in] rGroup The group owner of this filter
    //! @param[in] rContext The current execution context
    //! @return True if should execute
    virtual bool Filter(const CGroup& rGroup, const CExecutionContext& rContext) const = 0;
};

#endif