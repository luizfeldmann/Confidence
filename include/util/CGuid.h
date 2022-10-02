#ifndef _CGUID_H_
#define _CGUID_H_

#include <guiddef.h>
#include <string>
#include <JInterface/JsonSerialization.h>
#include <JInterface/ICustomSerializable.h>

//! @brief Distributed computing environment universally unique identifier
class CGuid : public GUID, public ICustomSerializable
{
protected:
    bool FromWString(const std::wstring& rStr);

public:
    //! @brief Creates a new random GUID
    CGuid();

    //! @brief Parses from a string
    CGuid(const std::string& rStr);

    //! @brief Parses from a string
    CGuid(const std::wstring& rStr);

    //! @brief Formats this GUID into a string representation
    operator std::wstring() const;

    //! @brief Formats this GUID into a string representation
    operator std::string () const;

    //! @brief Compares this GUID to another.
    //! @returns =0: if they match;
    //!          <0: The first byte that does not match has a lower value in this than in \p rOther;
    //!          >0: The first byte that does not match has a greater value in this than in \p rOther.
    int Compare(const CGuid& rOther) const;

    //! @brief Checks if two GUIDs are equal
    //! @param rOther Other instance to compare to this
    //! @return True if equal
    bool operator==(const CGuid& rOther) const;

    //! @brief Checks if two GUIDs are different
    //! @param rOther Other instance to compare to this
    //! @return True if different
    bool operator!=(const CGuid& rOther) const;

    //! @brief Checks if this GUID is greater than another
    //! @param rOther Other instance to compare to this
    //! @return True if this is greater than \p rOther
    bool operator>(const CGuid& rOther) const;

    //! @brief Checks if this GUID is greater than or equal to another
    //! @param rOther Other instance to compare to this
    //! @return True if this is greater than or equal to \p rOther
    bool operator>=(const CGuid& rOther) const;

    //! @brief Checks if this GUID is smaller than another
    //! @param rOther Other instance to compare to this
    //! @return True if this is greater than \p rOther
    bool operator<(const CGuid& rOther) const;

    //! @brief Checks if this GUID is smaller than or equal to another
    //! @param rOther Other instance to compare to this
    //! @return True if this is smaller than or equal to \p rOther
    bool operator<=(const CGuid& rOther) const;

    //! @copydoc ICustomSerializable::Serialize
    std::string Serialize() const override;

    //! @copydoc ICustomSerializable::Deserialize
    bool Deserialize(const std::string&) override;

    //! @brief Gets the "NULL" GUID (all zeroes) 
    static const CGuid& Null();
};

#endif