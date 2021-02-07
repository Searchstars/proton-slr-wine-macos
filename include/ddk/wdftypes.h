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

#ifndef _WDFTYPES_H_
#define _WDFTYPES_H_

typedef HANDLE WDFOBJECT, *PWDFOBJECT;
DECLARE_HANDLE(WDFCHILDLIST);
DECLARE_HANDLE(WDFCMRESLIST);
DECLARE_HANDLE(WDFDEVICE);
DECLARE_HANDLE(WDFDRIVER);
DECLARE_HANDLE(WDFFILEOBJECT);
DECLARE_HANDLE(WDFKEY);
DECLARE_HANDLE(WDFQUEUE);
DECLARE_HANDLE(WDFREQUEST);
DECLARE_HANDLE(WDFSPINLOCK);
DECLARE_HANDLE(WDFSTRING);
DECLARE_HANDLE(WDFTIMER);

typedef struct WDFDEVICE_INIT *PWDFDEVICE_INIT;

typedef enum _WDF_TRI_STATE
{
    WdfFalse = FALSE,
    WdfTrue = TRUE,
    WdfUseDefault = 2,
} WDF_TRI_STATE, *PWDF_TRI_STATE;

#endif
