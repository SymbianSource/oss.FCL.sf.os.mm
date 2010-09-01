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
#include <e32base.h>

#include "InputStream.h"
#include "MngChunks.h"
#include "MngPanic.h"
#include "PngLayer.h"
#include "MngPlayer.h"
#include "logger.h"

#include "PngProcessor.h"

/*static*/
CPngProcessor* CPngProcessor::NewL(CMngPlayer& aPlayer)
	{
	CPngProcessor* self=new (ELeave) CPngProcessor(aPlayer);
	return self;
	}

inline
CPngProcessor::CPngProcessor(CMngPlayer& aPlayer):
								CChunkProcessor(aPlayer)
	{
	}

CPngProcessor::~CPngProcessor()
	{
	delete iCurrentLayer;
	Stop();
	}

void CPngProcessor::DoRunL()
	{
	switch (iState)
		{
		case ECreateLayer:
			if (NULL != iCurrentLayer)
				{
				MngPanic(EPanicAlreadyCreated);
				}
			iCurrentLayer = CPngLayer::NewL();
			iState=TState(iState+1);
			// and go further
		case EReadHeader:
			if (iInputStream->DataAvailable() < TMNGChunkHeader::RequiredData() )
				{
				iInputStream->WaitForData(TMNGChunkHeader::RequiredData(), iStatus);
				SetActive();
				break;
				}
			else 
				{
				iCurrentChunkHeader.ReadL(*iInputStream);
				LOG3("Chunk %4s, len=%d", iCurrentChunkHeader.iChunkId.iChunkIdChr, iCurrentChunkHeader.iLength);
				// PNG decoder needs chunk headers, so we go back to the chunk header
				iInputStream->SeekL( iInputStream->Tell() - TMNGChunkHeader::RequiredData() );
				iState=TState(iState+1);
				}
			// go further for reading the chunk body
		case EReadChunk:
			{
			const TInt overallChunkSize=TMNGChunkHeader::RequiredData()+iCurrentChunkHeader.iLength + sizeof(TChunkCrc);
			if (iInputStream->DataAvailable() < overallChunkSize )
				{
				iInputStream->WaitForData(overallChunkSize, iStatus);
				SetActive();
				break;
				}
			else
				{
				switch (iCurrentChunkHeader.iChunkId.iChunkIdInt)
				{
				case KIhdrChunkId:
				case KIendChunkId:
				case KIdatChunkId:
				case KPlteChunkId:
				case KTrnsChunkId:
				case KBkgdChunkId:
				case KPhysChunkId:
					iCurrentLayer->AppendChunkL(iCurrentChunkHeader, *iInputStream);
					break;
				default:
					iInputStream->SeekL( iInputStream->Tell() + overallChunkSize );
				}

				if (iCurrentChunkHeader.iChunkId != KIendChunkId)
					{
					iState = EReadHeader;
					RunAgain();
					}
				else
					{
					iState = ECreateLayer;
					iMngPlayer.AppendObjectL(reinterpret_cast<CMngObject*&>(iCurrentLayer));
					NotifyCaller(KErrNone);
					}
				}
			}
			break;

		default:
			ASSERT(EFalse);
		}
	}

void CPngProcessor::Start(MInputStream& aInputStream, TRequestStatus& aRequest)
	{
	if (NULL != iCallerRequest)
		{
		MngPanic(EPanicAlreadyStarted);
		}
	iCallerRequest	= &aRequest;
	*iCallerRequest	= KRequestPending;
	iInputStream	= &aInputStream;
	iInputStream->AddRef();
	iState = ECreateLayer;
	RunAgain();
	}

TInt CPngProcessor::AddSubProcessor(MChunkProcessor& /*aSubProcessor*/)
	{
	ASSERT(EFalse);
	return KErrNotSupported;
	}

TBool CPngProcessor::OfferChunkType(const TChunkId& aChunkId)
	{
	return (aChunkId == KIhdrChunkId);
	}



