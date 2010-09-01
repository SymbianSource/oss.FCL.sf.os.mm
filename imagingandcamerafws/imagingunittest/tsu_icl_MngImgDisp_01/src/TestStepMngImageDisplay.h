// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if (!defined TESTSTEPMNGIMAGEDISPLAY_H)
#define TESTSTEPMNGIMAGEDISPLAY_H

#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>

class CTestSuite;
class CTestSuiteMngImageDisplay;

const TUint KDecodeFinishedMask		=(CImageDisplayPlugin::EStatusEraseOutputContents-1)|CImageDisplayPlugin::EStatusEraseOutputContents;

const TUint KFrameReadyStatus		=CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusPaused;

const TUint KLastFrameReadyStatus	=CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusNoMoreToDecode;

const TUint KDecodeFinishedKeepStatus=KLastFrameReadyStatus|CImageDisplayPlugin::EStatusDisplayThisFrameIndefinetely;
const TUint KDecodeFinishedEraseStatus=KLastFrameReadyStatus|CImageDisplayPlugin::EStatusEraseOutputContents;

const TInt KOneSecond=1000000;

struct TFrameCheckPoint 
	{
public:
	enum 
		{
		KEndMarkerFrame		=-1,
		KNodelayChecking	=-1,
		KNoStatusChecking	= 0
		};
	TInt			iAbsoluteFrameNumber;
	const TText*	iReferenceFileName;
	TInt			iDelayFromPreviuos;
	TUint			iPluginStatusMask;
	TUint			iPluginStatusValue;
	const TText*	iReferenceMaskName;

public:
	inline TBool CompareStatus(const TUint aStatus) const
		{
		return (aStatus&iPluginStatusMask) == iPluginStatusValue;
		}

	inline TBool IsValid() const
		{
		return iAbsoluteFrameNumber > KEndMarkerFrame;
		}
	};

#define ENDCHECKPOINT {TFrameCheckPoint::KEndMarkerFrame}

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
		EAlloc,
		ENegative
		};
	enum 
		{
		KNoFrameLimit=0
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
	TInt   iImageType;				// image type
	TInt   iImageSubType;			// image subtype
	TInt   iPluginUid;				// plugin UID
	TInt	iMaxFramesToDecode;		// Maximum number of frames to decode
	TImageTestType iTestType;		// test type
	const TFrameCheckPoint* iCheckPoints;	// Frame checkpoints
	TInt  iErrorCode;				// expected error code for a negative test
	};


/**
 *
 * CTestStepMngImageDisplay, which is the base class for all 
 * the ImageDisplay suite test steps, providing common functionality.
 *
 *
 */
class CTestStepMngImageDisplay : public CTestStep
	{
public:
	~CTestStepMngImageDisplay();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
	
	void SetSuite(CTestSuiteMngImageDisplay* aPtr );
 	CTestSuiteMngImageDisplay* Suite();

protected:
	explicit CTestStepMngImageDisplay(const TImageDisplayUnitTestParams& aTestParams);

	virtual TVerdict TestL()=0;
	virtual void Cleanup();
    void SetSourceFilename(const TDesC& aPathNameAndExtn);
    void SetDestFilenameL(const TDesC& aPathNameAndExtn);
    void SetRefFilename(const TDesC& aPathNameAndExtn);
	void DeleteDestFilenameL();
	TInt CompareFiles(const TDesC& aFile1,const TDesC& aFile2);
	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2);
	TVerdict CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const;
	TInt CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile) const;
	TVerdict CompareBitmapToRefBuffer(const CFbsBitmap* aBitmap, const TUint* aBuffer);
	TVerdict Compare32BitBitmapToRefBuffer(const CFbsBitmap* aBitmap, const TUint* aBuffer);
	TFileName& SourceFilename();
	TFileName& DestFilename();
	TFileName& RefFilename();
	void DefaultPath(TFileName& aName);
	TVerdict DoTestStepFbsL();

protected:
	mutable RFs iFs;
	// pointer to current test data
	const TImageDisplayUnitTestParams* iTestParams;
	TInt iLastError;
