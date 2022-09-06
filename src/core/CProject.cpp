#include "core/CProject.h"
#include "util/Log.h"
#include <fstream>

#include "core/CVariable.h"
#include "core/CGroup.h"

DEFINE_SERIALIZATION_SCHEME(CProject,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_id)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CProject);

CProject::CProject()
    : CStoredNameItem("<new project>")
    , CStoredDescriptionItem("<no project description>")
    , CStoredItemCollection(EGroup)
{
}

CProject::~CProject()
{

}

CProject& CProject::TheProject()
{
    static CProject theProject;
    return theProject;
}

ETreeItemType CProject::GetType() const
{
    return EProject;
}

bool CProject::OpenFile(const std::string& szOpenFileName)
{
    bool bSuccess = false;

    std::ifstream ifstr(szOpenFileName, std::ios_base::in | std::ios_base::binary);

    if (!ifstr.is_open())
        CERROR("failed to open file %s", szOpenFileName.c_str());
    else
    {
        CJsonObject jObj(*this);
        if (!jObj.Read(ifstr))
            CERROR("failed parsing file %s", szOpenFileName.c_str());
        else
        {
            bSuccess = PostDeserialize();
            if (bSuccess)
                m_currentPath = szOpenFileName;
        }
    }

    return bSuccess;
}

bool CProject::PostDeserialize()
{
    return IProjTreeItem::PostDeserialize();
}

bool CProject::SaveToFile(const std::string& szSaveFileName)
{
    bool bSuccess = false;

    if (PreSerialize())
    {
        std::ofstream ofstr(szSaveFileName, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

        if (!ofstr.is_open())
            CERROR("failed to write file %s", szSaveFileName.c_str());
        else
        {
            CJsonObject jObj(*this);
            bSuccess = jObj.Write(ofstr);
        }
    }

    m_currentPath = szSaveFileName;
    return true;
}

bool CProject::PreSerialize()
{
    return IProjTreeItem::PreSerialize();
}

const std::string& CProject::GetCurrentPath() const
{
    return m_currentPath;
}

bool CProject::Run()
{
    CERROR("FEATURE NOT YET IMPLEMENTED");
    return false;
}

void CProject::Stop()
{
    CERROR("FEATURE NOT YET IMPLEMENTED");
}

bool CProject::ExportDocumentation()
{
    CERROR("FEATURE NOT YET IMPLEMENTED");
    return false;
}