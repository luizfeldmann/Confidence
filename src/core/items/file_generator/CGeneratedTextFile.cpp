/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/file_generator/CGeneratedTextFile.h"
#include "vfs/CTempSymlinkGenerator.h"
#include "core/CExecutionContext.h"
#include "docs/IDocExporter.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CGeneratedTextFile,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_strOutputPath)
    SERIALIZATION_MEMBER(m_pGenerator)
    SERIALIZATION_MEMBER(m_pProvider)
)

REGISTER_POLYMORPHIC_CLASS(CGeneratedTextFile);

CGeneratedTextFile::CGeneratedTextFile()
    : CStoredNameItem("<new output file>")
    , CStoredDescriptionItem("<no file description>")
{

}

IProjTreeItem::ptr_t CGeneratedTextFile::Create()
{
    return ptr_t(new CGeneratedTextFile);
}

CGeneratedTextFile::~CGeneratedTextFile()
{

}

ETreeItemType CGeneratedTextFile::GetType() const
{
    return ETextFile;
}

ITextProvider* CGeneratedTextFile::GetProvider()
{
    return m_pProvider.get();
}

const ITextProvider* CGeneratedTextFile::GetProvider() const
{
    return m_pProvider.get();
}

void CGeneratedTextFile::SetProvider(provider_ptr_t pNewProvider)
{
    m_pProvider.swap(pNewProvider);
}

IFileGenerator* CGeneratedTextFile::GetGenerator() const
{
    return m_pGenerator.get();
}

void CGeneratedTextFile::SetGenerator(generator_ptr_t pNewGenerator)
{
    m_pGenerator.swap(pNewGenerator);
}

bool CGeneratedTextFile::PostDeserialize(CProject& rProject)
{
    bool bStatus = true;

    if (m_pProvider)
        bStatus = m_pProvider->PostDeserialize(rProject);

    return bStatus;
}

bool CGeneratedTextFile::PreSerialize()
{
    bool bStatus = true;

    if (m_pProvider)
        bStatus = m_pProvider->PreSerialize();

    return bStatus;
}

std::string CGeneratedTextFile::GetOutputPath() const
{
    return m_strOutputPath;
}

void CGeneratedTextFile::SetOutputPath(const std::string& strNewPath)
{
    m_strOutputPath = strNewPath;
}

bool CGeneratedTextFile::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "FILE:");
}

bool CGeneratedTextFile::DocumentCustom(IDocExporter& rExporter) const
{
    const std::string strOutputPath = GetOutputPath();
    bool bStatus = rExporter.FormField("Destination Path:", strOutputPath, true);

    if (bStatus && m_pProvider)
        bStatus = m_pProvider->Document(rExporter);

    if (bStatus)
    {
        auto vDependVariables = CEvaluationContext::ListVariables(strOutputPath);

        if (m_pProvider)
        {
            const auto vVariablesProvider = m_pProvider->GetDependencies();
            vDependVariables.insert(vDependVariables.end(), vVariablesProvider.cbegin(), vVariablesProvider.cend());
        }

        const std::string strName = GetName();
        for (const std::string strVarDep : vDependVariables)
            rExporter.Dependency(strVarDep, strName);
    }

    return bStatus;
}

bool CGeneratedTextFile::Execute(CExecutionContext& rContext) const
{
    bool bStatus = false;
    
    // Sanity check
    const std::string strName = GetName();

    if (!m_pProvider)
        CWARNING("Unespecified content provider for file '%s'", strName.c_str());
    else if (!m_pGenerator)
        CWARNING("Unespecified file generator for file '%s'", strName.c_str());
    else
    {
        // Get the text contents of for the file
        std::string strContents;
        bStatus = m_pProvider->GetText(strContents);

        if (!bStatus)
            CWARNING("Failed to get contents for file '%s'", strName.c_str());
        else
        {
            // Evaluate the expressions in the contents and the destination
            std::string strDestination = GetOutputPath();

            bStatus = rContext.Evaluate(strContents);
            bStatus = bStatus && rContext.Evaluate(strDestination);

            IFileGenerator::sptr_t pFile;
            if (bStatus)
            {
                pFile = m_pGenerator->NewFile(strDestination);
                bStatus = pFile && pFile->IsValid();
            }

            if (bStatus)
            {
                std::ostream& os = pFile->GetStream();
                os << strContents;
                os.flush();

                rContext.Store(pFile);
            }
        }
    }
    
    return bStatus;
}