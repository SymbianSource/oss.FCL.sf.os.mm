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

#ifndef __MNGCHUNKS_H__

#define __MNGCHUNKS_H__
/** 	@file
	@internalTechnology */

#include "MngPanic.h"
#include "MngReadStream.h"
#include "Crc32.h"

class CMngObject;
class MInputStream;

#define MNGSIGNATURE "\212MNG\r\n\032\n"
_LIT8(KMngSignature, MNGSIGNATURE);
static const TInt KMngSignatureSize=sizeof(MNGSIGNATURE)-1;

typedef TBuf8<KMngSignatureSize> TMngSignature;

#define FOUR_BYTE_2INT32(d,c,b,a) (TInt32)( (((TUint8)a)<<24) | (((TUint8)b)<<16) | (((TUint8)c)<<8) | ((TUint8)d) )

const TInt KMngInfinity=0x7fffffff;

const TInt32 KMendChunkId=FOUR_BYTE_2INT32('M','E','N','D');
const TInt32 KMhdrChunkId=FOUR_BYTE_2INT32('M','H','D','R');
const TInt32 KDefiChunkId=FOUR_BYTE_2INT32('D','E','F','I');
const TInt32 KTermChunkId=FOUR_BYTE_2INT32('T','E','R','M');
const TInt32 KBackChunkId=FOUR_BYTE_2INT32('B','A','C','K');
const TInt32 KMagnChunkId=FOUR_BYTE_2INT32('M','A','G','N');
const TInt32 KFramChunkId=FOUR_BYTE_2INT32('F','R','A','M');
const TInt32 KLoopChunkId=FOUR_BYTE_2INT32('L','O','O','P');
const TInt32 KEndlChunkId=FOUR_BYTE_2INT32('E','N','D','L');

// Unsupported chunks for MNG-LC
const TInt32 KPastChunkId=FOUR_BYTE_2INT32('P','A','S','T');
const TInt32 KBasiChunkId=FOUR_BYTE_2INT32('B','A','S','I');
const TInt32 KMoveChunkId=FOUR_BYTE_2INT32('M','O','V','E');
const TInt32 KGamaChunkId=FOUR_BYTE_2INT32('g','A','M','A');


// PNG Chunk Codes //

const TInt32 KIhdrChunkId=FOUR_BYTE_2INT32('I','H','D','R');
const TInt32 KIendChunkId=FOUR_BYTE_2INT32('I','E','N','D');
const TInt32 KIdatChunkId=FOUR_BYTE_2INT32('I','D','A','T');
const TInt32 KPlteChunkId=FOUR_BYTE_2INT32('P','L','T','E');
const TInt32 KTrnsChunkId=FOUR_BYTE_2INT32('t','R','N','S');
const TInt32 KBkgdChunkId=FOUR_BYTE_2INT32('b','K','G','D');
const TInt32 KPhysChunkId=FOUR_BYTE_2INT32('p','H','Y','s');

typedef TBuf8<79> TMngObjectName;

enum TMngTerminationAction 
	{
	EMngShowLastFrameIndef=0,
	EMngCeaseDisplAnything=1, 
	EMngShowFirstFrmAfterTerm=2,
	EMngRepeatImmidiately=3,
	};

enum TMngBackroundMandatory 
	{
	EMngTermBgColorAndImgAdvisory=0,		// Background color is advisory. Applications can use it if they choose to.
	EMngTermBgColorMandatoryImgAdvisory=1,	// Background color is mandatory. Applications must use it
	EMngTermBgColorAdvisoryImgMandatory=2,	// is not used in MNG-LC
	EMngTermBgColorAndImgMandatory=3,		// is not used in MNG-LC
	};

enum TMngMagnificationMethod 
	{
	EMngMagnNoMagnification=0,		// No magnification
	EMngMagnPixelReplication=1,		// Pixel replication of color and alpha samples.
	EMngMagnLinearInterpolation=2,	// Magnified intervals with linear interpolation of color and alpha samples.
	EMngMagnAlphaAndColorReplication=3,// Magnified intervals with replication of color and alpha samples from the closest pixel.
	EMngMagnLinearColorAplhaReplication=4, // Magnified intervals with linear interpolation of color samples and replication of alpha samples from the closest pixel.
	EMngMagnLinearAlphaClosestColor=5, // Magnified intervals with linear interpolation of alpha samples and replication of color samples from the closest pixel.
	};

