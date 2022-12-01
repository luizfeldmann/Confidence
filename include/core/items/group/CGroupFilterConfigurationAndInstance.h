#ifndef _CGROUPFILTERCONFIGURATIONANDINSTANCE_H_
#define _CGROUPFILTERCONFIGURATIONANDINSTANCE_H_

#include "CGroupFilterConfiguration.h"
#include "CGroupFilterInstance.h"

//! @brief The group only runs for the selected configurations AND the selected instances
//! @details A combination of both #CGroupFilterConfiguration and #CGroupFilterInstance
class CGroupFilterConfigurationAndInstance : public IGroupFilter
{
protected:
    //! @brief Subfilter used to filter the configurations
    CGroupFilterConfiguration m_byConfig;

    //! @brief Subfilter used to filter the instances
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

    //! @brief Reflects the type associated with this filter implementation
    static ETreeItemType GetOwnType();

    //! @copydoc IGroupFilter::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IGroupFilter::push_back
    void push_back(wptr_t pItem) override;

    //! @copydoc IGroupFilter::erase
    void erase(sptr_t pItem) override;

    //! @copydoc IGroupFilter::GetItems
    ptrvec_t GetItems() const override;

    //! @copydoc IGroupFilter::FilterItem
    bool FilterItem(sptr_t pItem) const override;

    //! @brief @copybrief IGroupFilter::Filter
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