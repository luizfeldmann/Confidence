#ifndef _SPROCESSSTARTINFO_H_
#define _SPROCESSSTARTINFO_H_

#include <string>
#include <vector>

//! @brief How the process window is supposed to start
enum class EProcessStartWindowMode : int {
    Minimized = -1,   //!< Starts the process with a minimized window
    Normal = 0,       //!< Starts the process with a normal window
    Maximized = 1,    //!< Starts the process with a maximized window
};

//! @brief Information on how to start the process
struct SProcessStartInfo
{
    //! If true, the process will start elevated
    bool m_bRunAsAdmin;

    //! The window mode to start the process
    EProcessStartWindowMode m_eStartMode;

    //! Type used as a collection of arguments to be passed to the starting process
    using vec_args_t = std::vector<std::string>;
    
    //! The collection of arguments used to start the process
    vec_args_t m_vArguments;
};

#endif