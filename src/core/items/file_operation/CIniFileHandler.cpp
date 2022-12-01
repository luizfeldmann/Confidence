#include "core/items/file_operation/CIniFileHandler.h"
#include "core/items/file_operation/IFileDataLocator.h"
#include "core/items/file_operation/IFileOperationContext.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"

#define MINI_CASE_SENSITIVE
#include <mini/ini.h>

/* CIniFileLocator */

static const std::string c_strIniDelimiter = "/";

//! @brief Stores a pair of section & key names to locate data in INI files
class CIniFileLocator : public IFileDataLocator
{
public:
    //! Stores the relevat section name
    std::string m_strSection;

    //! Stores the relevant key name 
    std::string m_strKey;

    //! @brief Constructs a locator for INI files, to be used by the #CIniFileContext
    CIniFileLocator()
        : m_strSection("Section")
        , m_strKey("Key")
    {

    }

    //! @copydoc IFileDataLocator::from_string
    bool from_string(const std::string& strLocator) override
    {
        bool bStatus = false;

        size_t nDelim = strLocator.find(c_strIniDelimiter);

        if (std::string::npos != nDelim)
        {
            m_strSection = strLocator.substr(0, nDelim);
            m_strKey = strLocator.substr(nDelim + 1);
        }

        return bStatus;
    }

    //! @copydoc IFileDataLocator::to_string
    std::string to_string() const override
    {
        return m_strSection + c_strIniDelimiter + m_strKey;
    }

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override
    {
        return rExporter.FormField("Section:", m_strSection)
            && rExporter.FormField("Key:", m_strKey);
    }
};

DECLARE_SERIALIZATION_SCHEME(CIniFileLocator);

DEFINE_SERIALIZATION_SCHEME(CIniFileLocator,
    SERIALIZATION_MEMBER(m_strSection)
    SERIALIZATION_MEMBER(m_strKey)
);

REGISTER_POLYMORPHIC_CLASS(CIniFileLocator);

/* CIniFileContext */

//! @brief Performs R/W operations on a particular INI file
class CIniFileContext : public IFileOperatorContext
{
protected:
    //! True if the file was modified by any operation and must be saved
    bool m_bModified = false;

    //! Stores the path to the INI file
    const std::filesystem::path m_path;

    //! @brief Accessor to the file
    //! @internal
    mINI::INIFile m_file;

    //! Holds the parsed structure of the file
    //! @internal
    mINI::INIStructure m_structure;

public:
    //! @brief Constructs a context to operate on a particular INI file
    //! @param[in] filePath @copybrief m_path
    CIniFileContext(const std::filesystem::path& filePath)
        : m_path(filePath)
        , m_file(filePath.string())
    {
        bool bStatus = m_file.read(m_structure);

        if (!bStatus)
            CERROR("Failed opening INI file '%s': 0x%X", filePath.string().c_str());
    }

    ~CIniFileContext()
    {
        if (m_bModified)
        {
            bool bStatus = m_file.write(m_structure);

            if (!bStatus)
                CERROR("Error saving INI file '%s'", m_path.string().c_str());
        }
    }

    //! @copydoc IFileOperatorContext::Write
    bool Write(const IFileDataLocator& rLocator, const std::string& strValue) override
    {
        bool bStatus = true;

        const CIniFileLocator& rIniLocator = dynamic_cast<const CIniFileLocator&>(rLocator);

        m_structure[rIniLocator.m_strSection][rIniLocator.m_strKey] = strValue;
        m_bModified = true;

        return bStatus;
    }

    //! @copydoc IFileOperatorContext::Read
    bool Read(const IFileDataLocator& rLocator, std::string& strValue) override
    {
        bool bStatus = false;

        const CIniFileLocator& rIniLocator = dynamic_cast<const CIniFileLocator&>(rLocator);

        if (!m_structure.has(rIniLocator.m_strSection))
            CERROR("Failed to read INI section '%s'", rIniLocator.m_strSection.c_str());
        else
        {
            const auto& rSection = m_structure[rIniLocator.m_strSection];

            if (!rSection.has(rIniLocator.m_strKey))
                CERROR("Failed to read INI key '%s'", rIniLocator.m_strKey.c_str());
            else
            {
                strValue = rSection.get(rIniLocator.m_strKey);
                bStatus = true;
            }
        }

        return bStatus;
    }

    //! @copydoc IFileOperatorContext::GetType
    EFileOperationHandlerType GetType() const override
    {
        return EFileOperationHandlerType::IniFile;
    }
};

/* CIniFileHandler */

DEFINE_SERIALIZATION_SCHEME(CIniFileHandler);

REGISTER_POLYMORPHIC_CLASS(CIniFileHandler);

CIniFileHandler::CIniFileHandler()
{

}

CIniFileHandler::~CIniFileHandler()
{

}

/* static */ CIniFileHandler::handler_ptr_t CIniFileHandler::Create()
{
    return std::make_unique<CIniFileHandler>();
}

EFileOperationHandlerType CIniFileHandler::GetType() const
{
    return EFileOperationHandlerType::IniFile;
}

CIniFileHandler::context_ptr_t CIniFileHandler::CreateContext(const std::filesystem::path& filePath) const
{
    return std::make_shared<CIniFileContext>(filePath);
}

CIniFileHandler::locator_ptr_t CIniFileHandler::CreateLocator() const
{
    return std::make_unique<CIniFileLocator>();
}

bool CIniFileHandler::Document(IDocExporter& rExporter) const
{
    return rExporter.Paragraph()
        && rExporter.Text("This file is handled as .INI format.")
        && rExporter.PopStack();
}