/*
 * Copyright (C) 2020 Zebediah Figura
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

#ifndef _WDFTIMER_H_
#define _WDFTIMER_H_

typedef void (WINAPI *PFN_WDF_TIMER)(WDFTIMER timer);

typedef struct _WDF_TIMER_CONFIG
{
    ULONG Size;
    PFN_WDF_TIMER EvtTimerFunc;
    ULONG Period;
    BOOLEAN AutomaticSerialization;
    ULONG TolerableDelay;
    BOOLEAN DECLSPEC_ALIGN(8) UseHighResolutionTimer;
} WDF_TIMER_CONFIG, *PWDF_TIMER_CONFIG;

#endif
