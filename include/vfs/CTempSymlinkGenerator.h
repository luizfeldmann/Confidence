#ifndef _CTEMPSYMLINKGENERATOR_H_
#define _CTEMPSYMLINKGENERATOR_H_

#include "IFileGenerator.h"

//! @brief The generated files are symlinks to temporary files
class CTempSymlinkGenerator : public IFileGenerator
{
public:
    CTempSymlinkGenerator() = default;

    //! @copydoc IFileGenerator::NewFile
    sptr_t NewFile(const path_t& filePath) const override;
};

#endif