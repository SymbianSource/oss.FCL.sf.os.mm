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
#include <s32mem.h>
#include "Crc32.h"
#include "MngChunks.h"
#include "MngObjects.h"

static const TInt KInt32Size = sizeof(TInt32);


void TMNGChunkHeader::ReadL(MInputStream& aInputStream)
   {
   aInputStream.ReadInt32L(iLength);
   TPtr8 ChunkBf(iChunkId.iChunkIdChr, sizeof(iChunkId) );
   aInputStream.ReadL(ChunkBf);
   if (iLength < 0)
	   {
	   User::Leave( KErrCorrupt);
	   }
   }

void TMngChunk::ReadAndCheckCrcL(MInputStream& aInputStream)
	{
	TInt32 crc;
	aInputStream.ReadInt32L(crc);
	crc^=(~(TUint32)0);
	if (crc != iCalculatedCrc.iCrc)
		{
		User::Leave(KErrCorrupt);
		}
	}

TMngChunk::TObjectFactoryFuncL TMngChunk::MngObjectFactoryFunc() const
	{
	return NULL;
	}

void TMngChunk::ReadNameL(MInputStream& aInputStream,TMngObjectName& aName, TInt aMaxDataSize)
	{
	TText8 LastChar=1;
	aName.Zero();
	TInt i=aMaxDataSize;
	for (; i && LastChar; --i)
		{
		aInputStream.ReadUint8L(LastChar, &iCalculatedCrc.iCrc);
		if (LastChar)
			{
			aName.Append(LastChar);
			}
		}
	
	if (i==0 && LastChar)
		{
		User::Leave(KErrCorrupt); // buffer overflow
		}
	}

void TUnknownChunk::ReadL(MInputStream& aInputStream)
	{
#ifdef _DEBUG
	for (TInt i=iHeader.iLength; i; --i)
		{
		TText8 dummy;
		aInputStream.ReadUint8L(dummy, &iCalculatedCrc.iCrc);
		}
	ReadAndCheckCrcL(aInputStream);
#else
	aInputStream.SeekL( aInputStream.Tell() + iHeader.iLength + sizeof(iCalculatedCrc) );
#endif
	}

void TMhdrChunk::ReadL(MInputStream& aInputStream)
	{
	if (iHeader.iLength != 28)
		{
		User::Leave(KErrCorrupt);
		}
	aInputStream.ReadInt32L(&iFrameWidth, iHeader.iLength/KInt32Size, &iCalculatedCrc.iCrc);
	ReadAndCheckCrcL(aInputStream);
	if (iZeroBit !=0 || iTicksPerSecond<0 || iFrameWidth <=0 || iFrameHeight <=0 || iNominalPlayTime <0 
		  || iNominalFrameCount <0 || iNominalLayerCount <0 )
		{
		User::Leave(KErrCorrupt);
		}
	}

void TDefiChunk::ReadL(MInputStream& aInputStream)
	{
	if (iHeader.iLength <2 || (iHeader.iLength >4 && iHeader.iLength !=12 && iHeader.iLength !=28))
		{
		User::Leave(KErrCorrupt);
		}
		
	TUint16 temp;
	aInputStream.ReadUint16L(temp, &iCalculatedCrc.iCrc);
	iObjectId = temp;

#ifdef STRICT_MNG_LC
	if (iObjectId !=0 )
		{
		User::Leave(KErrNotSupported); 
		}
#endif

	if (iHeader.iLength >2)
		{
		TUint8 tmp8;
		aInputStream.ReadUint8L(tmp8, &iCalculatedCrc.iCrc);
		iDoNotShow=tmp8;
		}
		
	if (iHeader.iLength >3)
		{
		TUint8 tmp8;
		aInputStream.ReadUint8L(tmp8, &iCalculatedCrc.iCrc);
		iConcrete=tmp8;
		}

	if (iHeader.iLength >4)
		{
		aInputStream.ReadInt32L( &iXLocation, (iHeader.iLength -4)/KInt32Size, &iCalculatedCrc.iCrc );
		}

	if (iHeader.iLength ==28 && (iLeftClippingBoundary > iRightClippingBoundary 
		  || iTopClippingBoundary > iBottomClippingBoundary))
		{
		User::Leave(KErrCorrupt);
		}

	ReadAndCheckCrcL(aInputStream);
	}

TMngChunk::TObjectFactoryFuncL TDefiChunk::MngObjectFactoryFunc() const
	{
	return CMngImageDefinition::NewL;
	}

