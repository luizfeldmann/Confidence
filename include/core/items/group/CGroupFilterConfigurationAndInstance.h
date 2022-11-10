#ifndef _CGROUPFILTERCONFIGURATIONANDINSTANCE_H_
#define _CGROUPFILTERCONFIGURATIONANDINSTANCE_H_

#include "CGroupFilterConfiguration.h"
#include "CGroupFilterInstance.h"

class CGroupFilterConfigurationAndInstance : public virtual CGroupFilterConfiguration, public virtual CGroupFilterInstance
{
public:
    CGroupFilterConfigurationAndInstance();
    ~CGroupFilterConfigurationAndInstance();

    //! @brief Instantiates a new filter of this type
    static std::unique_ptr<IGroupFilter> Create();

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