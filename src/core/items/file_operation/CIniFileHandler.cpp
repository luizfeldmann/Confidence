#include "core/items/file_operation/CIniFileHandler.h"
#include "core/items/file_operation/IFileDataLocator.h"
#include "core/items/file_operation/IFileOperationContext.h"

/* CIniFileLocator */

static const std::string c_strIniDelimiter = "/";

class CIniFileLocator : public IFileDataLocator
{
public:
    std::string m_strSection;
    std::string m_strKey;

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

class CIniFileContext : public IFileOperatorContext
{
public:
    CIniFileContext(const std::filesystem::path& filePath)
    {

    }

    //! @copydoc IFileOperatorContext::Write
    bool Write(const IFileDataLocator& rLocator, const std::string& strValue) override
    {
        return false;
    }

    //! @copydoc IFileOperatorContext::Read
    bool Read(const IFileDataLocator& rLocator, std::string& strValue) override
    {
        return false;
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