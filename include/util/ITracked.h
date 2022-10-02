#ifndef _ITRACKED_H_
#define _ITRACKED_H_

#include <unordered_set>
#include "CGuid.h"

// @brief Interface for objects with a tracking ID and a registry used to find them
template<typename T> class ITracked
{
private:
    //! @brief The copy-constructor is deleted because this class cannot be copied
    ITracked(const ITracked&) = delete;

    //! @brief The copy-assign operator is deleted because this class cannot be copied
    ITracked& operator=(const ITracked&) = delete;

public:
    using set_type = std::unordered_set<T*>;
    using const_iterator = typename set_type::const_iterator;

protected:
    //! The unique ID associated with this instance
    CGuid m_ID;
    SERIALIZATION_FRIEND(ITracked<T>);

    //! A set of all valid instances
    static set_type m_trackSet;

    //! Casts an ITracked to the specialized type
    inline T* Cast(ITracked<T>* p)
    {
        return static_cast<T*>(p);
    }

    //! @brief Casts this interface to the specialized type
    inline T* This()
    {
        return Cast(this);
    }

public:
    //! @brief Creates an instance with a new GUID, and registers it
    ITracked()
    {
        m_trackSet.insert(This());
    }

    //! @brief Registers the new location and unregisters the old one
    ITracked(ITracked&& rOther)
        : m_ID(rOther.m_ID)
    {
        rOther.m_ID = CGuid::Null();

        m_trackSet.erase(Cast(&rOther));
        m_trackSet.insert(This());
    }

    ITracked& operator=(ITracked&& rOther)
    {
        m_ID = rOther.m_ID;
        rOther.m_ID = CGuid::Null();

        m_trackSet.erase(Cast(&rOther));
        m_trackSet.insert(This());
        
        return *this;
    }

    //! @brief When destroyed, the instance is removed from the set
    ~ITracked()
    {
        m_trackSet.erase(This());
    }

    //! @brief Returns the unique ID associated with this object
    inline CGuid GetID() const
    {
        return m_ID;
    }

    //! @brief Returns the first iterator to the set of all instances of this type
    inline static const_iterator TrackedBegin()
    {
        return m_trackSet.cbegin();
    }

    //! @brief Returns a past-the-end iterator to the set of all instances of this type
    inline static const_iterator TrackedEnd()
    {
        return m_trackSet.cend();
    }

    //! @brief Finds an instance by the provided ID
    static T* FindByID(const CGuid& rID)
    {
        T* pFound = nullptr;

        const_iterator itFound = std::find_if(TrackedBegin(), TrackedEnd(), 
            [rID](const T* const pSearch) -> bool {
                return (pSearch->m_ID == rID);
            }
        );

        if (TrackedEnd() != itFound)
            pFound = *itFound;

        return pFound;
    }
};

#endif