// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <icl/imageconversionextension.h>
#include <icl/imagecodec.h>
#include <icl/imagedata.h>
#include "ImageClientMain.h"
#include "fwextconstants.h"
#include "pluginextensionmanager.h"
#include "ImageProcessorPriv.h"

const TUint KAllOperationCapabilities = TImageConvOperation::ERotation90DegreesClockwise |
										TImageConvOperation::ERotation180DegreesClockwise |
										TImageConvOperation::ERotation270DegreesClockwise |
										TImageConvOperation::EMirrorHorizontalAxis |
										TImageConvOperation::EMirrorVerticalAxis;

const TUint KImageCornerCount = 4;
const TUint8 KImageTLCoord = 0x00;
const TUint8 KImageTRCoord = 0x10;
const TUint8 KImageBLCoord = 0x01;
const TUint8 KImageBRCoord = 0x11;

/**
Generic extension container for codec plugin use.
*/
EXPORT_C CPluginExtensionManager* CPluginExtensionManager::NewL(MReadCodecExtension* aReadCodec)
	{
	return new (ELeave) CPluginExtensionManager(aReadCodec);
	}

CPluginExtensionManager::CPluginExtensionManager(MReadCodecExtension* aReadCodec)
:iReadCodec(aReadCodec)
	{
	}

EXPORT_C CPluginExtensionManager::~CPluginExtensionManager()
	{
	delete iOperationExtension;
	delete iScalerExtension;
	}

EXPORT_C void CPluginExtensionManager::ResetCodecExtension(MReadCodecExtension* aReadCodec)
	{
	iReadCodec = aReadCodec;
	}

EXPORT_C TInt CPluginExtensionManager::ConvertScalingCoeffToReductionFactor(TInt aScalingCoeff)
	{
	return aScalingCoeff == 1 ? 0 : -(aScalingCoeff+1);
	}

EXPORT_C void CPluginExtensionManager::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	switch(aExtUid.iUid)
		{
		case KUidImageConvExtOperationValue:
			{
			if(!iOperationExtension)
				{
				iOperationExtension = COperationExtension::NewL();
				aExtPtr = iOperationExtension;
				}
			else 
				{
				aExtPtr = iOperationExtension;
				//this extension might have been created for auto rotation before so reset the reference count
				if (!iOperationExtension->iRefCount) 
					{
					iOperationExtension->iRefCount = 1;
					}
				}
			break;
			}
		case KUidImageConvExtScalerValue:
			{
			if(!iScalerExtension)
				{
				iScalerExtension = CScalerExtension::NewL();
				aExtPtr = iScalerExtension;
				}
			else 
				{
				aExtPtr = iScalerExtension;
				}
			break;
			}
		default:
			{
			User::Leave(KErrNotSupported);
			}
		}
	}

EXPORT_C void CPluginExtensionManager::CreateExtensionForAutoRotateL() 
	{
	if(!iOperationExtension)
		{
		iOperationExtension = COperationExtension::NewL();
		iOperationExtension->iRefCount = 0;// this extension is not referenced as no high level API icl extension is created
		}
	}

/*
Called by the plugin with the size of the original image
*/
EXPORT_C TInt CPluginExtensionManager::GetDestinationSize(TSize& aOriginalSize)
	{
	TSize destinationSize = aOriginalSize;
	TInt err = KErrNone;
	
	if(!iClippingRect.IsEmpty())
		{
		// Check for validity, as client may have switched main<->thumb
		TRect clipRect(iClippingRect);
		clipRect.Intersection(aOriginalSize);
		if(clipRect == iClippingRect)
			{
			// Clipping rect set and valid
			destinationSize = iClippingRect.Size();
			}
		else
			{
			err = KErrArgument;
			return err;
			}
		}
	
	if(iOperationExtension && iOperationExtension->DimensionsSwapped())
		{
		// switch orientation
		destinationSize.SetSize(destinationSize.iHeight, destinationSize.iWidth);
		}

	if(iScalerExtension)
		{
		if(iScalerExtension->ScaleCmd() != CScalerExtension::EScalerExtFullSize)
			{
			TInt scalingCoeff = 1; // full size
			err = GetScalingCoefficient(scalingCoeff, &destinationSize);
			if(err != KErrNone)
				{
				return err;
				}

			ASSERT(iReadCodec);
			err = iReadCodec->GetReducedSize(destinationSize, scalingCoeff, destinationSize);
			if(err != KErrNone)
				{
				return err;
				}
			}
		}

	aOriginalSize = destinationSize;
	return err;
	}

