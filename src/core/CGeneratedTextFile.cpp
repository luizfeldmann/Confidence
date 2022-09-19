#include "core/CGeneratedTextFile.h"

DEFINE_SERIALIZATION_SCHEME(CGeneratedTextFile,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_strOutputPath)
    SERIALIZATION_MEMBER(m_cProvider)
)

REGISTER_POLYMORPHIC_CLASS(CGeneratedTextFile);

CGeneratedTextFile::CGeneratedTextFile()
    : CStoredNameItem("<new output file>")
    , CStoredDescriptionItem("<no file description>")
{

}

IProjTreeItem* CGeneratedTextFile::Create()
{
    return new CGeneratedTextFile();
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
    return m_cProvider.get();
}

void CGeneratedTextFile::SetProvider(ITextProvider* pNewProvider)
{
    m_cProvider.reset(pNewProvider);
}

std::string CGeneratedTextFile::GetOutputPath() const
{
    return m_strOutputPath;
}

void CGeneratedTextFile::SetOutputPath(const std::string& strNewPath)
{
    m_strOutputPath = strNewPath;
}