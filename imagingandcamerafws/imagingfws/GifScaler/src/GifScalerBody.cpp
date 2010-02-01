// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "NQColorQuantizer.h"
#include "FastColorQuantizer.h"
#include "GifScalerBody.h"
#include "GifScalerMain.h"
#include "gifscaler.h"

// Default transparency threshold.
const TUint8 KDefaultTransparencyThreshold = 128;

// Number of palette entries (8bpp)
const TInt KMaxPaletteEntries = 256;


/*
* NewL
* The function NewL constructs a CGifScalerBody
*
* @return CGifScalerBody*
*
*/
CGifScaler::CBody* CGifScaler::CBody::NewL(CFbsBitmap& aSource, CFbsBitmap* aSourceMask, CGifScaler::TOptions aOptions)
	{
	// Assert valid source bitmap.
	__ASSERT_ALWAYS(aSource.Handle() != 0, Panic(ENoSourceBitmap));

   if(aSource.ExtendedBitmapType()!=KNullUid)
        {
        User::Leave(KErrNotSupported);
        }
	
	// If any dimension is zero then send back an error.
	TSize sourceSize = aSource.SizeInPixels();
	if ((sourceSize.iWidth == 0) || (sourceSize.iHeight == 0))
		User::Leave(KErrArgument);

	if (aSourceMask)
		{
		// Assert valid source mask bitmap.
		__ASSERT_ALWAYS(aSourceMask->Handle() != 0, Panic(ENoSourceMaskBitmap));
		
		 if(aSourceMask->ExtendedBitmapType()!=KNullUid)
            {
            User::Leave(KErrNotSupported);
            }

		// If the mask dimensions are not the same as the source then send back an error.
		TSize maskSize = aSourceMask->SizeInPixels();
		if (maskSize != sourceSize)
			User::Leave(KErrArgument);

		// If the mask bitmap isn't in the correct display mode then send back an error.
		TDisplayMode maskMode = aSourceMask->DisplayMode();
		if ((maskMode != EGray2) && (maskMode != EGray256))
			User::Leave(KErrArgument);
		}

	// If the options are invalid send back an error.
	if ((aOptions<CGifScaler::ELowQualityQuantization) || (aOptions>CGifScaler::EMaximumQualityQuantization))
		User::Leave(KErrArgument);
	
	CBody* self = new(ELeave) CBody(aSource, aSourceMask, aOptions);
	CleanupStack::PushL(self);
	self->ConstructL();

	CleanupStack::Pop();
	return self;
	}


/*
*
* CGifScalerBody
* Constructor for this class. Adds itself to <code>CActiveScheduler</code>.
*
*/
CGifScaler::CBody::CBody(CFbsBitmap& aSource, CFbsBitmap* aSourceMask, CGifScaler::TOptions aOptions)
	: CActive(CActive::EPriorityIdle), iSource(aSource), iSourceMask(aSourceMask), iOptions(aOptions)
	{
	CActiveScheduler::Add(this);
	}


/*
* ConstructL
* Performs second phase of contruction
*
*/
void CGifScaler::CBody::ConstructL()
	{
	ASSERT(iCurrentState == EInactiveState);
	ASSERT(iBitmapScaler == NULL);
	iBitmapScaler = CBitmapScaler::NewL();
	}


/*
*
* ~CGifScalerBody
* This is the destructor for the CGifScalerBody 
* and is resposible for deallocating all resources 
* allocated by the CGifScalerBody
*
*/
CGifScaler::CBody::~CBody()
	{
	// Parent object should have cancelled the scaling,
	// so assert that we are not active here.
	ASSERT(!IsActive());

	ASSERT(iCurrentState == EInactiveState);
	ASSERT(iDestinationMask == NULL);
	delete iBitmapScaler;
	}

// Implementation of CGifScaler::Scale.
void CGifScaler::CBody::Scale(TRequestStatus* aStatus, CFbsBitmap& aDestination, CPalette& aPalette, TBool aMaintainAspectRatio)
	{
	Scale(aStatus, aDestination, aPalette, KDefaultTransparencyThreshold, aMaintainAspectRatio);
	}

