#ifndef _CGROUPFILTERBASE_H_
#define _CGROUPFILTERBASE_H_

#include "IGroupFilter.h"
#include <vector>

class IProjTreeItem;

class CGroupFilterBase : public virtual IGroupFilter
{
public:
    CGroupFilterBase();
    virtual ~CGroupFilterBase();

    using sptr_t = std::shared_ptr<const IProjTreeItem>;
    using wptr_t = sptr_t::weak_type;
    using ptrvec_t = std::vector<wptr_t>;
    using strvec_t = std::vector<std::string>;

    //! @brief Inserts an item in the filter
    void push_back(wptr_t pInstance);

    //! @brief Erases an item from the filter
    void erase(sptr_t pInstance);

    //! @brief Gets a copy of the list of instances from this filter
    ptrvec_t GetItems() const;

    //! @brief Checks if the item is listed in the filter
    bool Filter(sptr_t pItem) const;

    //! @copydoc IDocumentable::Document
    virtual bool Document(IDocExporter& rExporter) const override;

    //! @copydoc ISerializationNotify::PreSerialize
    virtual bool PreSerialize() override;

protected:
    //! Holds the names of the filtered instances for (de)-serialization
    strvec_t m_vItemNames;

    //! Holds the pointers to the filtered instances for runtime 
    ptrvec_t m_vItemPtrs;
};

#endif