/*
 * Copyright (C) 2020 Zebediah Figura for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef _WDFDEVICE_H_
#define _WDFDEVICE_H_

typedef enum _WDF_POWER_DEVICE_STATE
{
    WdfPowerDeviceInvalid = 0,
    WdfPowerDeviceD0 = 1,
    WdfPowerDeviceD1 = 2,
    WdfPowerDeviceD2 = 3,
    WdfPowerDeviceD3 = 4,
    WdfPowerDeviceD3Final = 5,
    WdfPowerDevicePrepareForHibernation = 6,
    WdfPowerDeviceMaximum = 7,
} WDF_POWER_DEVICE_STATE, *PWDF_POWER_DEVICE_STATE;

typedef enum _WDF_SPECIAL_FILE_TYPE
{
    WdfSpecialFileUndefined = 0,
    WdfSpecialFilePaging = 1,
    WdfSpecialFileHibernation = 2,
    WdfSpecialFileDump = 3,
    WdfSpecialFileBoot = 4,
    WdfSpecialFileMax = 5,
} WDF_SPECIAL_FILE_TYPE, *PWDF_SPECIAL_FILE_TYPE;

typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_D0_ENTRY)(WDFDEVICE device, WDF_POWER_DEVICE_STATE prev_state);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_D0_ENTRY_POST_INTERRUPTS_ENABLED)(WDFDEVICE device, WDF_POWER_DEVICE_STATE prev_state);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_D0_EXIT)(WDFDEVICE device, WDF_POWER_DEVICE_STATE prev_state);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_D0_EXIT_PRE_INTERRUPTS_DISABLED)(WDFDEVICE device, WDF_POWER_DEVICE_STATE prev_state);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_PREPARE_HARDWARE)(WDFDEVICE device, WDFCMRESLIST raw_resources, WDFCMRESLIST translated_resources);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_QUERY_REMOVE)(WDFDEVICE device);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_QUERY_STOP)(WDFDEVICE device);
typedef void (WINAPI *PFN_WDF_DEVICE_RELATIONS_QUERY)(WDFDEVICE device, DEVICE_RELATION_TYPE type);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_RELEASE_HARDWARE)(WDFDEVICE device, WDFCMRESLIST translated_resources);
typedef void (WINAPI *PFN_WDF_DEVICE_SELF_MANAGED_IO_CLEANUP)(WDFDEVICE device);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_SELF_MANAGED_IO_FLUSH)(WDFDEVICE device);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_SELF_MANAGED_IO_INIT)(WDFDEVICE device);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_SELF_MANAGED_IO_RESTART)(WDFDEVICE device);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND)(WDFDEVICE device);
typedef void (WINAPI *PFN_WDF_DEVICE_SURPRISE_REMOVAL)(WDFDEVICE device);
typedef void (WINAPI *PFN_WDF_DEVICE_USAGE_NOTIFICATION)(WDFDEVICE device, WDF_SPECIAL_FILE_TYPE type, BOOLEAN in_path);
typedef NTSTATUS (WINAPI *PFN_WDF_DEVICE_USAGE_NOTIFICATION_EX)(WDFDEVICE device, WDF_SPECIAL_FILE_TYPE type, BOOLEAN in_path);

typedef struct _WDF_PNPPOWER_EVENT_CALLBACKS
{
    ULONG Size;
    PFN_WDF_DEVICE_D0_ENTRY EvtDeviceD0Entry;
    PFN_WDF_DEVICE_D0_ENTRY_POST_INTERRUPTS_ENABLED EvtDeviceD0EntryPostInterruptsEnabled;
    PFN_WDF_DEVICE_D0_EXIT EvtDeviceD0Exit;
    PFN_WDF_DEVICE_D0_EXIT_PRE_INTERRUPTS_DISABLED EvtDeviceD0ExitPreInterruptsDisabled;
    PFN_WDF_DEVICE_PREPARE_HARDWARE EvtDevicePrepareHardware;
    PFN_WDF_DEVICE_RELEASE_HARDWARE EvtDeviceReleaseHardware;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_CLEANUP EvtDeviceSelfManagedIoCleanup;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_FLUSH EvtDeviceSelfManagedIoFlush;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_INIT EvtDeviceSelfManagedIoInit;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND EvtDeviceSelfManagedIoSuspend;
    PFN_WDF_DEVICE_SELF_MANAGED_IO_RESTART EvtDeviceSelfManagedIoRestart;
    PFN_WDF_DEVICE_SURPRISE_REMOVAL EvtDeviceSurpriseRemoval;
    PFN_WDF_DEVICE_QUERY_REMOVE EvtDeviceQueryRemove;
    PFN_WDF_DEVICE_QUERY_STOP EvtDeviceQueryStop;
    PFN_WDF_DEVICE_USAGE_NOTIFICATION EvtDeviceUsageNotification;
    PFN_WDF_DEVICE_RELATIONS_QUERY EvtDeviceRelationsQuery;
    PFN_WDF_DEVICE_USAGE_NOTIFICATION_EX EvtDeviceUsageNotificationEx;
} WDF_PNPPOWER_EVENT_CALLBACKS, *PWDF_PNPPOWER_EVENT_CALLBACKS;

typedef enum _WDF_FILEOBJECT_CLASS
{
    WdfFileObjectInvalid = 0,
    WdfFileObjectNotRequired = 1,
    WdfFileObjectWdfCanUseFsContext = 2,
    WdfFileObjectWdfCanUseFsContext2 = 3,
    WdfFileObjectWdfCannotUseFsContexts = 4,
    WdfFileObjectCanBeOptional = 0x80000000,
} WDF_FILEOBJECT_CLASS, *PWDF_FILEOBJECT_CLASS;

typedef void (WINAPI *PFN_WDF_DEVICE_FILE_CREATE)(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file);
typedef void (WINAPI *PFN_WDF_FILE_CLEANUP)(WDFFILEOBJECT file);
typedef void (WINAPI *PFN_WDF_FILE_CLOSE)(WDFFILEOBJECT file);

typedef struct _WDF_FILEOBJECT_CONFIG
{
    ULONG Size;
    PFN_WDF_DEVICE_FILE_CREATE EvtDeviceFileCreate;
    PFN_WDF_FILE_CLOSE EvtFileClose;
    PFN_WDF_FILE_CLEANUP EvtFileCleanup;
    WDF_TRI_STATE AutoForwardCleanupClose;
    WDF_FILEOBJECT_CLASS FileObjectClass;
} WDF_FILEOBJECT_CONFIG, *PWDF_FILEOBJECT_CONFIG;

typedef enum _WDF_DEVICE_IO_TYPE
{
    WdfDeviceIoUndefined = 0,
    WdfDeviceIoNeither = 1,
    WdfDeviceIoBuffered = 2,
    WdfDeviceIoDirect = 3,
    WdfDeviceIoBufferedOrDirect = 4,
    WdfDeviceIoMaximum = 5,
} WDF_DEVICE_IO_TYPE, *PWDF_DEVICE_IO_TYPE;

typedef NTSTATUS (WINAPI *PFN_WDFDEVICE_WDM_IRP_PREPROCESS)(WDFDEVICE device, IRP *irp);

#endif
