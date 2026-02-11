/*
 * WoW64 rtl helpers for 32-bit PE build
 *
 * Copyright 2026
 */

#include <stdarg.h>

#include "ntstatus.h"
#define WIN32_NO_STATUS
#include "windef.h"
#include "winbase.h"
#include "winternl.h"

#include "wow64_private.h"

#ifndef _WIN64

/******************************************************************************
 *              RtlWow64GetThreadContext
 */
NTSTATUS WINAPI RtlWow64GetThreadContext( HANDLE handle, WOW64_CONTEXT *context )
{
    return NtQueryInformationThread( handle, ThreadWow64Context, context, sizeof(*context), NULL );
}

/******************************************************************************
 *              RtlWow64SetThreadContext
 */
NTSTATUS WINAPI RtlWow64SetThreadContext( HANDLE handle, const WOW64_CONTEXT *context )
{
    return NtSetInformationThread( handle, ThreadWow64Context, context, sizeof(*context) );
}

/******************************************************************************
 *              RtlWow64GetThreadSelectorEntry
 */
NTSTATUS WINAPI RtlWow64GetThreadSelectorEntry( HANDLE handle, THREAD_DESCRIPTOR_INFORMATION *info,
                                                ULONG size, ULONG *retlen )
{
    DWORD sel;
    WOW64_CONTEXT context = { WOW64_CONTEXT_CONTROL | WOW64_CONTEXT_SEGMENTS };
    LDT_ENTRY entry = { 0 };

    if (size != sizeof(*info)) return STATUS_INFO_LENGTH_MISMATCH;
    if (RtlWow64GetThreadContext( handle, &context ))
    {
        /* hardcoded values */
#ifdef __arm64ec__
        context.SegCs = 0x23;
        context.SegSs = 0x2b;
        context.SegFs = 0x53;
#elif defined(__x86_64__)
        context.SegCs = 0x23;
        __asm__( "movw %%fs,%0" : "=m" (context.SegFs) );
        __asm__( "movw %%ss,%0" : "=m" (context.SegSs) );
#else
        context.SegCs = 0x1b;
        context.SegSs = 0x23;
        context.SegFs = 0x3b;
#endif
    }

    sel = info->Selector | 3;
    if (sel == 0x03) goto done; /* null selector */

    /* set common data */
    entry.HighWord.Bits.Dpl = 3;
    entry.HighWord.Bits.Pres = 1;
    entry.HighWord.Bits.Default_Big = 1;
    if (sel == context.SegCs)  /* code selector */
    {
        entry.LimitLow = 0xffff;
        entry.HighWord.Bits.LimitHi = 0xf;
        entry.HighWord.Bits.Type = 0x1b;  /* code */
        entry.HighWord.Bits.Granularity = 1;
    }
    else if (sel == context.SegSs)  /* data selector */
    {
        entry.LimitLow = 0xffff;
        entry.HighWord.Bits.LimitHi = 0xf;
        entry.HighWord.Bits.Type = 0x13;  /* data */
        entry.HighWord.Bits.Granularity = 1;
    }
    else if (sel == context.SegFs)  /* TEB selector */
    {
        THREAD_BASIC_INFORMATION tbi;

        entry.LimitLow = 0xfff;
        entry.HighWord.Bits.Type = 0x13;  /* data */
        if (!NtQueryInformationThread( handle, ThreadBasicInformation, &tbi, sizeof(tbi), NULL ))
        {
            ULONG addr = (ULONG_PTR)tbi.TebBaseAddress + 0x2000;  /* 32-bit teb offset */
            entry.BaseLow = addr;
            entry.HighWord.Bytes.BaseMid = addr >> 16;
            entry.HighWord.Bytes.BaseHi  = addr >> 24;
        }
    }
    else return STATUS_UNSUCCESSFUL;

done:
    info->Entry = entry;
    if (retlen) *retlen = sizeof(entry);
    return STATUS_SUCCESS;
}

/**********************************************************************
 *           RtlOpenCrossProcessEmulatorWorkConnection
 */
