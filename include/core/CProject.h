#ifndef _CPROJECT_H_
#define _CPROJECT_H_

#include "util/CGuid.h"
#include "core/CInstanceGroup.h"
#include "core/CConfigurationGroup.h"
#include "docs/IDocumentable.h"

//! @brief Top level item in the project tree / the main project document
class CProject : public IProjTreeItem
               , public CStoredNameItem
               , public CStoredDescriptionItem
               , public CStoredItemCollection
               , public IIdentifiable
               , public IDocumentable
{
public:
    //! @brief Creates a new, empty project
    CProject();
    ~CProject();

    //! @brief Default move-constructor
    CProject(CProject&&) = default;

    //! @brief Default move-assignment operator
    CProject& operator=(CProject&&) = default;

    /********************************/
    /* OVERRIDES FROM IProjTreeItem */
    /********************************/

    // @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    /**************************************/
    /* OVERRIDES FROM ITreeItemCollection */
    /**************************************/

    //! @copydoc ITreeItemCollection::SubItems
    vec_ref_t SubItems() override;

    //! @copydoc ITreeItemCollection::SubItems
    vec_cref_t SubItems() const override;

    /************/
    /* CProject */
    /************/

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
    const CConfigurationGroup& GetConfigurations() const;

    //! @brief Gets a reference to the project's instances
    const CInstanceGroup& GetInstances() const;

    //! @brief Executes the project
    //! @return True if success
    bool Run();

    //! @brief Stops the current execution, if any
    void Stop();

    using ptr_exporter_t = std::unique_ptr<IDocExporter>;
    using vec_exporters_t = std::vector<ptr_exporter_t>;

    //! @brief Gets a reference to the collection of documentation exporters
    vec_exporters_t& GetDocumentationExporters();

    //! @brief Generates the documentation associated to this project
    //! @return True if success
    bool ExportDocumentation();

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;

protected:
    /************/
    /* CProject */
    /************/

    //! Stores most recent open/save path to the project file, if any
    std::string m_currentPath;

    //! Stores the version of the application that created this project file
    std::string m_strAppVersion;

    //! Stores the collection of exporters
    vec_exporters_t m_vExportDocumentation;

    //! Group of all configurations supported by the project
    CConfigurationGroup m_cConfigurations;

    //! Group of all instances on this project
    CInstanceGroup m_cInstances;

    SERIALIZATION_FRIEND(CProject);

    /********************************/
    /* OVERRIDES FROM IProjTreeItem */
    /********************************/

    // @copydoc IProjTreeItem::PreSerialize
    bool PreSerialize() override;
};

DECLARE_SERIALIZATION_SCHEME(CProject)

#endif