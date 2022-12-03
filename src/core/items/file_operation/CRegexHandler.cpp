/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/file_operation/CRegexHandler.h"
#include "core/items/file_operation/IFileDataLocator.h"
#include "core/items/file_operation/IFileOperationContext.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"
#include <fstream>
#include <sstream>
#include <regex>

/* CRegexLocator */

//! @brief Stores and validades a regular expression
class CRegexLocator : public IFileDataLocator
{
public:
    //! Stores the regular expression used by the #CRegexOperationContext to perform the R/W operations
    std::string m_strREGEX;

    //! @brief Constructs an empty locator for regular expressions
    CRegexLocator()
    {

    }

    //! @copydoc IFileDataLocator::from_string
    bool from_string(const std::string& strLocator) override
    {
        bool bStatus = false;
        std::regex cRegex;

        try {
            cRegex = std::regex(strLocator);
            m_strREGEX = strLocator;
            bStatus = true;
        }
        catch (const std::regex_error& ex)
        {
            CWARNING("Failed parsing regular expression '%s': %s", strLocator.c_str(), ex.what());
        }

        return bStatus;
    }

    //! @copydoc IFileDataLocator::to_string
    std::string to_string() const override
    {
        return m_strREGEX;
    }

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override
    {
        return rExporter.FormField("Regular expression:", m_strREGEX);
    }
};

DECLARE_SERIALIZATION_SCHEME(CRegexLocator);

DEFINE_SERIALIZATION_SCHEME(CRegexLocator,
    SERIALIZATION_MEMBER(m_strREGEX)
);

REGISTER_POLYMORPHIC_CLASS(CRegexLocator);

/* CRegexOperationContext */

//! @brief Performs a series of R/W operations on a provided file of arbitrary format using regular expressions
class CRegexOperationContext : public IFileOperatorContext
{
protected:
    //! True if the file was modified by any operation and must be saved
    bool m_bModified = false;

    //! Stores the path to the file
    const std::filesystem::path m_path;

    //! Holds the contents of the read file
    std::string m_content;

public:
    //! @brief Constructs a context to operate on the requested file
    //! @param[in] filePath @copybrief m_path
    CRegexOperationContext(const std::filesystem::path& filePath)
        : m_path(filePath)
    {
        std::ifstream ifs(m_path, std::ios::binary);
        if (!ifs.is_open())
            CERROR("Failed opening file '%s' for reading", m_path.string().c_str());
        else
        {
            std::stringstream ss;
            ss << ifs.rdbuf();

            m_content = ss.str();
        }
    }

    ~CRegexOperationContext()
    {
        if (m_bModified)
        {
            std::ofstream ofs(m_path, std::ios::binary);
            if (!ofs.is_open())
                CERROR("Failed opening file '%s' for writing", m_path.string().c_str());
            else
            {
                ofs << m_content;
                ofs.flush();
            }
        }
    }

    //! @copydoc IFileOperatorContext::Write
    bool Write(const IFileDataLocator& rLocator, const std::string& strValue) override
    {
        const std::string strRegex = rLocator.to_string();
        const std::regex cRx(strRegex);
        
        std::string::const_iterator itBegin = m_content.cbegin();
        std::string::const_iterator itEnd = m_content.cend();

        std::smatch cMatch;
        size_t uReplaceCount = 0;
        while (std::regex_search(itBegin, itEnd, cMatch, cRx))
        {
            const std::ssub_match& cSub = cMatch[0];

            CLOG("Substitution [%u]: %s = %s", uReplaceCount++, cSub.str().c_str(), strValue.c_str());

            const size_t newBeginOffset = std::distance(m_content.cbegin(), cSub.first);

            m_content.replace(cSub.first, cSub.second, strValue);

            itBegin = std::next(m_content.cbegin(), newBeginOffset);
            itEnd = m_content.cend();
        }

        if (0 < uReplaceCount)
        {
            m_bModified = true;
        }
        else
        {
            CWARNING("Substitution of '%s' by '%s' in file '%s' produced no effect.",
                strRegex.c_str(), strValue.c_str(), m_path.string().c_str());
        }

        return m_bModified;
    }

    //! @copydoc IFileOperatorContext::Read
    bool Read(const IFileDataLocator& rLocator, std::string& strValue) override
    {
        bool bStatus = false;

        const std::string strRegex = rLocator.to_string();
        std::smatch cMatch;

        if (!std::regex_search(m_content, cMatch, std::regex(strRegex)))
        {
            CERROR("Failed to match regular expression '%s' in file '%s'", strRegex.c_str(), m_path.string().c_str());
        }
        else
        {
            strValue = cMatch[0].str();
            bStatus = true;
        }

        return bStatus;
    }

    //! @copydoc IFileOperatorContext::GetType
    EFileOperationHandlerType GetType() const override
    {
        return EFileOperationHandlerType::Regex;
    }
};

/* CRegexHandler */

DEFINE_SERIALIZATION_SCHEME(CRegexHandler);

REGISTER_POLYMORPHIC_CLASS(CRegexHandler);

CRegexHandler::CRegexHandler()
{

}

CRegexHandler::~CRegexHandler()
{

}

/* static */ CRegexHandler::handler_ptr_t CRegexHandler::Create()
{
    return std::make_unique<CRegexHandler>();
}

EFileOperationHandlerType CRegexHandler::GetType() const
{
    return EFileOperationHandlerType::Regex;
}

CRegexHandler::context_ptr_t CRegexHandler::CreateContext(const std::filesystem::path& filePath) const
{
    return std::make_shared<CRegexOperationContext>(filePath);
}

CRegexHandler::locator_ptr_t CRegexHandler::CreateLocator() const
{
    return std::make_unique<CRegexLocator>();
}

bool CRegexHandler::Document(IDocExporter& rExporter) const
{
    return rExporter.Paragraph()
        && rExporter.Text("This file is handled as an arbitrary text format using regular expressions.")
        && rExporter.PopStack();
}