/**
 * @file EProcessPolicy.h
 * @brief Defines enum #EProcessPolicy
 */

#ifndef _EPROCESSPOLICY_H_
#define _EPROCESSPOLICY_H_

//! @brief Types of process policies
//! @public
enum class EProcessPolicy : int
{
    Undefined = -1,     //!< The policy is unknown or not defined    
    FireForget = 0,     //!< The process is started and forked
    WaitCompletion = 1, //!< Wait for the process to complete and (optionally) capture output
};

#endif