// Implementation of CGifScaler::Scale (second version).
void CGifScaler::CBody::Scale(TRequestStatus* aStatus, CFbsBitmap& aDestination, CPalette& aPalette, TUint8 aTransparencyThreshold, TBool aMaintainAspectRatio)
	{
	// Panic if aStatus is NULL.
	__ASSERT_ALWAYS(aStatus != NULL, Panic(EBadRequestStatus));
	iRequestStatus = aStatus;
	*iRequestStatus = KRequestPending;

	// Panic if the target bitmap hasn't been created.
	__ASSERT_ALWAYS(aDestination.Handle() != 0, Panic(ENoDestinationBitmap));

	// If the target bitmap is not 256 color then send back an error.
	if (aDestination.DisplayMode() != EColor256)
		{
		RequestComplete(KErrArgument);
		return;
		}
	
   if(aDestination.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }

	// If the number of palette entries is not correct then send back an error.
	if (aPalette.Entries() != KMaxPaletteEntries)
		{
		RequestComplete(KErrArgument);
		return;
		}
	
	// Current state must be inactive.
	if (iCurrentState != EInactiveState)
		{
		RequestComplete(KErrGeneral);
		return;
		}

	// If any dimension is zero then send back KErrNone.
	TSize destSize = aDestination.SizeInPixels();
	if ((destSize.iWidth == 0) || (destSize.iHeight == 0))
		{
		RequestComplete(KErrNone);
		return;
		}

	iPalette = &aPalette;
	iPalette->Clear();
	iDestination = &aDestination;
	iTransparencyThreshold = aTransparencyThreshold;
	iMaintainAspectRatio = aMaintainAspectRatio;

	TInt err;
	if (iSourceMask != NULL)
		{
		ASSERT(iDestinationMask == NULL);
		iDestinationMask = new CFbsBitmap;
		if (!iDestinationMask)
			{
			RequestComplete(KErrNoMemory);
			return;
			}

		err = iDestinationMask->Create(destSize, EGray256);
		if (err != KErrNone)
			{
			delete iDestinationMask;
			iDestinationMask = NULL;
		
			RequestComplete(err);
			return;
			}
		}

	ASSERT(iIntermediate == NULL);
	iIntermediate = new CFbsBitmap;
	if (!iIntermediate)
		{
		delete iDestinationMask;
		iDestinationMask = NULL;
			
		RequestComplete(KErrNoMemory);
		return;
		}

	if (destSize == iSource.SizeInPixels())
		{
		// no scaling required
		iIntermediate->Duplicate(iSource.Handle());
		if (iSourceMask != NULL)
			{
			iDestinationMask->Duplicate(iSourceMask->Handle());
			}
		iCurrentState = EStartColorQuantization;

		SelfComplete(KErrNone);
		return;
		}

	err = iIntermediate->Create(destSize, EColor16M);
	if (err != KErrNone)
		{
		delete iDestinationMask;
		iDestinationMask = NULL;

		delete iIntermediate;
		iIntermediate = NULL;
		
		RequestComplete(err);
		return;
		}

	iCurrentState = EScalingState;

	// Start by scaling the source.
	iBitmapScaler->Scale(&iStatus, iSource, *iIntermediate, iMaintainAspectRatio);
	SetActive();
	}


