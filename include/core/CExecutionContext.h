#ifndef _CEXECUTIONCONTEXT_H_
#define _CEXECUTIONCONTEXT_H_

#include <map>
#include <list>
#include <memory>
#include <string>
#include <optional>
#include "core/IContextStorage.h"

class CProject;
class CInstance;
class CConfiguration;
class IProjTreeItem;

//! @brief Stores the execution state of the project
class CExecutionContext
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

    using variable_map_t = std::map<std::string, std::string>;

    //! Collection of variables names and values
    variable_map_t m_mVariables;

    using storage_list_t = std::list<std::shared_ptr<IContextStorage>>;

    //! Stores objects for the duration of the execution
    storage_list_t m_lStorage;

public:
    //! Reference to the project being executed
    const CProject& m_rProject;

    //! Reference to the instance being executed
    const CInstance& m_rInstance;

    //! Reference to the configuration being executed
    const CConfiguration& m_rConfiguration;

    CExecutionContext(const CProject& rProj, const CInstance& rInst, const CConfiguration& rCfg);

    //! @brief Gets the parent of the requested child item
    //! @param[in] rItem Reference to the item whose parent must be found
    //! @return The parent of the item or nullptr if not found (the project root has no parent)
    const IProjTreeItem* GetParent(const IProjTreeItem& rItem) const;

    //! @brief Assigns the variable (verbatim, no expression evaluation)
    //! @param[in] strKey The name of the variable to assign
    //! @param[in] strValue The new value for the variable
    void SetVariableLiteral(const std::string& strKey, const std::string& strValue);

    //! @brief Assigns the variable after evaluating the expression to a literal
    //! @param[in] strKey The name of the variable to assign
    //! @param[in] strExpression Expression whose evaluated value will be assigned
    //! @return True if success
    bool SetVariableEvaluate(const std::string& strKey, const std::string& strExpression);

    //! @brief Finds value of the variable of the given name, if it is defined
    //! @param[in] strKey The name of the variable to read
    //! @return If the variable is defined, returns it's value; otherwise the optional<> is empty
    std::optional<std::string> GetVariable(const std::string& strKey) const;

    //! @brief Modifies the provided expression by replacing the variables defined for the current context
    //! @param[in,out] strExpression Expression to evaluate (in place)
    //! @return True if success
    bool Evaluate(std::string& strExpression) const;

    //! @brief Stores an object in this context for the duration of the execution
    void Store(std::shared_ptr<IContextStorage> pStore);
};

#endif