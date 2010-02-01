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
// This file contains definitions of the test step classes for Conversion
// 
//


#ifndef __TS_MM_ICL_FRM_CP_H__
#define __TS_MM_ICL_FRM_CP_H__


#include <imageconversion.h>


const TInt KErrUser=(-1000);
const TInt KErrNotIdentical=(KErrUser - 1);

const TInt KStreamBreakpoint=100; // up to this point use initialincrement. then use secondary
const TInt KStreamInitialIncrement=1;
const TInt KStreamSecondaryIncrement=128;

const TInt KLenNameInSection = 7;	//len right characters 
									//ex MM-ICL-FRM-I-XXXX-XX	
									//last 7 chars defines name of section fully

const TInt	KLenBuffer	= 256;
const TInt	KLenBufferExtension	= 8;

const TUid KImageTypePPMUid = {0x101F45F7};

#define KPpm1DecoderImplementationUidValue	0x101F45F4
#define KPpm1EncoderImplementationUidValued  0x101F45F8

#define KPpm2DecoderImplementationUidValue	0x101F45F5
#define KPpm2EncoderImplementationUidValue	0x101F45F9


class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { iStatus = KRequestPending; SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

class CTest_MM_ICL_FRM_CP;

class CTestActiveScheduler : public CActiveScheduler
	{
public:
	CTestActiveScheduler() : CActiveScheduler()
		{
		iDecoder = NULL;
		iEncoder = NULL;
		iCurStep = 0;
		iNumStep = 0;
		iStep = NULL;

		}
	TAny	SetDecoder(CImageDecoder*	aDecoder, CActiveListener* aListener, TInt aStepBeforeCancel)
		{
		iDecoder = aDecoder;
		iListener = aListener;
		iCurStep = 0;
		iNumStep = aStepBeforeCancel;
		}
	TAny	SetEncoder(CImageEncoder*	aEncoder, CActiveListener* aListener, TInt aStepBeforeCancel)
		{
		iEncoder = aEncoder;
		iListener = aListener;
		iCurStep = 0;
		iNumStep = aStepBeforeCancel;
		}
	TAny	ResetValues()
		{
		iDecoder = NULL;
		iEncoder = NULL;
		iCurStep = 0;
		iNumStep = 0;
		iStep = NULL;
		}
	TAny	SetStep(CTest_MM_ICL_FRM_CP	*aStep) {iStep = aStep;}
	virtual void WaitForAnyRequest();

protected:
	CImageDecoder *	iDecoder;
	CImageEncoder *	iEncoder;

	
	CActiveListener* iListener;
	CTest_MM_ICL_FRM_CP	*iStep;
	TInt	iNumStep;
	TInt	iCurStep;
	};

 
class CTest_MM_ICL_FRM_CP : public CTestStep
	{
public:
	CTest_MM_ICL_FRM_CP(const TDesC& aTestName);
	void FrameDisplayMode(TDisplayMode	aDisplayMode, TDes& aDisplayModeDes);
	TAny	PrintBusyOrFreeImageConversion(TBool aBusy);


	enum TPlaceType
		{
			EInput,
			EOutput,
			EReference,
		};

	struct TConvertInfo
		{
		TUid	iImageTypeId;
		TDisplayMode	iDisplayMode;
		};

protected:
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();

	TAny	ReadConfig();
	void ReadMimeTypeFromConfig(TDes8& aMimeType);
	void ReadDecoderUidFromConfig(TUid& aUid);

	TInt CheckBitmapL(CFbsBitmap& aBitmap, const TDesC& aFileName);
	TInt CompareFileL(const TDesC& aSourceFilename, 
												HBufC8	*theDestinationImage,
												TUid				aTypeImage,
												TPlaceType			aDirectory );

	TAny GetImageExtension(	TUid	aTypeImageUid, 
							TDes& imageExtension);

	TAny	Directory( TPlaceType	aPlace, TDes& imagePlace);
	TAny	GetImageTypeUid(const TPtrC	aCodecName);
protected:


	TPtrC	iInputDir;
	TPtrC	iOutputDir;
	TPtrC	iRefDir;
	TPtrC	iFileNameSrc;
	TPtrC	iDriveName;


	CImageEncoder*		iImageEncoder;
	CImageDecoder*		iImageDecoder;
	CTestActiveScheduler*	iActiveScheduler;

	
	RFs					iFs;
	RFile				iFile;

	CFbsBitmap		*iBitmap;

	TUid			iImageTypeUid;
	
	};



#endif // __TS_MM_ICL_FRM_CP_H__
