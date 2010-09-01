// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

const TInt KLinesPerCall = 10;

/**
The function NewL constructs a CBitmapRotatorPlugin

@returns CBitmapRotatorPlugin* 


*/

MBitmapRotatorPlugin* CBitmapRotatorPlugin::NewL()
	{
	CBitmapRotatorPlugin* self = new(ELeave) CBitmapRotatorPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Constructor for this class. Adds itself to <code>CActiveScheduler</code>.
The priority of this active object is CActive::EPriorityIdle

*/

CBitmapRotatorPlugin::CBitmapRotatorPlugin()
	: CActive(CActive::EPriorityIdle), iScanlineDes(NULL,0)
	{
	CActiveScheduler::Add(this);
	}


/**
Performs second phase of contruction
*
*/

void CBitmapRotatorPlugin::ConstructL()
	{
	}

/**
*
* Default destructor for this class.
*
*/

CBitmapRotatorPlugin::~CBitmapRotatorPlugin()
	{
	Cancel();
	Cleanup();

	// should have been deleted by cleanup
	ASSERT(iScanlineBuffer==NULL);
	ASSERT(iTempBitmap==NULL);
	ASSERT(iDevice==NULL);
	ASSERT(iGc==NULL);
	}


/**
This function performs deallocation of memory allocated by the class

*/

void CBitmapRotatorPlugin::Cleanup()
	{
	delete [] iScanlineBuffer; iScanlineBuffer = NULL;
	delete iTempBitmap; iTempBitmap = NULL;
	delete iGc; iGc = NULL;
	delete iDevice; iDevice = NULL;
	delete iScanlineBitmap; iScanlineBitmap = NULL;
#if defined(ROTATION_PROFILING)
	TUint fcTaken=User::FastCounter() - iStartedAtFc;
	RDebug::Print(_L("BmpRotator: FC time taken %d"),fcTaken);
#endif // 	
	}

// MBitmapRotatorPlugin::Cancel() calls CActive::Cancel()
void CBitmapRotatorPlugin::Cancel()
	{
	CActive::Cancel();
	}

/**
The function Rotate schedules a rotate/mirror operation on a bitmap supplied in the 
aSrcBitmap whose output overwrites aSrcBitmap.
Preconditions:
aRequestStatus is not a NULL pointer
aSrcBitmap     is a fully constructed bitmap of unknown size including zero dimension
aAngle         is a member of the enumeration TRotationAngle

@param "aRequestStatus" 
       is a pointer to the completion status of the asynchronous operation.
@param "CFbsBitmap& aBitmap" 
       is a reference to a CFbsBitmap. This bitmap should have been created
       and is also an output
@param "TRotationAngle aAngle"
       aAngle is a member of the enumeration TRotationAngle and specifies the rotation mirror operation   

@panic  This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not been constructed
ie its handle is 0

Sucess Guarantee
aSrcBitmap      contains the rotated bitmap
aRequestStatus  points to the value KErrorNone

*/

void CBitmapRotatorPlugin::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle)
	{
	// [ panic if aRequestStatus is NULL ]
	__ASSERT_ALWAYS( (aRequestStatus != NULL), Panic( EBadArgumentScale ) );

	//[ panic if the src has not been created]
	__ASSERT_ALWAYS( (aBitmap.Handle() != 0), Panic( ENoSourceBitmap ) );

	//[ assert the angle is ok ]
	__ASSERT_ALWAYS( ( aAngle >= CBitmapRotator::ERotation90DegreesClockwise ) &&
					 ( aAngle <= CBitmapRotator::EMirrorVerticalAxis ), Panic(EBadArgumentRotate) );

	ASSERT(iTempBitmap==NULL);

	iRotateStatus = aRequestStatus;
	*iRotateStatus = KRequestPending;

	if(aBitmap.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }
	
	iTempBitmap = new CFbsBitmap;
	if (!iTempBitmap)
		{
		RequestComplete(KErrNoMemory);
		return;
		}
	Rotate(aRequestStatus, aBitmap, *iTempBitmap, aAngle);
	}

	/**
The Rotate function schedules a rotate/mirror operation on a bitmap supplied in the srcBitmap and 
produces the output in the tgtBitmap.
The CBitmapRotatorPlugin is an active object and as such provides asynchronous operations

Preconditions:
aRequestStatus is not a NULL pointer
aSrcBitmap     is a fully constructed bitmap of unknown size including zero dimension and 
of type EColor16M
aTgtBitmap     is a fully constructed bitmap of unknown size including zero dimension
and is of type EColor16M
aAngle         is a member of the enumeration TRotationAngle 

Sucess Guarantee:
aTgtBitmap      contains the rotated bitmap
aRequestStatus  points to the value KErrNone

Minimal Guarantee:
The bitmap supplied in aSrcBitmap is unaltered


@param "TRequestStatus* aRequestStatus"
       a pointer to the completion status of the asynchronous operation.
@param "CFbsBitmap& aSrcBitmap"
       This bitmap should have been created and be of type EColor16M
@param "CFbsBitmap& aTgtBitmap"
       This bitmap should have been created and be of type EColor16M
@param "TRotationAngle aAngle" 
       is a member of the enumeration TRotationAngle and specifies the rotation mirror operation


@panic This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not been constructed
       i.e. its handle is zero


*/

