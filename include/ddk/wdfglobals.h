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

#ifndef _WDFGLOBALS_H_
#define _WDFGLOBALS_H_

#define WDF_DRIVER_GLOBALS_NAME_LEN (32)

typedef struct _WDF_DRIVER_GLOBALS
{
    WDFDRIVER Driver;
    ULONG DriverFlags;
    ULONG DriverTag;
    char DriverName[WDF_DRIVER_GLOBALS_NAME_LEN];
    BOOLEAN DisplaceDriverUnload;
} WDF_DRIVER_GLOBALS, *PWDF_DRIVER_GLOBALS;

#endif
