#ifndef _CPROJECT_H_
#define _CPROJECT_H_

#include "util/CGuid.h"
#include "core/CInstanceGroup.h"
#include "core/CConfigurationGroup.h"

//! @brief Top level item in the project tree / the main project document
class CProject : public IProjTreeItem
               , public CStoredNameItem
               , public CStoredDescriptionItem
               , public CStoredItemCollection
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

    //! @brief Generates the documentation associated to this project
    //! @return True if success
    bool ExportDocumentation();

protected:
    /************/
    /* CProject */
    /************/

    //! Stores most recent open/save path to the project file, if any
    std::string m_currentPath;

    //! Group of all configurations supported by the project
    CConfigurationGroup m_cConfigurations;

    //! Group of all instances on this project
    CInstanceGroup m_cInstances;

    SERIALIZATION_FRIEND(CProject);
};

DECLARE_SERIALIZATION_SCHEME(CProject)

#endif