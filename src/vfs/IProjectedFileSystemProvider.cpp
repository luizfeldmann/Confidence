#include "vfs/IProjectedFileSystemProvider.h"

//! @brief ProjFS C callbacks
//! ProjFS will call these, and they in turn will call the methods on the instance passed by context
class CFileSystemProviderHelper
{
private:
    _Function_class_(PRJ_START_DIRECTORY_ENUMERATION_CB)
        static HRESULT CALLBACK StartDirEnumCallback_C(
        _In_     const PRJ_CALLBACK_DATA* CallbackData,
        _In_     const GUID* EnumerationId
    )
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->StartDirEnum(CallbackData, EnumerationId);
    }

    _Function_class_(PRJ_END_DIRECTORY_ENUMERATION_CB)
        static HRESULT CALLBACK EndDirEnumCallback_C(
        _In_    const PRJ_CALLBACK_DATA* CallbackData,
        _In_    const GUID* EnumerationId
    )
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->EndDirEnum(CallbackData, EnumerationId);
    }

    _Function_class_(PRJ_GET_DIRECTORY_ENUMERATION_CB)
    static HRESULT CALLBACK GetDirEnumCallback_C(
        _In_        const PRJ_CALLBACK_DATA* CallbackData,
        _In_        const GUID* EnumerationId,
        _In_opt_    PCWSTR                      SearchExpression,
        _In_        PRJ_DIR_ENTRY_BUFFER_HANDLE DirEntryBufferHandle
    )
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->GetDirEnum(CallbackData,
            EnumerationId,
            SearchExpression,
            DirEntryBufferHandle);
    }

    _Function_class_(PRJ_GET_PLACEHOLDER_INFO_CB)
    static HRESULT CALLBACK GetPlaceholderInfoCallback_C(
        _In_    const PRJ_CALLBACK_DATA* CallbackData
    )
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->GetPlaceholderInfo(CallbackData);
    }

    _Function_class_(PRJ_GET_FILE_DATA_CB)
    static HRESULT CALLBACK GetFileDataCallback_C(
        _In_    const PRJ_CALLBACK_DATA* CallbackData,
        _In_    UINT64                      ByteOffset,
        _In_    UINT32                      Length
    )
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->GetFileData(CallbackData,
            ByteOffset,
            Length);
    }

    _Function_class_(PRJ_NOTIFICATION_CB)
    static HRESULT CALLBACK NotificationCallback_C(
        _In_        const PRJ_CALLBACK_DATA* CallbackData,
        _In_        BOOLEAN                         IsDirectory,
        _In_        PRJ_NOTIFICATION                NotificationType,
        _In_opt_    PCWSTR                          DestinationFileName,
        _Inout_     PRJ_NOTIFICATION_PARAMETERS* NotificationParameters
    )
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->Notify(CallbackData,
            IsDirectory,
            NotificationType,
            DestinationFileName,
            NotificationParameters);
    }

    _Function_class_(PRJ_QUERY_FILE_NAME_CB)
    static HRESULT CALLBACK QueryFileName_C(
        _In_    const PRJ_CALLBACK_DATA* CallbackData)
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        return pInstance->QueryFileName(CallbackData);
    }

    _Function_class_(PRJ_CANCEL_COMMAND_CB)
    static void CALLBACK CancelCommand_C(
        _In_    const PRJ_CALLBACK_DATA* CallbackData)
    {
        auto pInstance = reinterpret_cast<IProjectedFileSystemProvider*>(CallbackData->InstanceContext);
        pInstance->CancelCommand(CallbackData);
    }

    //! @brief Prepares a struct will all the callbacks
    static PRJ_CALLBACKS GetCallbacksInternal() 
    {
        PRJ_CALLBACKS stCB;
        stCB.StartDirectoryEnumerationCallback = StartDirEnumCallback_C;
        stCB.EndDirectoryEnumerationCallback = EndDirEnumCallback_C;
        stCB.GetDirectoryEnumerationCallback = GetDirEnumCallback_C;
        stCB.GetPlaceholderInfoCallback = GetPlaceholderInfoCallback_C;
        stCB.GetFileDataCallback = GetFileDataCallback_C;
        stCB.QueryFileNameCallback = QueryFileName_C;
        stCB.NotificationCallback = NotificationCallback_C;
        stCB.CancelCommandCallback = CancelCommand_C;

        return stCB;
    };

public:

    //! Gets persistent pointer to structure containing all ProjFS callbacks
    static const PRJ_CALLBACKS* GetCallbacks()
    {
        static const PRJ_CALLBACKS stCB = GetCallbacksInternal();
        return &stCB;
    }
};