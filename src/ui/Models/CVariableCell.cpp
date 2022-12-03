/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "ui/Models/CVariableCell.h"
#include <unordered_map>

/* static */ void CVariableCell::SetCellStatus(ECellStatus eStatus, wxDataViewIconText& rCell, const wxString strLabel)
{
    struct SCellStyle
    {
        const wxString strText;
        const wxIcon cIcon;
    };

#define CELL_ICON(NAME) wxIcon(NAME, wxBITMAP_TYPE_ICO_RESOURCE, 16, 16)

    static const std::unordered_map<ECellStatus, SCellStyle> c_mIcons{
        {ECellStatus::Assign,    SCellStyle{"<set empty>",     CELL_ICON("RES_ID_ICON_EXPR_ASSIGN")} },
        {ECellStatus::Inherit,   SCellStyle{"<inherit empty>", CELL_ICON("RES_ID_ICON_EXPR_INHERIT")} },
        {ECellStatus::Undefined, SCellStyle{"<undefined>",     CELL_ICON("RES_ID_ICON_EXPR_UNDEFINED")} },
        {ECellStatus::Error,     SCellStyle{"<error>",         CELL_ICON("RES_ID_ICON_EXPR_ERROR")} },
    };

    const SCellStyle& rStyle = c_mIcons.at(eStatus);

    rCell.SetText(strLabel.empty() ? rStyle.strText : strLabel);
    rCell.SetIcon(rStyle.cIcon);
}