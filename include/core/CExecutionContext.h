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
    //! Pointer to the instance being executed
    std::weak_ptr<const CInstance> m_pInstance;

    //! Pointer to the configuration being executed
    std::weak_ptr<const CConfiguration> m_pConfiguration;

    //! Maps each item (key) to it's parent (value)
    using parent_map_t = std::map<const IProjTreeItem*, std::weak_ptr<const IProjTreeItem>>;
    parent_map_t m_mParents;

    //! @brief Recursively fills the paternity map for the provided item
    //! @param[in] pParent The parent item whose subitems will be mapped
    void MapParents(const std::shared_ptr<const IProjTreeItem>& pParent);

    using storage_list_t = std::list<std::shared_ptr<IContextStorage>>;

    //! Stores objects for the duration of the execution
    storage_list_t m_lStorage;

public:

    //! @brief Creates a new context to execute the project
    //! @param[in] pProject The project to execute
    //! @param[in] pInstance The instance to execute the project with
    //! @param[in] pConfiguration The configuration to execute the project with
    CExecutionContext(const std::shared_ptr<const CProject>& pProject, const std::shared_ptr<const CInstance>& pInstance, const std::shared_ptr<const CConfiguration>& pConfiguration);

    //! @brief Gets the associated instance of this execution
    std::shared_ptr<const CInstance> GetInstance() const;

    //! @brief Gets the associated configuration of this context
    std::shared_ptr<const CConfiguration> GetConfiguration() const;

    //! @brief Gets the parent of the requested child item
    //! @param[in] pItem Reference to the item whose parent must be found
    //! @return The parent of the item or nullptr if not found (the project root has no parent)
    std::shared_ptr<const IProjTreeItem> GetParent(const IProjTreeItem* pItem) const;

    //! @brief Stores an object in this context for the duration of the execution
    void Store(std::shared_ptr<IContextStorage> pStore);
};

#endif