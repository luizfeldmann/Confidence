#ifndef _CNOCHILDREN_H_
#define _CNOCHILDREN_H_

#include "ITreeItemCollection.h"

//! @brief Implementation of ITreeItemCollection returning no subitems
class CNoChildren : public virtual ITreeItemCollection
{
public:
    using ITreeItemCollection::ITreeItemCollection;

    //! @copydoc ITreeItemCollection::GetItems
    OptVecPtrItem GetItems() override;

    //! @copydoc ITreeItemCollection::GetItems
    OptCVecPtrItem GetItems() const override;

    //! @copydoc ITreeItemCollection::GetSupportedChildTypes
    ETreeItemType GetSupportedChildTypes() const override;
};

#endif