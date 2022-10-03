#ifndef _IFINDBYNAME_H_
#define _IFINDBYNAME_H_

#include "util/ITracked.h"

template <typename T>
class IFindByName : public ITracked<T>
{
public:
    static T* FindByName(const std::string& strSearchName)
    {
        typename T::const_iterator it = std::find_if(T::TrackedBegin(), T::TrackedEnd(), 
            [&strSearchName](const T* p) -> bool
            {
                return (p->GetName() == strSearchName);
            }
        );

        return (it == T::TrackedEnd()) 
            ? nullptr 
            : (*it);
    }
};

#endif