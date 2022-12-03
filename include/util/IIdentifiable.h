/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

 /**
 * @file IIdentifiable.h
 * @brief Defines interface #IIdentifiable
 */

#ifndef _IIDENTIFIABLE_H_
#define _IIDENTIFIABLE_H_

#include "CGuid.h"

//! @brief Interface for non-copyable classes containing an ID
//!        This ID is moved when the class is moved
class IIdentifiable
{
public:
    //! The type used as an ID
    using id_type = CGuid;

private:
    IIdentifiable(const IIdentifiable&) = delete;
    IIdentifiable& operator=(const IIdentifiable&) = delete;

protected:
    //! @brief The ID representing this instance
    id_type m_ID;

    SERIALIZATION_FRIEND(IIdentifiable);

public:
    //! @brief Initializes a default-constructed ID
    IIdentifiable() = default;

    //! @brief Constructs by taking the ID of another object; the other object's ID is null-ed
    IIdentifiable(IIdentifiable&& rOther) noexcept;

    //! @brief Assigns the ID by taking it from another object; the other object's ID is null-ed
    IIdentifiable& operator=(IIdentifiable&& rOther) noexcept;

    //! @brief Returns the unique ID associated with this object
    id_type GetID() const;
};

DECLARE_SERIALIZATION_SCHEME(IIdentifiable);

#endif