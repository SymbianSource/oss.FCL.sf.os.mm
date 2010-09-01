// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MNGREADCODEC_H

#define MNGREADCODEC_H

#include "MngPlayerObserver.h"

class CMngPlayer;
class CMngFileReadStream;

/**
	Observer for codec, allows codec to notify a client about 
	particular codec event such as frame readiness, decode completion etc.
*/
class MMngCodecObserver
	{
public:
	enum TCodecEvent 
		{
		KDataParseCompleted,		// 0
		KFrameReady,				// 1
		KDecodeCompleted=0xFFFF,	// 0xFFFF
		};
public:
	virtual void OnCodecEvent(TCodecEvent aEventCode, TInt aErrorCode)=0;
	};
	
/**
	Implementation of read codec for MNG-LC format, re-uses 
	format "player" implementation of Image Display one
*/
class CMngReadCodec:	public CImageMaskProcessorReadCodec,
						protected MMngPlayerObserver
	{
public:

	static CMngReadCodec * NewL(CMngFileReadStream& aMngReadStream, MMngCodecObserver& aObserver, TBool aMngSubframesNoLoops);  //Subframes with No Loops
	~CMngReadCodec();

	void Restart();
	void DecodeL();
	void StopDecode();

	inline TUint ImageFeatures() const;

// from the CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void InitFrameHeader(TFrameInfo& aFrameSettings, CFrameImageData& /* aFrameImageData */);
	virtual TInt ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const;
protected:
	inline CMngReadCodec(MMngCodecObserver& aObserver, TBool aMngSubframesNoLoops);   //Subframes with No Loops
	void ConstructL(CMngFileReadStream& aMngReadStream);
	TInt GetBitmaps();
	void SendFrameUpdate(TInt aError);
	void CreatePlayerL();
// from the MMngPlayerObserver // 
	virtual void OnDecodeEvent(TUint aEventFlags, TInt aErrorCode);

private:
	TBool iMngSubframesNoLoops;   //Subframes with No Loops
	MMngCodecObserver&	iObserver;
	TFrameInfo*			iFrameInfo;		// not owned
	CFbsBitmap*			iCurrentFrame;	// not owned
	CFbsBitmap*			iCurrentMask;	// not owned
	CMngPlayer*			iMngPlayer;
	CMngFileReadStream* iMngReadStream;
	TTimeIntervalMicroSeconds32 iFrameDelay;
	TUint						iFrameStatus;
	TInt						iFrameNumber;
	TUint						iImageFeatures;
	TRect         				iFrameRect;
	};

inline 
CMngReadCodec::CMngReadCodec(MMngCodecObserver& aObserver, TBool aMngSubframesNoLoops):iMngSubframesNoLoops(aMngSubframesNoLoops),  //Subframes with No Loops
																			  iObserver(aObserver)  //Subframes with No Loops
	{
	}

inline 
TUint CMngReadCodec::ImageFeatures() const
	{
	return iImageFeatures;
	}

#endif // ndef MNGREADCODEC_H


