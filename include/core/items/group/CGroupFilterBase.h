/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CGROUPFILTERBASE_H_
#define _CGROUPFILTERBASE_H_

#include "IGroupFilter.h"
#include <JInterface/JsonSerialization.h>

//! @brief Base implementation of #IGroupFilter where the pointers to the filtered items are stored in a collection.
//!        The pointers are (de)-serialized as a collection of names of the key items.
class CGroupFilterBase : public IGroupFilter
{
public:
    CGroupFilterBase();
    virtual ~CGroupFilterBase();

    //! @brief Type of collection holding the names of the filtered items
    using strvec_t = std::vector<std::string>;

    //! @copydoc IGroupFilter::push_back
    void push_back(wptr_t pItem) override;

    //! @copydoc IGroupFilter::erase
    void erase(sptr_t pItem) override;

    //! @copydoc IGroupFilter::GetItems
    ptrvec_t GetItems() const override;

    //! @copydoc IGroupFilter::FilterItem
    bool FilterItem(sptr_t pItem) const override;

    //! @copydoc IDocumentable::Document
    virtual bool Document(IDocExporter& rExporter) const override;

    //! @copydoc ISerializationNotify::PreSerialize
    virtual bool PreSerialize() override;

protected:
    //! Holds the names of the filtered instances for (de)-serialization
    strvec_t m_vItemNames;

    SERIALIZATION_FRIEND(CGroupFilterBase);

    //! Holds the pointers to the filtered instances for runtime 
    ptrvec_t m_vItemPtrs;
};

DECLARE_SERIALIZATION_SCHEME(CGroupFilterBase);

#endif