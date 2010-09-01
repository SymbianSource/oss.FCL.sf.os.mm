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
// This file contains definitions of the test step classes for Conversion
// 
//


#ifndef __TEST_STEP_CONVERSION_H__
#define __TEST_STEP_CONVERSION_H__


#include <imageconversion.h>
#include <caf/caf.h>
#include "TS_MM_ICL_FRM_CP.h"

using namespace ContentAccess;


const TInt	KShowFrameInformation	=	0x01;
const TInt	KShowFrameOption		=	0x02;



class CTestStepConversion : public CTest_MM_ICL_FRM_CP
	{
public:

	void OpenImageForDecodeFromFileWithIntentL(const TDesC& aFilename, 
												TIntent aIntent, 
												const TDesC8& aMimeType = KNullDesC8);

	void OpenImageForDecodeFromFileWithIntentL(const TDesC& aFilename, 
												TIntent aIntent, 
												TUid aImageTypeUid,
												TUid aDecoderUid = KNullUid);


	TInt	OpenImageForDecodeFromFileL(const TDesC& aFileName, TUid aCodecUid = KNullUid);
	TInt	OpenPpmImageForDecodeFromFileL(const TDesC& aFileName, TUid aCodecUid = KNullUid, const TUid aPpmDecodeUid = KNullUid);
	TInt	OpenImageForDecodeFromBufferL(const TDesC& aFileName, TUid aUid = KNullUid);
	void	OpenBufferedImageL(const TDesC& aFilename, TInt aFrameNumber=0, TUid aUid=KNullUid, const CImageDecoder::TOptions aOptions=CImageDecoder::EOptionNone);

	TInt	LoadPluginByContent();
	TInt	IdentifyFormatByContent();
	TInt	InitialazePluginWithImage();
	TInt	DecodeImageFrame();
	
	TInt	EncodeImageToMemoryL(const TDesC& aFilename, 
												   TUid		aTypeImage);
	TInt	EncodeImageFromBitmapToMemoryL(const TDesC& aSourceFilename, 
												CFbsBitmap	*theBitmap,   
												TUid		aTypeImageUid);

	TInt	EncodeImageFromBitmapToFileL(const TDesC& aSourceFilename, 
												   TUid		aTypeImageUid);

	
	TInt	ListAvailableMIMEDecoders();
	TInt	ListAllAvailableDecoders() ;
	TInt	ListAvailableMIMEEncoders();
	TInt	ListAllAvailableEncoders();
	
	TInt	FindMimeTypeL(const TDesC& aFilename);
	TInt	EnquireDecoderUIDAndImplementationInfoL(const TDesC& aFileName);
	TInt	EnquireEncoderUIDL(const TDesC& aFileName, TUid	aTypeImageUid);
	
	TInt	RequestNumberFramesL(const TDesC& aFileName);
	TInt	RequestDecodeDataL(const TDesC& aFileName, TInt aParam);

	TInt	EncodeImageSpecifyingParamL(TUid aTypeImageUid);
	TInt	RetrieveEmbeddedCommentsL(const TDesC& aFilename);
	TAny	PrintFrameOptions(const TFrameInfo& aFrameInfo);
	TAny	PrintFrameInfo(const TFrameInfo& aFrameInfo);

	TInt	RetrieveFrameInfoL(const TDesC& aFilename);
	
//------
	TInt ReadAndDecodeIncrementallyL(const TDesC& aFileName, TInt aStepBeforeCancel = 0);
	inline TInt StreamIncrement(TInt aValue);

	
	inline TAny	StartProfiling();
	inline TAny	StopProfiling();
	inline TAny	LogProfile();

	TInt	DummyHalCall();

	//For GT0210 MS3.4 File Handles API Testing.
	TInt DecodeUsingDecoderL(const TDesC& aFileName);
	void EncodeUsingEncoderL(const TDesC& aFileNameSrc);

protected:
//

	TInt	PrepareEncodeDataL(TConvertInfo &aConvInfo, 
											   TImageDataBlock** aImageData, 
											   TFrameDataBlock** aFrameData);

	CTestStepConversion(const TDesC& aTestName) :
		CTest_MM_ICL_FRM_CP(aTestName) {}

protected:
	//stub for profiling
	TInt	aProfile;

	};


#endif // __TS_MM_ICL_FRM_CP_H__