/*
Validate the clipping rectangle.
*/
EXPORT_C void CPluginExtensionManager::SetClippingRectL(const TRect* aClipRect, const RPointerArray<TFrameInfo>& aFrameInfo)
	{
	if(!aClipRect)
		{
		//reset and return
		iClippingRect.SetRect(0,0,0,0);
		return;
		}

	if(aClipRect->IsEmpty() || !aClipRect->IsNormalized())
		{
		User::Leave(KErrArgument);
		}

	// Check if this crop rect falls within at least one frame.
	for(TInt i = 0; i < aFrameInfo.Count(); i++)
		{
		const TFrameInfo& frame = *aFrameInfo[i];

		TRect clipTest(*aClipRect);
		clipTest.Intersection(frame.iFrameCoordsInPixels);
		if(clipTest == *aClipRect) // clipping rect within/same as the image rect
			{
			iClippingRect = *aClipRect;
			return;
			}
		}
	User::Leave(KErrArgument);
	}

/*
Has the clipping rectangle extension been set?
*/
EXPORT_C TBool CPluginExtensionManager::ClippingRectExtensionRequested() const
	{
	if(!iClippingRect.IsEmpty())
		{
		return ETrue;
		}
	return EFalse;
	}


/*
Has the scaler extension been set?
*/
EXPORT_C TBool CPluginExtensionManager::ScalerExtensionRequested() const
	{
	if (iScalerExtension)
		{
		return (iScalerExtension->ScaleCmd() != CScalerExtension::EScalerExtFullSize);
		}

 	return EFalse;
	}

/*
Has the operation extension been set?
*/
EXPORT_C TBool CPluginExtensionManager::OperationExtensionRequested() const
	{
	if(iOperationExtension && iOperationExtension->HasBeenSet())
		{
		return ETrue;
		}
	return EFalse;
	}

/*
Return the clipping rectangle (may or may not have been set by client).
*/
EXPORT_C TRect CPluginExtensionManager::ClippingRect()
	{
	return iClippingRect;
	}

/*
Returns the scaling coefficient set by the client.  If the scaling coefficient
has been set directly by the client (i.e. calling
TImageConvScaler::SetScalingL(TInt aScalingCoeff, ...) then this function will
return that.  If the client has set scaling using the desired size (i.e.
calling TImageConvScaler::SetScalingL(TSize& aDesiredSize, ...) then this function
will calculate the appropriate scaling coefficient.

@param	aScalingCoeff 		The scaling coefficient to be returned.
@param	aOriginalSize		A pointer to the original size of the image.  Default is NULL.
@return KErrNone 			If the call was successful.
@return KErrArgument		If the aOriginalSize argument is NULL when
							SetScaling() has been set with a desired size.
@return KErrNotSupported	If client has not requested a scaling operation or the plugin
							is not capable of scaling to the set size.
@return						One of the system wide error codes.

@see TImageConvScaler::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
@see TImageConvScaler::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
@see TScalerCaps
*/
EXPORT_C TInt CPluginExtensionManager::GetScalingCoefficient(TInt& aScalingCoeff, const TSize* aOriginalSize) const
	{
	if(iScalerExtension)
		{
		switch(iScalerExtension->ScaleCmd())
			{
			case CScalerExtension::EScalerExtFullSize:
				{
				aScalingCoeff = 1; // Full size
				return KErrNone;
				}
			case CScalerExtension::EScalerExtUseScalingCoeff:
				{
				return iScalerExtension->GetScalingCoefficient(aScalingCoeff);
				}
			case CScalerExtension::EScalerExtUseDesiredSize:
				{
				// desired size has been set so we need to validate the
				// aOriginalSize arg, which can be NULL
				if(aOriginalSize == NULL)
					{
					return KErrArgument;
					}

				TSize reducedSize;
				TInt err = iScalerExtension->GetDesiredSize(reducedSize);
				if(err != KErrNone)
					{
					return err;
					}

				ASSERT(iReadCodec);
				aScalingCoeff = iReadCodec->ScalingCoefficient(*aOriginalSize, reducedSize);
				if(!iScalerExtension->ScalingCoefficientSupported(aScalingCoeff))
					{
					return KErrNotSupported;
					}
				return KErrNone;
				}
			default:
				{
				Panic(EInvalidFwExtensionCall);
				}
			}
		}
	return KErrNotSupported;
	}

