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

#ifndef __MngImageDisplay_H__
#define __MngImageDisplay_H__

/** 	@file
	@internalTechnology */

#include <icl/imagedisplayplugin.h>
#include <imageconversion.h>
#include <bitmaptransforms.h>

#include "MngPlayerObserver.h"

class CMngPlayer;
class CFbsBitmap;
class CMngFileReadStream;
class CBitmapScaler;
class TMhdrChunk;
class CAsyncTransformer;

class CMngImageDisplayPlugin : public CImageDisplayPlugin, public MMngPlayerObserver
	{
private:
	enum TState
		{
		EIdle,
		EInitFailed,
		EInitCompleted,
		EProcessing,
		EPaused,
		ETransforming
		};
public:
 	static CMngImageDisplayPlugin* NewL();
	~CMngImageDisplayPlugin();

protected:

	void ReadOriginalImgSizeL();
	void CompleteInitL();

// from the CImageDisplayPlugin
	virtual void OpenL();
	virtual void Play();
	virtual void Pause();
	virtual void StopPlay();

	virtual TInt ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr);
	virtual void GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const;
	const CImageDisplay::RImageSizeArray& RecommendedImageSizes() const;

	virtual TInt NumFrames(TInt& aNumFrames) const;
	virtual TUint ImageStatus() const;
	virtual TBool ValidBitmap() const;

// from the MMngPlayerObserver 
	virtual void OnDecodeEvent(TUint aEventFlags, TInt aErrorCode);
	
private:
	CMngImageDisplayPlugin();
	void ConstructL();
	void Cleanup();
	static TInt ScaleCallback(TAny* aPtr);
	void OnScaleFinished();

protected:

private:
	CMngPlayer*		iMngPlayer;
	CMngFileReadStream* iMngStream;
	CAsyncTransformer*	iTransformer;
	TState			iState;
	CFbsBitmap*		iFrame;
	CFbsBitmap*		iMask;
	CFbsBitmap*		iScaledFrame;
	CFbsBitmap*		iScaledMask;
	TInt			iFrameNumber;
	TUint        	iPluginStatus;
	TRect			iOutputRect;
	TRect			iClipRect;
	TSize			iBitmapSize;
	TBool			iValidBitmap;
	TMhdrChunk*		iHeaderChunk;
	};	

class CAsyncTransformer:public CActive 
	{
public:
	static CAsyncTransformer* NewL(const TCallBack& aCallBack);
	~CAsyncTransformer();

	void Transform(CFbsBitmap* aImage, CFbsBitmap* aMask, 
							CFbsBitmap* aDestImage, CFbsBitmap* aDestMask, TBool aMantainAspect);
	TInt ScaleStatus() const;
	void SetTranformOptions(TUint aOptions);
	void Restart();
	void SetScaleSize(const TSize& aSize, TBool aScaleNeeded);

protected:
	CAsyncTransformer(const TCallBack& aCallBack);
	void CreateScalerL();
	void CreateTransformerL();
	TInt RunError(TInt aError);
	void RunL();
	void DoCancel();
	TInt NextTransform(TUint& aTransFormTodo1);

protected:
	TCallBack		iResultCallback;
	CBitmapScaler*	iBitmapScaler;
	CBitmapRotator* iBitmapRotator;
	CFbsBitmap*		iFirstBitmap;
	CFbsBitmap*		iFirstBitmapDest;
	CFbsBitmap*		iSecondBitmap;
	CFbsBitmap*		iSecondBitmapDest;
	CFbsBitmap*		iScaleSrc1;
	CFbsBitmap*		iScaleSrc2;
	TSize			iScaleDestSize;
	TBool			iScaleNeeded;
	TBool			iKeepAspect;
	TUint			iTransformOptions;
	TUint			iTransFormTodo1;
	TUint			iTransFormTodo2;
	TInt			iResultStatus;
	enum 
		{
		ETransform1,
		ETransform2,
		EScale1,
		EScale2,
		EDone
		}			iState;
	};

#endif // __MngImageDisplay_H__

