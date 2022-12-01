/**
 * @file INotifyItemOperation.h
 * @brief Defines interface #INotifyItemOperation
 */

#ifndef _INOTIFYITEMOPERATION_H_
#define _INOTIFYITEMOPERATION_H_

class IProjTreeItem;

//! @brief Interface for classes needing to receive notifications when a project item undergoes an operation
class INotifyItemOperation
{
public:
    //! @brief Notification called when any item is created
    virtual void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent);

    //! @brief Notification called when any item is erased
    virtual void OnAnyItemErased(const IProjTreeItem& rItem);

    //! @brief Notification called when any project item is renamed
    virtual void OnAnyItemRenamed(const IProjTreeItem& rItem);
};

#endif