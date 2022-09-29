#ifndef _CPROCESSARGSMODEL_H_
#define _CPROCESSARGSMODEL_H_

#include "ui/Models/CBaseModel.h"
#include <unordered_map>

class CRunProcess;
class CProcessArgument;

//! @brief Model to connect the GUI to the backend project structure
class CProcessArgsModel : public CBaseModel
{
protected:
    //! @brief The underlying process being edited
    CRunProcess& m_rEdit;

    //! @brief Defines the columns shown in the arguments list
    enum class EProcessArgsColumn
    {
        Description = 0,
        Value,

        /* MUST BE LAST */
        Count
    };

    //! Stores the callbacks to read/write each column
    std::unordered_map<EProcessArgsColumn, std::unique_ptr<IModelColumnHandler>> m_mapColumns;

    //! @copydoc CBaseModel::GetColumnInfo
    IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) override;

    //! @copydoc CBaseModel::GetColumnInfo
    const IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) const override;

public:
    CProcessArgsModel(CRunProcess& rEdit);
    ~CProcessArgsModel();

    /* Public interface */
    CProcessArgument* NewItem();
    bool DeleteItem(CProcessArgument*);

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif