#ifndef _IPROJFSPROVIDER_H_
#define _IPROJFSPROVIDER_H_

#include <Windows.h>
#include <projectedfslib.h>

class IProjectedFileSystemProvider
{
protected:
    friend class CFileSystemProviderHelper;
    // Inform the provider a directory enumeration is starting.
    virtual HRESULT StartDirEnum(
        _In_     const PRJ_CALLBACK_DATA* CallbackData,
        _In_     const GUID* EnumerationId
    ) = 0;

    // Inform the provider a directory enumeration is over.
    virtual HRESULT EndDirEnum(
        _In_     const PRJ_CALLBACK_DATA* CallbackData,
        _In_     const GUID* EnumerationId
    ) = 0;

    // Request directory enumeration information from the provider.
    virtual HRESULT GetDirEnum(
        _In_        const PRJ_CALLBACK_DATA* CallbackData,
        _In_        const GUID* EnumerationId,
        _In_opt_    PCWSTR                      SearchExpression,
        _In_        PRJ_DIR_ENTRY_BUFFER_HANDLE DirEntryBufferHandle
    ) = 0;

    // Request meta data information for a file or directory.
    virtual HRESULT GetPlaceholderInfo(
        _In_    const PRJ_CALLBACK_DATA* CallbackData
    ) = 0;

    // Request the contents of a file's primary data stream.
    virtual HRESULT GetFileData(
        _In_    const PRJ_CALLBACK_DATA* CallbackData,
        _In_    UINT64                      ByteOffset,
        _In_    UINT32                      Length
    ) = 0;

    // Deliver notifications to the provider that it has specified 
    virtual HRESULT Notify(
        _In_        const PRJ_CALLBACK_DATA* CallbackData,
        _In_        BOOLEAN                         IsDirectory,
        _In_        PRJ_NOTIFICATION                NotificationType,
        _In_opt_    PCWSTR                          DestinationFileName,
        _Inout_     PRJ_NOTIFICATION_PARAMETERS* NotificationParameters
    ) = 0;

    virtual HRESULT QueryFileName(
        _In_        const PRJ_CALLBACK_DATA* CallbackData
    ) = 0;

    virtual void CancelCommand(
        _In_        const PRJ_CALLBACK_DATA* CallbackData
    ) = 0;
};

#endif