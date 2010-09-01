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

#ifndef __TestImageDisplay_H__
#define __TestImageDisplay_H__

#include <icl/imagedisplayplugin.h>
#include <imageconversion.h>
#include <bitmaptransforms.h>
#include "TestHelperIface.h"

// fwd refs

class CFbsBitmap;

// Transformer
class CTestImageDisplayPlugin : public CImageDisplayPlugin, public MTestHelper
	{
public:
	enum TState
		{
		EIdle,
		EProcessing,
		EPaused
		};

public:
 	static CTestImageDisplayPlugin* NewL();
	~CTestImageDisplayPlugin();

	virtual void OpenL();
	virtual void Play();
	virtual void Pause();
	virtual void StopPlay();

	virtual TInt ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr);
	virtual void GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const;
	virtual TBool ValidBitmap() const;
	const CImageDisplay::RImageSizeArray& RecommendedImageSizes() const;

protected:	

	void CompleteInitL();
	// from the MTestHelper 

	virtual TInt CurrentFrameNumber(TInt& aFrameParam);
	virtual void TriggerPanic_4();

	// from the MImageDisplayPluginExtension
	virtual const TUid Uid() const;
	virtual void Release();

private:
	CTestImageDisplayPlugin();
	void ConstructL();
	void Cleanup();

protected:
	void DrawFrame();

private:

	RFs				iFs;
	TState iState;
	CFbsBitmap*	iFrame;
	CFbsBitmap*	iMask;
	CFbsBitmapDevice* iBitmapDevice;
	CFbsBitGc* iBitGc;
	TInt			iFrameNumber;
	TPluginStatus	iPluginStatus;
	TRect			iOutputRect;
	};	

#endif // __TestImageDisplay_H__