void WINAPI RtlOpenCrossProcessEmulatorWorkConnection( HANDLE process, HANDLE *section, void **addr )
{
    WOW64INFO wow64info;
    BOOLEAN is_wow64;
    HANDLE handle = 0;
    SIZE_T size = 0;

    *addr = NULL;
    *section = 0;

    if (RtlIsCurrentProcess( process )) return;
    if (RtlWow64GetSharedInfoProcess( process, &is_wow64, &wow64info )) return;

    if (is_wow64) handle = (HANDLE)(ULONG_PTR)wow64info.SectionHandle;
#if defined __aarch64__ || defined __arm64ec__
    else
    {
        PROCESS_BASIC_INFORMATION basic;
        CHPEV2_PROCESS_INFO info, *info_ptr;

        if (!NtQueryInformationProcess( process, ProcessBasicInformation, &basic, sizeof(basic), NULL ) &&
            !NtReadVirtualMemory( process, (char *)basic.PebBaseAddress + offsetof(PEB, ChpeV2ProcessInfo),
                                  &info_ptr, sizeof(info_ptr), NULL ) &&
            !NtReadVirtualMemory( process, info_ptr, &info, sizeof(info), NULL ))
            handle = info.SectionHandle;
    }
#endif

    if (!handle) return;
    if (NtDuplicateObject( process, handle, GetCurrentProcess(), section, 0, 0, DUPLICATE_SAME_ACCESS ))
        return;

    if (!NtMapViewOfSection( *section, GetCurrentProcess(), addr, 0, 0, NULL,
                             &size, ViewShare, 0, PAGE_READWRITE )) return;

    NtClose( *section );
    *section = 0;
}

/**********************************************************************
 *           RtlWow64PopAllCrossProcessWorkFromWorkList
 */
CROSS_PROCESS_WORK_ENTRY * WINAPI RtlWow64PopAllCrossProcessWorkFromWorkList( CROSS_PROCESS_WORK_HDR *list, BOOLEAN *flush )
{
    CROSS_PROCESS_WORK_HDR prev, new;
    UINT pos, prev_pos = 0;

    do
    {
        prev.hdr = list->hdr;
        if (!prev.first) break;
        new.first = 0;
        new.counter = prev.counter + 1;
    } while (InterlockedCompareExchange64( &list->hdr, new.hdr, prev.hdr ) != prev.hdr);

    *flush = (prev.first & CROSS_PROCESS_LIST_FLUSH) != 0;
    if (!(pos = prev.first & ~CROSS_PROCESS_LIST_FLUSH)) return NULL;

    /* reverse the list */
    for (;;)
    {
        CROSS_PROCESS_WORK_ENTRY *entry = CROSS_PROCESS_LIST_ENTRY( list, pos );
        UINT next = entry->next;
        entry->next = prev_pos;
        if (!next) return entry;
        prev_pos = pos;
        pos = next;
    }
}

/**********************************************************************
 *           RtlWow64PopCrossProcessWorkFromFreeList
 */
CROSS_PROCESS_WORK_ENTRY * WINAPI RtlWow64PopCrossProcessWorkFromFreeList( CROSS_PROCESS_WORK_HDR *list )
{
    CROSS_PROCESS_WORK_ENTRY *ret;
    CROSS_PROCESS_WORK_HDR prev, new;

    do
    {
        prev.hdr = list->hdr;
        if (!prev.first) return NULL;
        ret = CROSS_PROCESS_LIST_ENTRY( list, prev.first );
        new.first = ret->next;
        new.counter = prev.counter + 1;
    } while (InterlockedCompareExchange64( &list->hdr, new.hdr, prev.hdr ) != prev.hdr);

    ret->next = 0;
    return ret;
}

/**********************************************************************
 *           RtlWow64PushCrossProcessWorkOntoFreeList
 */
BOOLEAN WINAPI RtlWow64PushCrossProcessWorkOntoFreeList( CROSS_PROCESS_WORK_HDR *list, CROSS_PROCESS_WORK_ENTRY *entry )
{
    CROSS_PROCESS_WORK_HDR prev, new;

    do
    {
        prev.hdr = list->hdr;
        entry->next = prev.first;
        new.first = (char *)entry - (char *)list;
        new.counter = prev.counter + 1;
    } while (InterlockedCompareExchange64( &list->hdr, new.hdr, prev.hdr ) != prev.hdr);

    return TRUE;
}

/**********************************************************************
 *           RtlWow64PushCrossProcessWorkOntoWorkList
 */
BOOLEAN WINAPI RtlWow64PushCrossProcessWorkOntoWorkList( CROSS_PROCESS_WORK_HDR *list, CROSS_PROCESS_WORK_ENTRY *entry,
                                                          void **unknown )
{
    CROSS_PROCESS_WORK_HDR prev, new;

    *unknown = NULL;
    do
    {
        prev.hdr = list->hdr;
        entry->next = prev.first;
        new.first = ((char *)entry - (char *)list) | (prev.first & CROSS_PROCESS_LIST_FLUSH);
        new.counter = prev.counter + 1;
    } while (InterlockedCompareExchange64( &list->hdr, new.hdr, prev.hdr ) != prev.hdr);

    return TRUE;
}

#endif /* !_WIN64 */
