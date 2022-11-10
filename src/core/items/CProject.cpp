#include "core/items/CProject.h"
#include "util/Log.h"
#include <fstream>
#include <cassert>
#include "util/version.h"
#include "core/items/variable/CVariable.h"
#include "core/items/CInstance.h"
#include "core/items/CInstanceGroup.h"
#include "core/items/CConfigurationGroup.h"
#include "core/CExecutionContext.h"

DEFINE_SERIALIZATION_SCHEME(CProject,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(IIdentifiable)
    SERIALIZATION_MEMBER(m_strAppVersion)
    SERIALIZATION_MEMBER(m_vExportDocumentation)
    SERIALIZATION_MEMBER(m_pConfigurations)
    SERIALIZATION_MEMBER(m_pInstances)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CProject);

/* CProjectExecutionContext */

/* Stores the state of the current project execution*/
class CProjectExecutionContext
{
private:
    CProjectExecutionContext(const CProjectExecutionContext&) = delete;
    CProjectExecutionContext& operator=(const CProjectExecutionContext&) = delete;

public:
    CProjectExecutionContext() = default;
    ~CProjectExecutionContext() = default;

    //! Stores the execution context of each instance during the project execution
    std::list<CExecutionContext> m_contexts;
};

/* CProject */

CProject::CProject()
    : CStoredNameItem("<new project>")
    , CStoredDescriptionItem("<no project description>")
    , CStoredItemCollection(EGroup)
    , m_strAppVersion(Version::szVersion)
    , m_pConfigurations(CConfigurationGroup::Create())
    , m_pInstances(CInstanceGroup::Create())
{
}

CProject::~CProject()
{

}

/* static */ std::shared_ptr<CProject> CProject::Create()
{
    return std::shared_ptr<CProject>(new CProject);
}

/* static */ std::shared_ptr<CProject> CProject::Create(const std::string& szOpenFileName)
{
    std::shared_ptr<CProject> pProject = Create();

    if (pProject && !pProject->OpenFile(szOpenFileName))
        pProject.reset();

    return pProject;
}

std::shared_ptr<const CConfiguration> CProject::GetConfigurations() const
{
    assert(m_pConfigurations);
    return m_pConfigurations;
}

std::shared_ptr<const CConfiguration> CProject::GetConfiguration(const std::string& strName) const
{
    return std::dynamic_pointer_cast<const CConfiguration>( FindSubitemByName(strName, GetConfigurations() ) );
}

std::shared_ptr<const CInstance> CProject::GetInstance(const std::string& strName) const
{
    return std::dynamic_pointer_cast<const CInstance>( FindSubitemByName(strName, GetInstances() ) );
}

std::shared_ptr<const CInstanceGroup> CProject::GetInstances() const
{
    assert(m_pInstances);
    return m_pInstances;
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
            bSuccess = PostDeserialize(*this);
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

bool CProject::Run(const std::string& strConfigName) const
{
    bool bStatus;

    // Create a new context for the global project execution
    m_pExecution.reset( new CProjectExecutionContext );

    // Find the configuration to execute
    std::shared_ptr<const CConfiguration> pConfig = GetConfiguration(strConfigName);

    if (!pConfig)
    {
        CERROR("Configuration '%s' does not exist", strConfigName.c_str());
        bStatus = false;
    }
    else
    {
        bStatus = true;

        // Execute each instance in the project
        vec_cptr_t vInstances = GetInstances()->SubItems();

        for (const cptr_t& pInstanceItem : vInstances)
        {
            assert(pInstanceItem);

            // Create one execution context for each instance
            {
                std::shared_ptr<const CProject> pProject = std::dynamic_pointer_cast<const CProject>(shared_from_this());
                std::shared_ptr<const CInstance> pInstance = std::dynamic_pointer_cast<const CInstance>(pInstanceItem);

                m_pExecution->m_contexts.emplace_back(pProject, pInstance, pConfig);
            }
            bStatus = Execute(m_pExecution->m_contexts.back());

            if (!bStatus)
            {
                const std::string strInstName = pInstanceItem->GetName();
                CERROR("The execution of instance '%s' failed", strInstName.c_str());
                break;
            }
        }
    }

    return bStatus;
}

void CProject::Stop() const
{
    m_pExecution.reset();
}

CProject::vec_exporters_t& CProject::GetDocumentationExporters()
{
    return m_vExportDocumentation;
}

bool CProject::ExportDocumentation() const
{
    bool bGeneralStatus = true; //! Result of *all* the exporters

    CExecutionContextBase cContext( std::dynamic_pointer_cast<const CProject>(shared_from_this()) );

    for (const ptr_exporter_t& pExporter : m_vExportDocumentation)
    {
        std::string strPath = pExporter->GetOutputFileName();
        
        //! Status of *current* exporter
        bool bLocalStatus = cContext.Evaluate(strPath); 

        if (bLocalStatus)
        {
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

/* OVERRIDES FROM ITreeItemCollection */

CProject::vec_ptr_t CProject::SubItems()
{
    CProject::vec_ptr_t vSubItems = CStoredItemCollection::SubItems();

    vSubItems.emplace(vSubItems.begin(), m_pInstances);
    vSubItems.emplace(vSubItems.begin(), m_pConfigurations);

    return vSubItems;
}

CProject::vec_cptr_t CProject::SubItems() const
{
    CProject::vec_cptr_t vSubItems = CStoredItemCollection::SubItems();

    vSubItems.emplace(vSubItems.begin(), m_pInstances);
    vSubItems.emplace(vSubItems.begin(), m_pConfigurations);

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

bool CProject::Execute(CExecutionContext& rContext) const
{
    // Log the current execution
    #define PROJ_LOG_MSG_START \
        "Run project: %s \r\n" \
        "Configuration: %s \r\n" \
        "Instance: %s \r\n" \
        "----------------------------------------"

    #define PROJ_LOG_MSG_END \
        "========================================\r\n"

    const std::string strNameProj = rContext.GetProject()->GetName();
    const std::string strNameConf = rContext.GetConfiguration()->GetName();
    const std::string strNameInst = rContext.GetInstance()->GetName();

    CLOG(PROJ_LOG_MSG_START, strNameProj.c_str(), strNameConf.c_str(), strNameInst.c_str());

    // Execute each subitem in the project
    bool bStatus = ExecuteChildren(rContext);

    CLOG(PROJ_LOG_MSG_END);

    return bStatus;
}