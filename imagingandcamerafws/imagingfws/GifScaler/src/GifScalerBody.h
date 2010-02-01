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
// This is the implementation of the GifScaler Library
// 
//

#ifndef __GIFSCALERBODY_H__
#define __GIFSCALERBODY_H__

#include <e32base.h>
#include <fbs.h>
#include <bitmaptransforms.h>

#include "gifscaler.h" // Required for enums.

// Color quantizer class.
class CColorQuantizer;

/**
*
* The private API for clients to call the GifScaler Library scaling and color quantization.
* @internalTechnology
*
*/

/**
*
* Body of CGifScaler
*
* @internalTechnology
*
*/

NONSHARABLE_CLASS( CGifScaler::CBody ): public CActive
	{
	friend class CGifScaler;
public:
	// Current state.
	enum TScaleState
		{
		EInactiveState,					// Scaling is inactive.
		EScalingState,					// Scaling the image only.
		EStartColorQuantization,		// Starting the color quantization.
		ECleanUpPending,				// Scaling etc. complete, so start cleanup.
		ECleanUpState,					// Cleaning up.
		EScalingComplete				// All operations complete.
		};

private:
	static CBody* NewL(CFbsBitmap& aSource, CFbsBitmap* aSourceMask, CGifScaler::TOptions aOptions);
	~CBody();

	void Scale(TRequestStatus* aStatus, CFbsBitmap& aDestination, CPalette& aPalette, TBool aMaintainAspectRatio);
	void Scale(TRequestStatus* aStatus, CFbsBitmap& aDestination, CPalette& aPalette, TUint8 aTransparencyThreshold, TBool aMaintainAspectRatio);

	// From CActive
	void RunL();
	TInt RunError(TInt aError);
	void DoCancel();

	// New fns.
	CBody(CFbsBitmap& aSource, CFbsBitmap* aSourceMask, CGifScaler::TOptions aOptions);
	void ConstructL();

	// Cleanup fn.
	// Used after scaling is complete.
	void CleanUp();
	
	// Complete scaling.
	void RequestComplete(TInt aReason);

	// Complete current state.
	void SelfComplete(TInt aReason);

private:
	// Source bitmap. (not owned)
	CFbsBitmap& iSource;

	// Source bitmap mask. (not owned)
	CFbsBitmap* iSourceMask;

	// Intermediate bitmap. (owned)
	CFbsBitmap* iIntermediate;

	// Destination bitmap. (not owned)
	CFbsBitmap* iDestination;

	// Destination mask bitmap. (owned)
	CFbsBitmap* iDestinationMask;

	// Scaling and color quantization options.
	CGifScaler::TOptions iOptions;

	// MaintainAspectRatio.
	TBool iMaintainAspectRatio;

	// Bitmap scaler. (owned)
	CBitmapScaler* iBitmapScaler;
	
	// Request status to signal when done. (not owned)
	TRequestStatus* iRequestStatus;

	// Current state.
	TScaleState iCurrentState;

	// Tranparency indices.
	TUint8 iTransparencyIndex;
	TUint8 iReplacementIndex;
	TUint8 iTransparencyThreshold;

	// Color quantizer. (owned)
	CColorQuantizer* iColorQuantizer;

	// Output palette. (not owned)
	CPalette *iPalette;
	};

#endif  // __GIFSCALERBODY_H__