/*
Get the desired scaler size set by client.
*/
EXPORT_C TInt CPluginExtensionManager::GetScalerDesiredSize(TSize& aDesiredSize) const
	{
	if(iScalerExtension)
		{
		return iScalerExtension->GetDesiredSize(aDesiredSize);
		}
	return KErrNotSupported;
	}

/*
Get the scaler quality setting set by client.
*/
EXPORT_C TInt CPluginExtensionManager::GetScalerQuality(TImageConvScaler::TScalerQuality& aQuality) const
	{
	if(iScalerExtension)
		{
		return iScalerExtension->GetScalerQuality(aQuality);
		}
	return KErrNotSupported;
	}

/*
Has the client requested that scaling, when using the desired size, is to
maintain the aspect ratio.
*/
EXPORT_C TBool CPluginExtensionManager::ScalerMaintainAspectRatio() const
	{
	if(iScalerExtension)
		{
		return iScalerExtension->LockAspectRatioSet();
		}
	return EFalse;
	}

/*
Passes the data that has been set in the extensions by the client, to the image processor.

@param aImageProcessor	A pointer to the CImageProcessorExtension that is to have the extension data
						passed to it.
@leave					A range of system wide error codes.
*/
EXPORT_C void CPluginExtensionManager::TransferExtensionDataL(CImageProcessorExtension* aImageProcessor)
	{
	ASSERT(aImageProcessor);

	if(!iClippingRect.IsEmpty())
		{
		aImageProcessor->SetClippingRect(iClippingRect);
		}

	if(iScalerExtension)
		{
		switch(iScalerExtension->ScaleCmd())
			{
			case CScalerExtension::EScalerExtFullSize:
				{
				TInt scalingCoeff = 1; // assume full size
				aImageProcessor->SetScaling(scalingCoeff);
				break;
				}
			case CScalerExtension::EScalerExtUseScalingCoeff:
				{
				TInt scalingCoeff = 1; // assume full size
				User::LeaveIfError(iScalerExtension->GetScalingCoefficient(scalingCoeff));
				aImageProcessor->SetScaling(scalingCoeff);
				break;
				}
			case CScalerExtension::EScalerExtUseDesiredSize:
				{
				TSize desiredSize;
				User::LeaveIfError(iScalerExtension->GetDesiredSize(desiredSize));
				aImageProcessor->SetScaling(desiredSize);
				break;
				}
			default:
				{
				Panic(EInvalidFwExtensionCall);
				}
			}
		}

	aImageProcessor->SetOperation(Operation());
	}

/**
Gets the operations that have been set in the operation extension.  This is in the
form of TTransformOptions and so the caller can determine the overall orientation.

@return TTransformOptions	The orientation of the image.

@see TTransformOptions
@see MImageConvOperation::AddOperationL
*/
EXPORT_C TTransformOptions CPluginExtensionManager::Operation() const
	{
	if(iOperationExtension)
		{
		return iOperationExtension->Operation();
		}
	return EDecodeNormal;
	}

/**
Gets the operations that have been set in the operation extension. If the auto rotation flag 
is set the auto rotation operations are inserted before other operations. 
This is in the form of TTransformOptions and so the caller can determine the overall orientation.

@param aAutoRotationFlag	An auto rotation flag to define auto rotation operations required.

@return TTransformOptions	The orientation of the image.

@see TTransformOptions
@see MImageConvOperation::AddOperationL
*/
EXPORT_C TTransformOptions CPluginExtensionManager::OperationL(TUint16 aAutoRotationFlag) const
	{
	if(iOperationExtension)
		{
		switch (aAutoRotationFlag)
			{
			case 1:
				break;
		
			case 2:
				iOperationExtension->InsertOperationL(TImageConvOperation::EMirrorHorizontalAxis, 0);
				break;

			case 3:
				iOperationExtension->InsertOperationL(TImageConvOperation::ERotation180DegreesClockwise, 0);
				break;
			
			case 4:
				iOperationExtension->InsertOperationL(TImageConvOperation::EMirrorVerticalAxis, 0);
				break;
			
			case 5:
				iOperationExtension->InsertOperationL(TImageConvOperation::EMirrorVerticalAxis, 0);
				iOperationExtension->InsertOperationL(TImageConvOperation::ERotation90DegreesClockwise, 0);
				break;
			
			case 6:
				iOperationExtension->InsertOperationL(TImageConvOperation::ERotation90DegreesClockwise, 0);
				break;
			
			case 7:
				iOperationExtension->InsertOperationL(TImageConvOperation::EMirrorHorizontalAxis , 0);
				iOperationExtension->InsertOperationL(TImageConvOperation::ERotation90DegreesClockwise, 0);
				break;
			
			case 8:
				iOperationExtension->InsertOperationL(TImageConvOperation::ERotation270DegreesClockwise, 0);
				break;
			}
		}
		
	return iOperationExtension->Operation();
	}