void CBitmapRotatorPlugin::Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle)
	{
	// [ panic if aRequestStatus is NULL ]
	__ASSERT_ALWAYS( (aRequestStatus != NULL), Panic( EBadArgumentScale ) );

	//[ panic if the src has not been created]
	__ASSERT_ALWAYS( (aSrcBitmap.Handle() != 0), Panic( ENoSourceBitmap ) );

	//[ assert the angle is ok ]
	__ASSERT_ALWAYS( ( aAngle >= CBitmapRotator::ERotation90DegreesClockwise ) && 
					 ( aAngle <= CBitmapRotator::EMirrorVerticalAxis ), Panic(EBadArgumentRotate) );

	//[ we do not need to ensure a tgt has been created]
#if defined(ROTATION_PROFILING)
	iStartedAtFc = User::FastCounter();
#endif // ROTATION_PROFILING	
	// Initialize member variables
	iSrcBitmap = &aSrcBitmap;
	iTgtBitmap = &aTgtBitmap;
	iAngle = aAngle;
	iCurOffset = 0;
	iBitmapSize = iSrcBitmap->SizeInPixels();
	iDisplayMode = iSrcBitmap->DisplayMode();
	iRotateStatus = aRequestStatus;
	*iRotateStatus = KRequestPending;
	
	if(aSrcBitmap.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }
	
	if(aTgtBitmap.Handle()!=0 && aTgtBitmap.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }

	TSize newSize;
	if ((aAngle == CBitmapRotator::ERotation90DegreesClockwise) || (aAngle == CBitmapRotator::ERotation270DegreesClockwise))
		{
		newSize.SetSize(iBitmapSize.iHeight, iBitmapSize.iWidth); // Swap width and height
		}
	else
		{
		newSize.SetSize(iBitmapSize.iWidth, iBitmapSize.iHeight);
		}
	TInt err = iTgtBitmap->Create(newSize, iDisplayMode);
	
	TBool bmpDeviceNeeded=EFalse;
	if (iSrcBitmap->DisplayMode()==EColor16M || iSrcBitmap->DisplayMode()==EColor16MU ||
		iSrcBitmap->DisplayMode()==EColor16MA ||
		iSrcBitmap->DisplayMode()==EColor256 || iSrcBitmap->DisplayMode()==EGray256 || 
		iSrcBitmap->DisplayMode()==EColor64K )
		{
		iScanlineDisplayMode = iSrcBitmap->DisplayMode();
		}
	else
		{
		// we can't cope with that color mode, then use the highest one for intermediate buffer
		bmpDeviceNeeded		= ETrue;
		iScanlineDisplayMode= EColor16MA;
		}
	iPixelSizeInBytes = TDisplayModeUtils::NumDisplayModeBitsPerPixel(iScanlineDisplayMode) / 8;
	
	TInt scanlineLength=iSrcBitmap->ScanLineLength(newSize.iWidth, iScanlineDisplayMode);
	scanlineLength=Align4(scanlineLength);
	if (err == KErrNone)
		{
		ASSERT(iScanlineBuffer==NULL);
		iScanlineBuffer = new TUint32 [ scanlineLength ];
		if(!iScanlineBuffer)
			{
			err = KErrNoMemory;
			}
		}
	if (err != KErrNone)
		{
		Cleanup();
		RequestComplete(err);
		return;
		}
	iRows = newSize.iHeight;

	TPtr8 scanlineDes(reinterpret_cast<TText8*>(iScanlineBuffer),scanlineLength,scanlineLength); // Use a temporary to avoid compiler warnings
	iScanlineDes.Set(scanlineDes);
	
	ASSERT(iDevice==NULL);
	if (bmpDeviceNeeded)
		{
		iScanlineBitmap = new CFbsBitmap();
		err= (iScanlineBitmap? 	  iScanlineBitmap->Create(TSize(iTgtBitmap->SizeInPixels().iWidth, 1), iScanlineDisplayMode)
								: KErrNoMemory);
		if (err==KErrNone)
			{
			TRAP(err, iDevice = CFbsBitmapDevice::NewL(iTgtBitmap));
			}
		if (err == KErrNone)
			{
			err = iDevice->CreateContext(iGc);
			}
		}
	
	if (err != KErrNone)
		{
		Cleanup();
		RequestComplete(err);
		return;
		}

	// Start the active object
	SelfComplete(KErrNone);
	}

