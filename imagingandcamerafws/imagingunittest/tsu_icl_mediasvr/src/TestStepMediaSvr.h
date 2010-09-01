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

#ifndef __TESTSTEPMEDIASVR_H__
#define __TESTSTEPMEDIASVR_H__


#include <mdaimageconverter.h>
#include "MdaImageConverterPriv.h"
#include <mda/client/video.h>

class CTestSuite;
class CTestSuiteMediaSvr;

/**
 *
 * CTestStepMediaSvr, which is the base class for all 
 * the MediaSvr suite test steps.
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CTestStepMediaSvr : public CTestStep,
						  public MMdaImageUtilObserver
	{
public:
	CTestStepMediaSvr();

	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();

protected:
	// waits for the current asynchronous call to return and returns the stored error code
	TInt WaitForResult();

	// functions to test MediaClientImageAPIs
	CFbsBitmap* LoadImageL(const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	CFbsBitmap* CancelLoadImageL(const TDesC& aImageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	void LoadTransparentImageL(CFbsBitmap* aBitmap,
								CFbsBitmap* aMask,
								const TDesC& aFileName,
								TMdaClipFormat* aFormat,
								TMdaPackage* aCodec); 
	void FailLoadTransparentImageL(CFbsBitmap* aBitmap,
								CFbsBitmap* aMask,
								const TDesC& aFileName,
								TMdaClipFormat* aFormat,
								TMdaPackage* aCodec); 
	void SaveImageL(CFbsBitmap* aBitmap, const TDesC& aImageFileName, TMdaClipFormat* aFormat,TMdaPackage* aCodec); 
	void CancelSaveImageL(CFbsBitmap* aBitmap, const TDesC& aImageFileName, TMdaClipFormat* aFormat,TMdaPackage* aCodec); 

	CFbsBitmap* StreamImageL(const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec); 

	HBufC8* WriteImageToDescriptorL(CFbsBitmap* aBitmap,  TMdaClipFormat* aFormat, TMdaPackage* aCodec, TInt aDesSize=0); 
	CFbsBitmap* ReadImageFromDescriptorL(const TPtr8& aImageBuffer,  TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	CFbsBitmap* StreamImageFromDescriptorL(const TPtr8& aImageBuffer,  TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	void CheckImageToSmallDescriptorL(CFbsBitmap* aBitmap,  TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	
	void SaveImageWithAllocFailL(CFbsBitmap* aBitmap, const TDesC& aImageFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	CFbsBitmap* LoadImageWithAllocFailL(const TDesC& aImageFileName,TMdaClipFormat* aFormat, TMdaPackage* aCodec); 
	CFbsBitmap* StreamImageFromDescWithAllocFailL(const TPtr8& aImageBuffer, TMdaClipFormat* aFormat, TMdaPackage* aCodec); 

	TFrameInfo GetFrameInfoL(const TDesC& aFileName, TMdaClipFormat* aFormat, TMdaPackage* aCodec);
	// Bitmap checking APIS
	TVerdict CheckBitmaps(const CFbsBitmap& aBitmap1,const CFbsBitmap& aBitmap2) const;
	TVerdict CompareFilesL(const TDesC& aFileOne, const TDesC& aFileTwo) const;
	// Error logging
	void LogMessage(const TDesC& aContext,const TDesC& aMessage);
	void LogError(const TDesC& aContext,const TDesC& aMessage);
	const TDesC& DefaultPath() const;


private:
	// from MMdaImageUtilObserver
	virtual void MiuoCreateComplete(TInt aError);
	virtual void MiuoOpenComplete(TInt aError);
	virtual void MiuoConvertComplete(TInt aError);
	void InitSystemPath();
	void Complete(TInt aError);

private:
	// pointer to suite which owns this test 
	CActiveScheduler* iScheduler;
	TFileName iDefaultPath;

protected:
	//format classes for various tests
	TMdaMbmClipFormat iMbmFormat;
	TMdaJfifClipFormat iJfifFormat;
	TMdaBmpClipFormat iBmpFormat;

	TMdaMbmClipFormat iMbmFormatGray2;
	TMdaMbmClipFormat iMbmFormatGray4;
	TMdaMbmClipFormat iMbmFormatGray16;
	TMdaMbmClipFormat iMbmFormatGray256;
	TMdaMbmClipFormat iMbmFormatColor16;
	TMdaMbmClipFormat iMbmFormatColor256;
	TMdaMbmClipFormat iMbmFormatColor4K;
	TMdaMbmClipFormat iMbmFormatColor64K;
	TMdaMbmClipFormat iMbmFormatColor16M;

	TMdaJfifClipFormat iJfifFormatMonoQ50;
	TMdaJfifClipFormat iJfifFormatColor420Q50;
	TMdaJfifClipFormat iJfifFormatColor422Q50;
	TMdaJfifClipFormat iJfifFormatColor444Q50;

	TMdaJfifClipFormat iJfifFormatColor444Q10;
	TMdaJfifClipFormat iJfifFormatColor444Q30;
	TMdaJfifClipFormat iJfifFormatColor444Q60;
	TMdaJfifClipFormat iJfifFormatColor444Q100;

	TMdaWbmpClipFormat iWbmpFormat;
	TMdaOtaClipFormat iOtaFormat;
	TMdaPngClipFormat iPngFormat;
	TMdaGif87aClipFormat iGifFormat;
	TMdaGif89aClipFormat iGifAnimFormat;
	
	// codec classes for various tests
	TMda1BppBmpCodec iBmp1BppCodec;
	TMda4BppBmpCodec iBmp4BppCodec;
	TMda8BppBmpCodec iBmp8BppCodec;
	TMda24BppBmpCodec iBmp24BppCodec;

	TMdaGifCodec iGifCodec;
	TMdaPngCodec iPngCodec;
	
	// error code from asynchrynous callback
	TInt iError;
	};


#endif // __TESTSTEPMEDIASVR_H__
