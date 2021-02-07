/*
 * Windows Device Framework private and internal definitions
 *
 * Copyright 2020 Zebediah Figura for CodeWeavers
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

#ifndef __WINE_WDFLDR_PRIVATE_H
#define __WINE_WDFLDR_PRIVATE_H

typedef ULONG WDF_MAJOR_VERSION, WDF_MINOR_VERSION, WDF_BUILD_NUMBER;
typedef WDF_DRIVER_GLOBALS WDF_COMPONENT_GLOBALS;

typedef struct _WDF_VERSION
{
    WDF_MAJOR_VERSION Major;
    WDF_MINOR_VERSION Minor;
    WDF_BUILD_NUMBER Build;
} WDF_VERSION;

typedef struct _WDF_BIND_INFO
{
    ULONG Size;
    WCHAR *Component;
    WDF_VERSION Version;
    ULONG FuncCount;
    WDFFUNC *FuncTable;
    void *Module;
} WDF_BIND_INFO;

#define DECLARE_CRITICAL_SECTION(cs) \
    static CRITICAL_SECTION cs; \
    static CRITICAL_SECTION_DEBUG cs##_debug = \
    { 0, 0, &cs, { &cs##_debug.ProcessLocksList, &cs##_debug.ProcessLocksList }, \
      0, 0, { (DWORD_PTR)(__FILE__ ": " # cs) }}; \
    static CRITICAL_SECTION cs = { &cs##_debug, -1, 0, 0, 0, 0 };

struct wdf_object;

typedef void (*wdf_object_destroy_cb)(struct wdf_object *object);

struct wdf_object
{
    LONG refcount;
    wdf_object_destroy_cb destroy;
    struct wdf_object *parent;
    struct list children;
    struct list child_entry;
    const WDF_OBJECT_CONTEXT_TYPE_INFO *type;
    void *ctx;
};

#endif
