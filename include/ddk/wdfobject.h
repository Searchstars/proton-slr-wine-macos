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

#ifndef _WDFOBJECT_H_
#define _WDFOBJECT_H_

typedef enum _WDF_EXECUTION_LEVEL
{
    WdfExecutionLevelInvalid = 0,
    WdfExecutionLevelInheritFromParent,
    WdfExecutionLevelPassive,
    WdfExecutionLevelDispatch,
} WDF_EXECUTION_LEVEL;

typedef enum _WDF_SYNCHRONIZATION_SCOPE
{
    WdfSynchronizationScopeInvalid = 0,
    WdfSynchronizationScopeInheritFromParent,
    WdfSynchronizationScopeDevice,
    WdfSynchronizationScopeQueue,
    WdfSynchronizationScopeNone,
} WDF_SYNCHRONIZATION_SCOPE;

typedef struct _WDF_OBJECT_CONTEXT_TYPE_INFO WDF_OBJECT_CONTEXT_TYPE_INFO, *PWDF_OBJECT_CONTEXT_TYPE_INFO;
typedef const WDF_OBJECT_CONTEXT_TYPE_INFO *PCWDF_OBJECT_CONTEXT_TYPE_INFO;

typedef void (WINAPI EVT_WDF_OBJECT_CONTEXT_CLEANUP)(WDFOBJECT);
typedef EVT_WDF_OBJECT_CONTEXT_CLEANUP *PFN_WDF_OBJECT_CONTEXT_CLEANUP;

typedef void (WINAPI EVT_WDF_OBJECT_CONTEXT_DESTROY)(WDFOBJECT);
typedef EVT_WDF_OBJECT_CONTEXT_DESTROY *PFN_WDF_OBJECT_CONTEXT_DESTROY;

typedef const WDF_OBJECT_CONTEXT_TYPE_INFO *(__cdecl *PFN_GET_UNIQUE_CONTEXT_TYPE)(void);

struct _WDF_OBJECT_CONTEXT_TYPE_INFO
{
    ULONG Size;
    char *ContextName;
    size_t ContextSize;
    const struct _WDF_OBJECT_CONTEXT_TYPE_INFO *UniqueType;
    PFN_GET_UNIQUE_CONTEXT_TYPE EvtDriverGetUniqueContextType;
};

typedef struct _WDF_OBJECT_ATTRIBUTES
{
    ULONG Size;
    PFN_WDF_OBJECT_CONTEXT_CLEANUP EvtCleanupCallback;
    PFN_WDF_OBJECT_CONTEXT_DESTROY EvtDestroyCallback;
    WDF_EXECUTION_LEVEL ExecutionLevel;
    WDF_SYNCHRONIZATION_SCOPE SynchronizationScope;
    WDFOBJECT ParentObject;
    size_t ContextSizeOverride;
    const WDF_OBJECT_CONTEXT_TYPE_INFO *ContextTypeInfo;
} WDF_OBJECT_ATTRIBUTES, *PWDF_OBJECT_ATTRIBUTES;

#endif