enum TMngFramingMode 
	{
	/*
	Do not change framing mode.
	*/
	EMngFramDoNotChangeMode=0,				// 0
	/*
	1: No background layer is generated, except for one
	ahead of the very first foreground layer in the
	datastream. The interframe delay is associated
	with each foreground layer in the subframe.
	*/
	EMngFramNoBgLayerExcpt1stFgLayer=1,		// 1
	/*
	2: No background layer is generated, except for one
	ahead of the very first image in the datastream.
	The interframe delay is associated only with the
	final layer in the subframe. A zero interframe
	delay is associated with the other layers in the
	subframe.
	*/
	EMngFramNoBgLayerExcpt1stImage=2,		// 2
	/*
	3: A background layer is generated ahead of each
	foreground layer. The interframe delay is
	associated with each foreground layer, and a zero
	delay is associated with each background layer.
	*/
	EMngFramBgLayerForEachFgLayer=3,		// 3
	/*
	4: The background layer is generated only ahead of
	the first foreground layer in the subframe. The
	interframe delay is associated only with the final
	foreground layer in the subframe. A zero
	interframe delay is associated with the background
	layers, except when there is no foreground layer
	in the subframe, in which case the interframe delay
	is associated with the sole background layer.
	*/
	EMngFramBgLayerFor1stFgLayer=4,			// 4
	};


enum TMngFramChangeNextFrameOption 
	{
	EMngFramDoNotChange					=0,	// 0 - Do not change			
	EMngFramChangeUpcomingSubFrameOnly	=1,	// 1 - Yes, for the upcoming subframe only.
	EMngFramChangeAndResetDefault		=2,	// 2 - Yes, also reset default.
	};

enum TMngFramChangeTimeoutAndTermination 
	{
	EMngFramDoNotChangeTimeout=0,			// 0 - Do not change
	EMngFramUpcomingFrmDeterministic=1,		// 1 - Deterministic, for the upcoming subframe only.
	EMngFramDeterministicAndResetDef=2,		// 2 - Deterministic, also reset default.
	EMngFramUpcomingFrmDecoderDiscretion=3,	// 3 - Decoder-discretion, for the upcoming subframe only.
	EMngFramDecoderDiscretionAndResetDef=4,	// 4 - Decoder-discretion, also reset default.
	EMngFramUpcomingFrmUserDiscretion=5,	// 5 - User-discretion, for the upcoming subframe only.
	EMngFramUserDiscretionAndResetDef=6,	// 6 - User-discretion, also reset default.
	EMngFramUpcomingFrameExtSignal=7,		// 7 - External-signal, for the upcoming subframe only.
	EMngFramExtSignalAndResetDef=8,			// 8 - External-signal, also reset default.
	};

union TChunkId 
	{
public:

	inline static TChunkId FromPtr(const TText8* aPtr);

	inline TBool operator==(const TInt aRight) const;
	inline TBool operator==(const TDesC8& aRight) const;
	inline TBool operator!=(const TInt aRight) const;

public:
	TInt32	iChunkIdInt;
	TText8	iChunkIdChr[4];
	};

class TMNGChunkHeader 
	{
public:
	inline static TInt RequiredData();

	void ReadL(MInputStream& aInputStream);

public:
	TInt32		iLength;
	TChunkId	iChunkId;
	};

class TChunkCrc 
	{
public:
	inline TChunkCrc();
	inline operator const TInt32&() const;
public:
	TInt32	iCrc;
	};


class TMngChunk 
	{
public:
	typedef CMngObject* (*TObjectFactoryFuncL)(TMngChunk*&);

	inline explicit TMngChunk(const TMNGChunkHeader& aHeader);

	virtual void ReadL(MInputStream& aInputStream)=0;
	virtual TObjectFactoryFuncL MngObjectFactoryFunc() const;

	inline TInt RequiredData() const;

protected:

	void ReadNameL(MInputStream& aInputStream,TMngObjectName& aName, TInt aMaxDataSize);
	void ReadAndCheckCrcL(MInputStream& aInputStream);
public:
	TChunkCrc		iCalculatedCrc;
	TMNGChunkHeader	iHeader;
	};

