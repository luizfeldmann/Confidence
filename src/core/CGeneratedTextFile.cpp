#include "core/CGeneratedTextFile.h"

DEFINE_SERIALIZATION_SCHEME(CGeneratedTextFile,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
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