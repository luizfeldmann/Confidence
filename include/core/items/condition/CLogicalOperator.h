#ifndef _CLOGICALOPERATOR_H_
#define _CLOGICALOPERATOR_H_

#include "ICondition.h"
#include <memory>
#include <vector>

//! @brief The type of logical operation
enum class ELogicalOperator
{
    And,    //!< A && B && ...
    Or,     //!< A || B || ...
    Not,    //!< !A
};

//! @brief Performs a logical operation on the result of subconditions
class CLogicalOperator : public ICondition
{
private:
    CLogicalOperator(const CLogicalOperator&) = delete;
    CLogicalOperator& operator=(const CLogicalOperator&) = delete;

public:
    using condition_ptr_t = std::shared_ptr<ICondition>;
    using const_condition_ptr_t = std::shared_ptr<const ICondition>;
    using vec_cond_t = std::vector<condition_ptr_t>;
    using vec_ccond_t = std::vector<const_condition_ptr_t>;

    //! @brief Reflects the operator type
    virtual ELogicalOperator GetType() const = 0;

    //! @brief Gets a copy of the collection of subitems
    virtual vec_cond_t SubItems() = 0;

    //! @copydoc SubItems
    virtual vec_ccond_t SubItems() const = 0;

    //! @brief Adds the new item to the collection
    //! @param[in] pNewItem Pointer to the new item to add
    //! @return True if success
    virtual bool AddItem(condition_ptr_t pNewItem) = 0;

    //! @brief Removes the item from the collection
    //! @param[in] pItem Pointer to the item to remove
    //! @return True if success
    virtual bool EraseItem(const_condition_ptr_t pItem) = 0;

    //! @brief Swaps the positions of the requested items in the collection
    //! @return True if success
    virtual bool SwapItems(const_condition_ptr_t pA, const_condition_ptr_t pB) = 0;

    //! @brief Instantiates a new operator of the given type
    static std::shared_ptr<CLogicalOperator> Create(ELogicalOperator eType);

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
        
    CLogicalOperator();
    virtual ~CLogicalOperator();
};

#endif