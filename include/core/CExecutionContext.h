#ifndef _CEXECUTIONCONTEXT_H_
#define _CEXECUTIONCONTEXT_H_

#include <list>
#include <memory>
#include "core/CExecutionContextBase.h"
#include "core/IContextStorage.h"

class CInstance;
class CConfiguration;
class IProjTreeItem;

//! @brief Stores the execution state of the project
class CExecutionContext : public CExecutionContextBase
{
private:
    CExecutionContext(const CExecutionContext&) = delete;
    CExecutionContext& operator=(const CExecutionContext&) = delete;

protected:
    //! Maps each item (key) to it's parent (value)
    using parent_map_t = std::map<const IProjTreeItem*, const IProjTreeItem*>;
    parent_map_t m_mParents;

    //! @brief Recursively fills the paternity map for the provided item
    //! @param[in] rParent The parent item whose subitems will be mapped
    void MapParents(const IProjTreeItem& rParent);

    using storage_list_t = std::list<std::shared_ptr<IContextStorage>>;

    //! Stores objects for the duration of the execution
    storage_list_t m_lStorage;

public:
    //! Reference to the instance being executed
    const CInstance& m_rInstance;

    //! Reference to the configuration being executed
    const CConfiguration& m_rConfiguration;

    CExecutionContext(const CProject& rProj, const CInstance& rInst, const CConfiguration& rCfg);

    //! @brief Gets the parent of the requested child item
    //! @param[in] rItem Reference to the item whose parent must be found
    //! @return The parent of the item or nullptr if not found (the project root has no parent)
    const IProjTreeItem* GetParent(const IProjTreeItem& rItem) const;

    //! @brief Stores an object in this context for the duration of the execution
    void Store(std::shared_ptr<IContextStorage> pStore);
};

#endif