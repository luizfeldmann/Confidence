#ifndef _CGROUPFILTERCONFIGURATIONANDINSTANCE_H_
#define _CGROUPFILTERCONFIGURATIONANDINSTANCE_H_

#include "CGroupFilterConfiguration.h"
#include "CGroupFilterInstance.h"

class CGroupFilterConfigurationAndInstance : public IGroupFilter
{
protected:
    CGroupFilterConfiguration m_byConfig;
    CGroupFilterInstance m_byInstance;

    SERIALIZATION_FRIEND(CGroupFilterConfigurationAndInstance);

    //! @brief Gets the appropriate subfilter to handle the provided item
    IGroupFilter* GetSubFilter(sptr_t pItem);

    //! @copydoc GetSubFilter
    const IGroupFilter* GetSubFilter(sptr_t pItem) const;

public:
    CGroupFilterConfigurationAndInstance();
    ~CGroupFilterConfigurationAndInstance();

    //! @brief Instantiates a new filter of this type
    static std::unique_ptr<IGroupFilter> Create();

    static ETreeItemType GetOwnType();

    //! @copydoc IGroupFilter::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IGroupFilter::push_back
    void push_back(wptr_t pItem) override;

    //! @copydoc IGroupFilter::erase
    void erase(sptr_t pItem) override;

    //! @copydoc IGroupFilter::GetItems
    ptrvec_t GetItems() const override;

    //! @copydoc IGroupFilter::Filter
    bool Filter(sptr_t pItem) const override;

    //! @copydoc IGroupFilter::Filter
    bool Filter(const CGroup& rGroup, const CExecutionContext& rContext) const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;
};

DECLARE_SERIALIZATION_SCHEME(CGroupFilterConfigurationAndInstance);

#endif