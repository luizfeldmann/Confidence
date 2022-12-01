/**
 * @file IContextStorage.h
 * @brief Defines interface #IContextStorage
 */

#ifndef _ICONTEXTSTORAGE_H_
#define _ICONTEXTSTORAGE_H_

//! @brief Base class for objects stored in the execution context
class IContextStorage
{
public:
    IContextStorage() = default;
    virtual ~IContextStorage() = default;
};

#endif