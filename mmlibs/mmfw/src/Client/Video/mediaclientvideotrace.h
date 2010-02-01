// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// 
//


#ifndef MEDIACLIENTVIDEOTRACE_H_
#define MEDIACLIENTVIDEOTRACE_H_

#ifdef __ENABLE_MEDIA_CLIENT_VIDEO_TRACE__

#include <e32debug.h>

#define DEBUG_PRINT(a) RDebug::Print(a)
#define DEBUG_PRINT2(a, b) RDebug::Print(a, b)

#define DEBUG_PRINTF(a) RDebug::Printf(a)
#define DEBUG_PRINTF2(a,b) RDebug::Printf(a,b)
#define DEBUG_PRINTF3(a,b,c) RDebug::Printf(a,b,c)
#define DEBUG_PRINTF4(a,b,c,d) RDebug::Printf(a,b,c,d)
#define DEBUG_PRINTF5(a, b, c, d, e) RDebug::Printf(a,b,c,d,e)

#else // __ENABLE_MEDIA_CLIENT_VIDEO_TRACE__

#define DEBUG_PRINT(a)
#define DEBUG_PRINT2(a, b)

#define DEBUG_PRINTF(a)
#define DEBUG_PRINTF2(a, b)
#define DEBUG_PRINTF3(a, b, c)
#define DEBUG_PRINTF4(a, b, c, d)
#define DEBUG_PRINTF5(a, b, c, d, e)

#endif // __ENABLE_MEDIA_CLIENT_VIDEO_TRACE__

#endif // MEDIACLIENTVIDEOTRACE_H_
