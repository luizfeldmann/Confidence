#ifndef _ETREEITEMTYPE_H_
#define _ETREEITEMTYPE_H_

//! The enum type 'ETreeItemType' is unscoped. Prefer 'enum class' over 'enum'
#pragma warning(disable: 26812)

//! @brief A type of item contained in the project tree
//!        These values may be bitwise-OR'ed together 
enum ETreeItemType
{
    EInvalid    = 0x00, //! The type is unknown or invalid
    EProject    = 0x01, //! The top level item / the project
    EGroup      = 0x02, //! A group of subitems
    EVariable   = 0x04, //! The definition of a variable
    ETextFile   = 0x08, //! A generated output text file
    EInstance   = 0x10, //! Each realization of an evaluation
    EInstGroup  = 0x20, //! Special group containing only instances
    EConfig     = 0x40, //! Complete collection of configurations
    EConfGroup  = 0x80, //! Special group containing only configurations
    ERunProcess = 0x100,//! Creates a new process and (optionally) waits for return code and output
    EImportEnv  = 0x200,//! Imports a variable from the environment
    EOperations = 0x400,//! Allows reading/writing structured data to supported file formats
};

#endif