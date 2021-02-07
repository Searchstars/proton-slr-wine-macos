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

#ifndef _WDFIO_H_
#define _WDFIO_H_

typedef enum _WDF_IO_QUEUE_DISPATCH_TYPE
{
    WdfIoQueueDispatchInvalid = 0,
    WdfIoQueueDispatchSequential,
    WdfIoQueueDispatchParallel,
    WdfIoQueueDispatchManual,
    WdfIoQueueDispatchMax,
} WDF_IO_QUEUE_DISPATCH_TYPE;

typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_CANCELED_ON_QUEUE)(WDFQUEUE queue, WDFREQUEST request);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_DEFAULT)(WDFQUEUE queue, WDFREQUEST request);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_DEVICE_CONTROL)(WDFQUEUE queue, WDFREQUEST request, size_t out_len, size_t in_len, ULONG code);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL)(WDFQUEUE queue, WDFREQUEST request, size_t out_len, size_t in_len, ULONG code);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_READ)(WDFQUEUE queue, WDFREQUEST request, size_t len);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_RESUME)(WDFQUEUE queue, WDFREQUEST request);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_STOP)(WDFQUEUE queue, WDFREQUEST request, ULONG flags);
typedef void (WINAPI *PFN_WDF_IO_QUEUE_IO_WRITE)(WDFQUEUE queue, WDFREQUEST request, size_t len);

typedef struct _WDF_IO_QUEUE_CONFIG
{
    ULONG Size;
    WDF_IO_QUEUE_DISPATCH_TYPE DispatchType;
    WDF_TRI_STATE PowerManaged;
    BOOLEAN AllowZeroLengthRequests;
    BOOLEAN DefaultQueue;
    PFN_WDF_IO_QUEUE_IO_DEFAULT EvtIoDefault;
    PFN_WDF_IO_QUEUE_IO_READ EvtIoRead;
    PFN_WDF_IO_QUEUE_IO_WRITE EvtIoWrite;
    PFN_WDF_IO_QUEUE_IO_DEVICE_CONTROL EvtIoDeviceControl;
    PFN_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL EvtIoInternalDeviceControl;
    PFN_WDF_IO_QUEUE_IO_STOP EvtIoStop;
    PFN_WDF_IO_QUEUE_IO_RESUME EvtIoResume;
    PFN_WDF_IO_QUEUE_IO_CANCELED_ON_QUEUE EvtIoCanceledOnQueue;
    union
    {
        struct
        {
            ULONG NumberOfPresentedRequests;
        } Parallel;
    } Settings;
    WDFDRIVER Driver;
} WDF_IO_QUEUE_CONFIG, *PWDF_IO_QUEUE_CONFIG;

#endif
