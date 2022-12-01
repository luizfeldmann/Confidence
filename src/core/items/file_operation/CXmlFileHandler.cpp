#include "core/items/file_operation/CXmlFileHandler.h"
#include "core/items/file_operation/IFileDataLocator.h"
#include "core/items/file_operation/IFileOperationContext.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"
#include <pugixml.hpp>

/* CXmlFileLocator */

//! @brief Validades and stores an XPath string
class CXmlFileLocator : public IFileDataLocator
{
protected:
    //! @brief Parses a string into a structured xpath_query object
    //! @return True if success/valid
    static bool get_xpath(const std::string& strXPath, pugi::xpath_query& xQuery)
    {
        bool bStatus = true;

        pugi::xpath_parse_result result;

        try
        {
            xQuery = pugi::xpath_query(strXPath.c_str());
            result = xQuery.result();
        }
        catch (pugi::xpath_exception& ex)
        {
            result = ex.result();
        }

        if (!result)
        {
            bStatus = false;
            CWARNING("Failed parsing XPath '%s': %s", strXPath.c_str(), result.description());
        }

        return bStatus;
    }

public:
    //! @brief Stores the XPath string to be used by the #CXmlFileContext for a particular operation
    std::string m_XPath;

    //! @brief Constructs a blank locator for use with the #CXmlFileContext
    CXmlFileLocator()
    {

    }

    //! @brief Parses the current XPath string into a structured xpath_query object
    //! @return True if success/valid
    bool get_xpath(pugi::xpath_query& xQuery) const
    {
        return get_xpath(m_XPath, xQuery);
    }

    //! @copydoc IFileDataLocator::from_string
    bool from_string(const std::string& strLocator) override
    {
        pugi::xpath_query xQuery;
        bool bStatus = get_xpath(strLocator, xQuery);

        if (bStatus)
            m_XPath = strLocator;

        return bStatus;
    }

    //! @copydoc IFileDataLocator::to_string
    std::string to_string() const override
    {
        return m_XPath;
    }

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override
    {
        return rExporter.FormField("XPath:", m_XPath);
    }
};

DECLARE_SERIALIZATION_SCHEME(CXmlFileLocator);

DEFINE_SERIALIZATION_SCHEME(CXmlFileLocator,
    SERIALIZATION_MEMBER(m_XPath)
);

REGISTER_POLYMORPHIC_CLASS(CXmlFileLocator);

/* CXmlFileContext */

//! @brief Performs R/W operations a particular XML file
class CXmlFileContext : public IFileOperatorContext
{
protected:
    //! True if the file was modified by any operation and must be saved
    bool m_bModified = false;

    //! Stores the path to the XML file
    const std::string m_strPath;

    //! Stores the parsed TREE of the XML
    pugi::xml_document m_document;

    //! @brief Uses the provided locator to selected a node on the current document
    //! @param[in] rLocator A #CXmlFileLocator whose xPath will be used to select the node
    //! @param[out] xNode Receives the selected node
    //! @return True if success
    bool select_node(const IFileDataLocator& rLocator, pugi::xpath_node& xNode)
    {
        const CXmlFileLocator& rXmlLocator = dynamic_cast<const CXmlFileLocator&>(rLocator);

        pugi::xpath_query xQuery;
        bool bStatus = rXmlLocator.get_xpath(xQuery);

        if (bStatus)
        {
            xNode = m_document.select_node(xQuery);
            bStatus = (bool)xNode;

            if (!bStatus)
                CWARNING("Failed to select XML node using XPath");
        }

        return bStatus;
    }

public:
    //! @brief Constructs a context to operate on a particular XML file
    //! @param[in] filePath @copybrief m_strPath
    CXmlFileContext(const std::filesystem::path& filePath)
        : m_strPath(filePath.string())
    {
        pugi::xml_parse_result result = m_document.load_file(m_strPath.c_str());
        if (!result)
        {
            CERROR("Failed loading XML '%s': 0x%X - %s", m_strPath.c_str(), (int)result.status, result.description());
        }
    }

    ~CXmlFileContext()
    {
        if (m_bModified)
        {
            bool bStatus = m_document.save_file(m_strPath.c_str());
            if (!bStatus)
                CERROR("Failed saving XML file '%s'", m_strPath.c_str());
        }
    }

    //! @copydoc IFileOperatorContext::Write
    bool Write(const IFileDataLocator& rLocator, const std::string& strValue) override
    {
        pugi::xpath_node xNode;
        bool bStatus = select_node(rLocator, xNode);

        if (bStatus)
        {
            bStatus = xNode.node().text().set(strValue.c_str());

            if (!bStatus)
                CERROR("Failed updating XML node");
            else
                m_bModified = true;
        }

        return bStatus;
    }

    //! @copydoc IFileOperatorContext::Read
    bool Read(const IFileDataLocator& rLocator, std::string& strValue) override
    {
        pugi::xpath_node xNode;
        bool bStatus = select_node(rLocator, xNode);

        if (bStatus)
            strValue = xNode.node().text().as_string();

        return bStatus;
    }

    //! @copydoc IFileOperatorContext::GetType
    EFileOperationHandlerType GetType() const override
    {
        return EFileOperationHandlerType::XmlFile;
    }
};

/* CXmlFileHandler */

DEFINE_SERIALIZATION_SCHEME(CXmlFileHandler);

REGISTER_POLYMORPHIC_CLASS(CXmlFileHandler);

CXmlFileHandler::CXmlFileHandler()
{

}

CXmlFileHandler::~CXmlFileHandler()
{

}

/* static */ CXmlFileHandler::handler_ptr_t CXmlFileHandler::Create()
{
    return std::make_unique<CXmlFileHandler>();
}

EFileOperationHandlerType CXmlFileHandler::GetType() const
{
    return EFileOperationHandlerType::XmlFile;
}

CXmlFileHandler::context_ptr_t CXmlFileHandler::CreateContext(const std::filesystem::path& filePath) const
{
    return std::make_shared<CXmlFileContext>(filePath);
}

CXmlFileHandler::locator_ptr_t CXmlFileHandler::CreateLocator() const
{
    return std::make_unique<CXmlFileLocator>();
}

bool CXmlFileHandler::Document(IDocExporter& rExporter) const
{
    return rExporter.Paragraph()
        && rExporter.Text("This file is handled as .XML format.")
        && rExporter.PopStack();
}