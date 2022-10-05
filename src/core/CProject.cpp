#include "core/CProject.h"
#include "util/Log.h"
#include <fstream>
#include "util/version.h"
#include "core/CVariable.h"
#include "core/CGroup.h"

DEFINE_SERIALIZATION_SCHEME(CProject,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(IIdentifiable)
    SERIALIZATION_MEMBER(m_strAppVersion)
    SERIALIZATION_MEMBER(m_vExportDocumentation)
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
    , m_strAppVersion(Version::szVersion)
{
}

CProject::~CProject()
{

}

const CConfigurationGroup& CProject::GetConfigurations() const
{
    return m_cConfigurations;
}

const CInstanceGroup& CProject::GetInstances() const
{
    return m_cInstances;
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

CProject::vec_exporters_t& CProject::GetDocumentationExporters()
{
    return m_vExportDocumentation;
}

bool CProject::ExportDocumentation()
{
    bool bGeneralStatus = true; //! Result of *all* the exporters

    for (const ptr_exporter_t& pExporter : m_vExportDocumentation)
    {
        bool bLocalStatus = false; //! Status of *current* exporter
        const std::string strPath = pExporter->GetOutputFileName();

        // TODO: evaluate expressions in strPath

        std::ofstream ofs(strPath, std::ios::binary | std::ios::trunc);
        if (!ofs.is_open())
            CERROR("Failed to create file '%s' for documentation exporter", strPath.c_str());
        else
        {
            bLocalStatus = pExporter->Start(ofs);

            if (bLocalStatus)
            {
                bLocalStatus = Document(*pExporter);

                // If the exporter Start()'ed, Finish() it even in case of error for safe cleanup  
                bLocalStatus = bLocalStatus && pExporter->Finish();
            }

            // Dangerous case of failure
            // Inaccurate documentation may be generated, let the user know
            if (!bLocalStatus)
                CERROR("The documentation exporter failed. "
                    "Output file '%s' may be incomplete or corrupted.",
                    strPath.c_str());         
        }

        // General result is 'success' only if *all* the exporters succeed
        bGeneralStatus = bGeneralStatus && bLocalStatus;
    }

    if (m_vExportDocumentation.empty())
        CWARNING("The project has *no* documentation jobs perform.");
    else if (!bGeneralStatus)
        CWARNING("One or more documentation jobs were unsuccessful. "
            "Generated files may be missing, incomplete, inaccurate, corrupted or outdated.");
    else
        CINFO("Generation of documentation completed successfully.");

    return bGeneralStatus;
}

bool CProject::Document(IDocExporter& rExporter) const
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

bool CProject::PreSerialize()
{
    m_strAppVersion = Version::szVersion;

    return IProjTreeItem::PreSerialize();
}