void TTermChunk::ReadL(MInputStream& aInputStream)
	{
	if (iHeader.iLength !=1 && iHeader.iLength != 10)
		{
		User::Leave(KErrCorrupt);
		}
	TUint8 tmp8;
	aInputStream.ReadUint8L(tmp8, &iCalculatedCrc.iCrc);
	iTerminationAction=TMngTerminationAction( tmp8 );
	if (iHeader.iLength >1)
		{
		aInputStream.ReadUint8L(tmp8, &iCalculatedCrc.iCrc);
		iPostIterationAction = TMngTerminationAction( tmp8 );
		if (iTerminationAction != EMngRepeatImmidiately)
			{
#ifdef WRONG_TERM_HACK
			iPostIterationAction = iTerminationAction;
#else
			User::Leave(KErrCorrupt);
#endif
			}
		aInputStream.ReadInt32L(&iDelay, 2, &iCalculatedCrc.iCrc );
		iIterationMax+= (iIterationMax == 0);
		if (iIterationMax < 1)
			{
			User::Leave(KErrCorrupt);
			}
		}
	else
		{
		iIterationMax		= 1;
		iPostIterationAction= iTerminationAction;
		}

	if (iTerminationAction > EMngRepeatImmidiately || iPostIterationAction > EMngShowFirstFrmAfterTerm 
		 || iDelay < 0 || iIterationMax < 0)
		{
		User::Leave(KErrCorrupt);
		}
	ReadAndCheckCrcL(aInputStream);
	}

void TBackChunk::ReadL(MInputStream& aInputStream)
	{
#ifdef STRICT_MNG_LC
	if (iHeader.iLength == 9 || iHeader.iLength == 10) // we support LC profile only
		{
		User::Leave(KErrNotSupported);
		}
	if (iHeader.iLength != 6 && iHeader.iLength != 7) 
		{
		User::Leave(KErrCorrupt);
		}
#else
	if (iHeader.iLength < 6 || iHeader.iLength == 8 || iHeader.iLength > 10) 
		{
		User::Leave(KErrCorrupt);
		}
#endif

	aInputStream.ReadUint16L(&iRedBackground, 3, &iCalculatedCrc.iCrc);
	if (iHeader.iLength > 6)
		{
		TUint8 tmp8;
		aInputStream.ReadUint8L(tmp8, &iCalculatedCrc.iCrc);
		iBgMandatory = TMngBackroundMandatory(tmp8);
		}
#ifndef STRICT_MNG_LC
	if (iHeader.iLength > 7)
		{
		TUint8 temp[3];
		aInputStream.ReadUint8L(temp, iHeader.iLength - 7, &iCalculatedCrc.iCrc);
		}
#endif
	if (iBgMandatory > EMngTermBgColorAndImgMandatory)
		{
		User::Leave(KErrCorrupt);
		}
	ReadAndCheckCrcL(aInputStream);
	}

TMngChunk::TObjectFactoryFuncL TBackChunk::MngObjectFactoryFunc() const
	{
	return CMngBackground::NewL;
	}

void TMagnChunk::ReadL(MInputStream& aInputStream)
	{
#ifdef WRONG_MAGN_HACK
	TBool WrongLen= ((iHeader.iLength & 1) == 0 && iHeader.iLength > 4 && iHeader.iLength != 18 );
	if (iHeader.iLength > 20)
		{
		User::Leave(KErrCorrupt);
		}
#else
	if (iHeader.iLength > 18)
		{
		User::Leave(KErrCorrupt);
		}
#endif
	TInt DataLeft = iHeader.iLength;

	TUint8 temp8;

	if (DataLeft)
		{
		aInputStream.ReadUint16L(&iFirstMagnifiedObjId, 1, &iCalculatedCrc.iCrc);
		DataLeft -=sizeof(iFirstMagnifiedObjId);
		}
		
	if (DataLeft)
		{
		aInputStream.ReadUint16L(&iLastMagnifiedObjId, 1, &iCalculatedCrc.iCrc);
		DataLeft -=sizeof(iLastMagnifiedObjId);
		}
	else
		iLastMagnifiedObjId = iFirstMagnifiedObjId;

#ifdef STRICT_MNG_LC
	if (iLastMagnifiedObjId | iFirstMagnifiedObjId) // it is not supported in the MNG-LC
		{
		User::Leave(KErrNotSupported);
		}
#endif

	if (DataLeft)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc );
		--DataLeft;
#ifdef WRONG_MAGN_HACK
		if (WrongLen) 
			{
			aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc );
			--DataLeft;
			}
