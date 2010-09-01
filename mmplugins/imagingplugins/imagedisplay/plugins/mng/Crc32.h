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

#ifndef __CRC32_H__

#define __CRC32_H__
/** 	@file
	@internalTechnology */

class Crc32 
	{
public:

static void Calc(TInt32& aCrc, const TAny* aSrc, TInt aLength);

	};


#endif //ndef __CRC32_H__