private:
  	 CTestSuiteMngImageDisplay*	iImageDisplaySuite;
	
	 TFileName iSourceFilename;
	 TFileName iDestFilename;
	 TFileName iRefFilename;
	 CActiveScheduler* iScheduler;
	 TBool iFbsConnection;
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
class CTestStepImgDisplayGeneric : public CTestStepMngImageDisplay, public MIclImageDisplayObserver
	{
public:

	explicit CTestStepImgDisplayGeneric(const TImageDisplayUnitTestParams& aTestParams);
	~CTestStepImgDisplayGeneric();
	// from CTestStep
protected:
//
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	virtual TBool ConsumeFrameL(const CFbsBitmap* aBitmap);
//
	void SetImageSourceL();
	void SetFilenamesL(const TDesC& aSourceFilename);
	void SetupPluginL();
	void SetDefaultDestSize();
	virtual TVerdict TestL();
	virtual void Cleanup();

protected:

	TVerdict CheckTiming(const TTime aStart, const TTime aEnd, TInt aMicroNeeded, TInt aAllowedPercentage) const;

	CImageDisplay*				iImageDisplay;

	HBufC8* iSourceBuf;
	TInt iFrameNumber;
	TInt iFramesToDecode;
	RFile iFileHandle;
	TUint iLastPluginStatus;
	TRect iLastUpdateRect;
	};


class CTestStepMngCheckpoint : public CTestStepImgDisplayGeneric 
	{
	enum 
		{
#if !defined(__WINS__)
#if !defined(_DEBUG)
	#ifdef SYMBIAN_WRITABLE_DATA_PAGING
  	KAllowedTimePercentage=70
  	#else
	KAllowedTimePercentage=20
	#endif
#else
	#ifdef SYMBIAN_WRITABLE_DATA_PAGING
  	KAllowedTimePercentage=80
  	#else
  	KAllowedTimePercentage=40
  	#endif
#endif
#else
	KAllowedTimePercentage=35
#endif
		};
public:
	CTestStepMngCheckpoint(const TImageDisplayUnitTestParams& aTestParams);
protected:
	virtual TBool ConsumeFrameL(const CFbsBitmap* aBitmap);
private:
	virtual TVerdict TestL();
	const TFrameCheckPoint* iNextCheckPoint;
	TVerdict iLatestCheckpointVerdict;
	TTime	iPreviousFrameTime;
};


class CTestStepMagn : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepMagn(const TImageDisplayUnitTestParams& aTestParams, const TUint* aColorRef, const TUint* aAlphaRef);

protected:
	virtual TBool ConsumeFrameL(const CFbsBitmap* aBitmap);

private:
	virtual TVerdict TestL();
	const TUint* const  iColorRefData;
	const TUint* const  iAlphaRefData;
	};


class CTestStepNegative : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepNegative(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};


class CTestStepStopResponse : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepStopResponse(const TImageDisplayUnitTestParams& aTestParams);

protected:
	static TInt TimerGate(TAny* aPtr);
	void DoTimerGate();
private:
	virtual TVerdict TestL();
private:
	TBool iStoppedByTimer;
	TTime iDecodeStarted;
	TTime iResponseGot;
	TInt iDelayRequested;
	};
class CCallbackTimer;
class CTestStepPauseResponse : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepPauseResponse(const TImageDisplayUnitTestParams& aTestParams);
	void Cleanup();

protected:
	static TInt TimerGate(TAny* aPtr);
	void DoTimerGate();

	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	virtual TBool ConsumeFrameL(const CFbsBitmap* aBitmap);
private:
	virtual TVerdict TestL();
private:
	TBool iStoppedByTimer;
	TBool iWaitingBeforeCont;
	TTime iDecodeStarted;
	TTime iResponseGot;
	TInt iDelayRequested;
	CCallbackTimer* iTimer;
	};

class CTestStepClipping : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepClipping(const TImageDisplayUnitTestParams& aTestParams);

protected:
	virtual TBool ConsumeFrameL(const CFbsBitmap* aBitmap);

private:
	virtual TVerdict TestL();
private:
	};


class CTestStepPanicNotInit : public CTestStepNegative 
	{

public:
	CTestStepPanicNotInit(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};

class CTestStepImgDisplayCAF : public CTestStepImgDisplayGeneric
{
public:
	CTestStepImgDisplayCAF(const TImageDisplayUnitTestParams& aTestParams);
private:
	virtual TVerdict TestL();
private:
};

#endif // TESTSTEPMNGIMAGEDISPLAY_H

