#include "core/CProject.h"
#include "util/Log.h"
#include <fstream>
#include "util/version.h"
#include "core/CVariable.h"
#include "core/CGroup.h"
#include "core/CInstance.h"
#include "core/CConfiguration.h"
#include "core/CExecutionContext.h"

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

const CConfigurationGroup& CProject::GetConfigurations() const
{
    return m_cConfigurations;
}

const CConfiguration* CProject::GetConfiguration(const std::string& strName) const
{
    return dynamic_cast<const CConfiguration*>( FindSubitemByName(strName, m_cConfigurations) );
}

const CInstance* CProject::GetInstance(const std::string& strName) const
{
    return dynamic_cast<const CInstance*>( FindSubitemByName(strName, m_cInstances) );
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

bool CProject::Run(const std::string& strConfigName)
{
    bool bStatus;

    // Create a new context for the global project execution
    m_pExecution.reset( new CProjectExecutionContext );

    // Find the configuration to execute
    const CConfiguration* pConfig = GetConfiguration(strConfigName);

    if (!pConfig)
    {
        CERROR("Configuration '%s' does not exist", strConfigName.c_str());
        bStatus = false;
    }
    else
    {
        bStatus = true;

        // Execute each instance in the project
        vec_ref_t vInstances = m_cInstances.SubItems();
        for (vec_ref_t::iterator it = vInstances.begin(); bStatus && (it != vInstances.end()); ++it)
        {
            const CInstance& rInstance = dynamic_cast<const CInstance&>(it->get());

            // Create one execution context for each instance
            m_pExecution->m_contexts.emplace_back( *this, rInstance, *pConfig );
            bStatus = Execute(m_pExecution->m_contexts.back());

            if (!bStatus)
            {
                const std::string strInstName = rInstance.GetName();
                CERROR("The execution of instance '%s' failed", strInstName.c_str());
            }
        }
    }

    return bStatus;
}

void CProject::Stop()
{
    m_pExecution.reset();
}

CProject::vec_exporters_t& CProject::GetDocumentationExporters()
{
    return m_vExportDocumentation;
}

bool CProject::ExportDocumentation()
{
    bool bGeneralStatus = true; //! Result of *all* the exporters

    CExecutionContextBase cContext(*this);

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

    const std::string strNameProj = rContext.m_rProject.GetName();
    const std::string strNameConf = rContext.m_rConfiguration.GetName();
    const std::string strNameInst = rContext.m_rInstance.GetName();

    CLOG(PROJ_LOG_MSG_START, strNameProj.c_str(), strNameConf.c_str(), strNameInst.c_str());

    // Execute each subitem in the project
    bool bStatus = ExecuteChildren(rContext);

    CLOG(PROJ_LOG_MSG_END);

    return bStatus;
}