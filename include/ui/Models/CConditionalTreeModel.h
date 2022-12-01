#ifndef _CCONDITIONALTREEMODEL_H_
#define _CCONDITIONALTREEMODEL_H_

#include "ui/Models/CBaseModelMapped.h"
#include <map>

class CConditional;
class ICondition;
enum class ELogicalComparator;
enum class ELogicalOperator;

//! @brief Represents a tree of logically associated conditional operators and comparators
class CConditionalTreeModel : public CBaseModelMapped
{
protected:
    //! @brief The underlying conditional item being editted
    CConditional& m_rEdit;

    //! Type used to hold associations between the parent and child items in the tree model
    using parent_map_t = std::map<wxDataViewItem, wxDataViewItem>;

    //! Maps child (key) to parent (value)
    mutable parent_map_t m_mapParents;

public:
    //! @brief Constructs a table model to view/edit a tree of conditionals
    //! @param[in] rEdit @copybrief m_rEdit
    CConditionalTreeModel(CConditional& rEdit);

    ~CConditionalTreeModel();

    //! @brief Defines the columns shown in the table
    enum class EColumn : unsigned int
    {
        Icon = 0,   //!< The logic icon
        LHS,        //!< Left expression
        Infix,      //!< The description of the operation
        RHS,        //!< Right expression
        DataMode,   //!< To treat the expressions as string / number
        /* MUST BE LAST */
        Count
    };

    /* Public interface */

    //! @brief Casts the DataViewItem into a condition pointer
    static ICondition* GetPointer(const wxDataViewItem& item);

    //! @brief Casts a condition pointer into a DataViewItem
    static const wxDataViewItem GetViewItem(const ICondition* pCond);

    //! @brief Gets the icon associated to a condition
    static wxIcon GetIcon(const ICondition*);

    //! @brief Gets the icon associated to a comparator type
    static wxIcon GetIcon(const ELogicalComparator& eComparator);

    //! @brief Gets the icon associated to an operator type
    static wxIcon GetIcon(const ELogicalOperator& eOperator);

    //! @brief Creates a new operation item
    //! @returns Handle to the new item
    wxDataViewItem NewItem(std::shared_ptr<ICondition> pNewCondition, const wxDataViewItem& parent);

    //! @brief Deletes the item from the list
    //! @returns True if success
    bool DeleteItem(const wxDataViewItem&);

    //! @brief Moves the given item up or down in the list
    bool MoveItem(const wxDataViewItem& rMoveItem, bool bUp);

    /** @name wxDataViewModel overrides */
    //!@{

    //! @copydoc CBaseModel::GetColumnCount
    unsigned int GetColumnCount() const wxOVERRIDE;

    //! @copydoc CBaseModel::GetParent
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;

    //! @copydoc CBaseModel::IsContainer
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;

    //! @copydoc CBaseModel::GetChildren
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;

    //!@}
};

#endif