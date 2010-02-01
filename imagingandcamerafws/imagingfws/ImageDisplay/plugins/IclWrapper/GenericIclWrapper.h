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
 @internalComponent 
*/

#ifndef GENERICICLWRAPPER_H
#define GENERICICLWRAPPER_H

#include <icl/imagedisplayplugin.h>
#include <imageconversion.h>
#include <icl/geniclimagedisplayext.h>

#include "ImagePlayer.h"
#include "ImagePostprocess.h"

const TInt KMaxReductionFactor	=3;
const TInt KZeroReductionFactor	=0;

class MCallbackClient
	{
public:
	virtual void OnCallback() = 0;
	};


class CGenericImageDisplayPlugin;
class TExtTie: public MGenIclImageDisplayExtension
	{
public:
	explicit TExtTie(CGenericImageDisplayPlugin& aImplementor);
	virtual const TUid Uid() const;
	virtual void Release();
	virtual TInt SetScaleQuality(TInt aQualityLevel);
	virtual TInt SetMaximumReductionFactor(TInt aMaxReductionFactor);
	virtual TInt SetMaximumNonReducedSize(const TSize& aSize);
private:
	CGenericImageDisplayPlugin& iImplementor;
	};

class CDelayedCallback: protected CTimer 
   {
public:
	static CDelayedCallback* NewL(MCallbackClient& aClient);
	void CallAfter(const TTimeIntervalMicroSeconds& aAfter);
	void Cancel();

protected:
   inline CDelayedCallback(MCallbackClient& aClient);

   void RunL();

protected:
   MCallbackClient& iClient;
   TTime			iLatestCallbackAt;
   };

class CFbsBitmap;
class CImageDecoder;
class CAsyncTransformer;

class CGenericImageDisplayPlugin : public CImageDisplayPlugin,
										protected MImagePlayerObserver, 
										protected MPostProcessObserver,
										protected MCallbackClient
	{
private:
	enum TState
		{
		EIdle,			// 0
		EInitFailed,	// 1
		EInitCompleted,	// 2
		EProcessing,	// 3
		ETransforming,	// 4
		ETransformingMask//5
		};
	friend class TExtTie;
	typedef RArray<TSize> RSizeArray;

public:
 	static CGenericImageDisplayPlugin* NewL();
	~CGenericImageDisplayPlugin();

protected:
	explicit CGenericImageDisplayPlugin(const TDesC& aPanicCategoryStr);
	void ConstructL();
	
	void Panic(TInt aPanicNumber) const;
	TInt DoSetScaleQuality(TInt aQualityValue);
	TInt DoSetMaximumReductionFactor(TInt aMaxReductionFactor);
	
	virtual void  AfterOpenL();
	virtual TUint EffectiveRotation() const;
	virtual TBool ThumbnailExists() const;

	inline TBool ThumbnailRequested() const;
// from the MImagePlayerObserver
	virtual void OnPlayEvent(TInt aErrorCode, TUint aEvent, CFbsBitmap* aFrame, CFbsBitmap* aMask);

// fron the MPostProcessObserver
	virtual void OnTransformDone(TInt aError);

// from the MCallbackClient
	virtual void OnCallback();

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
	virtual TUid  RequiredImageClass() const;

private:
	void CompleteInitL();
	TBool SetupPlayerGetScalingNeeded(const TSize& aImageSize, const TSize& aTrueImgSize);
	void CacheImageStatus();
	void FillRecommendedSizesL();
	void GenRecommendedSizesL(RSizeArray& aArray,const TSize& aOriginalSize,TInt aMaxReduction);
	TInt SetMaxUnscaledSize(const TSize& aSize);
	inline TDisplayMode EffectiveDispMode() const;

	inline void SetStatusFlag(TUint aFlagOption);
	inline void ClearStatusFlag(TUint aFlagOption);
	inline TBool IsStatusSet(TUint aFlagOption) const;
	inline const TSize& MaxUnscaledSize() const;
	
protected:
	const TDesC&		iPanicCategory;
	CImageDecoder*		iDecoder;
	TInt				iMaxReductionFactor;
	TSize				iMaxUnscaledSize;
	
private:
	TExtTie				iExtHandler;
	CDelayedCallback*	iDelayedCb;
	CAsyncTransformer*	iTransformer;
	CAsyncTransformer*	iMaskTransformer;
	RFs					iFs;
	CImagePlayer*		iPlayer;
	TState			iState;
	CFbsBitmap*		iCurrentMask;	// not owned
	CFbsBitmap*		iCurrentFrame;	// not owned
	TBool			iCallBackIsPending;
	TBool			iIsPaused;
	TTimeIntervalMicroSeconds iNextFrameDelay;
	TInt			iNumberOfFrames;
	TInt			iScaleQuality;
	TInt			iLastError;
	TUint			iPluginImgStatus;	// used to cache value for ImageStatus()
	TUint        	iPluginStatus;
	TUint			iRotationOptions;
	TRect			iOutputRect;
	TBool			iValidBitmap;
	TBool			iImageIsThumbnail;
	TBool			iImageHasFloatingSubImgs;
	};	

#include "Genericiclwrapper.inl"

#endif // GENERICICLWRAPPER_H

