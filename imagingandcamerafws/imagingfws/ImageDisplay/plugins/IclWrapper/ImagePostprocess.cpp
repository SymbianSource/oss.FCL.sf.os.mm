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

/** 	@file
	@internalComponent */
#include <bitmaptransforms.h>
#include <icl/imagedisplayplugin.h>
#include "MiscUtils.h"

#include "ImagePostprocess.h"

/*static*/
CAsyncTransformer* CAsyncTransformer::NewL(MPostProcessObserver& aObserver,CTransformerSharedData& aSingleton,
										   const TSize& aDestSize)
	{
	return new (ELeave) CAsyncTransformer(aObserver, aSingleton, aDestSize);
	}

CAsyncTransformer::CAsyncTransformer(MPostProcessObserver& aObserver, CTransformerSharedData& aSingleton, const TSize& aDestSize):
														CActive(EPriorityNormal),
														iObserver(aObserver),
														iSingleton(&aSingleton),
														iDestSize(aDestSize),
														iScaleQuality(CBitmapScaler::EMinimumQuality)
														
	{
	iSingleton->AddRef();
	CActiveScheduler::Add(this);
	}

CAsyncTransformer::~CAsyncTransformer()
	{
	Cleanup();
	iSingleton->Release();
	iSingleton = NULL;
	delete iScaleDest;
	delete iTransformDest;
	}

TInt CAsyncTransformer::RunError(TInt aError)
	{
	iResultStatus = aError;
	iObserver.OnTransformDone(aError);
	return KErrNone;
	}

TInt CAsyncTransformer::NextTransform(TUint& aTransFormTodo1)
	{
	if ((aTransFormTodo1&CImageDisplay::EOptionRotateCw90)==CImageDisplay::EOptionRotateCw90)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionRotateCw90;
		return CBitmapRotator::ERotation90DegreesClockwise;
		}

	if ((aTransFormTodo1&CImageDisplay::EOptionRotateCw180)==CImageDisplay::EOptionRotateCw180)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionRotateCw180;
		return CBitmapRotator::ERotation180DegreesClockwise;
		}

	if ((aTransFormTodo1&CImageDisplay::EOptionRotateCw270)==CImageDisplay::EOptionRotateCw270)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionRotateCw270;
		return CBitmapRotator::ERotation270DegreesClockwise;
		}
	
	if ((aTransFormTodo1&CImageDisplay::EOptionMirrorVertical)==CImageDisplay::EOptionMirrorVertical)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionMirrorVertical;
		return CBitmapRotator::EMirrorVerticalAxis;
		}

	if ((aTransFormTodo1&CImageDisplay::EOptionMirrorHorizontal)==CImageDisplay::EOptionMirrorHorizontal)
		{
		aTransFormTodo1 ^= CImageDisplay::EOptionMirrorHorizontal;
		return CBitmapRotator::EMirrorHorizontalAxis;
		}
	
	ASSERT(EFalse);
	return -1;
	}

void CAsyncTransformer::RunL()
	{
	iResultStatus = iStatus.Int();
	User::LeaveIfError( iStatus.Int() );

	switch (iState)
		{
		//coverity[unterminated_case]
		case EStart:
				// go further
			CreateDestBitmapsL();
		//coverity[fallthrough]
		case EClip:
			{
			if (iSingleton->ClippingSet())
				{
				DeletePreTranformBmps();
				DoClipL(iCurrentSource, iClippingDest);
				iCurrentSource	= iClippingDest;
				iState=ETransform;
				RunAgain();
				break;
				}
			} // if no clipping just go further
		case ETransform:
			{
			iState = ETransform;
			if (iSingleton->TransformOptions()==0)
				{
				iState = EScale;
				RunAgain();
				break;
				}
			if (iTransformTodo==0)
				{
				iState = ETransformDone;
				}
			else
				{
				CBitmapRotator::TRotationAngle transfromInstr=CBitmapRotator::TRotationAngle(NextTransform(iTransformTodo));
				if (iCurrentSource != iTransformDest)
					{
					iSingleton->RotatorL().Rotate(&iStatus, *iCurrentSource, *iTransformDest, transfromInstr);
					}
				else
					{
					iSingleton->RotatorL().Rotate(&iStatus, *iTransformDest, transfromInstr);
					}
				
				iCurrentSource = iTransformDest;
				SetActive();
				break;
				}
			}
		case ETransformDone:
			{
			iCurrentSource	= iTransformDest;
			} // and go to scale
		case EScale:
			iState = EScale;
			if (iClippingDest!=iCurrentSource)
				{
				DeletePreTranformBmps();
				}			
			if (iScaleNeeded)
				{
				CBitmapScaler& scaler=iSingleton->ScalerL();
				User::LeaveIfError( scaler.SetQualityAlgorithm( CBitmapScaler::TQualityAlgorithm(iScaleQuality)) );
				scaler.Scale(&iStatus, *iCurrentSource, *iScaleDest, EFalse);
				// the EFalse is used because the Scaler uses an algorithm that is different for one 
				// that is used by CImageDecoder to calculate scaled sizes, so sizes sometime differ.
				// there is no workaround for now....
				iCurrentSource	= iScaleDest;
				SetActive();
				iState = EDone;
				break;
				}
		case EDone:
			iState = EDone;
			if (iClippingDest!=iCurrentSource)
				{
				DeletePreTranformBmps();
				}
			if (iTransformDest!=iCurrentSource)
				{
				delete iTransformDest;
				iTransformDest = NULL;
				}				
			iObserver.OnTransformDone(iResultStatus);
			break;
		default:
			ASSERT(EFalse);
		}
	}

