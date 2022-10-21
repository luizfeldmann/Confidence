#include "core/EBuiltInVariables.h"
#include <map>

#define xstringify(x) stringify(x)
#define stringify(x) #x

#define ENUM_PAIR(x) { EBuiltInVariable::x, xstringify(x) }

const std::string& GetBuiltinName(EBuiltInVariable e)
{
    static const std::map<EBuiltInVariable, std::string> m_map {
        ENUM_PAIR(ProjectName),
        ENUM_PAIR(InstanceName),
        ENUM_PAIR(ConfigurationName)
    };

    return m_map.at(e);
}