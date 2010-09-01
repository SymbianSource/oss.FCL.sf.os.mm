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

#ifndef IMAGEPOSTPROCESS_H

#define IMAGEPOSTPROCESS_H

/** 	@file
	@internalComponent */

class CBitmapScaler;
class CBitmapRotator;
class CFbsBitmap;

class MPostProcessObserver
	{
public:
	virtual void OnTransformDone(TInt aError)=0;
	};

class CTransformerSharedData: public CBase
	{
public:
	static CTransformerSharedData* NewL();
	void AddRef();
	void Release();
	void Cleanup();
	CBitmapScaler&	ScalerL();
	CBitmapRotator&	RotatorL();

	inline void SetTransformOptions(TUint aOptions);
	inline void SetClipRect(const TRect& aClipRect);
	inline void SetTrueSrcSize(const TSize& aSize);

	inline const TSize& TrueSrcSize() const;
	inline TUint TransformOptions() const;
	inline TBool ClippingSet() const;
	inline const TRect& ClipRect() const;

protected:
	~CTransformerSharedData();

protected:
	CBitmapRotator*		iRotator;
	CBitmapScaler*		iScaler;

	TUint				iTransformOptions;
	TRect				iClipRect;
	TBool				iClippingSet;
	TSize				iTrueSrcSize;
private:
	TInt				iRefCount;
	};

class CAsyncTransformer:public CActive 
	{
public:
	static CAsyncTransformer* NewL(MPostProcessObserver& aObserver, CTransformerSharedData& aSingleton, const TSize& aDestSize);
	~CAsyncTransformer();

	void Transform(CFbsBitmap* aImage, TBool aMantainAspect);
	void Restart();
	void Cleanup();

	inline void GetBitmap(CFbsBitmap*& aImage) const;
	TInt SetScaleQuality(TInt aQualityValue);
	inline CTransformerSharedData& Singleton();

protected:
	CAsyncTransformer(MPostProcessObserver& aObserver,CTransformerSharedData& aSingleton, const TSize& aDestSize);

	void CreateDestBitmapsL();
	TInt RunError(TInt aError);
	void RunL();
	void DoCancel();
	TInt NextTransform(TUint& aTransFormTodo1);
	void RunAgain();
	void DeletePreTranformBmps();
	void DoClipL(CFbsBitmap* aSrc, CFbsBitmap*& aDest);

protected:
	MPostProcessObserver& iObserver;
	CTransformerSharedData*	iSingleton; // ref. counted
	const TSize		iDestSize;
	CFbsBitmap*		iImgSource;			// not owned
	CFbsBitmap*		iCurrentSource;		// not owned

	CFbsBitmap*		iScaleDest;
	CFbsBitmap*		iClippingDest;
	CFbsBitmap*		iTransformDest;

	TBool			iKeepAspect;
	
	TUint			iTransformTodo;
	TInt			iScaleQuality;

	TInt			iResultStatus;
	TRect			iTrueClipRect;
	TBool			iScaleNeeded;
	enum TState 
		{
		EStart,
		EClip,
		ETransform,
		ETransformDone,
		EScale,
		EDone
		}			iState;
	};

#include "ImagePostprocess.inl"

#endif // ndef IMAGEPOSTPROCESS_H