/**
Based on the client-provided operations, have the dimensions of the image been
swapped?
*/
EXPORT_C TBool CPluginExtensionManager::DimensionsSwapped() const
	{
	return (iOperationExtension && iOperationExtension->DimensionsSwapped());
	}

/**
Generic plugin codec operation extension
*/
COperationExtension* COperationExtension::NewL()
	{
	return new (ELeave) COperationExtension();
	}

COperationExtension::COperationExtension()
:iRefCount(1), iCapabilities(KAllOperationCapabilities)
	{
	}

COperationExtension::~COperationExtension()
	{
	ASSERT(iRefCount == 0);
	iOperationStack.Close();
	}

TUid COperationExtension::Uid() const
	{
	return KICLOperationUid;
	}

void COperationExtension::IncrementRef()
	{
	iRefCount++;
	}

void COperationExtension::Release()
	{
	iRefCount--;
	}

TUint COperationExtension::Capabilities() const
	{
	return iCapabilities;
	}

void COperationExtension::AddOperationL(TImageConvOperation::TOperation aOperation)
	{
	iOperationStack.AppendL(aOperation);
	}

void COperationExtension::InsertOperationL(TImageConvOperation::TOperation aOperation, TInt aPos)
	{
	iOperationStack.InsertL(aOperation, aPos);
	}

void COperationExtension::ClearOperationStack()
	{
	iOperationStack.Reset();
	}

TBool COperationExtension::HasBeenSet()
	{
	return iOperationStack.Count() > 0;
	}

/**
Based on the client-provided operations, have the dimensions of the image been
swapped?
*/
TBool COperationExtension::DimensionsSwapped() const
	{
	TBool rotate = EFalse;
	for(TInt i = 0; i < iOperationStack.Count(); i++)
		{
		if(iOperationStack[i] == TImageConvOperation::ERotation90DegreesClockwise ||
			iOperationStack[i] == TImageConvOperation::ERotation270DegreesClockwise)
			{
			rotate = !rotate;
			}
		}
	return rotate;
	}

TTransformOptions COperationExtension::Operation() const
	{
	TUint8 imageCornerCoord[KImageCornerCount];
	imageCornerCoord[0] = KImageTLCoord;
	imageCornerCoord[1] = KImageTRCoord;
	imageCornerCoord[2] = KImageBLCoord;
	imageCornerCoord[3] = KImageBRCoord;

	for(TInt i = 0; i < iOperationStack.Count(); i++)
		{
		switch(iOperationStack[i])
			{
			case TImageConvOperation::ERotation90DegreesClockwise:
				{
				TUint8 tempVal = imageCornerCoord[0];
				imageCornerCoord[0] = imageCornerCoord[2];
				imageCornerCoord[2] = imageCornerCoord[3];
				imageCornerCoord[3] = imageCornerCoord[1];
				imageCornerCoord[1] = tempVal;

				break;
				}
			case TImageConvOperation::ERotation180DegreesClockwise:
				{
				TUint8 tempVal = imageCornerCoord[0];
				imageCornerCoord[0] = imageCornerCoord[3];
				imageCornerCoord[3] = tempVal;
				tempVal = imageCornerCoord[1];
				imageCornerCoord[1] = imageCornerCoord[2];
				imageCornerCoord[2] = tempVal;

				break;
				}
			case TImageConvOperation::ERotation270DegreesClockwise:
				{
				TUint8 tempVal = imageCornerCoord[0];
				imageCornerCoord[0] = imageCornerCoord[1];
				imageCornerCoord[1] = imageCornerCoord[3];
				imageCornerCoord[3] = imageCornerCoord[2];
				imageCornerCoord[2] = tempVal;

				break;
				}
			case TImageConvOperation::EMirrorHorizontalAxis:
				{
				TUint8 tempVal = imageCornerCoord[0];
				imageCornerCoord[0] = imageCornerCoord[2];
				imageCornerCoord[2] = tempVal;
				tempVal = imageCornerCoord[1];
				imageCornerCoord[1] = imageCornerCoord[3];
				imageCornerCoord[3] = tempVal;

				break;
				}
			case TImageConvOperation::EMirrorVerticalAxis:
				{
				TUint8 tempVal = imageCornerCoord[0];
				imageCornerCoord[0] = imageCornerCoord[1];
				imageCornerCoord[1] = tempVal;
				tempVal = imageCornerCoord[2];
				imageCornerCoord[2] = imageCornerCoord[3];
				imageCornerCoord[3] = tempVal;

				break;
				}
			default:
				{
				Panic(EInvalidFwExtensionCall);
				}
			}
		}
	return *(reinterpret_cast<TTransformOptions*>(&imageCornerCoord));
	}