#endif
		iXMagnMethod = TMngMagnificationMethod( temp8 );
		}
		
	if (DataLeft && 0 != iXMagnMethod)
		{
		aInputStream.ReadUint16L(&iXMagnFactor, 1, &iCalculatedCrc.iCrc );
		DataLeft -=sizeof(iXMagnFactor);
		if (iXMagnFactor <= 0)
			{
			User::Leave(KErrCorrupt);
			}
		}

 	iRightXMagnFactor = iLeftXMagnFactor = iYMagnFactor = iXMagnFactor;

	if (DataLeft)
		{
		TInt ToRead=Min(TInt(DataLeft/sizeof(iYMagnFactor)), 3);
		aInputStream.ReadUint16L(&iYMagnFactor, ToRead , &iCalculatedCrc.iCrc );
		DataLeft -= ToRead*sizeof(iYMagnFactor);
		if (!(iYMagnFactor && iLeftXMagnFactor && iRightXMagnFactor))
			{
			User::Leave(KErrCorrupt);
			}
		}

	if (DataLeft)
		{
		aInputStream.ReadUint16L(&iTopYMagnFactor, 1, &iCalculatedCrc.iCrc );
		DataLeft -=sizeof(iTopYMagnFactor);
		if (iTopYMagnFactor <= 0)
			{
			User::Leave(KErrCorrupt);
			}
		}
	else
		{
		iTopYMagnFactor = iYMagnFactor;
		}

	if (DataLeft)
		{
		aInputStream.ReadUint16L(&iBottomYMagnFactor, 1, &iCalculatedCrc.iCrc );
		DataLeft -=sizeof(iBottomYMagnFactor);
		if (iBottomYMagnFactor <= 0)
			{
			User::Leave(KErrCorrupt);
			}
		}
	else
		iBottomYMagnFactor = iYMagnFactor;

	if (DataLeft)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc );
#ifdef WRONG_MAGN_HACK
		if (WrongLen) 
			{
			aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc );
			--DataLeft;
			}
#endif
		iYMagnMethod = TMngMagnificationMethod( temp8 );
		}
	else
		{
		iYMagnMethod = iXMagnMethod;
		}

	if (0 == iYMagnMethod)
		{
		iTopYMagnFactor = 0;
		}
	
	if (iXMagnMethod > EMngMagnLinearAlphaClosestColor || iYMagnMethod > EMngMagnLinearAlphaClosestColor)
		{
		User::Leave(KErrCorrupt);
		}

	ReadAndCheckCrcL(aInputStream);
	}

TMngChunk::TObjectFactoryFuncL TMagnChunk::MngObjectFactoryFunc() const
	{
	return CMngMagnDefinition::NewL;
	}


void TFramChunk::ReadL(MInputStream& aInputStream)
	{
	TInt DataLeft=iHeader.iLength;
	TUint8 temp8;

	if ( DataLeft != 0)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iFramingMode = TMngFramingMode( temp8 );
		--DataLeft;
		}
		
	if (DataLeft > 0)
		{
		TMngObjectName frameName;		// read and ignore the frame name
		ReadNameL(aInputStream, frameName, 
					DataLeft>frameName.MaxLength()? frameName.MaxLength(): DataLeft );
		DataLeft -= (frameName.Length() +1 ); // zero byte also
		}

	if ( DataLeft > 0)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iChangeInterFrmDelay=TMngFramChangeNextFrameOption( temp8 );
		--DataLeft;
		}

	if ( DataLeft > 0)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iChangeTimeoutAndTerm=TMngFramChangeTimeoutAndTermination( temp8 );	
		--DataLeft;
		}
		
	if ( DataLeft > 0)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iChangeClippingBoundaries=TMngFramChangeNextFrameOption( temp8 );
		--DataLeft;
		}
		
	if ( DataLeft > 0)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iChangeSyncIdList=TMngFramChangeNextFrameOption( temp8 );	
		--DataLeft;
		}

	if (iChangeInterFrmDelay)
		{
		DataLeft -=KInt32Size;
		if ( DataLeft < 0 )
			{
			User::Leave(KErrCorrupt);
			}
		aInputStream.ReadInt32L(iInterframeDelay, &iCalculatedCrc.iCrc );
		}

	if (iChangeTimeoutAndTerm)
		{
		DataLeft -=KInt32Size;
		if ( DataLeft < 0 )
			{
			User::Leave(KErrCorrupt);
			}
		aInputStream.ReadInt32L(iDelay, &iCalculatedCrc.iCrc);
		}
	
	if (iChangeClippingBoundaries)
		{
		DataLeft -=(KInt32Size*4 + 1);
		if ( DataLeft < 0 )
			{
			User::Leave(KErrCorrupt);
			}	
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iAddLayerClipping = TBool(temp8);
		aInputStream.ReadInt32L(&iLeftLayerClippingB, 4, &iCalculatedCrc.iCrc);
		}
