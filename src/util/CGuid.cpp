/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "util/CGuid.h"
#include <Objbase.h>
#include <cassert>
#include <locale>
#include <sstream>

#define GUID_BUFFER_SIZE 39 // 32 hexadecimal characters + 4 hyphens + 2 braces + '\0'

CGuid::CGuid()
{
    HRESULT Status = CoCreateGuid(this);
    assert((S_OK == Status) && "Failed to create new GUID");
}

CGuid::CGuid(const std::string& rStr) :
    CGuid(std::wstring(rStr.cbegin(), rStr.cend()))
{

}

/* static */ const CGuid& CGuid::Null()
{
    const static CGuid c_nullGuid("{00000000-0000-0000-0000-000000000000}");
    return c_nullGuid;
}

CGuid::CGuid(const std::wstring& rStr)
{
    bool Status = FromWString(rStr);
    assert(Status && "Failed to parse GUID");
}

bool CGuid::FromWString(const std::wstring& rStr)
{
    HRESULT Status = CLSIDFromString(rStr.c_str(), this);
    return (NOERROR == Status);
}

CGuid::operator std::wstring() const
{
    wchar_t szText[GUID_BUFFER_SIZE];
    const int iSize = StringFromGUID2(*this, szText, GUID_BUFFER_SIZE);

    assert((0 != iSize) && "Failed to convert GUID to string");

    return szText;
}

CGuid::operator std::string() const
{
    const static char cDefault = '?';                   // Symbol replaced in string when wchar_t is out of ASCII range
    const static std::locale& rLocale = std::locale();  // Represents the system language

    // Convert self to wide string format
    const std::wstring cWideStr(*this);

    // Narrow each character to a temporary buffer
    std::ostringstream stringstream;
    for (wchar_t w : cWideStr)
        stringstream << std::use_facet< std::ctype<wchar_t> >(rLocale).narrow(w, cDefault);

    return stringstream.str();
}

int CGuid::Compare(const CGuid& rOther) const
{
    using BaseType = GUID;
    const BaseType* const pSelf  = static_cast<const BaseType*>(this);
    const BaseType* const pOther = static_cast<const BaseType*>(&rOther);

    return memcmp(pSelf, pOther, sizeof(BaseType));
}

bool CGuid::operator==(const CGuid& rOther) const
{
    return Compare(rOther) == 0;
}

bool CGuid::operator!=(const CGuid& rOther) const
{
    return Compare(rOther) != 0;
}

bool CGuid::operator>(const CGuid& rOther) const
{
    return Compare(rOther) > 0;
}

bool CGuid::operator>=(const CGuid& rOther) const
{
    return Compare(rOther) >= 0;
}

bool CGuid::operator<(const CGuid& rOther) const
{
    return Compare(rOther) < 0;
}

bool CGuid::operator<=(const CGuid& rOther) const
{
    return Compare(rOther) <= 0;
}

std::string CGuid::Serialize() const
{
    return CGuid::operator std::string();
}

bool CGuid::Deserialize(const std::string& rStr)
{
    return FromWString(std::wstring(rStr.cbegin(), rStr.cend()));
}