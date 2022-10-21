#ifndef _EBUILTINVARIABLES_H_
#define _EBUILTINVARIABLES_H_

#include <string>

//! @brief Enumeration of variables tacitly defined for the project execution
enum class EBuiltInVariable
{
    ProjectName,        //!< The name of the project being executed
    InstanceName,       //!< The name of the current instance
    ConfigurationName,  //!< The name of the current configuration
};

//! @brief Returns the string name of the variable
//! @param[in] eVariable Identifies the variable whose name is requested
const std::string& GetBuiltinName(EBuiltInVariable eVariable);

#endif
