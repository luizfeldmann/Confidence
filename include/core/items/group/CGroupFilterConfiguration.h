/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CGROUPFILTERCONFIGURATION_H_
#define _CGROUPFILTERCONFIGURATION_H_

#include "CGroupFilterBase.h"
#include <JInterface/JsonSerialization.h>

//! @brief The group only runs for the selected configurations
class CGroupFilterConfiguration : public CGroupFilterBase
{
public:
    CGroupFilterConfiguration();
    virtual ~CGroupFilterConfiguration();

    //! @brief Instantiates a new filter of this type
    static std::unique_ptr<IGroupFilter> Create();

    //! @brief Reflects the type associated with this filter implementation
    static ETreeItemType GetOwnType();

    //! @copydoc IGroupFilter::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IGroupFilter::Filter
    bool Filter(const CGroup& rGroup, const CExecutionContext& rContext) const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

private:
    CGroupFilterConfiguration(const CGroupFilterConfiguration&) = delete;
    CGroupFilterConfiguration& operator=(const CGroupFilterConfiguration&) = delete;
};

DECLARE_SERIALIZATION_SCHEME(CGroupFilterConfiguration);

#endif