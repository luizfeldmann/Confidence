#ifndef _EBUILTINVARIABLES_H_
#define _EBUILTINVARIABLES_H_

#include <string>
#include <regex>

//! @brief Enumeration of variables tacitly defined for the project execution
enum class EBuiltInVariable
{
    ProjectName,        //!< The name of the project being executed
    InstanceName,       //!< The name of the current instance
    ConfigurationName,  //!< The name of the current configuration
    ProjectFile,        //!< Full path to the saved project file
    ProjectDir,         //!< Directory containing the saved project file
};

//! @brief Returns the string name of the variable
//! @param[in] eVariable Identifies the variable whose name is requested
const std::string& GetBuiltinName(EBuiltInVariable eVariable);

//! @brief Wraps the variable name in the expression marker
//! @param[in] strVariableName Name of variable for the expression
//! @returns String in format $(strVariableName)
std::string MakeExpression(const std::string& strVariableName);

//! @brief Gets the standard regex used to process variables
const std::regex& GetVariableRegex();

#endif