void CAsyncTransformer::Cleanup()
	{
	iSingleton->Cleanup();
	DeletePreTranformBmps();
	}

void CAsyncTransformer::DoCancel()
	{
	Cleanup();
	iStatus = KErrCancel;
	}

void CAsyncTransformer::CreateDestBitmapsL()
	{
	delete iScaleDest; iScaleDest = NULL;

	iScaleDest = new (ELeave) CFbsBitmap();
	User::LeaveIfError(iScaleDest->Create(iDestSize, iCurrentSource->DisplayMode()) );

	delete iTransformDest; iTransformDest= NULL;

	iTransformDest = new (ELeave) CFbsBitmap();
	User::LeaveIfError(iTransformDest->Create(TSize(1,1), iCurrentSource->DisplayMode()) );
	}

void CAsyncTransformer::DeletePreTranformBmps()
	{
	delete iClippingDest;
	iClippingDest = NULL;
	}

void CAsyncTransformer::DoClipL(CFbsBitmap* aSrc, CFbsBitmap*& aDest)
	{
	ASSERT(aDest==NULL);
	aDest = new (ELeave) CFbsBitmap();
	User::LeaveIfError( aDest->Create(iTrueClipRect.Size(), aSrc->DisplayMode()) );
	
	CFbsBitmapDevice* device=CFbsBitmapDevice::NewL(aDest);
	CleanupStack::PushL(device);
	CFbsBitGc* gc=CFbsBitGc::NewL();
	CleanupStack::PushL(gc);
	gc->Activate(device);
	gc->BitBlt(TPoint(0,0), aSrc, iTrueClipRect);
	CleanupStack::PopAndDestroy(2,device);
	}

void CAsyncTransformer::Transform(CFbsBitmap* aImage, TBool aMantainAspect)
	{
	ASSERT(!IsActive());

	iImgSource	= iCurrentSource = aImage;
	iKeepAspect	= aMantainAspect;

	const TSize srcSize(aImage->SizeInPixels());
	
	iTrueClipRect = TRect(TPoint(0,0), srcSize.AsPoint());
	TSize clipRectSize(srcSize);
	if (iSingleton->ClippingSet())
		{
		const TRect& clipRect=iSingleton->ClipRect();
		iTrueClipRect.iTl.iX=MulDiv(clipRect.iTl.iX, srcSize.iWidth, iSingleton->TrueSrcSize().iWidth);
		iTrueClipRect.iTl.iY=MulDiv(clipRect.iTl.iY, srcSize.iHeight,iSingleton->TrueSrcSize().iHeight);
		iTrueClipRect.SetWidth(MulDiv(clipRect.Width(), srcSize.iWidth, iSingleton->TrueSrcSize().iWidth));
		iTrueClipRect.SetHeight(MulDiv(clipRect.Height(),srcSize.iHeight, iSingleton->TrueSrcSize().iHeight));
		clipRectSize=ScaleSize(srcSize, iSingleton->TrueSrcSize(), clipRect.Size());
		}
	RotateSize(clipRectSize, iSingleton->TransformOptions());
	const TInt KMaxDelta=Min(7, Max(0, Max(iDestSize.iWidth / clipRectSize.iWidth, iDestSize.iHeight / clipRectSize.iHeight)-1 ));
	// apply some treshold to size comparison because of decoder rounding algorithm
	iScaleNeeded = (Abs(iDestSize.iWidth-clipRectSize.iWidth)>KMaxDelta || Abs(iDestSize.iHeight-clipRectSize.iHeight)>KMaxDelta);
	Restart();
	}


TInt CAsyncTransformer::SetScaleQuality(TInt aQualityValue)
	{
	iScaleQuality = aQualityValue;
	return KErrNone;
	}

void CAsyncTransformer::Restart()
	{
	iTransformTodo	= iSingleton->TransformOptions();
	iCurrentSource	= iImgSource;
	iState			= EStart;
	
	iResultStatus = KErrNone;
	RunAgain();
	}

void CAsyncTransformer::RunAgain()
	{
	SetActive();
	TRequestStatus* pR=&iStatus;
	User::RequestComplete(pR,KErrNone);
	}

/*static*/
CTransformerSharedData* CTransformerSharedData::NewL()
	{
	CTransformerSharedData* self = new (ELeave) CTransformerSharedData();
	self->AddRef();
	return self;
	}

void CTransformerSharedData::AddRef()
	{
	++iRefCount;
	}

void CTransformerSharedData::Release()
	{
	ASSERT(iRefCount>0);
	if (--iRefCount==0)
		{
		delete this;
		}
	}


void CTransformerSharedData::Cleanup()
	{
	delete iScaler;
	iScaler = NULL;
	delete iRotator;
	iRotator= NULL;
	}
	
CTransformerSharedData::~CTransformerSharedData()
	{
	ASSERT(iRefCount==0);
	Cleanup();
	}

inline 
CBitmapScaler& CTransformerSharedData::ScalerL()
	{
	if (iScaler==NULL)
		{
		iScaler=CBitmapScaler::NewL();
		}
	return *iScaler;
	}

inline 
CBitmapRotator&	CTransformerSharedData::RotatorL()
	{
	if (iRotator == NULL)
		{
		iRotator = CBitmapRotator::NewL();
		}
	return *iRotator;
	}

