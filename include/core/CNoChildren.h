/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CNOCHILDREN_H_
#define _CNOCHILDREN_H_

#include "ITreeItemCollection.h"

//! @brief Implementation of ITreeItemCollection returning no subitems
class CNoChildren : public virtual ITreeItemCollection
{
public:
    //! @brief Constructs a new #CNoChildren
    CNoChildren();

    //! @copydoc ITreeItemCollection::SubItems
    vec_ptr_t SubItems() override;

    //! @copydoc ITreeItemCollection::SubItems
    vec_cptr_t SubItems() const override;

    //! @copydoc ITreeItemCollection::TakeItem
    ptr_t TakeItem(const IProjTreeItem& rItem) override;

    //! @copydoc ITreeItemCollection::AddItem
    bool AddItem(ptr_t pNewItem) override;

    //! @copydoc ITreeItemCollection::SwapItems
    bool SwapItems(const IProjTreeItem& rA, const IProjTreeItem& rB) override;

    //! @copydoc ITreeItemCollection::GetSupportedChildTypes
    ETreeItemType GetSupportedChildTypes() const override;
};

#endif