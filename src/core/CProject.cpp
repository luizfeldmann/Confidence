#include "core/CProject.h"
#include "util/Log.h"
#include <fstream>

#include "core/CVariable.h"
#include "core/CGroup.h"

DEFINE_SERIALIZATION_SCHEME(CProject,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_id)
    SERIALIZATION_MEMBER(m_cConfigurations)
    SERIALIZATION_MEMBER(m_cInstances)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CProject);

/* CProject */

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

/* OVERRIDES FROM ITreeItemCollection */

CProject::vec_ref_t CProject::SubItems()
{
    CProject::vec_ref_t vSubItems = CStoredItemCollection::SubItems();

    vSubItems.emplace(vSubItems.begin(), std::ref(m_cInstances));
    vSubItems.emplace(vSubItems.begin(), std::ref(m_cConfigurations));

    return vSubItems;
}

CProject::vec_cref_t CProject::SubItems() const
{
    CProject::vec_cref_t vSubItems = CStoredItemCollection::SubItems();

    vSubItems.emplace(vSubItems.begin(), std::cref(m_cInstances));
    vSubItems.emplace(vSubItems.begin(), std::cref(m_cConfigurations));

    return vSubItems;
}

/* OVERRIDES FROM IProjTreeItem */

ETreeItemType CProject::GetType() const
{
    return EProject;
}

bool CProject::PostDeserialize()
{
    return IProjTreeItem::PostDeserialize();
}

bool CProject::PreSerialize()
{
    return IProjTreeItem::PreSerialize();
}