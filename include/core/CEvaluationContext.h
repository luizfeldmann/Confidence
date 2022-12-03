/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CEVALUATIONCONTEXT_H_
#define _CEVALUATIONCONTEXT_H_

#include <map>
#include <string>
#include <optional>
#include <vector>

//! @brief Stores variables and uses them to evaluate expressions
class CEvaluationContext
{
protected:
    //! Type used to store associations of variable names to variable values
    using variable_map_t = std::map<std::string, std::string>;

    //! Collection of variables names and values
    variable_map_t m_mVariables;

public:
    CEvaluationContext();

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

    //! @brief Returns a collection of names of variables contained in the expression
    static std::vector<std::string> ListVariables(const std::string& strExpression);
};

#endif