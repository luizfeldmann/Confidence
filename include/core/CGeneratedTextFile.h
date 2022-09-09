#ifndef _CGENERATEDTEXTFILE_H_
#define _CGENERATEDTEXTFILE_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"

//! @brief Text file with dynamically generated content
class CGeneratedTextFile : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
public:
    //! @brief Creates a new, empty group
    CGeneratedTextFile();
    ~CGeneratedTextFile();

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CGeneratedTextFile)

#endif