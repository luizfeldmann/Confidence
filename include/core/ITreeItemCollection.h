#ifndef _ICHILDRENCOLLECTION_H_
#define _ICHILDRENCOLLECTION_H_

#include <memory>
#include <vector>
#include <optional>

#include "ETreeItemType.h"

class IProjTreeItem;

//! @brief Interface for collections of tree items
class ITreeItemCollection
{
public:
    ITreeItemCollection() = default;
    virtual ~ITreeItemCollection() = default;

    using PtrItem = std::unique_ptr<IProjTreeItem>;
    using VecPtrItem = std::vector<PtrItem>;
    using OptVecPtrItem = std::optional<std::reference_wrapper<VecPtrItem>>;
    using OptCVecPtrItem = std::optional<std::reference_wrapper<const VecPtrItem>>;

    //! @brief Gets a reference to the container for tree items
    virtual OptVecPtrItem GetItems() = 0;

    //! @brief Gets a reference to the container for tree items
    virtual OptCVecPtrItem GetItems() const = 0;

    //! @brief OR'd enums of item types whose nesting is supported inside the this type
    virtual ETreeItemType GetSupportedChildTypes() const = 0;
};

#endif