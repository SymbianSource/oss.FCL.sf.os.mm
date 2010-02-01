/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#ifndef __RDEBUG_H__
#define __RDEBUG_H__

#include <e32svr.h>

#ifdef _DEBUG

	#define RDP(s) //RDebug::Print(s)
	#define RDP1(s,x) RDebug::Print(s,x)
	#define RDP2(s,x,y) //RDebug::Print(s,x,y)
	#define RDP3(s) RDebug::Print(s)
#else

	#define RDP(s) 
	#define RDP1(s,x) 
	#define RDP2(s,x,y)
	#define RDP3(s)

#endif // _DEBUG		 

#endif // __RDEBUG_H__ 
