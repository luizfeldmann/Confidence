#ifndef _IPROJTREEITEM_H_
#define _IPROJTREEITEM_H_

#include "INamedItem.h"
#include "IDescribedItem.h"
#include "ITreeItemCollection.h"
#include "ISerializationNotify.h"
#include "util/ITracked.h"

#pragma warning(disable: 4250)

class IProjTreeItem;
using IProjTreeItem_Tracker_t = ITracked<IProjTreeItem>;

//! @brief Common base for all items placed on the project tree structure
class IProjTreeItem : public virtual INamedItem
                    , public virtual IDescribedItem
                    , public virtual ITreeItemCollection
                    , public ISerializationNotify
                    , public IProjTreeItem_Tracker_t
{
private:
    //! @brief Deleted copy-constructor; class is non-copyable
    IProjTreeItem(const IProjTreeItem&) = delete;

    //! @brief Deleted copy-assignment operator; class is non-copyable
    IProjTreeItem& operator=(const IProjTreeItem&) = delete;

public:
    //! @brief Constructs an empty item
    IProjTreeItem() = default;
    virtual ~IProjTreeItem() = default;

    //! @brief Default move-constructor
    IProjTreeItem(IProjTreeItem&&) = default;

    //! @brief Default move-assignment operator
    IProjTreeItem& operator=(IProjTreeItem&&) = default;

    //! @brief Reflects the type of this item
    virtual ETreeItemType GetType() const = 0;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize() override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;
};

DECLARE_SERIALIZATION_SCHEME(IProjTreeItem_Tracker_t);

#endif