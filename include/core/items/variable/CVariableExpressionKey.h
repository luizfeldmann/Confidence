/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CVARIABLEEXPRESSIONKEY_H_
#define _CVARIABLEEXPRESSIONKEY_H_

#include <string>
#include "util/CGuid.h"
#include "core/ISerializationNotify.h"
#include "core/CStoredExpression.h"
#include "docs/IDocumentable.h"

class CInstance;
class CConfiguration;

//! @brief A "key" used to identify an object based on associated configuration and instance
class CVariableExpressionKey : public ISerializationNotify, public CStoredExpression, public IDocumentable
{
protected:
    //! The name of the associated configuration
    //! Used only for (de-)serialization
    //! This value is updated on #PreSerialize
    std::string m_strConfiguration;

    //! The name of the associated instance
    //! Used only for (de-)serialization
    //! This value is updated on #PreSerialize
    std::string m_strInstance;

    //! When true, the instance key is required
    bool m_bPerInstance;

    SERIALIZATION_FRIEND(CVariableExpressionKey);

    //! @brief The pointer to the associated configuration
    //! @details This value is updated on #PostDeserialize
    std::weak_ptr<const CConfiguration> m_pConfiguration;
    
    //! @brief The pointer the associated instance
    //! @details This value is updated on #PostDeserialize
    std::weak_ptr<const CInstance> m_pInstance;

    //! @brief Gets a pointer to the associated configuration
    std::shared_ptr<const CConfiguration> GetConfiguration() const;

    //! @brief Gets a pointer to the associated instance
    std::shared_ptr<const CInstance> GetInstance() const;

public:
    // @brief Constructs a blank key
    CVariableExpressionKey();

    //! @brief Constructs a key associating an expression to a particular #CConfiguration
    //! @param[in] pConfig @copybrief m_pConfiguration
    CVariableExpressionKey(const std::weak_ptr<const CConfiguration>& pConfig);

    //! @brief Constructs a key associating an expression to a particular pair of #CConfiguration and #CInstance
    //! @param[in] pConfig @copybrief m_pConfiguration
    //! @param[in] pInst @copybrief m_pInstance
    CVariableExpressionKey(const std::weak_ptr<const CConfiguration>& pConfig, const std::weak_ptr<const CInstance>& pInst);

    //! @brief Checks if the provided pair of instance and configuration match this key's
    //! @param[in] pConfig The configuration to compare to this'
    //! @param[in] pInst The instance to compare to this'
    bool Compare(const CConfiguration* pConfig, const CInstance* pInst) const;

    //! @brief Checks if the provided configuration match this key's
    //! @param[in] pConfig The configuration to compare to this'
    bool CompareConfig(const CConfiguration* pConfig) const;

    //! @brief Checks if this key is associated to instances and configurations that are still valid
    bool IsValid() const;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CVariableExpressionKey);

#endif