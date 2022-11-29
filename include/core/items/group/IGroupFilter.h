#ifndef _IGROUPFILTER_H_
#define _IGROUPFILTER_H_

#include "core/ISerializationNotify.h"
#include "core/ETreeItemType.h"
#include "docs/IDocumentable.h"
#include <memory>
#include <vector>

class CGroup;
class IProjTreeItem;
class CExecutionContext;

//! @brief Filters under which cases a group should execute
class IGroupFilter : public IDocumentable, public ISerializationNotify
{
public:
    virtual ~IGroupFilter() = default;

    //! @brief Type of the pointer to the class used as "keys" for the filter
    using sptr_t = std::shared_ptr<const IProjTreeItem>;

    //! @copydoc IGroupFilter::sptr_t
    using wptr_t = sptr_t::weak_type;

    //! @brief Type of the collection holding pointers to the filtered items
    using ptrvec_t = std::vector<wptr_t>;

    //! @brief Inserts an item in the filter
    virtual void push_back(wptr_t pItem) = 0;

    //! @brief Erases an item from the filter
    virtual void erase(sptr_t pItem) = 0;

    //! @brief Gets a list of filter items
    virtual ptrvec_t GetItems() const = 0;

    //! @brief Reads the filter status of an item
    //! @param[in] pItem The item whose status is to be read
    virtual bool Filter(sptr_t pItem) const = 0;

    //! @brief Gets the accepted type of filtered item
    virtual ETreeItemType GetType() const = 0;

    //! @brief Checks if the group should execute
    //! @param[in] rGroup The group owner of this filter
    //! @param[in] rContext The current execution context
    //! @return True if should execute
    virtual bool Filter(const CGroup& rGroup, const CExecutionContext& rContext) const = 0;
};

#endif