/**
This function is called by the Active Scheduler
to perform the rotate operation

*/

void CBitmapRotatorPlugin::DoRotate()
	{
	TInt linesLeftPerCall = KLinesPerCall;
	while ((linesLeftPerCall > 0) && (iCurOffset < iRows))
		{
		switch (iAngle)
			{
			// Rotation of 90 degrees
			case CBitmapRotator::ERotation90DegreesClockwise:
				{
				iSrcBitmap->GetVerticalScanLine(iScanlineDes, iCurOffset, iScanlineDisplayMode);
				FlipScanLine(iScanlineDes, iTgtBitmap->SizeInPixels().iWidth);
				PutScanline(iCurOffset);
				break;
				}
			
			// Rotation of 180 degrees
			case CBitmapRotator::ERotation180DegreesClockwise:
				{
				iSrcBitmap->GetScanLine(iScanlineDes,TPoint(0, iCurOffset), iBitmapSize.iWidth, iScanlineDisplayMode);
				FlipScanLine(iScanlineDes, iBitmapSize.iWidth);
				PutScanline(iBitmapSize.iHeight - 1 - iCurOffset);
				break;
				}

			// Rotation of 270 degrees
			case CBitmapRotator::ERotation270DegreesClockwise:
				{
				iSrcBitmap->GetVerticalScanLine(iScanlineDes, iCurOffset, iScanlineDisplayMode);
				PutScanline(iBitmapSize.iWidth - 1 - iCurOffset);
				break;
				}

			// Flip about the vertical Axis
			case CBitmapRotator::EMirrorVerticalAxis:    
				{
				iSrcBitmap->GetScanLine(iScanlineDes,TPoint(0,iCurOffset),iBitmapSize.iWidth,iScanlineDisplayMode);
				FlipScanLine(iScanlineDes, iBitmapSize.iWidth);
				PutScanline(iCurOffset);
				break;
				}

			// Flip about the horizontal axis
			case  CBitmapRotator::EMirrorHorizontalAxis:
				{
				iSrcBitmap->GetScanLine(iScanlineDes,TPoint(0,iCurOffset),iBitmapSize.iWidth,iScanlineDisplayMode);
				PutScanline(iBitmapSize.iHeight-1-iCurOffset);
				break;
				}

			default:
				{
				ASSERT( EFalse );
				}
			}

		iCurOffset++;
		linesLeftPerCall--;
		}

	if (iCurOffset == iRows)
		{
		TInt duplicateErr = KErrNone;
		if (iTempBitmap)
			{
			duplicateErr = iSrcBitmap->Duplicate(iTgtBitmap->Handle());
			}
		Cleanup();
		RequestComplete(duplicateErr);
		}
	else
		{
		// Start the active object
		SelfComplete(KErrNone);
		}
	}

/**
Handles an active object’s request completion event.
The function is called by the active scheduler
when a request completion event occurs,

*/

void CBitmapRotatorPlugin::RunL()
	{
	DoRotate();
	}

/**
Implements cancellation of an outstanding request. 
This function is called as part of the active object’s Cancel().
It must call the appropriate cancel function offered by the active object’s asynchronous service provider.
The asynchronous service provider’s cancel is expected to act immediately.

*/

void CBitmapRotatorPlugin::DoCancel()
	{
	Cleanup();
	RequestComplete(KErrCancel);
	}

/**
Signals to the current thread that the asynchronous request 
associated with the request status is complete.
@param aReason is a TInt and is the completion code of this request.

*/

