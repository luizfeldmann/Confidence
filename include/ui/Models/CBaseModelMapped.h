#ifndef _CBASEMODELMAPPED_H_
#define _CBASEMODELMAPPED_H_

#include "CBaseModel.h"
#include <unordered_map>

//! @brief Implementation of CBaseModel using an unordered_map to register the columns
class CBaseModelMapped : public CBaseModel
{
public:
    using column_map_t = std::unordered_map<unsigned int, std::unique_ptr<IModelColumnHandler>>;

    CBaseModelMapped();

protected:
    //! Stores the callbacks to read/write each column
    column_map_t m_mapColumns;

    //! @copydoc CBaseModel::GetColumnInfo
    IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) override;

    //! @copydoc CBaseModel::GetColumnInfo
    const IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) const override;
};

#endif