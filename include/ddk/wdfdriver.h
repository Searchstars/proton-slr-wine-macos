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

#ifndef _WDFDRIVER_H_
#define _WDFDRIVER_H_

typedef NTSTATUS (WINAPI EVT_WDF_DRIVER_DEVICE_ADD)(WDFDRIVER, PWDFDEVICE_INIT);
typedef EVT_WDF_DRIVER_DEVICE_ADD *PFN_WDF_DRIVER_DEVICE_ADD;

typedef void (WINAPI EVT_WDF_DRIVER_UNLOAD)(WDFDRIVER);
typedef EVT_WDF_DRIVER_UNLOAD *PFN_WDF_DRIVER_UNLOAD;

typedef enum _WDF_DRIVER_INIT_FLAGS
{
    WdfDriverInitNonPnpDriver       = 0x01,
    WdfDriverInitNoDispatchOverride = 0x02,
    WdfVerifyOn                     = 0x04,
    WdfVerifierOn                   = 0x08,
    WdfDriverInitCompanion          = 0x10,
} WDF_DRIVER_INIT_FLAGS;

typedef struct _WDF_DRIVER_CONFIG
{
    ULONG Size;
    PFN_WDF_DRIVER_DEVICE_ADD EvtDriverDeviceAdd;
    PFN_WDF_DRIVER_UNLOAD EvtDriverUnload;
    ULONG DriverInitFlags;
    ULONG DriverPoolTag;
} WDF_DRIVER_CONFIG, *PWDF_DRIVER_CONFIG;

#endif
