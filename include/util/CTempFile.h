/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CTEMPFILE_H_
#define _CTEMPFILE_H_

#include <string>

//! @brief Managed temporary file;
//!        File is deleted when object is destroyed
class CTempFile
{
private:
    CTempFile(const CTempFile&) = delete;
    CTempFile& operator=(const CTempFile&) = delete;

protected:
    //! Stores the file path
    std::string m_strName;

public:
    //! @brief Creates a new temporary file on disk
    CTempFile();

    //! @brief Deletes the temporary file from the disk
    ~CTempFile();

    //! @brief Moves temporary file from another object
    CTempFile(CTempFile&& rMove) noexcept;

    //! @brief Transfer the ownership of the underlying temporary file to another object
    CTempFile& operator=(CTempFile&& rMove) noexcept;

    //! @brief Gets the path to the temp file
    std::string path() const;
};

#endif