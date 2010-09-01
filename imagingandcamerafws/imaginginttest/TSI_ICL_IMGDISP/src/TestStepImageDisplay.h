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
#include <e32base.h>


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
	TBool iContinueOnToNextStep;	// Re-use test: continue onto next test step
	TImageTestType iTestType;		// test type
	};

/**
 *
 * CActiveListener provides the asynchronous operation
 * of an active object
 *
 * @lib "TSU_ICL_IMAGE_CONVERT.lib"
 *
 *
 */
class CImageDisplay;
class CActiveListener : public CActive
	{
public:
	CActiveListener(CImageDisplay& iImageDisplay);
	~CActiveListener();

    //From CActive
	virtual void RunL();
	virtual void DoCancel();

	void InitialiseActiveListener();

	TBool RequestCancelled()
		{ 
		return iStatus == KErrCancel;
		}
private:
	CImageDisplay& iImageDisplay;
	};

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
    void SetRefFilename(const TDesC& aPathNameAndExtn);
	void DeleteDestFilenameL();
	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2) const;
	TVerdict CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const;
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
	virtual TBool ConsumeFrame(const CFbsBitmap& aBitmap);
//
	void SetImageSourceL();
	TInt CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile) const;
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
	};

class CTestStepDecode2Frames : public CTestStepImgDisplayGeneric {
public:
	CTestStepDecode2Frames(const TImageDisplayUnitTestParams& aTestParams):CTestStepImgDisplayGeneric(aTestParams)
		{
		}
protected:
	virtual TBool ConsumeFrame(const CFbsBitmap& aBitmap);
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


#endif // __TESTSTEPIMAGEDISPLAY_H__

