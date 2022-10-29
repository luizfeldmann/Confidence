#ifndef _STARTPROCESS_H_
#define _STARTPROCESS_H_

#include "core/items/process/SProcessStartInfo.h"

//! @brief Starts the process as described by the startup information
//! @param[in] rInfo Specification for starting the process
//! @param[out] ppProcHandle If not null, receives the handle to the created process
//! @returns True if success
bool StartProcess(const SProcessStartInfo& rInfo, void** ppProcHandle);

//! @brief Waits until the process terminates
//! @param[in] pProcHandle Handle to the process to wait for
//! @param[out] pnExitCode If not NULL, receives the return code of the process
//! @returns True if success
bool WaitProcess(void* pProcHandle, unsigned long* pnExitCode);

#endif