#ifndef _CVARIABLECELL_H_
#define _CVARIABLECELL_H_

#include <wx/dataview.h>

//! @brief Utility to manage table cells associated to variable rules
class CVariableCell
{
public:
    //! @brief Possible status for the expression cells of the variable
    enum class ECellStatus {
        Assign,     //!< An expression is set for the cell
        Inherit,    //!< The cell inherits the expression from the parent configuration
        Undefined,  //!< The expression is undefined
        Error,      //!< The expression rule is invalid
    };

    //! @brief Updates a table view cell text and icon
    static void SetCellStatus(ECellStatus eStatus, wxDataViewIconText& rCell, const wxString strLabel = wxEmptyString);
};

#endif