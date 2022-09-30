#ifndef _CRUNPROCESS_H_
#define _CRUNPROCESS_H_

#include <list>
#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"
#include "CProcessArgument.h"
#include "IProcessPolicy.h"

//! @brief How the process window is supposed to start
enum class EProcessStartWindowMode : int {
    Minimized = -1,   //!< Starts the process with a minimized window
    Normal    = 0,    //!< Starts the process with a normal window
    Maximized = 1,    //!< Starts the process with a maximized window
};

//! @brief Stores a constant or an expression
class CRunProcess   : public IProjTreeItem
                    , public CStoredNameItem
                    , public CStoredDescriptionItem
                    , public CNoChildren
{
public:
    //! @brief Starts a new process and optionally waits for output
    CRunProcess();

    ~CRunProcess();

    //! @brief If the process should start as admin
    bool m_bRunAsAdmin;

    //! @brief The window mode to start the process
    EProcessStartWindowMode m_eStartMode;

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @brief Gets a pointer to the underlying policy
    IProcessPolicy* GetPolicy() const;

    //! @brief Sets the underlying policy
    //! @details The old policy is destroyed; assumes exclusive ownership of the new pointer
    //! @param[in] pNewPolicy New policy to assign
    void SetPolicy(IProcessPolicy* pNewPolicy);

    using vec_args_t = std::list<CProcessArgument>;

    //! @brief Gets the collection of arguments used when starting the process
    vec_args_t& GetArguments();

    //! @bcopydoc GetArguments
    const vec_args_t& GetArguments() const;

protected:
    //! Policy to start this process
    std::unique_ptr<IProcessPolicy> m_pPolicy;

    //! Collection of arguments passed to the process
    vec_args_t m_vArgs;

    SERIALIZATION_FRIEND(CRunProcess);
};

DECLARE_SERIALIZATION_SCHEME(CRunProcess)

#endif