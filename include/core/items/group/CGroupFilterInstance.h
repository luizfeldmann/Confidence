#ifndef _CGROUPFILTERINSTANCE_H_
#define _CGROUPFILTERINSTANCE_H_

#include "CGroupFilterBase.h"
#include <JInterface/JsonSerialization.h>

//! @brief The group only runs for the selected instances
class CGroupFilterInstance : public virtual CGroupFilterBase
{
public:
    CGroupFilterInstance();
    virtual ~CGroupFilterInstance();

    //! @brief Instantiates a new filter of this type
    static std::unique_ptr<IGroupFilter> Create();

    //! @copydoc IGroupFilter::Filter
    bool Filter(const CGroup& rGroup, const CExecutionContext& rContext) const override;
    
    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
    
    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

protected:
    strvec_t& m_vInstanceNames;

    SERIALIZATION_FRIEND(CGroupFilterInstance);

private:
    CGroupFilterInstance(const CGroupFilterInstance&) = delete;
    CGroupFilterInstance& operator=(const CGroupFilterInstance&) = delete;
};

DECLARE_SERIALIZATION_SCHEME(CGroupFilterInstance);

#endif