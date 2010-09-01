// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "BitmapTransformsPlugin.h"

#include "BitmapConverter.h"
#include <bitmtrans/bitmtranspanic.h>
#include <e32math.h>

const TInt KFixedPointBits = 4;
const TInt KFixedPointScale = (1 << KFixedPointBits);
const TInt KLinesPerCall = 10;
const TInt KBitsPerByte = 8;

/*
*The function NewL constructs a CBitmapScalerPlugin
*
*@return CBitmapScalerPlugin* 
*/
MBitmapScalerPlugin* CBitmapScalerPlugin::NewL()
	{
	CBitmapScalerPlugin* self = new(ELeave) CBitmapScalerPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
*
*CBitmapScalerPlugin()
*Constructor for this class. Adds itself to CActiveScheduler.
*/
CBitmapScalerPlugin::CBitmapScalerPlugin()
	: CActive(CActive::EPriorityIdle), iOrigDes(NULL, 0), iDestDes(NULL, 0),
	iQualityLevel(CBitmapScaler::EMaximumQuality)
	{
	CActiveScheduler::Add(this);
	}

/*
*
*ConstructL()
*Performs second phase of construction.
*/
void CBitmapScalerPlugin::ConstructL()
	{
	iScanlineBitmap = new(ELeave) CFbsBitmap;

	SetPostProcessingEnabled(ETrue);	
	ASSERT( GetCurrentState() == EInactiveState );
	}


/*
*
*~CBitmapScalerPlugin
*This is the destructor for the CBitmapScalerPlugin 
*and is resposible for deallocating all resources 
*allocated by the CBitmapScalerPlugin.
*/
CBitmapScalerPlugin::~CBitmapScalerPlugin()
	{
	Cancel();
	Cleanup();

	delete iScanlineBitmap;

	// should have been deleted by cleanup
	ASSERT(iScaleBitmap==NULL || !(iFlags & EScaleBitmapIsOwned));
	ASSERT(iFilterIndexTable==NULL);
	ASSERT(iFilterCoeffsTable==NULL);
	ASSERT(iDevice==NULL);
	ASSERT(iGc==NULL);
	ASSERT(iPostProcessBitmap == NULL );
	ASSERT(iBitmapConverter == NULL );
	ASSERT( GetCurrentState() == EInactiveState );
	}

/*
*This function performs deallocation of memory allocated by the class
*which is allocated on each scale as opposed to the entire liftime
*of the scaler object.
*/
void CBitmapScalerPlugin::Cleanup()
	{
	delete[] iFilterIndexTable; 
	iFilterIndexTable = NULL;
	delete[] iFilterCoeffsTable; 
	iFilterCoeffsTable = NULL;

	if (iOrigDes.Length() != 0)	// Test is required to avoid PANIC 45
		{
		User::Free( reinterpret_cast<TAny*>( const_cast<TUint8*>( iOrigDes.Ptr() ) ) );
		iOrigDes.SetLength(0);
		}

	if (iDestDes.Length() != 0)	// Test is required to avoid PANIC 45
		{
		User::Free( reinterpret_cast<TAny*>( const_cast<TUint8*>( iDestDes.Ptr() ) ) );
		iDestDes.SetLength(0);
		}

	if (iFlags & ECreatedTempBitmap)
		{
		delete iTempBitmap; 
		iTempBitmap = NULL;
		iFlags &= ~ECreatedTempBitmap;
		}

	// Reset the EIsSourceBitmapResized flag here
	if (iFlags & EIsSourceBitmapResized)
		{
		iFlags &= ~EIsSourceBitmapResized;
		ASSERT((iFlags & EIsSourceBitmapResized) == 0);
		}

	// Only delete the scale bitmap if we created it.
	if (iFlags & EScaleBitmapIsOwned)
		{
		delete iScaleBitmap;
		iFlags &= ~EScaleBitmapIsOwned;
		}
	iScaleBitmap = NULL;

	delete iGc; 
	iGc = NULL;
	delete iDevice; 
	iDevice = NULL;
	iDestBmpPtr = NULL; // we don't own it
	delete iPostProcessBitmap; 
	iPostProcessBitmap = NULL;
	delete iBitmapConverter;
	iBitmapConverter = NULL;
	SetCurrentState( EInactiveState );
	}

/*
* Allocates some memory and assigns a pointer to it.
* @param aPtr Pointer to the allocated memory
* @param aAllocSize The size in bytes of the memory to allocate
* @pre aAllocSize >= 0
* @pre InVariant
*/
TInt CBitmapScalerPlugin::AllocPtr( TPtr8& aPtr, TInt aAllocSize )
	{
	//[ preconditions ]
	ASSERT(aAllocSize >= 0 );
	ASSERT(InVariant());

	TInt rc = KErrNone;

	// User::Alloc() returns NULL if no memory
	TUint8* addr = static_cast<TUint8*>(User::Alloc(aAllocSize + sizeof(TUint32) ));
	if (addr)
		{
		// coverity [memory_leak]
		addr = reinterpret_cast<TUint8*>( _ALIGN_UP(TLinAddr(addr), sizeof(TUint32)) );
		aPtr.Set(addr, aAllocSize, aAllocSize);
		}
	else
		{
		rc = KErrNoMemory;
		}
	
	return rc;
	}

// MBitmapScalerPlugin::Cancel() calls CActive::Cancel()
void CBitmapScalerPlugin::Cancel()
	{
	CActive::Cancel();
	}

/*
*
* ScaleRequestPostcondition
* @return 'ETrue' when post conditions for scale request are 
* correct
*
*/
TBool CBitmapScalerPlugin::ScaleRequestPostcondition() const 
	{
	TBool result = ETrue;

	if (QualityAlgorithm() != CBitmapScaler::EMaximumQuality )
		{
		if(	( iFinalOffset != iDestSize.iHeight ) ||
			( iFilterIndexTable != NULL ) ||
			( iFilterCoeffsTable != 0 ) )
			{
			result = EFalse;
			}
		}
	else
		{
		// Using the original (maximum quality) algorithm
		if(	( iFlags & EVerticalScan) ||
			( iFilterIndexTable == NULL ) ||
			( iFinalOffset != iOrigSize.iHeight ) ||
			( iFilterCoeffsTable == 0 ) )
			{
			result = EFalse;
			}

	if (( iDevice == NULL ) ||
		( iScanlineBitmap->Handle() == 0 ) ||
		( iCurOffset != 0 ))
		{
		result = EFalse;
			}			
		}

	return result;
	}

/**
 *
 * Begins the bitmap re-scaling operation.
 *
 * The scaling factor is based on the relative value of the source bitmap
 * size and the explicitly supplied size. The operation is asynchronous.
 * When it is complete, successfully or otherwise, the
 * <code>TRequestStatus</code>& is set, passing the state of the operation.
 *
 * @param     "TRequestStatus* aRequestStatus"
 *
 * @param     "CFbsBitmap& aBitmap"
 *            The bitmap to be re-scaled. This reference is also the
 *            target location for the re-scaled bitmap.
 * @param     "const TSize& aDestinationSize"
 *            The requested target size for the re-scaled bitmap.
 * @param     "TBool aMaintainAspectRatio = ETrue"
 *            <code>ETrue</code> - the aspect ratio is retained;
 *            this is the default. The same scaling factor is
 *            applied in both the horizontal and vertical
 *            directions. This is the smaller of the horizontal
 *            scaling factor and the vertical scaling factor.
 *            <code>EFalse</code> - the aspect ratio need not be
 *            retained.
 *
 */
void CBitmapScalerPlugin::Scale(TRequestStatus* aRequestStatus,
									 CFbsBitmap& aBitmap,
									 const TSize& aDestinationSize,
									 TBool aMaintainAspectRatio)
	{
	//[ assert preconditions ]
	// [ panic if aRequestStatus is NULL ]
	__ASSERT_ALWAYS( (aRequestStatus != NULL), Panic( EBadArgumentScale ) );

	//[ panic if the src has not been created]
	__ASSERT_ALWAYS( (aBitmap.Handle() != 0), Panic( ENoSourceBitmap ) );
	
	iScaleStatus = aRequestStatus;
	*iScaleStatus = KRequestPending;

	if(aBitmap.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }

	// Set up the sizes before they're lost
	iOrigSize = aBitmap.SizeInPixels();
	iDestSize = aDestinationSize;

	// Dithering is not required if the tgt's display mode
	// is none of EGray256, EColor16M, EColor16MU, EColor16MA
	TBool dither = ETrue;
	iTgtDisplayMode = aBitmap.DisplayMode();
	if ((iTgtDisplayMode == EGray256) ||
		(iTgtDisplayMode == EColor16M) ||
		(iTgtDisplayMode == EColor16MA)||
		(iTgtDisplayMode == EColor16MU) )
		{
		dither = EFalse;
		}

	// Create a temporary bitmap if we're not using the low memory algorithm
	// OR if we're using the low memory algorithm AND dithering
	TInt leaveErr = KErrNone;
	if (!UseLowMemoryAlgorithm() || 
		(UseLowMemoryAlgorithm() && dither) )
		{
		if (iFlags & ECreatedTempBitmap)
			{
			delete iTempBitmap;
			iTempBitmap = NULL;
			iFlags &= ~ECreatedTempBitmap;
			}

		iTempBitmap = new CFbsBitmap;

		if (iTempBitmap != NULL)
			{
			// iTempBitmap has been created so flag that it needs to be deleted
			iFlags |= ECreatedTempBitmap;
			leaveErr = iTempBitmap->Create(aDestinationSize, iTgtDisplayMode);
			}
		else
			{
			// No memory to new the temp bitmap
			leaveErr = KErrNoMemory;
			}
		}
	
	if (leaveErr == KErrNoMemory ||
		(UseLowMemoryAlgorithm() && !dither) )
		{
		// Either failed to create the tempbitmap OR
		// we explicitly want to use the low-memory algorithm.
		// Either way use the source directly in the manner of the low-memory algorithm.
		// Check if we need to delete the tempbitmap 
		if (iFlags & ECreatedTempBitmap)
			{
			delete iTempBitmap;
			iTempBitmap = NULL;
			iFlags &= ~ECreatedTempBitmap;
			}
		iTempBitmap = &aBitmap;
		iFlags |= EIsSourceBitmapResized;
		}
	else if (leaveErr != KErrNone)
		{
		iScaleStatus = aRequestStatus;
		*iScaleStatus = KRequestPending;
		ProcessError(leaveErr);
		return;
		}

	Scale(aRequestStatus, aBitmap, *iTempBitmap, aMaintainAspectRatio);
	}

/*
 *
 * Begins the bitmap re-scaling operation.
 *
 * The scaling factor is based on the relative sizes of the source
 * and target bitmaps. The operation is asynchronous. When it is
 * complete, successfully or otherwise, the TRequestStatus &
 * aStatus is set, passing the state of the operation.
 *
 * @param     TRequestStatus* aRequestStatus
 *
 * @param     "CFbsBitmap& aSrcBitmap"
 *            The bitmap to be re-scaled.
 * @param     "CFbsBitmap& aTgtBitmap"
 *            The target location for the re-scaled bitmap.
 * @param     "TBool aMaintainAspectRatio = ETrue"
 *            ETrue  - the aspect ratio is retained;
 *            this is the default. The same scaling factor is
 *            applied in both the horizontal and vertical
 *            directions. This is the smaller of the horizontal
 *            scaling factor and the vertical scaling factor.
 *             EFalse  - the aspect ratio need not be
 *            retained.
 *
 *
 */
void CBitmapScalerPlugin::Scale(TRequestStatus* aRequestStatus,
							  CFbsBitmap& aSrcBitmap,
							  CFbsBitmap& aTgtBitmap,
							  TBool aMaintainAspectRatio)
	{
	// [ precondition checking on arguments ]
	// [ panic if aRequestStatus is NULL ]
	__ASSERT_ALWAYS( (aRequestStatus != NULL), Panic( EBadArgumentScale ) );
	
	//[ panic if the src has not been created]
	__ASSERT_ALWAYS( (aSrcBitmap.Handle() != 0), Panic( ENoSourceBitmap ) );
	
	//[ panic if the tgt has not been created]
	__ASSERT_ALWAYS( (aTgtBitmap.Handle() != 0), Panic( ENoDestinationBitmap ) );

	iScaleStatus = aRequestStatus;
	*iScaleStatus = KRequestPending;

	//[ current state must be inactive ]
	if( GetCurrentState() != EInactiveState )
		{
		ProcessError( KErrGeneral );
		return;
		}
		
	ASSERT(iDevice==NULL);
	
	if(aSrcBitmap.ExtendedBitmapType()!=KNullUid || aTgtBitmap.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }
	
	iSrcBitmap = &aSrcBitmap;
	iTgtBitmap = &aTgtBitmap;

	if (!(iFlags & EIsSourceBitmapResized))
		{
		// As we're not scaling directly to the source bitmap
		// we can determine the scaling sizes here (otherwise
		// we'd overwrite the values that we got prior to setting
		// the tgtBitmap to the srcBitmap!!)
		iOrigSize = iSrcBitmap->SizeInPixels();
		iDestSize = iTgtBitmap->SizeInPixels();
		}

	TInt err = KErrNone;

	// Target's colour depth
	iTgtDisplayMode = iTgtBitmap->DisplayMode();

	// Check the aspect ratio and adjust the resolution if needed
	if (aMaintainAspectRatio)
		{
		//[ update destination height to preserve aspect ratio ]
	    HeightAndWidthPreservingAspectRatio( iDestSize, iOrigSize );
		}

	iIntermediateSize.iWidth = iDestSize.iWidth;
	if ( QualityAlgorithm() != CBitmapScaler::EMaximumQuality )
		{
		// Using one of the lower quality algorithms so use the destination size
		// that we are scaling to.
		iIntermediateSize.iHeight = iDestSize.iHeight;
		}
	else
		{
		// Using the original algorithm (which firstly scales horizontally to
		// an intermediate bitmap), so the intermediate size requires the original
		// height unchanged.
		iIntermediateSize.iHeight = iOrigSize.iHeight;
		}

	// if either of the destination dimension is zero then 
	// complete with KErrNone
	if ((iDestSize.iWidth == 0) ||
		(iDestSize.iHeight == 0))
		{
		ProcessError(KErrNone);
		return;
		}

	// if either of the source dimension is zero then send back an error 
	if ((iOrigSize.iWidth == 0) || 
		(iOrigSize.iHeight == 0)) 
		{
		ProcessError( KErrArgument );
		return;
		}

	// Scale to bitmap of display mode EColor16M except for 
	// target bitmaps of EGray256, EColor16MU and EColor16MA
	if ( (iTgtDisplayMode == EGray256) || (iTgtDisplayMode == EColor16MU) 
			|| (iTgtDisplayMode == EColor16MA))
		{
		iLocalDisplayMode = iTgtDisplayMode;
		}
	else
		{
		iLocalDisplayMode = EColor16M;
		}

	if (iTgtDisplayMode != EColor16M && iTgtDisplayMode != EColor16MA && iTgtDisplayMode != EColor16MU)
		{
		// Create an intermediate bitmap to scale to for cases other than EColor16M
		if (iScaleBitmap == NULL)
			{
			// Only want one instance of iScaleBitmap
			iScaleBitmap = new CFbsBitmap;
			iFlags |= EScaleBitmapIsOwned;	// flag that we're the owner of the scale bitmap
			if (iScaleBitmap == NULL)
				{
				err = KErrNoMemory;
				}
			}
		if( err != KErrNone )
			{
			ProcessError( err );
			return;
			}

		err = iScaleBitmap->Create(iIntermediateSize, iLocalDisplayMode);

		if( err != KErrNone )
			{
			ProcessError( err );
			return;
			}
			
		iDestBmpPtr = iScaleBitmap;
		}
	else
		{
		// For EColor16M scale directly to TgtBitmap
		iScaleBitmap = iTgtBitmap;

		// Downscaling one bitmap to another requires 
		// the scale bitmap to be resized to ensure that
		// the correct amount of data is read from the source.
		if (!(iFlags & EIsSourceBitmapResized))
			{
			err = iScaleBitmap->Resize(iIntermediateSize);
			if (err != KErrNone)
				{
				ProcessError(err);
				return;
				}
			}
		}

	iComponentsPerPixel = TDisplayModeUtils::NumDisplayModeBitsPerPixel(iLocalDisplayMode) / KBitsPerByte;
	if (iComponentsPerPixel == 0)
		{
		iComponentsPerPixel = 1;
		}

		iSrcY = 0;
	if (iDestSize.iHeight < iOrigSize.iHeight)
		{
		iCurY = 0;
		}
	else
		{
		iCurY = (iOrigSize.iHeight << KFixedPointScale);
		}

	if (QualityAlgorithm() == CBitmapScaler::EMaximumQuality)
		{
		// Try to create and size the scanline bitmap here instead of in the ConstructL
	if ((err = iScanlineBitmap->Create(TSize(1, iDestSize.iHeight), iLocalDisplayMode)) != KErrNone)
		{
		ProcessError(err);
		return;
		}
		}

	// Upscaling so ensure we're writing to a suitably
	// large target
	TInt newWidth = iTgtBitmap->SizeInPixels().iWidth;
	TInt newHeight = iTgtBitmap->SizeInPixels().iHeight;
	
	TBool needsResize = EFalse;
	if(iDestSize.iWidth > iOrigSize.iWidth)
		{
		newWidth = iDestSize.iWidth;
		needsResize = ETrue;
		}
		
	if(iDestSize.iHeight > iOrigSize.iHeight)
		{
		newHeight = iDestSize.iHeight;	
		needsResize = ETrue;
		}
		
	if(needsResize)
		{
		err = iTgtBitmap->Resize(TSize(newWidth, newHeight));
			
		if( err != KErrNone )
			{
			ProcessError(err);
			return;
			}	
		}

	if (QualityAlgorithm() == CBitmapScaler::EMaximumQuality)
		{
		// [Create device for normal condition where target bitmap is the
		// destination]
	TRAP(err, iDevice = CFbsBitmapDevice::NewL(iTgtBitmap));
	if( err != KErrNone )
		{
		ProcessError(KErrNoMemory);
		return;
			}
		}

	iDestBmpPtr = iTgtBitmap;

	SetProcessingNeeded( EFalse );

	//[ Is Post processing required ]
	if( ( iTgtDisplayMode != EColor16M) && 
		( iTgtDisplayMode != EColor16MU) &&
		( iTgtDisplayMode != EColor16MA) &&
		( iTgtDisplayMode != EGray256) &&
		(! IsPostProcessingDisabled() )  ||
		((iTgtDisplayMode == EGray256) && (QualityAlgorithm() != CBitmapScaler::EMaximumQuality) &&
											(iLocalDisplayMode != EGray256))
		)
		{
		SetProcessingNeeded( ETrue );	
		
		//[ create a bitmap converter to colour quantize the image
		// to the number of colours available in the output
		// note CFbsBitmap has a private constructor whence new is not
		//used ]
		TRAP(err, iBitmapConverter = CBitmapConverter::NewL());
		if(err != KErrNone )
			{
			ProcessError(err);
			return;
			}
		
		//[ create a post processing bitmap]
		iPostProcessBitmap = new CFbsBitmap;
		if( iPostProcessBitmap == NULL )
			{
			err = KErrNoMemory;
			ProcessError(err);
			return;
			}
		
		//[ size the post processing bitmap ]
		if ((err = iPostProcessBitmap->Create(iDestSize, EColor16M )) != KErrNone)
			{
			ProcessError(err);
			return;
			}
		
		// Create device with post processing bitmap as the target
		delete iDevice;
		iDevice = NULL;
		iDestBmpPtr = NULL;
		if (QualityAlgorithm() == CBitmapScaler::EMaximumQuality)
			{
		TRAP(err, iDevice = CFbsBitmapDevice::NewL(iPostProcessBitmap));
		if( err != KErrNone )
			{
			ProcessError(err);
			return;
				}			
			}
		iDestBmpPtr = iPostProcessBitmap;
		}
	
	iCurOffset = 0;
	iCurOffsetUp = iDestSize.iHeight;
	if (QualityAlgorithm() != CBitmapScaler::EMaximumQuality)
		{
		// Using one of the lower quality algorithms
		iFinalOffset = iDestSize.iHeight;
		if (iTgtBitmap->SizeInPixels() != iDestSize && iTgtBitmap->Handle() != iSrcBitmap->Handle())
			{
			err = iTgtBitmap->Resize(iDestSize);
			if( err != KErrNone )
				{
				ProcessError(err);
				return;
				}
			}
		}
	else
		{
		// We're using the original scaling algorithm
		// Set Horizontal Scan flag
		iFlags &= ~EVerticalScan;
		iFinalOffset = iOrigSize.iHeight;
		iCurLineSize = iOrigSize.iWidth;

		// Set up filter tables
		err = FilterTables();
		if( err != KErrNone )
			{
			ProcessError( err );
			return;
			}
		}

	// Allocate the buffers
	TInt allocErr = AllocPtr( iOrigDes, iOrigSize.iWidth * iComponentsPerPixel );
	if (allocErr == KErrNone)
		{
		allocErr = AllocPtr( iDestDes, iDestSize.iWidth * iComponentsPerPixel );
		}

	if( allocErr != KErrNone )
		{
		ProcessError(allocErr);
		return;
		}
	
	if (QualityAlgorithm() == CBitmapScaler::EMaximumQuality)
		{
		err = iDevice->CreateContext(iGc);
		}

	if (err != KErrNone)
		{
		ProcessError(err);
		return;
		}

	//[ set state to scaling ]
	SetCurrentState(EScalingState);

	//[ assert post conditions of scale request ]
	ASSERT( ScaleRequestPostcondition() );

	// Start the active object
	SelfComplete(KErrNone);
	}

/*
*
* FilterTables
* Initialise the filter tables
*
*/
TInt CBitmapScalerPlugin::FilterTables()
	{
	TInt result = KErrNone;
	
	ASSERT(iFilterCoeffsTable==NULL);
	ASSERT(iFilterIndexTable==NULL);
	
	if (iDestSize.iWidth > iDestSize.iHeight)
		{
		iFilterIndexNum = 2 * iDestSize.iWidth;
		}
	else
		{
		iFilterIndexNum = 2 * iDestSize.iHeight;
		}

	if (iDestSize.iWidth > iOrigSize.iWidth) 
		{
		iFilterCoeffsNum = iDestSize.iWidth;
		}
	else
		{
		iFilterCoeffsNum = 2 * iOrigSize.iWidth;
		}
	
	TInt filterCoeffsNumVert;
	
	if (iDestSize.iHeight > iOrigSize.iHeight)
		{
		filterCoeffsNumVert = iDestSize.iHeight;
		}
	else
		{
		filterCoeffsNumVert = 2 * iOrigSize.iHeight;
		}
	
	if (filterCoeffsNumVert > iFilterCoeffsNum)
		{
		iFilterCoeffsNum = filterCoeffsNumVert;
		}

	//[ allocate a filter index table and check for memory alloc failure]
	iFilterIndexTable = new TInt[iFilterIndexNum];
	if( iFilterIndexTable  == NULL )
		{
		return KErrNoMemory;
		}
	
	//[ alloc filter coefficient table and check for memory alloc failure]
	iFilterCoeffsTable = new TInt[iFilterCoeffsNum];
	if( iFilterCoeffsTable == NULL )
		{
		return KErrNoMemory;
		}
	
	// Calculate filter tables for horizontal scaling
	iFilterIndexTablePtr = iFilterIndexTable;
	iFilterCoeffsTablePtr = iFilterCoeffsTable;
	result = CalcFilterTables(iOrigSize.iWidth, iDestSize.iWidth, iFilterIndexTablePtr, iFilterCoeffsTablePtr);

	return result;
	}

/*
*
* DoScale
* This function does the scaling of a bitmap in chunks
*
*/
void CBitmapScalerPlugin::DoScale()
	{
	//[assert the invariant ]
	__ASSERT_DEBUG( InVariant(), Panic( EBadInvariant ) );

	switch( iCurrentState )
		{
		case EScalingState:
			{
			//[ we are scaling the image]
			if (QualityAlgorithm() != CBitmapScaler::EMaximumQuality)
				{
				FastScale();
				}
			else
				{
				Scale();
				}
			break;
			}
		case EStartPostProcessState:
			{
			//[ we are setting up post processing]
			StartPostProcessing();
			break;
			}
		case ECleanUpState:
			{
			//[ cleaning up ]
			ScaleCleanUp();
			break;
			}
		default:
			{
			//[we can never get here
			// because the invariant has been asserted
			//assert the invariant ]
			__ASSERT_DEBUG( InVariant(), Panic( EBadInvariant ) );
			}
		}

	//[assert the invariant ]
	__ASSERT_DEBUG( InVariant(), Panic( EBadInvariant ) );
	}

/**
*
* Scale
* This method does the scaling
* @pre current state is ScalingState
*/
void CBitmapScalerPlugin::Scale()
	{
#if defined (__SCALING_PROFILING)
	RDebug::ProfileStart(7);
#endif // __SCALING_PROFILING

	//[ assert precondition ]
    ASSERT( GetCurrentState() == EScalingState );
	//[ assert invariant ]
	ASSERT( InVariant() );

	TInt linesLeftPerCall = KLinesPerCall;
	
	while (linesLeftPerCall>0 && iCurOffset<iFinalOffset)
		{
		if (!(iFlags & EVerticalScan))
			{
			//
			// Get current scan line
			#if defined (__SCALING_PROFILING)
				RDebug::ProfileStart(8);
			#endif // __SCALING_PROFILING
			iSrcBitmap->GetScanLine(iOrigDes, TPoint(0, iCurOffset), iOrigSize.iWidth, iLocalDisplayMode);
			#if defined (__SCALING_PROFILING)
				RDebug::ProfileEnd(8);
			#endif // __SCALING_PROFILING
			}
		else
			{
			#if defined (__SCALING_PROFILING)
				RDebug::ProfileStart(11);
			#endif // __SCALING_PROFILING
			// Get the vertical scan line...
			iScaleBitmap->GetVerticalScanLine(iOrigDes, iCurOffset, iLocalDisplayMode);
			#if defined (__SCALING_PROFILING)
				RDebug::ProfileEnd(11);
			#endif // __SCALING_PROFILING
			}

		if (!(iFlags & EVerticalScan))
			{
			// Scale the scan line horizontally
			for (TInt i=0; i < iComponentsPerPixel; i++)
				{
				ScaleLine(iOrigDes, iDestDes, i, iOrigSize.iWidth, iDestSize.iWidth, iFilterIndexTable, iFilterCoeffsTable);
				}
			}
		else
			{
			//
			// Scale vertically
			for (TInt i=0; i < iComponentsPerPixel; i++)
				{
				ScaleLine(iOrigDes, iDestDes, i, iOrigSize.iHeight, iDestSize.iHeight, iFilterIndexTable, iFilterCoeffsTable);
				}
			}

#if defined (__SCALING_PROFILING)
	RDebug::ProfileStart(10);
#endif //__SCALING_PROFILING

		if (!(iFlags & EVerticalScan))
			{
			// Set the scaled horizontal line back into the bitmap
			iScaleBitmap->SetScanLine(iDestDes, iCurOffset);
			}
		else
			{
			// Write to output bitmap
			// Lock & unlock the heap to prevent the Font & Bitmap server moving the bitmap around
			// Now writing to iScaleBitmap
			if (iDestBmpPtr->DisplayMode()==EColor16MA && iLocalDisplayMode==EColor16MA)
				{
				TUint8* dataAddress = reinterpret_cast<TUint8*>( iDestBmpPtr->DataAddress());
				dataAddress +=iCurOffset*iComponentsPerPixel;
				const TInt scanLineLength = iDestBmpPtr->ScanLineLength(iDestBmpPtr->SizeInPixels().iWidth, iDestBmpPtr->DisplayMode() );

				const TInt limit = iDestSize.iHeight * iComponentsPerPixel;
				const TUint8* ptr = iDestDes.Ptr();
				for (TInt y = 0; y < limit; y += iComponentsPerPixel, dataAddress += scanLineLength)
					{
					CopyPixel(dataAddress,ptr);
					ptr += iComponentsPerPixel;
					}
				}
			else
				{
				TUint8* dataAddress = reinterpret_cast<TUint8*>( iScanlineBitmap->DataAddress());
				ASSERT( iScanlineBitmap->SizeInPixels().iWidth==1 ); // that must be a "column"
				const TInt scanLineLength = iScanlineBitmap->ScanLineLength(1, iScanlineBitmap->DisplayMode() );

				const TInt limit = iDestSize.iHeight * iComponentsPerPixel;
				const TUint8* ptr = iDestDes.Ptr();
				for (TInt y = 0; y < limit; y += iComponentsPerPixel, dataAddress += scanLineLength)
					{
					CopyPixel(dataAddress, ptr);
					ptr += iComponentsPerPixel;					
					}
		
				iGc->BitBlt(TPoint(iCurOffset, 0), iScanlineBitmap);
				}
			}

#if defined (__SCALING_PROFILING)
		RDebug::ProfileEnd(10);
#endif //__SCALING_PROFILING

		linesLeftPerCall--;
		iCurOffset++;
		}

	if ( IsScalingComplete() && !(iFlags & EVerticalScan))
		{
		iCurLineSize = iOrigSize.iHeight;
		iCurOffset = 0;
		iFinalOffset = iDestSize.iWidth;
		iFlags |= EVerticalScan;
		// Finished the horizontal scaling so now need to setup descriptors
		// for the intermediate size as we're now longer using an intermediate
		// bitmap.
		iOrigSize = iIntermediateSize;

		User::Free( reinterpret_cast<TAny*>( const_cast<TUint8*>( iOrigDes.Ptr() ) ) );
		iOrigDes.SetLength(0);

		TInt allocErr = AllocPtr(iOrigDes, iOrigSize.iHeight * iComponentsPerPixel);
		if( allocErr != KErrNone )
			{
			ProcessError( allocErr );
			return;
			}

		User::Free( reinterpret_cast<TAny*>( const_cast<TUint8*>( iDestDes.Ptr() ) ) );
		iDestDes.SetLength(0);

		allocErr = AllocPtr(iDestDes, iDestSize.iHeight * iComponentsPerPixel);
		if( allocErr != KErrNone )
			{
			ProcessError( allocErr );
			return;
			}
		iFilterIndexTablePtr = iFilterIndexTable;
		iFilterCoeffsTablePtr = iFilterCoeffsTable;
		TInt err = CalcFilterTables(iOrigSize.iHeight, iDestSize.iHeight, iFilterIndexTablePtr, iFilterCoeffsTablePtr);
		if( err != KErrNone )
			{
			ProcessError(err);
			return;
			}
		}
	else if ( IsScalingComplete() )
		{
		// Finished the vertical scaling so, as the tgtbitmap has been scaled in place (ie
		// to the intermediate size) we need to resize it to the true destination size
		TInt err = iTgtBitmap->Resize(iDestSize);
		if( err != KErrNone )
			{
			ProcessError(err);
			return;
			}

		if( IsPostProcessingNeeded() )
			{
			//[ transition to start post processing state]
			SetCurrentState( EStartPostProcessState );
			}
		else
			{
			//[ transition to cleanup state ]
			SetCurrentState( ECleanUpState );
			}
		}
	
	// Start the active object
	SelfComplete(KErrNone);

#if defined (__SCALING_PROFILING)
	RDebug::ProfileEnd(7);
#endif // __SCALING_PROFILING

	//[ assert invariant ]
	ASSERT( InVariant() );
	}

/*
*
* StartPostProcessing
*
*/
void CBitmapScalerPlugin::StartPostProcessing()
	{
	ASSERT( InVariant() );
	ASSERT( GetCurrentState() == EStartPostProcessState );
	// set state to post processing in progress
	// lauch async post process operation
	SetCurrentState(ECleanUpState);	

	iBitmapConverter->Convert( &iStatus, *iTgtBitmap, *iDestBmpPtr);	

	SetActive();
	ASSERT( InVariant() );
	ASSERT( GetCurrentState() == ECleanUpState );
	}

/*
*
* GetCurrentState
* @return CBitmapScalerPlugin::TScaleState The current state
* of the state machine
*
*/
CBitmapScalerPlugin::TScaleState CBitmapScalerPlugin::GetCurrentState() const
	{
	return iCurrentState;
	}

/*
*
* SetCurrentState
* @param aState
* @pre aState <= CleanUpState
* @pre aState >= InactiveState
* @post Invariant
*
*/
void CBitmapScalerPlugin::SetCurrentState( TScaleState aState )
	{
	iCurrentState = aState;
	}

/*
*
* CleanUp
*
*/
void CBitmapScalerPlugin::ScaleCleanUp()
	{
	TInt errStatus = iStatus.Int();

	if( errStatus == KErrNone )
		{
		if ((iFlags & ECreatedTempBitmap) != 0)
			{
			if (iTempBitmap)
				{
				TInt handle = 0;
				if (QualityAlgorithm() != CBitmapScaler::EMaximumQuality)
					{
					// medium OR min qualitity algorithm
					handle = iDestBmpPtr->Handle();
					}
				else
					{
					// Normal (maximum quality) algorithm
					handle = iTgtBitmap->Handle();
					}
				errStatus = iSrcBitmap->Duplicate(handle);
				}
			}
		}

	ProcessError( errStatus );
	}

/*
*
* ProcessError
* @param aErrorStatus
*
*/
void CBitmapScalerPlugin::ProcessError( TInt aErrorStatus )
	{
	if( iPostProcessBitmap )
		{
		iPostProcessBitmap->Reset();
		}
	Cleanup();
	RequestComplete( aErrorStatus );
	}

/*
*
* IsScalingComplete
* @return TBool scaling process has been completed
*
*/
TBool CBitmapScalerPlugin::IsScalingComplete() const
	{
	TBool result = EFalse ;
	if(iCurOffset == iFinalOffset || iCurOffsetUp == 0) 
		{
		result = ETrue;
		}
	return result;
	}

/*
*
* RunL
* Handles an active object’s request completion event.
* The function is called by the active scheduler 
* when a request completion event occurs.
*/
void CBitmapScalerPlugin::RunL()
	{
	DoScale();
	}

/*
*
* DoCancel
* Called by active object to prematurely terminate this bitmap scaling.
*
*/
void CBitmapScalerPlugin::DoCancel()
	{
	ASSERT( InVariant());
	
	if( iBitmapConverter )
		{
		iBitmapConverter->Cancel();
		}
	
	Cleanup();
	ASSERT( GetCurrentState() == EInactiveState );
	ASSERT( InVariant());
	RequestComplete(KErrCancel);
	}

/*
* 
* RequestComplete
* @param "TInt aReason"
*
*/
void CBitmapScalerPlugin::RequestComplete(TInt aReason)
	{
	ASSERT(iScaleStatus);
	TRequestStatus* status = iScaleStatus;
	User::RequestComplete(status, aReason);
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
void CBitmapScalerPlugin::SelfComplete(TInt aReason)
	{
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aReason);
	}

/*
*
* This function calculates the filter tables for scaling
* CalcFilterTables
* @param "TInt aOrigWidth"
* @param "TInt aDestWidth"
* @param "TInt*& aFilterCoeffsTablePtr"
*
*/
TInt CBitmapScalerPlugin::CalcFilterTables(TInt aOrigWidth, TInt aDestWidth, TInt*& aFilterIndexTablePtr, TInt*& aFilterCoeffsTablePtr)
	{
	TInt inputWidth = aOrigWidth;
	TInt scaledWidth = aDestWidth;
	//this is added since to test indX<< KFixedPointBits for overflow 
	TInt fixedPointsVal = 1<<KFixedPointBits;

	if ( (MAKE_TUINT64(0,inputWidth) * MAKE_TUINT64(0,scaledWidth)) > KMaxTInt )
    	{
    	return KErrOverflow;	
    	}
	//Checking OverFlow for ((indX << KFixedPointBits) * inputWidth) 
    if ( (MAKE_TUINT64(0,inputWidth) * MAKE_TUINT64(0,scaledWidth) * MAKE_TUINT64(0,fixedPointsVal)) > KMaxTInt )
    	{
    	return KErrOverflow;	
    	}
	if ( scaledWidth > inputWidth )
		{
		// Calculate tables for interpolation (scaling up)
		for (TInt indX = 0; indX < scaledWidth; indX++)
			{
			const TInt indFilt = ((indX << KFixedPointBits) * inputWidth) / scaledWidth;
			const TInt firstX  = indFilt >> KFixedPointBits;
			const TInt lastX = (firstX >= inputWidth - 1) ? firstX : firstX+1;

			*aFilterIndexTablePtr++ = firstX;
			*aFilterIndexTablePtr++ = lastX;
			*aFilterCoeffsTablePtr++ = indFilt - (firstX << KFixedPointBits);
			}
		}
	else
		{
		// Calculate tables for decimation (scaling down)		
		const TInt ftNew = (inputWidth << KFixedPointBits)/scaledWidth;
		TInt firstX = 0;
		TInt lastX = 0;
		for (TInt indX = 0; indX < scaledWidth; indX++)
			{
			firstX = lastX;
			lastX = ((indX + 1) * inputWidth) / scaledWidth;

			*aFilterIndexTablePtr++ = firstX;
			*aFilterIndexTablePtr++ = lastX;

			const TInt indOrig	= ((indX << KFixedPointBits) * inputWidth) / scaledWidth;
			const TInt indOrigInt = indOrig >> KFixedPointBits;
			for (TInt indFilt = firstX; indFilt <= lastX; indFilt++)
				{
				TInt fixTap;
				if(indFilt <= indOrigInt)
					{
					fixTap = ftNew - (indOrig - (indFilt << KFixedPointBits));
					}
				else
					{
					fixTap = ftNew - ((indFilt << KFixedPointBits) - indOrig);
					}

				if (fixTap < 0) 
					{
					fixTap = 0;
					}
				*aFilterCoeffsTablePtr++ = (fixTap << KFixedPointBits) / ftNew;
				}
			}
		}
	return KErrNone;
	}


/*
* This function calculates the filter tables for scaling
* ScaleLine
* @param "const TDesC8& aInDes"
* @param "TDes8& aOutDes" 
* @param "TInt* aTableCoeffsPtr"
*
*/
void CBitmapScalerPlugin::ScaleLine(const TDesC8& aInDes,
								  TDes8& aOutDes,
								  TInt aStartIndex,
								  TInt aInputWidth,
								  TInt aOutputWidth,
								  TInt* aTableIndexPtr, TInt* aTableCoeffsPtr)
	{
#if defined (__SCALING_PROFILING)
	RDebug::ProfileStart(9);
#endif // __SCALING_PROFILING
	
	// Initialize variables
	TInt* tableIndexesPtr = aTableIndexPtr;
	TInt* tableCoeffsPtr  = aTableCoeffsPtr;

	if (aOutputWidth == aInputWidth)
		{
		aOutDes = aInDes;
		}
	else if (aOutputWidth > aInputWidth)
		{
		// Perform interpolation when scaling up
		const TInt startIndex = aStartIndex;
		const TUint8* inputDesPtr = aInDes.Ptr();
		TUint8* outputDesPtr = const_cast<TUint8*>( aOutDes.Ptr() );

		for (TInt indX = 0; indX < aOutputWidth; indX++, aStartIndex += iComponentsPerPixel)
			{
			TInt firstX = *tableIndexesPtr++;
			TInt lastX  = *tableIndexesPtr++;
			TInt fixTap = *tableCoeffsPtr++;

			// Stay in bounds
			if (firstX >= aInputWidth)
				{
				firstX = aInputWidth - 1;	
				}
			// Factor for aggregated colour component
			firstX *= iComponentsPerPixel;
			firstX += startIndex;

			// Stay in bounds
			if (lastX >= aInputWidth)
				{
				lastX = aInputWidth - 1;	
				}
			lastX *= iComponentsPerPixel;
			lastX += startIndex;

			// Filtering
			outputDesPtr[aStartIndex] = 
				TUint8(((fixTap * TInt(inputDesPtr[lastX])+ 
						 (KFixedPointScale - fixTap) * TInt(inputDesPtr[firstX])) 
						 >> KFixedPointBits));
			}
		}
	else
		{
		// Perform decimation if scaling down requested
		const TInt startIndex = aStartIndex;
		const TInt widthLimit = iComponentsPerPixel * aInputWidth;
		const TInt16 startWidthlimit = widthLimit - iComponentsPerPixel + startIndex;
		const TUint8* inputDesPtr = aInDes.Ptr();
		TUint8* outputDesPtr = const_cast<TUint8*>( aOutDes.Ptr() );

		for (TInt indX = 0; indX < aOutputWidth; indX++, aStartIndex += iComponentsPerPixel)
			{
			TInt firstX = *tableIndexesPtr++;
			firstX *= iComponentsPerPixel;
			firstX += startIndex;
			
			TInt lastX  = *tableIndexesPtr++;
			if (lastX >= aInputWidth)
				{
				lastX = aInputWidth - 1;	
				}
			lastX *= iComponentsPerPixel;
			lastX += startIndex;
			
			// Filtering
			TInt sum       = 0;
			TInt fixTapSum = 0;
			for(TInt indFilt = firstX; indFilt <= lastX; indFilt += iComponentsPerPixel)
				{
				TInt fixTap = *tableCoeffsPtr++;
				TInt indIn;
				if (indFilt < 0)
					{
					indIn = startIndex;
					}
				else if (indFilt >= widthLimit )
					{
					indIn = startWidthlimit;
					}
				else
					{
					indIn = indFilt;
					}

				sum += fixTap * TInt(inputDesPtr[indIn]);
				fixTapSum = TInt(fixTapSum + fixTap);
				}

			// Let's not divide by zero
			if (fixTapSum == 0)
				{
				outputDesPtr[aStartIndex] = TUint8(sum);
				}
			else
				{
				outputDesPtr[aStartIndex] = TUint8(sum/fixTapSum);	
				}
			}
		}
#if defined (__SCALING_PROFILING)
	RDebug::ProfileEnd(9);
#endif // __SCALING_PROFILING
	}


/*
*
* InVariant
* Used to determine the state of health of the scaler body
*
*/
TBool CBitmapScalerPlugin::InVariant() const
	{
    return ((iCurrentState <= ECleanUpState ) && (iCurrentState >= EInactiveState ));
	}

/**
*
* SetProcessingNeeded
* @param aStatus ETrue EFalse
*
*/
void CBitmapScalerPlugin::SetProcessingNeeded( TBool aStatus )
	{
	if(aStatus)
		{
		iFlags |= EIsPostProcessingNeeded;
		}
	else
		{
		iFlags &= ~EIsPostProcessingNeeded;
		}
	}

/**
*
* SetDisablePostProcessing
* @param aStatus ETrue EFalse
*
*/
void CBitmapScalerPlugin::SetDisablePostProcessing( TBool aIsDisabled )
	{
	if(aIsDisabled)
		{
		iFlags |= EDisablePostProcessing;
		}
	else
		{
		iFlags &= ~EDisablePostProcessing;
		}
	}

/**
*
* IsPostProcessingDisabled
* @return ETrue, EFalse
*
*/
TBool CBitmapScalerPlugin::IsPostProcessingDisabled()
	{
	return ((iFlags & EDisablePostProcessing) != 0);
	}

/**
*
* IsPostProcessingNeeded
* @return TBool scaling process has been completed
*
*/
TBool CBitmapScalerPlugin::IsPostProcessingNeeded() const
	{
	return ((iFlags & EIsPostProcessingNeeded) != 0);
	}

/*
*
* HeightAndWidthPreservingAspectRatio
* @param aDestSize
* @param aOrigSize
* Calculates the destination bitmap size based on preserving
* the aspect ratio of the original bitmap
* @pre aOrigSize is not zero in any dimension
*/
void CBitmapScalerPlugin::HeightAndWidthPreservingAspectRatio( TSize& aDestSize, const TSize& aOrigSize ) const
    {
    
    if ((aOrigSize.iWidth == 0) || (aOrigSize.iHeight == 0))
        {
        return;
        }

    // no need to optimise, it is not a kernal function which may get called many times
    TReal widthRatio = (TReal)aDestSize.iWidth / (TReal)aOrigSize.iWidth;
    TReal heightRatio = (TReal)aDestSize.iHeight / (TReal)aOrigSize.iHeight;

    // choose the smaller ratio to use
    if(widthRatio < heightRatio)
        {
        TReal height;
        Math::Round(height, aOrigSize.iHeight * widthRatio, 0);
        aDestSize.iHeight = static_cast<TInt>(height);
        }
    else
        {
        TReal width;
        Math::Round(width, aOrigSize.iWidth * heightRatio, 0);
        aDestSize.iWidth = static_cast<TInt>(width);
        }
    }

/*
*
* IsPostProcessingEnabled
* @return 'TBool'
* 
*/
TBool CBitmapScalerPlugin::IsPostProcessingEnabled() const
	{
	return ((iFlags & EPostProcessingEnabled) != 0);
	}

/*
*
* SetPostProcessingEnabled
* @param aIsEnabled
* 
*/
void CBitmapScalerPlugin::SetPostProcessingEnabled( TBool aIsEnabled )
	{
	if(aIsEnabled)
		{
		iFlags |= EPostProcessingEnabled;
		}
	else
		{
		iFlags &= ~EPostProcessingEnabled;
		}
	}

/*
*
* CustomCommand
* @param aUid
* @param aParam
* @return 'TInt' an error code indicating success or failure of the 
* command
*
*/
TInt CBitmapScalerPlugin::CustomCommand(TUid aUid, TAny* aParam)
	{
	TInt status = KErrNotSupported;
	if( ( aUid == KICLUidPostProcessCommand ) && ( aParam != NULL ) )
		{
		TBool postProcess = *( reinterpret_cast<TBool*>(aParam));
		SetDisablePostProcessing( postProcess ); 
		status = KErrNone;
		}
	else if( ( aUid == KICLUidUseLowMemoryAlgorithmCommand ) && ( aParam != NULL ) )
		{
		TBool useLowMemAlgorithm = *( reinterpret_cast<TBool*>(aParam) );
		SetUseLowMemoryAlgorithm( useLowMemAlgorithm );
		status = KErrNone;
		}
	else if( ( aUid == KICLUidSetQualityAlgorithmCommand ) && ( aParam != NULL ) )
		{
		CBitmapScaler::TQualityAlgorithm quality = *( reinterpret_cast<CBitmapScaler::TQualityAlgorithm*>(aParam) );
		SetQualityAlgorithm(quality);
		status = KErrNone;
		}
 
	return status;
	}

/*
*
* UseLowMemoryAlgorithm
* @return 'TBool'
* 
*/
TBool CBitmapScalerPlugin::UseLowMemoryAlgorithm() const
	{
	return ((iFlags & EUseLowMemoryAlgorithm) != 0);
	}

/*
*
* SetUseLowMemoryAlgorithm
* @param aUseLowMemoryAlgorithm
* 
*/
void CBitmapScalerPlugin::SetUseLowMemoryAlgorithm(TBool aUseLowMemoryAlgorithm)
	{
	if(aUseLowMemoryAlgorithm)
		{
		iFlags |= EUseLowMemoryAlgorithm;
		}
	else
		{
		iFlags &= ~EUseLowMemoryAlgorithm;
		}
	}

/*
*
* SetQualityAlgorithm
* @param CBitmapScaler::TQualityAlgorithm aQuality
* 
*/
void CBitmapScalerPlugin::SetQualityAlgorithm( CBitmapScaler::TQualityAlgorithm aQuality)
	{
	iQualityLevel = aQuality;
	}

/*
*
*	QualityAlgorithm
*	@return CBitmapScaler::TQualityAlgorithm
*
*/
CBitmapScaler::TQualityAlgorithm CBitmapScalerPlugin::QualityAlgorithm() const
	{
	return iQualityLevel;
	}

/**
*	This function is called when the state iCurrentState == EScalingState 
*	and either the UseMediumQualityAlgorithm or the UseMinimumQualityAlgorithm custom
*	commands have been invoked.
*	In terms of speed and quality, the normal algorithm, as implemented in Scale(),
*	is the slowest / highest quality, whereas the fastest / lowest quality
*	implementation is for the case when UseMinimumQualityAlgorithm is specified.
*	The difference between the minimum quality and fast algorithms is that
*	the fast algorithm forms the destination value from weighted averages of
*	the x values prior to and proceeding the value under consideration.
*/
void CBitmapScalerPlugin::FastScale()
	{
#if defined (__SCALING_PROFILING)
	RDebug::ProfileStart(7);
#endif // __SCALING_PROFILING
	TInt linesLeftPerCall = KLinesPerCall;
	
	// Height and width to scale to
	const TInt maxX = iDestSize.iWidth * iComponentsPerPixel;
	const TInt stepY = (iOrigSize.iHeight << KFixedPointScale) / iDestSize.iHeight;
	const TInt destBufferLength = iDestDes.Length();
	const TInt divisor = (destBufferLength < iDestSize.iWidth ? destBufferLength : iDestSize.iWidth);
	const TInt stepX = (iOrigSize.iWidth << KFixedPointScale) / divisor;
	TInt sourceX = 0;	// x
	const TInt roundVal = 1;
	if (iDestSize == iOrigSize)
		{
		// Same size so just directly copy values.
		while(linesLeftPerCall > 0 && iCurOffset < iFinalOffset)
			{
			iSrcBitmap->GetScanLine(iOrigDes, TPoint(0, iCurOffset), iOrigSize.iWidth, iLocalDisplayMode);
			iDestBmpPtr->SetScanLine(iOrigDes, iCurOffset);
			linesLeftPerCall--;
			iCurOffset++;
			}
		}
	else if (iDestSize.iHeight < iOrigSize.iHeight)
		{
		TInt currentX = 0;
		// For downscaling (destination bitmap size is smaller than the original bitmap size)
		while(linesLeftPerCall > 0 && iCurOffset < iFinalOffset)
			{
			sourceX = 0;			
			// Fetch the relevant row from the source bitmap
			iSrcBitmap->GetScanLine(iOrigDes, TPoint(0, iSrcY), iOrigSize.iWidth, iLocalDisplayMode);

			// Get each colour component of each relevant pixel
			for (TInt x = 0; x < maxX; x += iComponentsPerPixel)
				{
				//Line scaling
				ScaleBitmapLine(x,sourceX);
				// Increment to the next pixel
				currentX += stepX;
				// Avoid TInt rounding error
				sourceX = ( currentX + (roundVal << KFixedPointScale - 1) ) >> KFixedPointScale;
				// Factor for the number of components
				sourceX *= iComponentsPerPixel;
				// Stay in bounds
				if (sourceX >= iOrigDes.Length())
					{
					sourceX = iOrigDes.Length() - iComponentsPerPixel;
					}
				}
			currentX = 0;	// reset the x counter
			iDestBmpPtr->SetScanLine(iDestDes, iCurOffset);	// set the scaled line into the target
				
			// increment to the next row
			iCurY += stepY;
			// avoiding any rounding errors (using TInt = (TInt + 0.5) )
			iSrcY = (iCurY + (roundVal << KFixedPointScale - 1) ) >> KFixedPointScale;
			// Stay in bounds
			if (iSrcY >= iOrigSize.iHeight)
				{
				iSrcY = iOrigSize.iHeight - 1;
				}
			linesLeftPerCall--;
			iCurOffset++;
			}
		}
	else
		{
		TInt currentX = 0;
		//For upscaling (Destination bitmap size is bigger than the original bitmap size)
		while(linesLeftPerCall > 0 && iCurOffsetUp !=0 )
			{
			sourceX = 0;			
			// avoiding any rounding errors (using TInt = (TInt + 0.5) )
			iSrcY = (iCurY + (roundVal << KFixedPointScale - 1) ) >> KFixedPointScale;
			// Fetch the relevant row from the source bitmap
			iSrcBitmap->GetScanLine(iOrigDes, TPoint(0, iSrcY), iOrigSize.iWidth, iLocalDisplayMode);
			
			// Get each colour component of each relevant pixel
			for (TInt x = 0; x < maxX; x += iComponentsPerPixel)
				{
				//Line scaling
				ScaleBitmapLine(x,sourceX);
				// Increment to the next pixel
				currentX += stepX;
				// Avoid TInt rounding error
				sourceX = ( currentX + (roundVal << KFixedPointScale - 1) ) >> KFixedPointScale;
				// Factor for the number of components
				sourceX *= iComponentsPerPixel;
				// Stay in bounds
				if (sourceX >= iOrigDes.Length())
					{
					sourceX = iOrigDes.Length() - iComponentsPerPixel;
					}
				}
			currentX = 0;	// reset the x counter
			iDestBmpPtr->SetScanLine(iDestDes, iCurOffsetUp);	// set the scaled line into the target				
			// Decrement to the next row from the end
			iCurY -= stepY;	
			// Stay in bounds
			if (iSrcY >= iOrigSize.iHeight)
				{
				iSrcY = iOrigSize.iHeight - 1;
				}
			linesLeftPerCall--;
			iCurOffsetUp--;
			}
		}
	if (IsScalingComplete())
		{
		// in case of low memory algo and 1 bitmap we have to resize the resulting bitmap
		if (iDestBmpPtr->Handle() == iSrcBitmap->Handle()
				&& iDestBmpPtr->SizeInPixels() != iDestSize )
			{
			TInt err=iDestBmpPtr->Resize(iDestSize);
			if (err != KErrNone)
				{
				ProcessError( err );
				return;
				}
			}		
		if( IsPostProcessingNeeded() )
			{
			//[ transition to start post processing state]
			SetCurrentState( EStartPostProcessState );
			}
		else
			{
			//[ transition to cleanup state ]
			SetCurrentState( ECleanUpState );
			}
		}
	// Start the active object
	SelfComplete(KErrNone);
#if defined (__SCALING_PROFILING)
	RDebug::ProfileEnd(7);
#endif // __SCALING_PROFILING
	}
	
/*
*
* ScaleBitmapLine
* @param aX
* @param aSourceX
*
*/
void CBitmapScalerPlugin::ScaleBitmapLine(TInt aX,TInt aSourceX)
	{
	TUint8* destDesPtr = const_cast<TUint8*>( iDestDes.Ptr() );
	const TUint8* orgDesPtr = iOrigDes.Ptr();
	
	if (QualityAlgorithm() == CBitmapScaler::EMinimumQuality)
		{
		CopyPixel(destDesPtr + aX,orgDesPtr + aSourceX);
		}
	else	// must be CBitmapScaler::EMediumQuality
		{
		// Better quality
		TInt nextValue = 0;
		TInt prevValue = 0;
		TInt value = 0;
		// eg weightedAv. = (w1*v1 + w2*v2 + w3*v3) / (w1 + w2 + w3)
		// so if w1=w3=8 and w2=16
		// (v1 * 2<<3 + v2 * 2<<4 + v3 * 2<<3) >> 2 * 5
		const TInt minorShift = 3;	// prev and next value weighting
		const TInt majorShift = 4;	// current value weighting
		const TInt backShift = 5;	// average

		for (TInt i = 0; i < iComponentsPerPixel; i++)
			{
			// Get the previous value
			TInt index = aSourceX - iComponentsPerPixel;
			if (index < 0)
				{
				index = i;
				}
			else
				{
				index += i;
				}
			prevValue = TInt(orgDesPtr[index]);
				
			// Weight the previous value
			prevValue = prevValue << minorShift;
			// Get the next value
			index = aSourceX;
			if (index  + iComponentsPerPixel >= iOrigDes.Length())
				{
				index += i;
				}
			else
				{
				index += (i + iComponentsPerPixel);
				}
			nextValue = TInt(orgDesPtr[index]);

			// Weight the next value
			nextValue = nextValue << minorShift;
			// Weight the current value
			value = TInt(orgDesPtr[aSourceX+ i]);

			value = value << majorShift;
			// Sum all values
			value += prevValue;
			value += nextValue;
			// Average
			value = value >> backShift;
			destDesPtr[aX + i] = TUint8(value);
			}
		}
	}

