#ifndef _ETREEITEMTYPE_H_
#define _ETREEITEMTYPE_H_

//! @brief A type of item contained in the project tree
//!        These values may be bitwise-OR'ed together 
enum ETreeItemType
{
    EInvalid    = 0x00, //! The type is unknown or invalid
    EProject    = 0x01, //! The top level item / the project
    EGroup      = 0x02, //! A group of subitems
    EVariable   = 0x04, //! The definition of a variable
    ETextFile   = 0x08, //! A generated output text file
};

#endif