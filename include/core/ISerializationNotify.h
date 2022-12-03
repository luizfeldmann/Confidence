/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file ISerializationNotify.h
 * @brief Defines interface #ISerializationNotify
 */

#ifndef _ISERIALIZATIONNOTIFY_H_
#define _ISERIALIZATIONNOTIFY_H_

class CProject;

//! @brief Interface for notifying a class before and after serialization
class ISerializationNotify
{
public:
    //! @brief Post-processing and validation of read data after de-serialization from a project file
    //! @param[in] rProject Reference to the project being deserialized
    virtual bool PostDeserialize(CProject& rProject) = 0;

    //! @brief Pre-processing and validation of any data required before serializing to a project file
    virtual bool PreSerialize() = 0;
};

#endif