/**
Generic plugin codec scaler extension
*/
CScalerExtension* CScalerExtension::NewL()
	{
	return new (ELeave) CScalerExtension();
	}

CScalerExtension::CScalerExtension()
:iRefCount(1), iCommand(CScalerExtension::EScalerExtFullSize),
iScalingCoefficient(1), iQuality(TImageConvScaler::EMediumQuality),
iLockAspectRatio(ETrue)
	{
	}

CScalerExtension::~CScalerExtension()
	{
	ASSERT(iRefCount == 0);
	}

TUid CScalerExtension::Uid() const
	{
	return KICLScalerUid;
	}

void CScalerExtension::IncrementRef()
	{
	iRefCount++;
	}

void CScalerExtension::Release()
	{
	iRefCount--;
	}

void CScalerExtension::GetCapabilities(TScalerCaps& aCaps) const
	{
	aCaps = TScalerCaps();
	}

void CScalerExtension::SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio)
	{
	if(aDesiredSize.iWidth <= 0 || aDesiredSize.iHeight <= 0)
		{
		User::Leave(KErrArgument);
		}

	if(!aLockAspectRatio && iCapabilities.MustPreserveAspectRatio())
		{
		User::Leave(KErrNotSupported);
		}

	// reset scaling coeff to full size
	iScalingCoefficient = 1;

	iCommand = CScalerExtension::EScalerExtUseDesiredSize;
	iDesiredSize = aDesiredSize;
	iQuality = aQuality;
	}

void CScalerExtension::SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality)
	{
	if(!ScalingCoefficientSupported(aScalingCoeff))
		{
		User::Leave(KErrNotSupported);
		}

	// reset desired size
	iDesiredSize.SetSize(0,0);

	// check if reset to full size
	if(aScalingCoeff == -1 || aScalingCoeff == 1)
		{
		iCommand = CScalerExtension::EScalerExtFullSize;
		}
	else
		{
		iCommand = CScalerExtension::EScalerExtUseScalingCoeff;
		}
	iScalingCoefficient = aScalingCoeff;
	iQuality = aScalingQuality;
	}

CScalerExtension::TScalerCmd CScalerExtension::ScaleCmd() const
	{
	return iCommand;
	}

TInt CScalerExtension::GetScalingCoefficient(TInt& aScalingCoeff) const
	{
	if(iCommand == CScalerExtension::EScalerExtUseScalingCoeff)
		{
		aScalingCoeff = iScalingCoefficient;
		return KErrNone;
		}
	return KErrNotSupported;
	}

TInt CScalerExtension::GetDesiredSize(TSize& aDesiredSize) const
	{
	if(iCommand == CScalerExtension::EScalerExtUseDesiredSize)
		{
		aDesiredSize = iDesiredSize;
		return KErrNone;
		}
	return KErrNotSupported;
	}

// Checks if a given scaling coefficient is supported.
TBool CScalerExtension::ScalingCoefficientSupported(TInt aScalingCoeff) const
	{
	if(aScalingCoeff > iCapabilities.MaxUpscaleLimit() ||
		aScalingCoeff < iCapabilities.MaxDownscaleLimit() ||
		aScalingCoeff == 0)
		{
		return EFalse;
		}
	return ETrue;
	}

TInt CScalerExtension::GetScalerQuality(TImageConvScaler::TScalerQuality& aQuality) const
	{
	aQuality = iQuality;
	return KErrNone;
	}

TBool CScalerExtension::LockAspectRatioSet() const
	{
	return iLockAspectRatio;
	}

