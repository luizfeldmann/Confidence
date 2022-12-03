/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CPROJECT_H_
#define _CPROJECT_H_

#include "util/IIdentifiable.h"
#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CStoredItemCollection.h"

class CInstance;
class CInstanceGroup;
class CConfiguration;
class CConfigurationGroup;
class CProjectExecutionContext;

//! @brief Top level item in the project tree / the main project document
class CProject : public IProjTreeItem
               , public CStoredNameItem
               , public CStoredDescriptionItem
               , public CStoredItemCollection
               , public IIdentifiable
{
private:
    CProject(const CProject&) = delete;
    CProject& operator=(const CProject&) = delete;

public:
    ~CProject();
    /********************************/
    /* OVERRIDES FROM IProjTreeItem */
    /********************************/

    // @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    /**************************************/
    /* OVERRIDES FROM ITreeItemCollection */
    /**************************************/

    //! @copydoc ITreeItemCollection::SubItems
    vec_ptr_t SubItems() override;

    //! @copydoc ITreeItemCollection::SubItems
    vec_cptr_t SubItems() const override;

    /************/
    /* CProject */
    /************/

    //! @brief Creates a new empty project
    static std::shared_ptr<CProject> Create();

    //! @brief Creates a new instance by opening a project file
    //! @param[in] szOpenFileName Path to the file to be opened 
    static std::shared_ptr<CProject> Create(const std::string& szOpenFileName);

    //! @brief Opens a project from a file
    //! @param[in] szOpenFileName Path to the file to be opened
    //! @return True if success
    bool OpenFile(const std::string& szOpenFileName);

    //! @brief Saves the project to the specified file
    //! @param[in] szSaveFileName Output path of the save file
    //! @return True if success
    bool SaveToFile(const std::string& szSaveFileName);

    //! @brief Gets the most recent path where this project was opened/saved
    const std::string& GetCurrentPath() const;

    //! @brief Gets a reference to the project's configurations
    std::shared_ptr<const CConfiguration> GetConfigurations() const;

    //! @brief Finds a configuration with the provided name
    //! @param[in] strName Name of the configuration to search
    std::shared_ptr<const CConfiguration> GetConfiguration(const std::string& strName) const;

    //! @brief Gets a reference to the project's instances
    std::shared_ptr<const CInstanceGroup> GetInstances() const;

    //! @brief Finds an instance with the provided name
    //! @param[in] strName Name of the instance to search
    std::shared_ptr<const CInstance> GetInstance(const std::string& strName) const;

    //! @brief Executes the project using a provided configuration
    //! @return True if success
    bool Run(const std::string& strConfigName) const;

    //! @brief Stops the current execution, if any
    void Stop() const;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext& rContext) const override;

    //! Pointer type used to pass/store the configured instances of documentation exports
    using ptr_exporter_t = std::unique_ptr<IDocExporter>;

    //! Type used to pass/store the collection of document exports used by the project
    using vec_exporters_t = std::vector<ptr_exporter_t>;

    //! @brief Gets a reference to the collection of documentation exporters
    vec_exporters_t& GetDocumentationExporters();

    //! @brief Generates the documentation associated to this project
    //! @return True if success
    bool ExportDocumentation() const;

protected:
    /************/
    /* CProject */
    /************/

    //! @brief Creates a new, empty project
    CProject();
    FACTORY_FRIEND();

    //! Stores most recent open/save path to the project file, if any
    std::string m_currentPath;

    //! Stores the version of the application that created this project file
    std::string m_strAppVersion;

    //! Stores the collection of exporters
    vec_exporters_t m_vExportDocumentation;

    //! Group of all configurations supported by the project
    std::shared_ptr<CConfigurationGroup> m_pConfigurations;

    //! Group of all instances on this project
    std::shared_ptr<CInstanceGroup> m_pInstances;

    //! Stores the current state of execution
    mutable std::unique_ptr<CProjectExecutionContext> m_pExecution;

    SERIALIZATION_FRIEND(CProject);

    /********************************/
    /* OVERRIDES FROM IProjTreeItem */
    /********************************/

    // @copydoc IProjTreeItem::PreSerialize
    bool PreSerialize() override;
};

DECLARE_SERIALIZATION_SCHEME(CProject)

#endif