/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "vfs/CTempSymlinkGenerator.h"
#include "util/Log.h"
#include <fstream>
#include <system_error>

DEFINE_SERIALIZATION_SCHEME(CTempSymlinkGenerator);

REGISTER_POLYMORPHIC_CLASS(CTempSymlinkGenerator);

//! @brief Creates and writes to a temporary file symlinked to a relevant location
class CTempSymlinkFile : public IGeneratedFile
{
private:
    CTempSymlinkFile(const CTempSymlinkFile&) = delete;
    CTempSymlinkFile& operator=(const CTempSymlinkFile&) = delete;

protected:
    //! The status code of the class logic
    std::error_code m_eStatus;

    //! Stores the path to the relevant location where the symlink is created
    path_t const m_dstPath;

    //! Stores the path to the temporary file
    path_t const m_tmpPath;

    //! Stream used to write data to the file
    std::ofstream m_stream;

public:
    //! @brief Constructs a #CTempSymlinkFile managing a symlink from \p path to a temporary location
    //! @param[in] path @copybrief m_dstPath
    CTempSymlinkFile(const path_t& path)
        : m_eStatus()
        , m_dstPath(path)
        , m_tmpPath(std::tmpnam(nullptr))
        , m_stream(m_tmpPath, std::ios::binary)

    {
        if (!m_stream.is_open())
        {
            CERROR("Failed to create temporary file '%s'", m_tmpPath.string().c_str());
            m_eStatus = std::make_error_code(std::errc::io_error);
            return;
        }

        // Delete the destination file if it already exists
        if (std::filesystem::exists(m_dstPath, m_eStatus))
        {
            CWARNING("Destination file already exists and will be replaced: '%s'", m_dstPath.string().c_str());

            if (!std::filesystem::remove(m_dstPath, m_eStatus))
            {
                std::string strErr = m_eStatus.message();
                CERROR("Failed to remove file '%s': %s", m_dstPath.string().c_str(), strErr.c_str());
                return;
            }
        }

        // Create the symlink between temp and destination
        std::filesystem::create_symlink(m_tmpPath, m_dstPath, m_eStatus);
        if (m_eStatus)
        {
            std::string strErr = m_eStatus.message();
            CERROR("Failed to create symlink '%s' -> '%s': %s", m_tmpPath.string().c_str(), m_dstPath.string().c_str(), strErr.c_str());
            return;
        }
    }

    ~CTempSymlinkFile()
    {
        // Remove the symlink to the destination
        if (std::filesystem::exists(m_dstPath, m_eStatus))
        {
            if (!std::filesystem::remove(m_dstPath, m_eStatus))
            {
                std::string strErr = m_eStatus.message();
                CERROR("Failed to remove generated file '%s': %s", m_dstPath.string().c_str(), strErr.c_str());
            }
        }

        // Close the stream to the temporary file
        if (m_stream.is_open())
            m_stream.close();

        // Remove the temporary file
        if (std::filesystem::exists(m_tmpPath, m_eStatus))
        {
            if (!std::filesystem::remove(m_tmpPath, m_eStatus))
            {
                std::string strErr = m_eStatus.message();
                CERROR("Failed to remove temporary file '%s': %s", m_tmpPath.string().c_str(), strErr.c_str());
            }
        }
    }

    //! @copydoc IGeneratedFile::IsValid
    bool IsValid() const override
    {
        return !m_eStatus;
    }

    //! @copydoc IGeneratedFile::GetPath
    path_t GetPath() const override
    {
        return m_dstPath;
    }

    //! @copydoc IGeneratedFile::GetStream
    std::ostream& GetStream() override
    {
        return m_stream;
    }
};

/* static */ std::unique_ptr<IFileGenerator> CTempSymlinkGenerator::Create()
{
    return std::make_unique<CTempSymlinkGenerator>();
}

CTempSymlinkGenerator::sptr_t CTempSymlinkGenerator::NewFile(const path_t& filePath) const
{
    return std::make_shared<CTempSymlinkFile>(filePath);
}

EFileGeneratorType CTempSymlinkGenerator::GetType() const
{
    return EFileGeneratorType::Symlink;
}