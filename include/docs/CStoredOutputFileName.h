/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CSTOREDOUTPUTFILENAME_H_
#define _CSTOREDOUTPUTFILENAME_H_

#include "IOutputFileName.h"
#include <JInterface/JsonSerialization.h>

//! @brief Implementation of IOutputFileName where the file name is stored in a serialized member
class CStoredOutputFileName : public virtual IOutputFileName
{
protected:
    //! Stores the output file name
    std::string m_strOutputPath;

    SERIALIZATION_FRIEND(CStoredOutputFileName);

public:
    //! @brief Initializes an empty file name
    CStoredOutputFileName() = default;

    //! @brief Constructs with an initial file path
    //! @param[in] strInitial @copybrief m_strOutputPath
    CStoredOutputFileName(const std::string& strInitial);

    //! @copydoc IOutputFileName::GetOutputFileName
    std::string GetOutputFileName() const override;

    //! @copydoc IOutputFileName::SetOutputFileName
    void SetOutputFileName(const std::string&) override;

    //! @brief The default / suggested file path
    static const std::string& GetDefaultPath();
};

DECLARE_SERIALIZATION_SCHEME(CStoredOutputFileName);

#endif