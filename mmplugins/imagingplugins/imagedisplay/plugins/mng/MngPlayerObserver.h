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

#ifndef __MNGPLAYEROBSERVER_H__

#define __MNGPLAYEROBSERVER_H__

/** 	@file
	@internalTechnology */

class MMngPlayerObserver 
	{
public:
	virtual void OnDecodeEvent(TUint aEventFlags, TInt aErrorCode)=0;
	};


#endif // ndef __MNGPLAYEROBSERVER_H__