class TMngRawChunk: public TMngChunk 
	{
public:

	static TMngRawChunk* CreateL(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);
	TObjectFactoryFuncL MngObjectFactoryFunc() const;
	const TUint8* Data() const;

protected:
	inline explicit TMngRawChunk(const TMNGChunkHeader& aHeader);
	union 
		{
		TUint32	iAlignWord[1];
		TUint8	iBite[1];
		};
	};

class TUnknownChunk: public TMngChunk 
	{
public:
	inline explicit TUnknownChunk(const TMNGChunkHeader& aHeader);
	void ReadL(MInputStream& aInputStream);
	};

class TMhdrChunk: public TMngChunk 
	{
public:

	inline explicit TMhdrChunk(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);

public:
	TInt32		iFrameWidth;
	TInt32		iFrameHeight;
	TInt32		iTicksPerSecond;
	TInt32		iNominalLayerCount;
	TInt32		iNominalFrameCount;
	TInt32		iNominalPlayTime;
	TUint		iProfileValidity:1;
	TUint		iSimpleMngFeaturesPresence:1;
	TUint		iComplexMngFeaturesPresence:1;
	TUint		iInternalTransparencyPresence:1;
	TUint		iJngPresence:1;
	TUint		iDeltaPngPresence:1;
	TUint		iBkgAndSemiTransparencyValidity:1;
	TUint		iBkgTransparencyPresence:1;
	TUint		iSemiTransparencyPresence:1;
	TUint		iStoredObjectBuffersPresence:1;
	TUint		iReservedBits:6;
	TUint		iPrivateBits:15;
	TUint		iZeroBit:1;
	};


/*
The DEFI chunk sets the default set of object attributes (object-id, do-not-show flag,
concrete-flag, location, and clipping boundaries) for any subsequent images that are defined with
IHDR-IEND, or JHDR-IEND datastreams.
*/
class TDefiChunk: public TMngChunk 
	{
public:
	inline explicit TDefiChunk(const TMNGChunkHeader& aHeader);
	void ReadL(MInputStream& aInputStream);

	TMngChunk::TObjectFactoryFuncL MngObjectFactoryFunc() const;

public:
	TInt	iObjectId;				// must be 0 for MNG-LC
	TBool	iDoNotShow;				// The object is not visible if it is ETrue
	TBool	iConcrete;				// MNG-LC decoders can ignore this flag.
	TInt32	iXLocation;				// The X-location and Y-location fields can be omitted as a pair.
	TInt32	iYLocation;				// The Y-location and Y-location fields can be omitted as a pair.
	TInt32	iLeftClippingBoundary;	// Left clipping boundary. The left-cb, right-cb, top-cb, and bottom-cb fields can be omitted as a group.
	TInt32	iRightClippingBoundary;	// Right clipping boundary. The left-cb, right-cb, top-cb, and bottom-cb fields can be omitted as a group.
	TInt32	iTopClippingBoundary;	// Top clipping boundary. The left-cb, right-cb, top-cb, and bottom-cb fields can be omitted as a group.
	TInt32	iBottomClippingBoundary;// Bottom clipping boundary. The left-cb, right-cb, top-cb, and bottom-cb fields can be omitted as a group.
	};

/*
The TERM chunk suggests how the end of the MNG datastream should be handled, when a MEND chunk
is found. It contains either a single byte or ten bytes.
*/
class TTermChunk: public TMngChunk 
	{
public:
	inline explicit TTermChunk(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);

public:
	TMngTerminationAction	iTerminationAction;
	TMngTerminationAction	iPostIterationAction;
	TInt32					iDelay;				// delay in ticks
	TInt32					iIterationMax;		// Infinity is represented by 0x7fffffff.
	};


