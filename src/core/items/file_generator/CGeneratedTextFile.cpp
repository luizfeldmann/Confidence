#include "core/items/file_generator/CGeneratedTextFile.h"
#include "vfs/CTempSymlinkGenerator.h"
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

ITextProvider* CGeneratedTextFile::GetProvider() const
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
    bool bStatus = rExporter.FormField("Destination Path:", GetOutputPath(), true);

    if (bStatus && m_pProvider)
        bStatus = m_pProvider->Document(rExporter);

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
        // Evaluate the contents and the destination
        IFileGenerator::sptr_t pFile;
        std::string strContents = m_pProvider->GetText();
        std::string strDestination = GetOutputPath();

        bStatus = rContext.Evaluate(strContents)
            && rContext.Evaluate(strDestination);

        if (bStatus)
            pFile = m_pGenerator->NewFile(strDestination);

        if (pFile && pFile->IsValid())
        {
            std::ostream& os = pFile->GetStream();
            os << strContents;
            os.flush();

            rContext.Store(pFile);
        }
    }
    
    return bStatus;
}