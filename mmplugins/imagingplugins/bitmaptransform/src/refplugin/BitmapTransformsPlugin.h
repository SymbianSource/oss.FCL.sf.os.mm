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
// This is the implementation of the BitmapTransforms Library
// 
//

#ifndef BITMAPTRANSFORMSPLUGIN_H
#define BITMAPTRANSFORMSPLUGIN_H

#include <e32base.h>
#include <fbs.h>
#include <bitdev.h>

#include <bitmaptransforms.h> // required for enums
#include <bitmtrans/bitmtransplugin.h>

class CBitmapConverter;

/**
*
* The private API for clients to call the BitmapTransforms Library bitmap rotation
* @internalTechnology
*            
*/

class CBitmapRotatorPlugin : public CActive, public MBitmapRotatorPlugin
	{
private:
	typedef CBitmapRotator::TRotationAngle TRotationAngle;

public:
	static MBitmapRotatorPlugin* NewL();
	~CBitmapRotatorPlugin();

	// from MBitmapRotatorPlugin
	void Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TRotationAngle aAngle);
	void Rotate(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, TRotationAngle aAngle);
	TInt CustomCommand(TUid aUid, TAny* aParam);
	void Cancel();

private:
	// from CActive
	void RunL(); 
	void DoCancel();

	CBitmapRotatorPlugin();
	void ConstructL();
	void Cleanup();
	void DoRotate();
	void FlipScanLine(TDes8& aDes, TInt aWidth);
	void PutScanline(TInt aYPos);
	void RequestComplete(TInt aReason);
	void SelfComplete(TInt aReason);

private:
	TRequestStatus*		iRotateStatus;
	TInt				iCurOffset;
	TInt				iRows;
	TRotationAngle		iAngle;
	TSize				iBitmapSize;
	TDisplayMode		iDisplayMode;
	CFbsBitmap*			iTempBitmap; // For the Rotate() with only one bitmap supplied
	CFbsBitmap*			iSrcBitmap;
	CFbsBitmap*			iTgtBitmap;
	TUint32*			iScanlineBuffer;
	TDisplayMode		iScanlineDisplayMode;
	TInt				iPixelSizeInBytes;
	TPtr8				iScanlineDes;
	CFbsBitmapDevice*	iDevice;
	CFbsBitGc*			iGc;
	CFbsBitmap*			iScanlineBitmap; // Intermediate scanline bitmap
#if defined(ROTATION_PROFILING)	
	TUint				iStartedAtFc;	 // Used for profiling
#endif	
	};

/**
*
* Body of CBitmapScalerPlugin
*
* @internalTechnology
*
*/
class CBitmapScalerPlugin : public CActive, public MBitmapScalerPlugin
	{
public:
	//[ fsm states  ]
	enum TScaleState
		{
		EInactiveState,                  // scaling is inactive
		EScalingState,                   // scaling the image only
		EStartPostProcessState,          // starting post processing
		ECleanUpState                    // cleaning up 
		};

private:
	enum TScaleFlags
		{
		EVerticalScan				= 0x00000001,		//	TBool iVerticalScan;
		EPostProcessingInProgress	= 0x00000002,		//	TBool iPostProcessingInProgress;
		EIsPostProcessingNeeded		= 0x00000004,		//	TBool iIsPostProcessingNeeded;
		EPostProcessingEnabled		= 0x00000008,		//	TBool iPostProcessingEnabled;
		EDisablePostProcessing		= 0x00000010,		//	TBool iDisablePostProcessing;
		EIsSourceBitmapResized		= 0x00000020,
		EUseLowMemoryAlgorithm		= 0x00000040,
		ECreatedTempBitmap			= 0x00000080,		// flag that we need to delete the temporary bitmap
		EScaleBitmapIsOwned			= 0x00000100
		};

public:
	static MBitmapScalerPlugin* NewL();
	~CBitmapScalerPlugin();

	// From MBitmapScalerPlugin
	void Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TBool aMaintainAspectRatio = ETrue);
	void Scale(TRequestStatus* aRequestStatus, CFbsBitmap& aBitmap, const TSize& aDestinationSize, TBool aMaintainAspectRatio = ETrue);
	void Cancel();
	TInt CustomCommand(TUid aUid, TAny* aParam);

