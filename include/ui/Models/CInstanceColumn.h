#ifndef _CINSTANCECOLUMN_H_
#define _CINSTANCECOLUMN_H_

#include <wx/dataview.h>
#include <memory>

class CInstance;

//! @brief Implementation of wxDataViewColumn where the column is associated to a CInstance
class CInstanceColumn : public wxDataViewColumn
{
protected:
    //! @brief Pointer to the instance associated to this column
    std::weak_ptr<const CInstance> m_pInstance;

    //! @brief Returns the icon of the column
    static const wxIcon& GetIcon(bool bHasInstances);

public:
    //! @brief Creates a column related to a given instance
    //! @param[in] pInst The instance associated to this column
    //! @param[in] nModelColumn Index used to track this column in the model
    //! @param[in] eMode Defines if the cells are inert, activatable or editable
    CInstanceColumn(std::shared_ptr<const CInstance> pInst, unsigned int nModelColumn, wxDataViewCellMode eMode);

    //! @brief Creates a default column related to no specific instance
    //! @param[in] nModelColumn Index used to track this column in the model
    //! @param[in] eMode Defines if the cells are inert, activatable or editable
    CInstanceColumn(unsigned int nModelColumn, wxDataViewCellMode eMode);

    //! @brief Gets a pointer to the associated instance, if any
    std::shared_ptr<const CInstance> GetInstance() const;

    //! @brief Searches the wxDataViewCtrl for the column with the given model index
    //! @param[in] nModelColumn Index in the model of the column being searched
    //! @param[in] pCtrl Pointer to the control containing the searched column
    static CInstanceColumn* GetColumnByModelIndex(unsigned nModelColumn, const wxDataViewCtrl* const pCtrl);
};

#endif