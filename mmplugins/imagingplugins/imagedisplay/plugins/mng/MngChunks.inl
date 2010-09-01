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

/**
 @file
 @internalTechnology 
*/
/*static*/
inline
TChunkId TChunkId::FromPtr(const TText8* aPtr)
	{
	TChunkId temp;
	Mem::Copy(&temp, aPtr, sizeof(TChunkId));
	return temp;
	}

inline
TBool TChunkId::operator==(const TInt aRight) const
	{
	return (iChunkIdInt == aRight);
	}

inline
TBool TChunkId::operator==(const TDesC8& aRight) const
	{
	__ASSERT_DEBUG(aRight.Length()==4, MngPanic(EPanicWrongChunkIdLength));
	return (aRight.Compare( TPtrC8(iChunkIdChr, 4) ) == 0 );
	}

inline
TBool TChunkId::operator!=(const TInt aRight) const
	{
	return !(*this==aRight);
	}

/*static*/
inline
TInt TMNGChunkHeader::RequiredData()
		{
		return sizeof(TInt32) + sizeof(TChunkId);
		}

inline
TChunkCrc::TChunkCrc():iCrc((TInt32)(TUint32)~0)
	{
	}

inline
TChunkCrc::operator const TInt32&() const
	{	
	return iCrc;
	}

inline
TMngChunk::TMngChunk(const TMNGChunkHeader& aHeader):iHeader(aHeader)
	{
	Crc32::Calc(iCalculatedCrc.iCrc, &iHeader.iChunkId.iChunkIdInt, sizeof(iHeader.iChunkId));
	}

inline
TInt TMngChunk::RequiredData() const
	{
	return iHeader.iLength + sizeof(TInt32); // and Crc
	}

inline
TMngRawChunk::TMngRawChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader)
	{
	}

inline 
TUnknownChunk::TUnknownChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader)
	{
	}

inline
TMhdrChunk::TMhdrChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader) 
	{
	}

inline 
TDefiChunk::TDefiChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader)
														
	{
	Mem::Fill(&iObjectId, _FOFF(TDefiChunk, iBottomClippingBoundary) - _FOFF(TDefiChunk, iObjectId) 
					+ sizeof(iBottomClippingBoundary) , 0);
	iRightClippingBoundary	=-1;
	}

inline 
TTermChunk::TTermChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader),
														iPostIterationAction(EMngShowLastFrameIndef),
														iDelay(0),
														iIterationMax(1)
	{
	}

inline 
TBackChunk::TBackChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader),
														iBgMandatory(EMngTermBgColorAndImgAdvisory)
	{
	}

inline TMagnChunk::TMagnChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader)
	{
	Mem::Fill(&iFirstMagnifiedObjId, _FOFF(TMagnChunk, iYMagnMethod) - 
				_FOFF(TMagnChunk, iFirstMagnifiedObjId) + sizeof(iFirstMagnifiedObjId), 0);
	iXMagnFactor =1;
	}

inline 
TFramChunk::TFramChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader),
															iFramingMode(EMngFramDoNotChangeMode)
	{
	Mem::Fill(&iChangeInterFrmDelay, _FOFF(TFramChunk, iFirstSyncId)-_FOFF(TFramChunk, iChangeInterFrmDelay)+sizeof(iFirstSyncId), 0);
	}

inline 
TLoopChunk::TLoopChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader),
														iNestLevel(0)
	{
	}

inline 
TEndlChunk::TEndlChunk(const TMNGChunkHeader& aHeader):TMngChunk(aHeader),
															iNestLevel(0)
	{
	}

inline
const TUint8* TMngRawChunk::Data() const
	{
	return iBite;
	}



