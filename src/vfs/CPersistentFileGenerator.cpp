/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "vfs/CPersistentFileGenerator.h"
#include <fstream>

DEFINE_SERIALIZATION_SCHEME(CPersistentFileGenerator);

REGISTER_POLYMORPHIC_CLASS(CPersistentFileGenerator);

//! @brief Creates and writes to a file stored on disk
class CPersistentFile : public IGeneratedFile
{
private:
    CPersistentFile(const CPersistentFile&) = delete;
    CPersistentFile& operator=(const CPersistentFile&) = delete;

protected:
    //! Stores the path of this file
    const path_t m_path;

    //! Stores the file stream used to write to this file
    std::ofstream m_stream;

public:
    //! Opens or creates a new file on disk in the specified path
    //! @param[in] path @copybrief m_path
    CPersistentFile(const path_t& path)
        : m_path(path)
        , m_stream(path, std::ios::binary)
    {

    }

    ~CPersistentFile()
    {

    }

    //! @copydoc IGeneratedFile::IsValid
    bool IsValid() const override
    {
        return m_stream.is_open();
    }

    //! @copydoc IGeneratedFile::GetPath
    path_t GetPath() const override
    {
        return m_path;
    }

    //! @copydoc IGeneratedFile::GetStream
    std::ostream& GetStream() override
    {
        return m_stream;
    }
};

/* static */ std::unique_ptr<IFileGenerator> CPersistentFileGenerator::Create()
{
    return std::make_unique<CPersistentFileGenerator>();
}

CPersistentFileGenerator::sptr_t CPersistentFileGenerator::NewFile(const path_t& filePath) const
{
    return std::make_shared<CPersistentFile>(filePath);
}

EFileGeneratorType CPersistentFileGenerator::GetType() const
{
    return EFileGeneratorType::Persistent;
}