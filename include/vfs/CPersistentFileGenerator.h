#ifndef _CPERSISTENTFILEGENERATOR_H_
#define _CPERSISTENTFILEGENERATOR_H_

#include "IFileGenerator.h"

//! @brief The generated files are real files saved to disk
class CPersistentFileGenerator : public IFileGenerator
{
public:
    CPersistentFileGenerator() = default;

    //! @copydoc IFileGenerator::NewFile
    sptr_t NewFile(const path_t& filePath) const override;
};

#endif