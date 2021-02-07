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

#ifndef _WDF_H_
#define _WDF_H_

typedef void (WINAPI *WDFFUNC)(void);

#include "wdftypes.h"

#include "wdfchildlist.h"
#include "wdfdevice.h"
#include "wdfdriver.h"
#include "wdffuncenum.h"
#include "wdfglobals.h"
#include "wdfio.h"
#include "wdfobject.h"
#include "wdftimer.h"

#endif
