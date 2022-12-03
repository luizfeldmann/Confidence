/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CENVIRONMENTVARIABLE_H_
#define _CENVIRONMENTVARIABLE_H_

#include <string>
#include "core/IContextStorage.h"

//! @brief Sets/unsets an environment variable
//! Automatically deletes the variable when object is destroyed
class CEnvironmentVariable : public IContextStorage
{
private:
    CEnvironmentVariable(const CEnvironmentVariable&) = delete;
    CEnvironmentVariable& operator=(const CEnvironmentVariable&) = delete;

protected:
    //! True if it was set in the environment
    bool m_bSet;

    //! Name of the variable
    const std::string m_strName;

public:
    //! @brief Constructs the object with the provided variable name
    //! @param[in] strName The name of the variable to set
    CEnvironmentVariable(const std::string& strName);

    //! @brief When destroyed, the variable is removed from the environment
    ~CEnvironmentVariable();

    //! @brief Sets the variable to the environment
    //! @param[in] szValue New value of the variable to set
    //! @returns True if success
    bool Set(const char* szValue);

    //! @brief Removes the variable from the environment
    //! @returns True if success
    bool Unset();

    //! @brief Reads the value of an environment variable
    //! @param[in] strName The name of the variable to read
    //! @param[out] strValue Receives the read value
    //! @returns True if success 
    static bool Read(const std::string& strName, std::string& strValue);
};

#endif