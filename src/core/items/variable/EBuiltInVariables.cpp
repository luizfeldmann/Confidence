#include "core/items/variable/EBuiltInVariables.h"
#include <map>

#define xstringify(x) stringify(x)
#define stringify(x) #x

#define ENUM_PAIR(x) { EBuiltInVariable::x, xstringify(x) }

const std::string& GetBuiltinName(EBuiltInVariable e)
{
    static const std::map<EBuiltInVariable, std::string> m_map {
        ENUM_PAIR(ProjectName),
        ENUM_PAIR(InstanceName),
        ENUM_PAIR(ConfigurationName),
        ENUM_PAIR(ProjectFile),
        ENUM_PAIR(ProjectDir),
    };

    return m_map.at(e);
}

const std::regex& GetVariableRegex()
{
    static const std::regex c_varRegex("\\$\\(([\\w|\\d]+)\\)");

    return c_varRegex;
}

std::string MakeExpression(const std::string& strVariableName)
{
    return "$(" + strVariableName + ")";
}