#include "vfs/CPersistentFileGenerator.h"
#include <fstream>

class CPersistentFile : public IGeneratedFile
{
private:
    CPersistentFile(const CPersistentFile&) = delete;
    CPersistentFile& operator=(const CPersistentFile&) = delete;

protected:
    const path_t m_path;
    std::ofstream m_stream;

public:
    CPersistentFile(const path_t& path)
        : m_path(path)
        , m_stream(path, std::ios::binary)
    {

    }

    ~CPersistentFile()
    {

    }

    bool IsValid() const override
    {
        return m_stream.is_open();
    }

    path_t GetPath() const override
    {
        return m_path;
    }

    std::ostream& GetStream() override
    {
        return m_stream;
    }
};

CPersistentFileGenerator::sptr_t CPersistentFileGenerator::NewFile(const path_t& filePath) const
{
    return std::make_shared<CPersistentFile>(filePath);
}