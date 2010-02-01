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

#ifndef IMAGEPLAYER_H
#define IMAGEPLAYER_H

/** 	@file
	@internalComponent */

class CFbsBitmap;

/**
	Synchronously called upon next image frame
*/
class MImagePlayerObserver
	{
public:
	virtual void OnPlayEvent(TInt aErrorCode, TUint aEvent, CFbsBitmap* aFrame, CFbsBitmap* aMask)=0;
	};
	
/**
	Provides abstraction layer over the CImageDecoder, see design document ICL_Design_Generic_ImgDisplPlugin.doc
	for more detailed description.
*/
class CImagePlayer: public CActive 
	{
public:
	enum
		{
		KNoFrameLimit	=0	// means that player should play (decode) all the image frames
		};
	CImagePlayer(MImagePlayerObserver& aObserver);
	~CImagePlayer();
	
	void Play(CImageDecoder& aDecoder, TDisplayMode aDisplayMode, TInt aFrameLimit=KNoFrameLimit);
	void Setup(const TSize& aDestSize,TInt aMaxReduction, TInt aMinReduction);
	void Pause();
	void Resume();

	inline const TFrameInfo& CurrentFrameInfo() const;
	TSize OriginalFrameSize() const;
protected:
	void RunAgain();
	inline CFbsBitmap* FrameBitmap();
	inline CFbsBitmap* MaskBitmap();
	void CreateBitmapsL();
	void MergeFrameL();
	static void CreateBitmapsL(CFbsBitmap*& aFrame, CFbsBitmap** aMask, const TSize& aSize, TDisplayMode aFrameMode);
// from the CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	MImagePlayerObserver&	iObserver;
	TSize					iDestSize;
	TInt					iMaxReduction;
	TInt					iMinReduction;
	TInt					iFrameLimitToDecode;
	CImageDecoder*			iDecoder;	// not owned
	CFbsBitmap*				iFrame;		// owned
	CFbsBitmap*				iMask;		// owned
	const TFrameInfo*		iCurrentFrameInfo; // not owned
	TInt					iCurrentReduction;
	TBool					iUseTempBitmap; // temporary bitmaps are used in case of animed GIFs to have precomposed animation done
	CFbsBitmap*				iTmpFrame;		// owned
	CFbsBitmap*				iTmpMask;		// owned
	TDisplayMode			iDisplayMode;
	TInt					iFrameNumber;
	TBool					iIsPaused;		// flag to indicate that all the subsequent operations should be paused until further notice
	TBool					iNeedRunAfterResume;// if the AO needs run after resume (i.e. if it was "paused" while being active)
	TBool					iIsAnimationSubframe; // indicates if the frame being decoded is a frame of animated GIF
	enum TState
		{
		EIdle,
		EDecoding,
		}					iState;
	};

#include "ImagePlayer.inl"

#endif // ndef IMAGEPLAYER_H


