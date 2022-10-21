#ifndef _IPROCESSPOLICY_H_
#define _IPROCESSPOLICY_H_

#include <JInterface/JsonSerialization.h>
#include <docs/IDocumentable.h>

class CExecutionContext;

//! @brief Types of process policies
enum class EProcessPolicy : int
{
    Undefined       = -1,   //!< The policy is unknown or not defined    
    FireForget      = 0,    //!< The process is started and forked
    WaitCompletion  = 1,    //!< Wait for the process to complete and (optionally) capture output
};

//! @brief Defines the behaviour when starting a process
class IProcessPolicy : public IDocumentable
{
public:
    IProcessPolicy() = default;
    virtual ~IProcessPolicy() = default;

    //! @brief Reflects the type of the object
    virtual EProcessPolicy GetType() const = 0;

    using vec_args_t = std::vector<std::string>;

    //! @brief Executes the process using the provided context and arguments
    //! @param[in] rContext Reference to the execution context
    //! @param[in] vArgs Vector of arguments to start the process
    //! @return True if success
    virtual bool Execute(CExecutionContext& rContext, const vec_args_t& vArgs) const = 0;
};

DECLARE_SERIALIZATION_SCHEME(IProcessPolicy);

#endif