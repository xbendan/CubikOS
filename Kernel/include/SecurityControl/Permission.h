namespace SecurityControl
{
    typedef enum PermissionEnumeration
    {
        /*  */
        perm_CamaraTakePicture,
        perm_CamaraRecordVideo,
        perm_CamaraFlashlightTurnOn,
        perm_CamaraFlashlightTurnOff,
        perm_MicrophoneRecord,
        perm_Location_ALL
        perm_LocationGet,
        perm_LocationTurnOn,
        perm_LocationTurnOff,
        perm_Notification_ALL,
        perm_NotificationPushToast,
        perm_NotificationPushFloatDialog,
        perm_NotificationPushTopDialog,
        perm_NotificationReadList,
        perm_NotificationSilent,
        perm_NotificationClear,
        perm_QuietMode_ALL
        perm_QuietModeEnableOrDisable,
        perm_QuietModeSetTime,
        perm_Bluetooth_ALL
        perm_BluetoothEnableOrDisable,
        perm_BluetoothConnectOrDisconnectDevice,
        perm_BluetoothPairOrUnpairDevice,
        perm_BluetoothReadDeviceInformation,
    } permission_t;

    typedef class PermissionControlSet
    {

    } perm_set_t;

    
}