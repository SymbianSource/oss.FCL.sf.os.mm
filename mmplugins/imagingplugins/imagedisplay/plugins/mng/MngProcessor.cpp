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

/** 	@file
	@internalTechnology */
#include "MngPlayer.h"
#include "MngPanic.h"
#include "logger.h"

#include "MngProcessor.h"

class MngChunkFactory 
	{
public:
	static	TMngChunk*	CreateChunkObjectL(const TMNGChunkHeader& aHeader);
	};

/*static*/
TMngChunk* MngChunkFactory::CreateChunkObjectL(const TMNGChunkHeader& aHeader)
	{
	TMngChunk* pChunk=NULL;
	switch (aHeader.iChunkId.iChunkIdInt)
		{
		case KMhdrChunkId:
			pChunk=new (ELeave) TMhdrChunk(aHeader);
			break;
		case KDefiChunkId:
			pChunk=new (ELeave) TDefiChunk(aHeader);
			break;
		case KTermChunkId:
			pChunk=new (ELeave) TTermChunk(aHeader);
			break;
		case KBackChunkId:
			pChunk=new (ELeave) TBackChunk(aHeader);
			break;
		case KMagnChunkId:
			pChunk=new (ELeave) TMagnChunk(aHeader);
			break;
		case KFramChunkId:
			pChunk=new (ELeave) TFramChunk(aHeader);
			break;
		case KLoopChunkId:
			pChunk=new (ELeave) TLoopChunk(aHeader);
			break;
		case KEndlChunkId:
			pChunk=new (ELeave) TEndlChunk(aHeader);
			break;

		case KMendChunkId:
		case KPlteChunkId:
		case KTrnsChunkId:
		case KBkgdChunkId:
		case KPhysChunkId:
			pChunk=TMngRawChunk::CreateL(aHeader);
			break;
			
		//With MNG_LC we do not support the following chunks
		case KPastChunkId:
		case KBasiChunkId:
		case KMoveChunkId:
		case KGamaChunkId:
			User::Leave(KErrNotSupported);
			break;
			
		default:
			// Unknown chunk
			pChunk = NULL;
		}
	return pChunk;
	}


/*static*/
CMngProcessor* CMngProcessor::NewL(CMngPlayer& aMngPlayer, TBool aMngSubframesNoLoops)  //Subframes with No Loops
	{
	CMngProcessor* self=new (ELeave) CMngProcessor(aMngPlayer, aMngSubframesNoLoops); //Subframes with No Loops
	return self;
	}

CMngProcessor::CMngProcessor(CMngPlayer& aMngPlayer, TBool aMngSubframesNoLoops):  //Subframes with No Loops
							           		CChunkProcessor(aMngPlayer),
							           		iMngSubframesNoLoops(aMngSubframesNoLoops)  //Subframes with No Loops
	{
	}

CMngProcessor::~CMngProcessor()
	{
	delete iCurrentChunk;
	iCurrentChunk = NULL;
	Stop();
	}

void CMngProcessor::DoRunL()
	{
	switch (iState)
		{
		case EReadMngSignature:
			iInputStream->WaitForData(KMngSignatureSize, iStatus);
			iState=TState(iState+1);
			SetActive();
			break;

		case ECheckSignature:
			iInputStream->ReadL(iSignature);
			if (0 != iSignature.Compare(KMngSignature))
				{
				User::Leave(KErrCorrupt);
				}
			iState=TState(iState+1);
			RunAgain();
			break;

		case EReadHeader:
			delete iCurrentChunk;
			iCurrentChunk = NULL;
			if (iInputStream->DataAvailable() < TMNGChunkHeader::RequiredData() )
				{
				iInputStream->WaitForData(TMNGChunkHeader::RequiredData(), iStatus);
				SetActive();
				break;
				}
			else 
				{
				TMNGChunkHeader Header;
				Header.ReadL(*iInputStream);
				LOG3("Chunk %4s, len=%d", Header.iChunkId.iChunkIdChr, Header.iLength);
				iCurrentChunk=MngChunkFactory::CreateChunkObjectL(Header);
				if (NULL == iCurrentChunk) //unknown one 
					{
					if (NULL != iSubProcessor && iSubProcessor->OfferChunkType(Header.iChunkId))
						{
						iInputStream->SeekL(iInputStream->Tell() - TMNGChunkHeader::RequiredData());
						iSubProcessor->Start(*iInputStream, iStatus);
						SetActive();
						iState=EReadHeader;
						break;
						}
					else
						{
						iCurrentChunk = new (ELeave) TUnknownChunk(Header);
						}
					}
				}
			// go further for reading the chunk body
		case EReadChunk:
			iState=EReadChunk;
			if (iInputStream->DataAvailable() < iCurrentChunk->RequiredData() )
				{
				iInputStream->WaitForData(iCurrentChunk->RequiredData(), iStatus);
				SetActive();
				break;
				}
			else
				{
				iCurrentChunk->ReadL(*iInputStream);
				switch (iCurrentChunk->iHeader.iChunkId.iChunkIdInt)
					{
					case KMhdrChunkId:
						iMngPlayer.InitL(reinterpret_cast<TMhdrChunk*&>(iCurrentChunk));
						iState = EReadHeader;
						RunAgain();
						break;
					case KTermChunkId:
						{
						CMngTermination* pTerm=CMngTermination::NewL(reinterpret_cast<TTermChunk*&>(iCurrentChunk), iMngSubframesNoLoops); //Subframes with No Loops
						iMngPlayer.SetTerminationCtrlL(pTerm);
						iState = EReadHeader;
						RunAgain();
						}

						break;
					case KMendChunkId:
						NotifyCaller(KErrNone);
						delete iCurrentChunk;
						iCurrentChunk = NULL;
						break;
					
					//Subframes with No Loops
					case KLoopChunkId:  //deliberate fall through to default case
					case KEndlChunkId:
					    if(iMngSubframesNoLoops)
					    	{
					    	iState = EReadHeader;
							RunAgain();
					    	break;
					    	}
								
					default:
						{
						TMngChunk::TObjectFactoryFuncL FuncL=iCurrentChunk->MngObjectFactoryFunc();
						if (NULL==FuncL)
							{
							delete iCurrentChunk;
							iCurrentChunk = NULL;
							}
						else 
							{
							CMngObject* pObject=FuncL(iCurrentChunk);
							iMngPlayer.AppendObjectL( pObject );
							}
							
						iState = EReadHeader;
						RunAgain();
						}
					}
				}
			break;

		default:
			ASSERT(EFalse);
		}
	}

void CMngProcessor::Start(MInputStream& aInputStream, TRequestStatus& aRequest)
	{
	if (NULL != iCallerRequest)
		{
		MngPanic(EPanicAlreadyStarted);
		}
	iCallerRequest	= &aRequest;
	*iCallerRequest	= KRequestPending;
	iInputStream	= &aInputStream;
	iInputStream->AddRef();
	iIsPaused = EFalse;
	RunAgain();
	}



TInt CMngProcessor::AddSubProcessor(MChunkProcessor& aSubProcessor)
	{
	if (NULL != iSubProcessor)
		{
		MngPanic(EPanicNotImplementedYet);
		}
	iSubProcessor = &aSubProcessor;
	return KErrNone;
	}



TBool CMngProcessor::OfferChunkType(const TChunkId& /*aChunkId*/)
	{
	ASSERT(EFalse);
	return EFalse;
	}
