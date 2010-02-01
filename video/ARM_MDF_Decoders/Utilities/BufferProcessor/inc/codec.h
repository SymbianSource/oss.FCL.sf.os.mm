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

#ifndef __CODEC_H
#define __CODEC_H

#include <e32def.h>

class MBaseCodec
	{
	public:
		enum TCodecState {
			EInputConsumed,
			EOutputConsumed,
			EConsumed,
			ENotConsumed,
			EBadOutputBuffer,
			EBadInputBuffer
			};
	public:// Virtual functions
		// For initializing the codec 
		virtual void InitializeL () = 0;
		//For processing the required and returns one of the TCodecState
		virtual TInt DoProcessL ( TAny *aInpBuf, TAny* aOutBuf = NULL ) = 0;
		//For deinitializing the codec
		virtual void DeInitialize () = 0;
	// Reset the codec
		virtual void Reset () = 0;
	};

#endif //__CODEC_H