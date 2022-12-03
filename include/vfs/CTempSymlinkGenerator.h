/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CTEMPSYMLINKGENERATOR_H_
#define _CTEMPSYMLINKGENERATOR_H_

#include "IFileGenerator.h"
#include <JInterface/JsonSerialization.h>

//! @brief The generated files are symlinks to temporary files
class CTempSymlinkGenerator : public IFileGenerator
{
public:
    CTempSymlinkGenerator() = default;

    //! @brief Returns a new instance of this generator
    static std::unique_ptr<IFileGenerator> Create();

    //! @copydoc IFileGenerator::NewFile
    sptr_t NewFile(const path_t& filePath) const override;

    //! @copydoc IFileGenerator::GetType
    EFileGeneratorType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CTempSymlinkGenerator);

#endif