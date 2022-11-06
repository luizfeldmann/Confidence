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

    //! The pointer to the associated configuration
    //! This value is updated on #PostDeserialize
    std::weak_ptr<const CConfiguration> m_pConfiguration;
    
    //! The pointer the associated instance
    //! This value is updated on #PostDeserialize
    std::weak_ptr<const CInstance> m_pInstance;

    //! @brief Gets a pointer to the associated configuration
    std::shared_ptr<const CConfiguration> GetConfiguration() const;

    //! @brief Gets a pointer to the associated instance
    std::shared_ptr<const CInstance> GetInstance() const;

public:
    CVariableExpressionKey();

    CVariableExpressionKey(const std::weak_ptr<const CConfiguration>& pConfig);
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