/*
*
* RunL
* Handle the next chunk of scaling/color quantization.
*/
void CGifScaler::CBody::RunL()
	{
	if (iStatus.Int() != KErrNone)
		{
		iCurrentState = ECleanUpPending;
		}

	switch (iCurrentState)
		{
		case EScalingState:
			// Scaling of source successful, so
			// start scaling the mask if there is one.
			iCurrentState = EStartColorQuantization;
			if (iSourceMask)
				{
				iBitmapScaler->Scale(&iStatus, *iSourceMask, *iDestinationMask);
				SetActive();
				break;
				}

		case EStartColorQuantization:
			// Scaling is done,
			// so start color quantization if required.
			{
			// Resize the destination.
			// (The scaler may have changed the size)
			TSize intermediateSize = iIntermediate->SizeInPixels();
			if (intermediateSize != iDestination->SizeInPixels())
				{
				if ((intermediateSize.iWidth == 0) || (intermediateSize.iHeight == 0))
					{
					// Complete the operation if the scaler
					// has reduced either dimension to zero.
					iCurrentState = ECleanUpState;
					break;
					}

				User::LeaveIfError(iDestination->Resize(intermediateSize));
				if (iDestinationMask)
					{
					User::LeaveIfError(iDestinationMask->Resize(intermediateSize));
					}
				}

			// Initialize settings for color quantizer.
			TInt numPaletteEntries = iSourceMask ? KMaxPaletteEntries-1 : KMaxPaletteEntries;
			TInt sampleFactor = 1;
			switch (iOptions)
				{
				case CGifScaler::ELowQualityQuantization:
					sampleFactor = 10;
					break;

				case CGifScaler::EMediumQualityQuantization:
					sampleFactor = 6;
					break;

				case CGifScaler::EHighQualityQuantization:
					sampleFactor = 3;
					break;

				case CGifScaler::EMaximumQualityQuantization:
					sampleFactor = 1;
					break;

				default:
					ASSERT(0);
				}

			// decide which color quantizer to use
			// CFastColorQuantiser can be used if the number of unique colors in
			// the image data is less than numPaletteEntries
			TBool useFastColorQuantiser = EFalse;
			TInt totalPixels = intermediateSize.iWidth * intermediateSize.iHeight;
			TInt nonTransparentPixels = totalPixels;
			if (totalPixels <= numPaletteEntries)
				{
				useFastColorQuantiser = ETrue;
				}			
			else
				{
				// check the number of unique colors in the image data
				// if a transparency mask is supplied:
				// - exclude transparent pixels from the count
				// - update nonTransparentPixels
				
				CPalette* colorPalette = CPalette::NewL(numPaletteEntries);

				TPoint pixelPos(0, 0);
				TRgb pixelColor;
				TInt paletteIndex = 0;
				TBool tooManyColors = EFalse;
				TBool finished = EFalse;
				TBitmapUtil bitmapUtil(iIntermediate);
				bitmapUtil.Begin(pixelPos);

				TInt scanlineLengthMask = 0;
				TUint8* basePosMask = NULL;				

				if (iDestinationMask)
					{
					scanlineLengthMask = CFbsBitmap::ScanLineLength(intermediateSize.iWidth, iDestinationMask->DisplayMode());
					basePosMask = reinterpret_cast<TUint8*>(iDestinationMask->DataAddress());
					}
				
				for (; (pixelPos.iY < intermediateSize.iHeight) && !finished; pixelPos.iY++)
						{
						pixelPos.iX = 0;
						bitmapUtil.SetPos(pixelPos);
						for (; (pixelPos.iX < intermediateSize.iWidth) && !finished; pixelPos.iX++)
							{
							pixelColor.SetInternal(bitmapUtil.GetPixel());
							TBool pixelTransparent = iDestinationMask && (basePosMask[pixelPos.iX] <= iTransparencyThreshold);

							if (pixelTransparent)
								{
								nonTransparentPixels--;
								}
							else if (colorPalette->NearestEntry(pixelColor) != pixelColor)
								{
								if (paletteIndex < numPaletteEntries)
									{
									// current pixel color is not in the palette so add it
									colorPalette->SetEntry(paletteIndex, pixelColor);
									paletteIndex++;
									}
								else
									{
									// there are more colors in the image data than palette spaces
									tooManyColors = ETrue;
									if (!iDestinationMask)
										{
										// not counting nontransparent pixels so can finish
										finished = ETrue;
										}
									}
								}

							bitmapUtil.IncXPos();
							}

						basePosMask += scanlineLengthMask;
						}
					bitmapUtil.End();
					delete colorPalette;
				
					if (!tooManyColors)
						{
						useFastColorQuantiser = ETrue;
						}
					}


				// Create the color quantizer.
				if (useFastColorQuantiser)
					{
					// The number of pixels in the image is small,
					// so we can color quantize the image perfectly and quickly.
					iColorQuantizer = CFastColorQuantizer::NewL(iPalette, iTransparencyThreshold);
					}
				else
					{
					iColorQuantizer = CNQColorQuantizer::NewL(iPalette, numPaletteEntries, sampleFactor, iTransparencyThreshold);
					}
					
				// Kick off the color quantization.
				iColorQuantizer->Quantize(&iStatus, *iIntermediate, *iDestination, iDestinationMask, nonTransparentPixels);

				// Move to the cleanup state when the quantization is done.
				iCurrentState = ECleanUpPending;
				SetActive();
				}

			break;

		case ECleanUpPending:
			// All operations are complete,
			// so move to the cleanup state.
			iCurrentState = ECleanUpState;
			break;

		default:
			ASSERT(0);
			break;
		}

	if (iCurrentState == ECleanUpState)
		{
		CleanUp();
		iCurrentState = EScalingComplete;
		}

	if (iCurrentState == EScalingComplete)
		{
		RequestComplete(iStatus.Int());
		iCurrentState = EInactiveState;
		}
	}

/*
*
* RunError
* Handle leaving errors from RunL.
*/
TInt CGifScaler::CBody::RunError(TInt aError)
	{
	CleanUp();
	RequestComplete(aError);
	iCurrentState = EInactiveState;

	return KErrNone;
	}
	
/*
*
* DoCancel
* Called by active object to prematurely terminate this bitmap scaling.
*
*/
void CGifScaler::CBody::DoCancel()
	{
	// Cancel scaling.
	ASSERT(iBitmapScaler);
	iBitmapScaler->Cancel();

	// Cancel color quantization.
	if (iColorQuantizer)
		iColorQuantizer->Cancel();
	
	CleanUp();
	iCurrentState = EInactiveState;
	RequestComplete(KErrCancel);
	}

/*
*
* CleanUp
* Delete all the memory allocated during scaling.
*
*/
void CGifScaler::CBody::CleanUp()
	{
	// Delete the intermediate.
	delete iIntermediate;
	iIntermediate = NULL;

	// Delete the color quantizer.
	delete iColorQuantizer;
	iColorQuantizer = NULL;

	// Delete the destination mask.
	delete iDestinationMask;
	iDestinationMask = NULL;
	}
	
/*
* 
* RequestComplete
* @param "TInt aReason"
*
*/
void CGifScaler::CBody::RequestComplete(TInt aReason)
	{
	TRequestStatus* stat = iRequestStatus;
	User::RequestComplete(stat, aReason);
	}

/*
*
* SelfComplete
* This function activates the active object and 
* signals completion of the current asynchronous operation
*
* @param "TInt aReason"
*
*/
void CGifScaler::CBody::SelfComplete(TInt aReason)
	{
	SetActive();

	TRequestStatus* stat = &iStatus;
	User::RequestComplete(stat, aReason);
	}
