#include "vfs/CTempSymlinkGenerator.h"
#include "util/Log.h"
#include <fstream>
#include <system_error>

class CTempSymlinkFile : public IGeneratedFile
{
private:
    CTempSymlinkFile(const CTempSymlinkFile&) = delete;
    CTempSymlinkFile& operator=(const CTempSymlinkFile&) = delete;

protected:
    std::error_code m_eStatus;
    path_t const m_dstPath;
    path_t const m_tmpPath;
    std::ofstream m_stream;

public:
    CTempSymlinkFile(const path_t& path)
        : m_eStatus()
        , m_dstPath(path)
        , m_tmpPath(std::tmpnam(nullptr))
        , m_stream(m_tmpPath, std::ios::binary)

    {
        if (!m_stream.is_open())
        {
            CERROR("Failed to create temporary file '%s'", m_tmpPath.c_str());
            m_eStatus = std::make_error_code(std::errc::io_error);
            return;
        }

        // Delete the destination file if it already exists
        if (std::filesystem::exists(m_dstPath, m_eStatus))
        {
            CWARNING("Destination file already exists and will be replaced: '%s'", m_dstPath.c_str());

            if (!std::filesystem::remove(m_dstPath, m_eStatus))
            {
                std::string strErr = m_eStatus.message();
                CERROR("Failed to remove file '%s': %s", m_dstPath.c_str(), strErr.c_str());
                return;
            }
        }

        // Create the symlink between temp and destination
        std::filesystem::create_symlink(m_tmpPath, m_dstPath, m_eStatus);
        if (m_eStatus)
        {
            std::string strErr = m_eStatus.message();
            CERROR("Failed to create symlink '%s' -> '%s': %s", m_tmpPath.c_str(), m_dstPath.c_str(), strErr.c_str());
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
                CERROR("Failed to remove generated file '%s': %s", m_dstPath.c_str(), strErr.c_str());
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
                CERROR("Failed to remove temporary file '%s': %s", m_tmpPath.c_str(), strErr.c_str());
            }
        }
    }

    bool IsValid() const override
    {
        return !m_eStatus;
    }

    path_t GetPath() const override
    {
        return m_dstPath;
    }

    std::ostream& GetStream() override
    {
        return m_stream;
    }
};

CTempSymlinkGenerator::sptr_t CTempSymlinkGenerator::NewFile(const path_t& filePath) const
{
    return std::make_shared<CTempSymlinkFile>(filePath);
}