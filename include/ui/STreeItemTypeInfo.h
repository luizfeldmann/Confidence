/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _STREEITEMTYPEINFO_H_
#define _STREEITEMTYPEINFO_H_

#include <core/IProjTreeItem.h>
#include <ui/Editors/ITreeItemEditor.h>
#include <wx/string.h>
#include <wx/icon.h>

//! @brief Information on how tree items should be presented according to their types
struct STreeItemTypeInfo
{
    wxString m_strTypeName;                                         //!< Label to the item category
    wxIcon m_icon;                                                  //!< Icon associated to the item type
    std::function<ITreeItemCollection::ptr_t()>  m_fnFactory;       //!< Factory function to create a new instance of that type
    ITreeItemEditor::fnFactory_t m_fnNewEditor;                     //!< Factory function to create a new editor matching the item type

    //! @brief Gets a list of all *valid* elements in enumeration ETreeItemType
    static const std::vector<ETreeItemType>& GetTypesList();

    //! @brief Returns the STreeItemTypeInfo associated to the requested ETreeItemType
    static const STreeItemTypeInfo& GetInfo(ETreeItemType eType);

    //! @brief Returns the name associated to the requested ETreeItemType
    static const wxString& GetName(ETreeItemType eType);

    //! @brief Gets the icon associated to the requested ETreeItemType
    static const wxIcon& GetIcon(ETreeItemType eType);
};

#endif