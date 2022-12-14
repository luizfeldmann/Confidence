/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "util/Log.h"
#include "util/version.h"
#include "CLI/CLI.hpp"
#include "ui/CMainApp.h"
#include "core/items/CProject.h"

//! @brief Represents the set of options passed to the application when launched
//! @internal
class CCommandLineArguments
{
public:
    //! The name of the file to be opened when the application launches
    std::string m_strOpenFileName;

    //! The name of the configuration to run when using the Run mode
    std::string m_strRunConfigurationName;

    //! The mode/command requested for this run
    enum class EMode {
        Invalid,//!< The application was called incorrectly
        Help,   //!< User requested a help message
        Version,//!< Show application version
        Edit,   //!< Opens the project editor as a GUI
        Run,    //!< Runs the project in a console
        ExpDocs //!< Exports the documentation and then closes
    };
    
    //! The action/command the user requested
    EMode m_eMode = EMode::Invalid;

    //! Zero if the command line was parsed and is valid
    int m_iExitCode = 0;

    //! @brief Constructs this structure of parsed arguments from the raw arguments passaed to main()
    //! @param[in] argc Arguments count
    //! @param[in] argv Arguments values array
    CCommandLineArguments(int argc, const char* const* argv)
    {
        CLI::App app{ "Confidence - A configuration management tool you can trust" };

        bool bShowVersion = false;
        CLI::Option* pOptVersion = app.add_flag("-v,--version", bShowVersion, "Shows the version of this software");

        CLI::Option* pOptFileName = app.add_option("-f,--file,filename", m_strOpenFileName, "Path to the project file");
        pOptFileName->check(CLI::ExistingFile);

        bool bExportDocs = false;
        CLI::Option* pOptExport = app.add_flag("-d,--docs", bExportDocs, "Export the documentation and exit");
        CLI::Option* pOptRun = app.add_option("-r,--run", m_strRunConfigurationName, "Run the project in a console, using a supplied configuration name");

        pOptVersion->excludes(pOptRun);
        pOptVersion->excludes(pOptExport);
        pOptVersion->excludes(pOptFileName);

        pOptRun->excludes(pOptVersion);
        pOptRun->excludes(pOptExport);
        pOptRun->needs(pOptFileName);

        pOptExport->excludes(pOptVersion);
        pOptExport->excludes(pOptRun);
        pOptExport->needs(pOptFileName);

        try {
            app.parse(argc, argv);
        }
        catch (const CLI::CallForHelp& e)
        {
            std::string strHelp = app.help();
            CLOG("%s", strHelp.c_str());

            m_iExitCode = e.get_exit_code();
            m_eMode = EMode::Help;
        }
        catch (const CLI::ParseError& e)
        {
            CERROR("%s", e.what());
            m_iExitCode = e.get_exit_code();
        }

        if ((0 == m_iExitCode) && (EMode::Help != m_eMode))
        {
            if (bShowVersion)
                m_eMode = EMode::Version;
            else
                m_eMode = EMode::Edit;

            if (!m_strOpenFileName.empty())
            {
                if (bExportDocs)
                    m_eMode = EMode::ExpDocs;
                else if (!m_strRunConfigurationName.empty())
                    m_eMode = EMode::Run;
            }
        }
    }
};

static int ExportDocs(const std::string& rFileName)
{
    CINFO("Exporting documentation for project '%s' ...", rFileName.c_str());

    bool bSuccess = false;
    std::shared_ptr<CProject> pProject = CProject::Create(rFileName);
    if (pProject)
    {
        bSuccess = pProject->ExportDocumentation();
    }

    return bSuccess ? 0 : -1;
}

static int RunProj(const std::string& rFileName, const std::string& rConfigName)
{
    CINFO("Running configuration '%s' of project '%s' ...", rConfigName.c_str(), rFileName.c_str());

    bool bSuccess = false;
    std::shared_ptr<CProject> pProject = CProject::Create(rFileName);
    if (pProject)
    {
        bSuccess = pProject->Run(rConfigName);
        if (bSuccess)
        {
            system("PAUSE");
            pProject->Stop();
        }
    }

    return bSuccess ? 0 : -1;
}

static int EditProj(const std::string& rFileName, int argc, char** argv)
{
    CINFO("Launching editor for project '%s' ...", rFileName.empty() ? "<new project>" : rFileName.c_str());
    
    CMainApp::m_strOpenProjectFileName = rFileName;
    
    return wxEntry(argc, argv);
}

static int PrintVersion()
{
    CLOG("%s (%s)", Version::szVersion, Version::szTimestamp);
    return 0;
}

int main(int argc, char** argv)
{
    CCommandLineArguments cArgs(argc, argv);
    int Status = cArgs.m_iExitCode;
    switch (cArgs.m_eMode)
    {
    default:
    case CCommandLineArguments::EMode::Help:
    case CCommandLineArguments::EMode::Invalid:
        break;

    case CCommandLineArguments::EMode::Version:
        Status = PrintVersion();
        break;

    case CCommandLineArguments::EMode::ExpDocs:
        Status = ExportDocs(cArgs.m_strOpenFileName);
        break;

    case CCommandLineArguments::EMode::Run:
        Status = RunProj(cArgs.m_strOpenFileName, cArgs.m_strRunConfigurationName);
        break;

    case CCommandLineArguments::EMode::Edit:
        Status = EditProj(cArgs.m_strOpenFileName, argc, argv);
        break;
    }

    return Status;
}