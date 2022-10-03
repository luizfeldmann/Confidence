#include "util/IIdentifiable.h"

DEFINE_SERIALIZATION_SCHEME(IIdentifiable,
    SERIALIZATION_MEMBER(m_ID)
)

IIdentifiable::IIdentifiable(IIdentifiable&& rOther) noexcept
    : m_ID(rOther.m_ID)
{
    rOther.m_ID = id_type::Null();
}

IIdentifiable& IIdentifiable::operator=(IIdentifiable&& rOther) noexcept
{
    m_ID = rOther.m_ID;
    rOther.m_ID = id_type::Null();

    return *this;
}

IIdentifiable::id_type IIdentifiable::GetID() const
{
    return m_ID;
}