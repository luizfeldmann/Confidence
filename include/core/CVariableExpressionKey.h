#ifndef _CVARIABLEEXPRESSIONKEY_H_
#define _CVARIABLEEXPRESSIONKEY_H_

#include <string>
#include "util/CGuid.h"
#include "ISerializationNotify.h"
#include "CStoredExpression.h"
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

    SERIALIZATION_FRIEND(CVariableExpressionKey);

    //! The ID of the associated configuration
    //! This value is updated on #PostDeserialize
    CGuid m_gIdConfiguration;
    
    //! The ID of the associated instance
    //! This value is updated on #PostDeserialize
    CGuid m_gIdInstance;

    //! @brief Gets a pointer to the associated configuration
    const CConfiguration* GetConfiguration() const;

    //! @brief Gets a pointer to the associated instance
    const CInstance* GetInstance() const;

public:
    CVariableExpressionKey();

    CVariableExpressionKey(const CConfiguration& rConfig, const CInstance& rInst);

    //! @brief Checks if the provided pair of instance and configuration match this key's
    //! @param[in] rConfig The configuration to compare to this'
    //! @param[in] rInst The instance to compare to this'
    bool Compare(const CConfiguration& rConfig, const CInstance& rInst) const;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize() override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CVariableExpressionKey);

#endif