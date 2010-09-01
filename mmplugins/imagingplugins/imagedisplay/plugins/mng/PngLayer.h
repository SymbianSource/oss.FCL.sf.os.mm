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

#ifndef __PNGLAYER_H__

#define __PNGLAYER_H__
/** 	@file
	@internalTechnology */

#include "Providers.h"
#include "MngObjects.h"
#include "PNGCodec.h"

class MInputStream;
class MImageStreamDecoder;

class CPngLayer: public CMngLayer 
	{
public:
	static CPngLayer* NewL();
	~CPngLayer();

	void AppendChunkL(const TMNGChunkHeader& aHeader,MInputStream& aInpStream);
	void ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& aResult);
	
protected:
	CBufFlat*				iPngBuffer;
	TInt					iDataSize;
	MImageStreamDecoder*	iPngCodec;
	};


#endif // ndef __PNGLAYER_H__
