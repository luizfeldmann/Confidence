#ifndef _EFILEOPERATIONHANDLERTYPE_H_
#define _EFILEOPERATIONHANDLERTYPE_H_

//! @brief Reflects the types of supported files for structured operations
enum class EFileOperationHandlerType : int
{
    IniFile,    //!< Handles operations on INI files
    XmlFile,    //!< Handles operations on XML files
    JsonFile,   //!< Handles operations on JSON files
};

#endif