private:
	// From CActive
	void RunL();
	void DoCancel();

	CBitmapScalerPlugin();
	void ConstructL();
	void Cleanup();
	void DoScale();
	TInt CalcFilterTables(TInt aOrigWidth, TInt aDestWidth, TInt*& aFilterIndexTablePtr, TInt*& aFilterCoeffsTablePtr );
	void ScaleLine(const TDesC8& aInDes, TDes8& aOutDes, TInt aStartIndex, TInt aInputWidth, TInt aOutputWidth, TInt* aTableIndexPtr, TInt* aTableCoeffsPtr);
	void RequestComplete(TInt aReason);
	void SelfComplete(TInt aReason);
	TBool IsPostProcessingNeeded() const;
	void  SetProcessingNeeded( TBool aStatus );
	TBool IsScalingComplete() const;
	void ProcessError( TInt aErrorStatus );
	void HeightAndWidthPreservingAspectRatio( TSize& aDestSize, const TSize& aOrigSize ) const;
	void SetPostProcessingInProgress( TBool aStatus );	
	void SetCurrentState( TScaleState aState );
	TScaleState GetCurrentState() const;
	TBool InVariant() const;
	TInt AllocPtr( TPtr8& aPtr, TInt aAllocSize );
	TBool ScaleRequestPostcondition() const ;
	TInt FilterTables();
	TBool IsPostProcessingEnabled() const;
	void SetPostProcessingEnabled( TBool aIsEnabled );

	// User Preference
	// default is 
	void SetDisablePostProcessing( TBool aIsDisabled );
	TBool IsPostProcessingDisabled();

	void SetUseLowMemoryAlgorithm(TBool aUseLowMemoryAlgorithm);
	TBool UseLowMemoryAlgorithm() const;

	void SetQualityAlgorithm( CBitmapScaler::TQualityAlgorithm aQuality);
	CBitmapScaler::TQualityAlgorithm QualityAlgorithm() const;

	//[ state machine functions ]
	void Scale();
	void StartPostProcessing();
	void ScaleCleanUp();
	void FastScale();
	void ScaleBitmapLine(TInt aX,TInt aSourceX);
	inline void CopyPixel(TUint8* aTo, const TUint8* aFrom) const;

private:
	TRequestStatus*		iScaleStatus;
	CFbsBitmap*			iSrcBitmap;
	CFbsBitmap*			iTgtBitmap;
	CFbsBitmap*			iPostProcessBitmap;
	CFbsBitmap*			iScaleBitmap;
	CFbsBitmap*			iScanlineBitmap;
	CFbsBitmap*			iDestBmpPtr; // points to the real target bitmap, not owned
	CFbsBitmapDevice*	iDevice;
	CFbsBitGc*			iGc;
	TInt*				iFilterIndexTable;
	TInt*				iFilterIndexTablePtr;
	TInt				iFilterIndexNum;
	TInt*				iFilterCoeffsTable;
	TInt*				iFilterCoeffsTablePtr;
	TInt				iFilterCoeffsNum;
	TSize				iOrigSize;
	TSize				iDestSize;
	CFbsBitmap*			iTempBitmap;
	TPtr8				iOrigDes;
	TPtr8				iDestDes;
	TInt				iLinesPerCall;
	TInt				iCurOffset;
	TInt 				iCurOffsetUp;
	TInt				iFinalOffset;
	TInt				iCurLineSize;
	CBitmapConverter*   iBitmapConverter;
	TScaleState			iCurrentState;
	TUint32				iFlags;
	TSize				iIntermediateSize;
	TInt				iComponentsPerPixel;	// The number of components per pixel of the target bitmap at the given colour depth 
	TDisplayMode		iTgtDisplayMode;
	TDisplayMode		iLocalDisplayMode;
	TInt				iSrcY;				// For fast / min quality scaling
	TInt				iCurY;
	CBitmapScaler::TQualityAlgorithm	iQualityLevel;
	};

#include "BitmapTransformsPlugin.inl"
/**
The function panic raises a panic from within this plugin DLL
@param The argument is a member of the enumeration TBitmapTransformsPanic
@internalComponent
*/

GLDEF_C void Panic(TInt aReason);

#endif  // BITMAPTRANSFORMSPLUGIN_H

