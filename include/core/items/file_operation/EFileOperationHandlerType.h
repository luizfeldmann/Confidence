/**
 * @file EFileOperationHandlerType.h
 * @brief Defines enum #EFileOperationHandlerType
 */

#ifndef _EFILEOPERATIONHANDLERTYPE_H_
#define _EFILEOPERATIONHANDLERTYPE_H_

//! @brief Reflects the types of supported files for structured operations
enum class EFileOperationHandlerType : int
{
    Regex,      //!< Operates on arbitrary formats using regular expressions
    IniFile,    //!< Handles operations on INI files
    XmlFile,    //!< Handles operations on XML files
    JsonFile,   //!< Handles operations on JSON files
};

#endif