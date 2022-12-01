/**
 * @file IDependsOnVariables.h
 * @brief Defines interface #IDependsOnVariables
 */

#ifndef _IDEPENDSONVARIABLES_H_
#define _IDEPENDSONVARIABLES_H_

#include <vector>
#include <string>

//! @brief Interface for objects with dependency on variables
//!        Provides capability to list those dependencies
class IDependsOnVariables
{
public:
    virtual ~IDependsOnVariables() = default;

    //! @brief Returns a collection of names of variables on which this object depends
    virtual std::vector<std::string> GetDependencies() const = 0;
};

#endif