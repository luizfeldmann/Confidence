#ifndef _IPROCESSPOLICY_H_
#define _IPROCESSPOLICY_H_

#include <JInterface/JsonSerialization.h>

//! @brief Types of process policies
enum class EProcessPolicy : int
{
    EProcessFireForget      = 0,    //!< The process is started and forked
    EProcessWaitCompletion  = 1,    //!< Wait for the process to complete and (optionally) capture output
};

//! @brief Defines the behaviour when starting a process
class IProcessPolicy
{
public:
    IProcessPolicy() = default;
    virtual ~IProcessPolicy() = default;

    //! @brief Reflects the type of the object
    virtual EProcessPolicy GetType() const = 0;
};

DECLARE_SERIALIZATION_SCHEME(IProcessPolicy);

#endif