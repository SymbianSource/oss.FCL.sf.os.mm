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
#include <hal.h>
#include "MngPlayer.h"
#include "MngRenderer.h"
#include "PNGCodec.h"

#include "PngLayer.h"

/*static*/
CPngLayer* CPngLayer::NewL()
	{
	CPngLayer* self = new (ELeave) CPngLayer();
	CleanupStack::PushL(self);
	self->iPngBuffer = CBufFlat::NewL(256);
	CleanupStack::Pop(self);
	return self;
	}

CPngLayer::~CPngLayer()
	{
	if (iPngCodec)
		{
		iPngCodec->Release();
		iPngCodec = NULL;
		}
		
	delete iPngBuffer;
	}

void CPngLayer::AppendChunkL(const TMNGChunkHeader& aHeader,MInputStream& aInpStream)
	{
	TInt sz=iPngBuffer->Size();
	iPngBuffer->ResizeL(sz + TMNGChunkHeader::RequiredData() + aHeader.iLength + sizeof(TChunkCrc));
//  PNG decoder should check CRC for PNG streams...
	TPtr8 buf( iPngBuffer->Ptr(sz) );
	aInpStream.ReadL(buf);
	}

void CPngLayer::ExecuteL(CMngPlayer& aPlayer, TMngExecuteResult& /*aResult*/)
	{
#if defined(ENABLE_PNG_LAYER_PROFILE)	
	TUint32 begin=User::FastCounter();
#endif // ENABLE_PNG_LAYER_PROFILE

	CMngRenderer* Renderer = aPlayer.Renderer();
	if (iPngBuffer->Size()!=iPngBuffer->Capacity() && iPngBuffer->Size()!=0)
		{
		iPngBuffer->Compress();
		}
		
	Renderer->OnNewImageL();
	if (Renderer->OutputEnabled())
		{
		if (NULL==iPngCodec)
			{
			TBufPtr8 ptr;
			ptr.Set(iPngBuffer->Ptr(0));
			aPlayer.DecoderFactory().CreatePngDecoderL(iPngCodec);
			iPngCodec->InitL(Renderer->ImageProcessor(), Renderer->MaskProcessor(), aPlayer.GetDestinationBitmap(), ptr, aPlayer);
			}
		else
			{
			iPngCodec->ResetL();
			}
		iPngCodec->DecodeL();
		}
		
#if defined(ENABLE_PNG_LAYER_PROFILE)
	TUint32 end=User::FastCounter();
	TInt freq=0;
	HAL::Get(HAL::EFastCounterFrequency, freq);
	RDebug::Print(_L("PNG layer took %d, freq=%d"), end-begin, freq);
#endif // ENABLE_PNG_LAYER_PROFILE
	}