void CBitmapRotatorPlugin::RequestComplete(TInt aReason)
	{
	ASSERT(iRotateStatus);
	TRequestStatus* status = iRotateStatus;
	User::RequestComplete(status, aReason);
	}

/**
This function activates the active object and 
signals completion of the current asynchronous operation

*/

void CBitmapRotatorPlugin::SelfComplete(TInt aReason)
	{
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aReason);
	}
	
/**
  template function that can be used for swapping of memory locations 
  of particular pointer type
  @param ptr1 - pointer to value1
  @param ptr2 - pointer to value2
*/
template <class T>
inline void SwapPixels(T* ptr1, T* ptr2)
	{
	T temp 	= *ptr1;
	*ptr1	= *ptr2;
	*ptr2  	= temp;
	}
	
/**
  template function that can be used for mirroring of linear
  memory location of a particular scalar type
  @param aScanLinePtr location address
  @param aWidth width of location in elements, not bytes
*/	
template <class T>
inline void FlipLine(TUint8* aScanLinePtr, TInt aWidth)
	{
	T* startPixelPtr 	= reinterpret_cast<T*>(aScanLinePtr);
	T* endPixelPtr 		= startPixelPtr + aWidth - 1;
	if (aWidth&1)
		{
		SwapPixels(startPixelPtr++, endPixelPtr--);
		}
	if(aWidth&3)
		{
		while (startPixelPtr < endPixelPtr)
			{
			SwapPixels(startPixelPtr++, endPixelPtr--);
			}
		}
	else
		{
		while (startPixelPtr < endPixelPtr)
			{
			SwapPixels(startPixelPtr++, endPixelPtr--);
			SwapPixels(startPixelPtr++, endPixelPtr--);
			}			
		}
	}
	
/**
This function flips a scan line buffer of width aWidth
PreConditions:
aWidth >= 0 && aWidth is the length of the buffer
aDes is a reference to a buffer of rgb pixels
Postcondition:
The contents of the buffer have flipped about the buffers centre

@param TDes8 aDes
       reference to a buffer of rgb pixels of lenth aWidth
@param Tint aWidth
       is the width of the buffer

*/
void CBitmapRotatorPlugin::FlipScanLine(TDes8& aDes, TInt aWidth)
	{
	//[ assert consistency between descriptor length and width ]
	ASSERT( (aDes.Length() == (iPixelSizeInBytes * aWidth) ));
	
	TUint8* const scanLinePtr=const_cast<TUint8*>(aDes.Ptr());
	
	switch (iPixelSizeInBytes)
		{
		case 4:
			{
			FlipLine<TUint32>(scanLinePtr, aWidth);
			}
			break;
		case 3:
			{
			TUint8* startPixelPtr = scanLinePtr;
			TUint8* endPixelPtr = startPixelPtr + (aWidth - 1)*iPixelSizeInBytes;
	while (startPixelPtr < endPixelPtr)
		{
		TUint8 temp0 = startPixelPtr[0];
		TUint8 temp1 = startPixelPtr[1];
		TUint8 temp2 = startPixelPtr[2];
		startPixelPtr[0] = endPixelPtr[0];
		startPixelPtr[1] = endPixelPtr[1];
		startPixelPtr[2] = endPixelPtr[2];
		endPixelPtr[0] = temp0;
		endPixelPtr[1] = temp1;
		endPixelPtr[2] = temp2;
				startPixelPtr += iPixelSizeInBytes;
				endPixelPtr -= iPixelSizeInBytes;				
				}
			}
			break;
		case 2:
			{
			FlipLine<TUint16>(scanLinePtr, aWidth);
			}
			break;
		case 1:
			{
			FlipLine<TUint8>(scanLinePtr, aWidth);
			}
			break;

		default:
			ASSERT(EFalse);
		}
	}

void CBitmapRotatorPlugin::PutScanline(TInt aYPos)
	{
	if (iScanlineBitmap)
		{
		iScanlineBitmap->SetScanLine(iScanlineDes, 0);
		iGc->BitBlt(TPoint(0, aYPos), iScanlineBitmap);	
		}
	else
		{
		iTgtBitmap->SetScanLine(iScanlineDes, aYPos);
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
TInt CBitmapRotatorPlugin::CustomCommand(TUid /*aUid*/, TAny* /*aParam*/)
	{
	TInt status = KErrNotSupported;
	return status;
	}
