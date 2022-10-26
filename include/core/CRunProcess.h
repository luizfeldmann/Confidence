#ifndef _CRUNPROCESS_H_
#define _CRUNPROCESS_H_

#include <list>
#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"
#include "CProcessArgument.h"
#include "IProcessPolicy.h"

//! @brief Starts a new process and optionally waits for output
class CRunProcess   : public IProjTreeItem
                    , public CStoredNameItem
                    , public CStoredDescriptionItem
                    , public CNoChildren
{
protected:
    //! @brief Creates a new, empty process
    CRunProcess();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:
    ~CRunProcess();

    //! @brief If the process should start as admin
    bool m_bRunAsAdmin;

    //! @brief The window mode to start the process
    EProcessStartWindowMode m_eStartMode;

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

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

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;

protected:
    //! Policy to start this process
    std::unique_ptr<IProcessPolicy> m_pPolicy;

    //! Collection of arguments passed to the process
    vec_args_t m_vArgs;

    SERIALIZATION_FRIEND(CRunProcess);
};

DECLARE_SERIALIZATION_SCHEME(CRunProcess)

#endif