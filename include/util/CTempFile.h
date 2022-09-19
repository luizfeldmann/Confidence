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
    std::string m_strName;

public:
    //! @brief Creates a new temporary file
    CTempFile();
    ~CTempFile();

    //! @brief Moves temporary file from another object
    CTempFile(CTempFile&& rMove) noexcept;
    CTempFile& operator=(CTempFile&& rMove) noexcept;

    //! @brief Gets the path to the temp file
    std::string path() const;
};

#endif