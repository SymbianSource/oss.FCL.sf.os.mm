// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MNGPANIC_H__

#define __MNGPANIC_H__

/** 	@file
	@internalTechnology */

enum TMngPanicCategory 
	{
	EPanicAlreadyStarted=1,			// 1
	EPanicAlreadyCreated=2,			// 2
	EPanicObserverIsBusy=3,			// 3
	EPanicNotImplementedYet=4,		// 4
	EPanicWrongChunkIdLength=5,		// 5
	EPanicNotInitialized=6,			// 6
	EPanicInvalidFrameNumber=7,		// 7
	};

void MngPanic(TMngPanicCategory aPanicCat);


#endif // ndef __MNGPANIC_H__
