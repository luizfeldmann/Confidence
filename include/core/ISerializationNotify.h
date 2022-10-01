#ifndef _ISERIALIZATIONNOTIFY_H_
#define _ISERIALIZATIONNOTIFY_H_

//! @brief Interface for notifying a class before and after serialization
class ISerializationNotify
{
public:
    //! @brief Post-processing and validation of read data after de-serialization from a project file
    virtual bool PostDeserialize() = 0;

    //! @brief Pre-processing and validation of any data required before serializing to a project file
    virtual bool PreSerialize() = 0;
};

#endif