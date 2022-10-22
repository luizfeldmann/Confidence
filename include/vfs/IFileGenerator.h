#ifndef _IFILEGENERATOR_H_
#define _IFILEGENERATOR_H_

#include <ostream>
#include <filesystem>
#include <memory>
#include <core/IContextStorage.h>

//! @brief Interface for accessing the generated files
class IGeneratedFile : public IContextStorage
{
public:
    virtual ~IGeneratedFile() = default;

    using path_t = std::filesystem::path;

    //! @brief Returns true if this file is valid and can be written to
    virtual bool IsValid() const = 0;

    //! @brief Reads the destination path of this file
    virtual path_t GetPath() const = 0;

    //! @brief Gets a reference to the underlying stream used to write to the file
    //! @throws std::runtime_error If IsValid() is false
    virtual std::ostream& GetStream() = 0;
};

//! @brief Interface for generators of files
class IFileGenerator
{
public:
    virtual ~IFileGenerator() = default;

    using path_t = IGeneratedFile::path_t;
    using sptr_t = std::shared_ptr<IGeneratedFile>;

    //! @brief Creates a new file in the provided location
    //! @param[in] filePath Path where to create the new file
    //! @returns Shared pointer to the created file object
    virtual sptr_t NewFile(const path_t& filePath) const = 0;
};

#endif