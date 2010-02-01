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

#ifndef __TestCONVERTER_H__
#define __TestCONVERTER_H__

#include <icl/imagetransformplugin.h>
#include <imageconversion.h>
#include <bitmaptransforms.h>
#include "TestTransformerBase.h"

// fwd refs
class CActiveListener;	
class CTestTransformerExtension;

class MListenerCallback
	{
public:	
	virtual void Complete(TInt aError) = 0;
	};

// Transformer
class CTestTransformer : public CTestTransformerBase, public MListenerCallback
	{
public:
	enum TState
		{
		EIdle,
		EDecodingImage,
		EScalingBitmap,
		EEncodingImage
		};

public:
 	static CTestTransformer* NewL();
	~CTestTransformer();

	// from MListenerCallback
	void Complete(TInt aError);

	//from CTestTransformerBase
	TBool GetPreserveImageData() const;

protected:	
	// from CImageTransformPlugin
	void OpenL();
	void Transform(TRequestStatus& aRequest);
	void CancelTransform();
	CImageTransformPluginExtension* Extension() const;

private:
	CTestTransformer();
	void ConstructL();
	void Cleanup();

protected:

private:
	TRequestStatus* iClientStatus;	// not owned
	TSize iDestSize;

	TBool iIsSourceFilename;
	TBool iIsSourceData;
	const TDesC8* iSourceMimeType;	// not owned
	TUid iSourceImageType;
	TUid iSourceImageSubType;

	TBool iIsDestFilename;
	TBool iIsDestData;
	const TDesC8* iDestMimeType;	// not owned
	TUid iDestImageType;
	TUid iDestImageSubType;

	TSize iSourceSizeInPixels;
	TSize iDestSizeInPixels;

	TBool iMaintainAspectRatio;
	TBool iPreserveImageData;

	TPtrC iSourceFilename;
	TPtrC iDestFilename;

	TPtrC8 iSourceData;
	HBufC8** iDestData;

	TUint iOptions;
	TRect iSourceRect;

	CImageDecoder* iImageDecoder;
	CBitmapScaler* iScaler;
	CImageEncoder* iImageEncoder;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iDestBitmap;

	CTestTransformerExtension* iExtension;

	RFs iFs;

	CActiveListener* iListener;
	TState iState;
	};	

class CActiveListener : public CActive
	{
public:
 	static CActiveListener* NewL(MListenerCallback &aListenerCallback);
    CActiveListener(MListenerCallback &aListenerCallback);
    void InitialiseActiveListener();
    
    // From CActive
    void RunL();
    void DoCancel();

private:
	MListenerCallback& iListenerCallback;
	};

#endif // __TestCONVERTER_H__