/*
The BACK chunk suggests or mandates a background color, image, or both against which transparent,
clipped, or less-than-full-frame images can be displayed. This information will be used whenever the application
subsequently needs to insert a background layer, unless another BACK chunk provides new background
information before that happens.
The BACK chunk contains 6, 7, 9, or 10 bytes. 
If any field is omitted, all subsequent fields must also be omitted.
*/
class TBackChunk: public TMngChunk 
	{
public:
	inline explicit TBackChunk(const TMNGChunkHeader& aHeader);
	void ReadL(MInputStream& aInputStream);

	TObjectFactoryFuncL MngObjectFactoryFunc() const;

public:
	TUint16					iRedBackground;
	TUint16					iGreenBackground;
	TUint16					iBlueBackground;
	TMngBackroundMandatory	iBgMandatory;
	};

/*
This chunk provides mandatory magnification factors for existing 
objects and/or for subsequent embedded
images whose object id is 0.
The chunk contains 0 to 18 bytes. If any field is omitted, 
all subsequent fields must also be omitted.
*/
class TMagnChunk: public TMngChunk 
	{
public:
	inline explicit TMagnChunk(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);

	TObjectFactoryFuncL MngObjectFactoryFunc() const;

public:
	TUint16					iFirstMagnifiedObjId;// If omitted, any previous MAGN chunk is nullified. MUST be 0 for MNG-LC??
	TUint16					iLastMagnifiedObjId;// If omitted, last object-id = first object-id.
	TMngMagnificationMethod	iXMagnMethod;		
	TUint16					iXMagnFactor;		// X magnification factor, range 1-65535. If omitted, MX=1. Ignored if X-method is 0 and assumed to be 1.
	TUint16					iYMagnFactor;		// Y magnification factor. If omitted, MY=MX.
	TUint16					iLeftXMagnFactor;	// Left X magnification factor. If omitted, ML=MX.
	TUint16					iRightXMagnFactor;	// Right X magnification factor. If omitted, MR=MX.
	TUint16					iTopYMagnFactor;	// Top Y magnification factor. If omitted, MT=MY. Ignored if Y-method is 0 and assumed to be 1.
	TUint16					iBottomYMagnFactor; //Bottom Y magnification factor. If omitted, MB=MY.
	TMngMagnificationMethod	iYMagnMethod;
	};


/*
The FRAM chunk provides information that a decoder needs for generating frames and interframe delays.
The FRAM parameters govern how the decoder is to behave when it encounters a FRAM chunk, or an
embedded image. The FRAM chunk also delimits subframes.
If bit 1 of the MHDR simplicity profile is 0 and bit 0 is 1, the FRAM chunk must not be present.
*/
class TFramChunk: public TMngChunk 
	{
public:
	inline explicit TFramChunk(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);

	virtual TObjectFactoryFuncL MngObjectFactoryFunc() const;

public:
	TMngFramingMode			iFramingMode;	// 
	TMngFramChangeNextFrameOption iChangeInterFrmDelay;
	TMngFramChangeTimeoutAndTermination iChangeTimeoutAndTerm;
	TMngFramChangeNextFrameOption	iChangeClippingBoundaries;
	TMngFramChangeNextFrameOption	iChangeSyncIdList;
	TInt32							iInterframeDelay;
	TInt32							iDelay;
	TBool							iAddLayerClipping;
	TInt32							iLeftLayerClippingB;
	TInt32							iRightLayerClippingB;
	TInt32							iTopLayerClippingB;
	TInt32							iBottomLayerClippingB;
	TInt32							iFirstSyncId;
	};


class TLoopChunk: public TMngChunk 
	{
public:

	inline explicit TLoopChunk(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);

	TMngChunk::TObjectFactoryFuncL MngObjectFactoryFunc() const;

public:
	TInt	iNestLevel;
	TInt32	iIterationCount;
	TInt	iTerminationCondition;
	};


class TEndlChunk: public TMngChunk 
	{
public:

	inline explicit TEndlChunk(const TMNGChunkHeader& aHeader);

	void ReadL(MInputStream& aInputStream);

	TMngChunk::TObjectFactoryFuncL MngObjectFactoryFunc() const;

public:
	TInt	iNestLevel;
	};

#include "MngChunks.inl"

#endif // ndef __MNGCHUNKS_H__
