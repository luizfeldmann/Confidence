#include "ui/Models/CBaseModelMapped.h"

CBaseModelMapped::CBaseModelMapped()
{

}

IModelColumnHandler* CBaseModelMapped::GetColumnInfo(unsigned int nModelColumn)
{
    IModelColumnHandler* pInfo = nullptr;

    const column_map_t::iterator itFound = m_mapColumns.find(nModelColumn);
    if (m_mapColumns.end() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

const IModelColumnHandler* CBaseModelMapped::GetColumnInfo(unsigned int nModelColumn) const
{
    const IModelColumnHandler* pInfo = nullptr;

    const column_map_t::const_iterator itFound = m_mapColumns.find(nModelColumn);
    if (m_mapColumns.cend() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}