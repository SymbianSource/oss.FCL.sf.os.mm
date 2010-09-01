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

#ifndef __PNGPROCESSOR_H__

#define __PNGPROCESSOR_H__
/** 	@file
	@internalTechnology */

#include "ChunkProcessor.h"

class MInputStream;
class TMngChunk;
class CMngPlayer;
class CPngLayer;

class CPngProcessor: public CChunkProcessor 
	{
public:
	static CPngProcessor* NewL(CMngPlayer& aPlayer);
	~CPngProcessor();
// MChunkProcessor //	
	TBool OfferChunkType(const TChunkId& aChunkId);

	void Start(MInputStream& aInputStream, TRequestStatus& aRequest);

	TInt AddSubProcessor(MChunkProcessor& aSubProcessor);

protected:
	void DoRunL();
	
private:
	CPngProcessor(CMngPlayer& aPlayer);

private:
	TMNGChunkHeader 	iCurrentChunkHeader;
	CPngLayer*			iCurrentLayer;
	enum TState 
		{
		ECreateLayer=0, 
		EReadHeader, 
		EReadChunk
		}				iState;
	};


#endif // ndef __PNGPROCESSOR_H__
