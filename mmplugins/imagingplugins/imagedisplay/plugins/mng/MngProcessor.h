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

#ifndef __MNGPROCESSOR_H__

#define __MNGPROCESSOR_H__

/** 	@file
	@internalTechnology */

#include "ChunkProcessor.h"

class MInputStream;
class TMngChunk;
class CMngPlayer;

class CMngProcessor: public CChunkProcessor 
	{
public:
	static CMngProcessor* NewL(CMngPlayer& aObjectList, TBool aMngSubframesNoLoops);  //Subframes with No Loops
	~CMngProcessor();
// MChunkProcessor //	
	TBool OfferChunkType(const TChunkId& aChunkId);

	void Start(MInputStream& aInputStream, TRequestStatus& aRequest);

	TInt AddSubProcessor(MChunkProcessor& aSubProcessor);

protected:
	void DoRunL();

private:
	CMngProcessor(CMngPlayer& aMngPlayer, TBool aMngSubframesNoLoops);  //Subframes with No Loops

private:
	TBool  iMngSubframesNoLoops; //Subframes with No Loops
	TMngChunk*			iCurrentChunk;
	enum TState 
		{
		EReadMngSignature=0, 
		ECheckSignature, 
		EReadHeader, 
		EReadChunk, 
		EProcessStream
		}			iState;
	TMngSignature iSignature;
	};


#endif // ndef __MNGPROCESSOR_H__