// Proper reading of sync_id_list will be needed for full MNG implementation. We don't need it for MNG-LC
	if (iChangeSyncIdList && DataLeft>0)
		{
		aInputStream.ReadInt32L(&iFirstSyncId, 1, &iCalculatedCrc.iCrc);
		for (DataLeft-=4; DataLeft>0 ; --DataLeft)
			{
			TInt32 Dummy;
			aInputStream.ReadInt32L(&Dummy, 1, &iCalculatedCrc.iCrc);
			}
		}
	if ( DataLeft < 0
		 ||	iFramingMode > EMngFramBgLayerFor1stFgLayer || iChangeInterFrmDelay > EMngFramChangeAndResetDefault 
		 || iChangeTimeoutAndTerm > EMngFramExtSignalAndResetDef || iChangeClippingBoundaries > EMngFramChangeAndResetDefault
		 || iDelay < 0  || iInterframeDelay < 0
		 || (!iAddLayerClipping && (iLeftLayerClippingB > iRightLayerClippingB || iTopLayerClippingB > iBottomLayerClippingB)))
		{
		User::Leave(KErrCorrupt);
		}

	ReadAndCheckCrcL(aInputStream);
	}

TFramChunk::TObjectFactoryFuncL TFramChunk::MngObjectFactoryFunc() const
	{
	return CMngKeyFrame::NewL;
	}

void TLoopChunk::ReadL(MInputStream& aInputStream)
	{
	TInt DataLeft=iHeader.iLength;
	TUint8 temp8;
	if (DataLeft < 5)
		{
		User::Leave(KErrCorrupt);
		}
		
	aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
	iNestLevel = temp8;

	aInputStream.ReadInt32L(iIterationCount, &iCalculatedCrc.iCrc);
	DataLeft -=5;

	if ( DataLeft > 0)
		{
		aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
		iTerminationCondition = temp8;
		--DataLeft;
		}
// now ignore iteration_min, max and all the signals
	TInt32 Temp32;
	for (;DataLeft>0; DataLeft-=sizeof(Temp32))
		{
		aInputStream.ReadInt32L(Temp32, &iCalculatedCrc.iCrc);
		}
	if (iIterationCount < 0 )
		{
		User::Leave(KErrCorrupt);
		}
	ReadAndCheckCrcL(aInputStream);	
	}


TMngChunk::TObjectFactoryFuncL TLoopChunk::MngObjectFactoryFunc() const
	{
	return CMngLoopEntry::NewL;
	}

void TEndlChunk::ReadL(MInputStream& aInputStream)
	{
	if (1 != iHeader.iLength)
		{
		User::Leave(KErrCorrupt);
		}

	TUint8 temp8;
	aInputStream.ReadUint8L(temp8, &iCalculatedCrc.iCrc);
	iNestLevel = temp8;
	ReadAndCheckCrcL(aInputStream);	
	}


TMngChunk::TObjectFactoryFuncL TEndlChunk::MngObjectFactoryFunc() const
	{
	return CMngLoopEnd::NewL;
	}

/*static*/
TMngRawChunk* TMngRawChunk::CreateL(const TMNGChunkHeader& aHeader)
	{
	if (aHeader.iLength > 1024)
		{
		User::Leave(KErrOverflow);
		}

	TInt size=TInt(sizeof(TMngRawChunk) + aHeader.iLength);
	TAny* Ptr= new (ELeave) TUint8 [ size ];
	Mem::FillZ(Ptr, size );
	TMngRawChunk* self= ::new (Ptr) TMngRawChunk(aHeader);
	return self;
	}

void TMngRawChunk::ReadL(MInputStream& aInputStream)
	{
	if (0 != iHeader.iLength)
		{
		aInputStream.ReadUint8L(iBite, iHeader.iLength, &iCalculatedCrc.iCrc);
		}
	ReadAndCheckCrcL(aInputStream);
	}

TMngChunk::TObjectFactoryFuncL TMngRawChunk::MngObjectFactoryFunc() const
	{
	return CMngRawChunkHolder::NewL;
	}

