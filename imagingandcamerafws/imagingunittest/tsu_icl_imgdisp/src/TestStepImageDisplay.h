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

#if (!defined __TESTSTEPIMAGEDISPLAY_H__)
#define __TESTSTEPIMAGEDISPLAY_H__

#include <imagedisplay.h>
#include <apgcli.h>


class TImageDisplayUnitTestParams
	{
public:	
	enum TDataType 
		{
		ETypeUndefined	=-1, 
		ETypeFileName	= 0, 
		ETypeData, 
		ETypeFileHandle
		};
	enum TImageTestType 
		{
		ENormal,
		EAlloc
		};
	enum
		{
		KNoIntent=-1,
		};
	
public:
	const TText*  iTestName;		// name of the test
	const TText*  iTestDescription;	// description
	const TText*  iInputFilename;	// source filename
	TInt iDestWidth;				// dest width
	TInt iDestHeight;				// dest height

	TInt		 iDisplayMode;		// display mode
	TInt		 iImageOptions;		// image options 
	
	TBool iMaintainAspectRatio;		// maintain aspect ratio
	TDataType iSourceType;			// Source is a filename
	const TText8* iMimeType;		// Mime type
	TInt32 iImageType;				// image type
	TInt32 iImageSubType;			// image subtype
	TInt32 iPluginUid;				// plugin UID
	TImageTestType iTestType;		// test type
	const TText* const* iUniqueId;		// DRM content Unique ID
	ContentAccess::TIntent iIntent;  // DRM intent
	TInt	iExpectedError;			// expected error Code
	TInt    iPlayLoops;				// number of "play" attempts
	};

class TMimeTypeRecogTestParam
	{
public:
	enum TParamType
		{
		EParamIsFileName,	
		EParamIsDataBuffer
		};
public:		
	const TText*  	iTestName;			// name of the test
	const TText*  	iTestDescription;	// description
	TParamType		iParamType;			// what next parameter to use
	const TText8*  	iDataBufferParam;	// databuffer parameter value
	const TText*  	iFileNameParam;		// filename parameter value
	const TText8*	iExpectedMimeType;  // expected MIME type value
	CApaDataRecognizerType::TRecognitionConfidence iMinConfidence; // minimum confidence
	};

class CImageDisplay;
class CTestSuite;
class CTestSuiteImageDisplay;

/**
 *
 * CTestStepImageDisplay, which is the base class for all 
 * the ImageTransform suite test steps, providing common functionality.
 *
 * @lib "TSU_ICL_ImageTransform.lib"
 *
 */
class CImageDisplay;
class CTestStepImageDisplay : public CTestStep, public MIclImageDisplayObserver
	{
public:
	CTestStepImageDisplay();
	~CTestStepImageDisplay();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
	void SetSuite(CTestSuiteImageDisplay* aPtr );
 	CTestSuiteImageDisplay* Suite();

protected:
	virtual void Cleanup(){};
    void SetSourceFilename(const TDesC& aPathNameAndExtn);
    void SetDestFilenameL(const TDesC& aPathNameAndExtn);
    void SetRefFilenameL(const TDesC& aPathNameAndExtn);
	void DeleteDestFilenameL();
	TVerdict CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile) const;
	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2);

	TFileName& SourceFilename();
	TFileName& DestFilename();
	TFileName& RefFilename();
	void DefaultPath(TFileName& aName);

protected:
	RFs iFs;
	CImageDisplay*				iImageDisplay;

private:
  	 CTestSuiteImageDisplay*	iImageDisplaySuite;
	 
	 TFileName iSourceFilename;
	 TFileName iDestFilename;
	 TFileName iRefFilename;
	 CActiveScheduler* iScheduler;
	};


/**
 *
 * The following classes provide test steps to Transform an image
 * See TestStepImageTransform.cpp or the test specification 
 * for details of individual test cases.
 *
 */

/**
 * CTestStepImgDisplayGeneric
 */
class CTestStepImgDisplayGeneric : public CTestStepImageDisplay
	{
public:

	CTestStepImgDisplayGeneric(const TImageDisplayUnitTestParams& aTestParams);
	~CTestStepImgDisplayGeneric();
	// from CTestStep
	virtual TVerdict DoTestStepL();

protected:
//
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	virtual TBool ConsumeFrame(const CFbsBitmap* aBitmap);
//
	void SetImageSourceL();
	void SetFilenamesL(const TDesC& aSourceFilename);
	void SetupPluginL();
	virtual TVerdict TestL();
	virtual void Cleanup();

private:
	TVerdict DoTestStepFbsL();

protected:
	// pointer to current test data
	const TImageDisplayUnitTestParams* iTestParams;

	// pointer to previous test data (for API re-use tests)
	const TImageDisplayUnitTestParams* iTestParamsPrev;

	HBufC8* iSourceBuf;
	TInt iLastError;
	TInt iFrameNumber;
	RFile iFileHandle;
	TBool iFbsConnection;
	TUint iLatestPlugintStatus;
	const TText* const* iCurrentUId;
	};

class CTestStepDecode2Frames : public CTestStepImgDisplayGeneric {
public:
	CTestStepDecode2Frames(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
protected:
	virtual TBool ConsumeFrame(const CFbsBitmap* aBitmap);
private:
	virtual TVerdict TestL();
};

class CTestStepPanic_1 : public CTestStepImgDisplayGeneric {
public:
	CTestStepPanic_1(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
private:
	virtual TVerdict TestL();
};

class CTestStepPanic_2 : public CTestStepImgDisplayGeneric {
public:
	CTestStepPanic_2(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
private:
	virtual TVerdict TestL();
};

class CTestStepPanic_3 : public CTestStepImgDisplayGeneric {
public:
	CTestStepPanic_3(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
private:
	virtual TVerdict TestL();
};

class CTestStepPanic_4 : public CTestStepImgDisplayGeneric {
public:
	CTestStepPanic_4(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
private:
	virtual TVerdict TestL();
};

class CTestStepPanic_6 : public CTestStepImgDisplayGeneric {
public:
	CTestStepPanic_6(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
private:
	virtual TVerdict TestL();
};

// DRM tests //
class CTestStepDrm : public CTestStepImgDisplayGeneric {
public:
	CTestStepDrm(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
protected:
	virtual TBool ConsumeFrame(const CFbsBitmap* aBitmap);
private:
	virtual TVerdict TestL();
};

// MNG Mime Type recognition tests //
class CTestStepImageDisplayRecog : public CTestStepImageDisplay
	{
public:
	CTestStepImageDisplayRecog(const TMimeTypeRecogTestParam& aTestParam);
	virtual TVerdict DoTestStepL();
protected:
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	const TMimeTypeRecogTestParam& iTestParam;
	RApaLsSession iApaLsSession;
	};

//
// Regression test case for defect: DEF082108: Browsing to www.mobil.se will crash the phone 
class CTestStepImageDisplayMimeCheck : public CTestStepImageDisplay
	{
public:
	CTestStepImageDisplayMimeCheck();
	virtual TVerdict DoTestStepL();
protected:
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	RApaLsSession iApaLsSession;
	};

#endif // __TESTSTEPIMAGEDISPLAY_H__

