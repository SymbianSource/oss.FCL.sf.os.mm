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

#if (!defined TESTSTEPEXIFIMAGEDISPLAY_H)
#define TESTSTEPEXIFIMAGEDISPLAY_H

#include <imagedisplay.h>
#include <icl/imagedisplayplugin.h>

class CTestSuite;
class CTestSuiteExifImageDisplay;

const TUint KDecodeFinishedMask		=(CImageDisplayPlugin::EStatusEraseOutputContents-1)|CImageDisplayPlugin::EStatusEraseOutputContents;

const TUint KFrameReadyStatus		=CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusPaused;

const TUint KLastFrameReadyStatus	=CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusNoMoreToDecode;

const TUint KDecodeFinishedKeepStatus=KLastFrameReadyStatus|CImageDisplayPlugin::EStatusDisplayThisFrameIndefinetely;
const TUint KDecodeFinishedEraseStatus=KLastFrameReadyStatus|CImageDisplayPlugin::EStatusEraseOutputContents;

const TUint KPartialFrameReadyStatus=CImageDisplayPlugin::EStatusFrameReady|CImageDisplayPlugin::EStatusPartialFrame
										|CImageDisplayPlugin::EStatusPaused;

const TInt KOneSecond=1000000;

struct TFrameCheckPoint 
	{
public:
	enum 
		{
		KEndMarkerFrame		=-1,
		KNodelayChecking	=-1,
		KNoStatusChecking	= 0,
		KLastFrameCheck		= (1<<30)
		};
	TInt			iAbsoluteFrameNumber;
	const TText*	iReferenceFileName;
	TInt			iDelayFromPreviuos;
	TUint			iPluginStatusMask;
	TUint			iPluginStatusValue;
	const TText*	iReferenceMaskFileName;

public:

	inline TBool CompareStatus(const TUint aStatus) const
		{
		return (aStatus&iPluginStatusMask) == iPluginStatusValue;
		}

	inline TBool LastFramePoint() const
		{
		return iAbsoluteFrameNumber == KLastFrameCheck;
		}

	inline TBool IsValid() const
		{
		return iAbsoluteFrameNumber != KEndMarkerFrame;
		}

	};

#define ENDCHECKPOINT {TFrameCheckPoint::KEndMarkerFrame}

class TFunctionalTstParams
	{
public:
	enum 
		{
		KNoValueCheck			=0,
		KMaxNumberOfUids		=4,
		KScalingQualityNotSet	=-1,
		KReductionFactorNotSet	=-2,
		};

	inline TBool IsValid() const;
	TInt	iExpectedNumFrames;						// expected number of frames
	TUint	iExpectedImgStatus;						// ImageStatus() value
	TInt iSupportedExtsUidValues[KMaxNumberOfUids]; // Extesion i-face Uids
	TInt	iScalingQuality;						// parameter for scaling quality
	TInt    iMaxReductionFactor;					// parameter values for max reduction factor
	TInt	iMaxUnscaledSzWidth;					// max unscaled size width
	TInt	iMaxUnscaledSzHeight;					// max unscaled size height
	};

const TFunctionalTstParams KNoFunctionalParams =
		{
		TFunctionalTstParams::KNoValueCheck, 
		TFunctionalTstParams::KNoValueCheck, 
		{0} 
		};
inline 
TBool TFunctionalTstParams::IsValid() const
	{
	return (iExpectedNumFrames || iExpectedImgStatus || *iSupportedExtsUidValues);
	}

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
	enum 
		{
		KDestSizeIsSecondDefSize=-2,
		KDestSizeIsDoubleSize	=-10
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
	TReal iTolerance;				// tolerance to per-byte image diffs
	TInt  iErrorCode;				// expected error code for a negative test
	const TFunctionalTstParams* iFuncParams; // values of getter functions which are expected
	const TInt*					iClipRect; //clipping rect, must point to 4 TInt's if not NULL
	};
	
//inline operator TRect() const;

const TReal KQuaterPercent	= 1.0/400.0;
const TReal KHalfPercent	= 1.0/200.0;
const TReal KZeroTolerance	= 0.0;


/**
 *
 * CTestStepIclImageDisplay, which is the base class for all 
 * the ImageTransform suite test steps, providing common functionality.
 *
  *
 */
class CTestStepIclImageDisplay : public CTestStep, public MIclImageDisplayObserver
	{
public:
	CTestStepIclImageDisplay();
	~CTestStepIclImageDisplay();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
	void SetSuite(CTestSuiteExifImageDisplay* aPtr );
 	CTestSuiteExifImageDisplay* Suite();

protected:
	virtual void Cleanup(){};
    void SetSourceFilename(const TDesC& aPathNameAndExtn);
    void SetDestFilenameL(const TDesC& aPathNameAndExtn);
    void SetRefFilename(const TDesC& aPathNameAndExtn);
	void DeleteDestFilenameL();
	TInt CompareFiles(const TDesC& aFile1,const TDesC& aFile2);
	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2);
	TVerdict CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const;
	TInt CompareBitmapWithFileL(const CFbsBitmap& aBitmap,const TDesC& aFile, TBool aAllowDifDispMode=EFalse);
	TFileName& SourceFilename();
	TFileName& DestFilename();
	TFileName& RefFilename();
	void DefaultPath(TFileName& aName);
	void SetDiffTolerance(const TReal& aTolerance);

protected:
	mutable RFs				iFs;
	CImageDisplay*			iImageDisplay;
	TReal					iDiffTolerance;

private:
  	 CTestSuiteExifImageDisplay*	iImageDisplaySuite;
	 
	 TFileName iSourceFilename;
	 TFileName iDestFilename;
	 TFileName iRefFilename;
	 CActiveScheduler* iScheduler;
	};

inline
void CTestStepIclImageDisplay::SetDiffTolerance(const TReal& aTolerance)
	{
	iDiffTolerance = aTolerance;
	}

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
class CTestStepImgDisplayGeneric : public CTestStepIclImageDisplay
	{
public:

	CTestStepImgDisplayGeneric(const TImageDisplayUnitTestParams& aTestParams);
	~CTestStepImgDisplayGeneric();
	// from CTestStep
	virtual TVerdict DoTestStepL();

protected:
//
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	virtual TBool ConsumeFrame(const CFbsBitmap* aBitmap, TBool aNoDelayCheck=EFalse);
//
	void SetImageSourceL();
	void SetFilenamesL(const TDesC& aSourceFilename);
	void SetupPluginL();
	void SetDefaultDestSize();
	virtual TVerdict TestL();
	virtual void Cleanup();

	TBool AllocMode() const;

private:
	TVerdict DoTestStepFbsL();

protected:

	TVerdict CheckTiming(const TTime aStart, const TTime aEnd, TInt aMicroNeeded, TInt aAllowedPercentage) const;

	// pointer to current test data
	const TImageDisplayUnitTestParams* iTestParams;

	HBufC8* iSourceBuf;
	TInt iLastError;
	TInt iFrameNumber;
	TInt iFramesToDecode;
	RFile iFileHandle;
	TBool iFbsConnection;
	TUint iLastPluginStatus;
	TRect iLastUpdateRect;
	TBool iIsNaviEngine;
	TBool iIsH6;
	};

inline
TBool CTestStepImgDisplayGeneric::AllocMode() const
	{
	return iTestParams->iTestType==TImageDisplayUnitTestParams::EAlloc;
	}

class CTestStepWithCheckpoint : public CTestStepImgDisplayGeneric 
	{
	enum 
		{
		#ifdef SYMBIAN_WRITABLE_DATA_PAGING
		KAllowedTimePercentage=80	
		#else
		KAllowedTimePercentage=20
		#endif
		};
public:
	CTestStepWithCheckpoint(const TImageDisplayUnitTestParams& aTestParams);
protected:
	void ResetCheckPoints();
	TInt FuncTestL();
	virtual TBool ConsumeFrame(const CFbsBitmap* aBitmap, TBool aNoDelayCheck=EFalse);

	TVerdict CheckBitmap(const TDesC& aRefFileName, TBool aIsMask);

protected:
	TVerdict iLatestCheckpointVerdict;
	const TFrameCheckPoint* iNextCheckPoint;
	
private:
	virtual TVerdict TestL();
	const TFrameCheckPoint* const iFirstCheckPoint;
	
	TTime	iPreviousFrameTime;
};

class CTestStepExifGeneric : public CTestStepWithCheckpoint 
	{

public:
	CTestStepExifGeneric(const TImageDisplayUnitTestParams& aTestParams);

protected:

private:
	virtual TVerdict TestL();
private:
	};



class CCallbackTimer;
class CTestStepPauseResponse : public CTestStepWithCheckpoint 
	{

public:
	CTestStepPauseResponse(const TImageDisplayUnitTestParams& aTestParams);
	void Cleanup();

protected:
	static TInt TimerGate(TAny* aPtr);
	void DoTimerGate();

	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);

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


class CTestStepStopResponse : public CTestStepWithCheckpoint 
	{

public:
	CTestStepStopResponse(const TImageDisplayUnitTestParams& aTestParams);

protected:
	static TInt TimerGate(TAny* aPtr);
	void DoTimerGate();
	
	virtual void MiidoImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);
	
private:
	virtual TVerdict TestL();
private:
	TBool iStoppedByTimer;
	TTime iDecodeStarted;
	TTime iResponseGot;
	TInt iDelayRequested;
	CCallbackTimer* iCancelTimer;
	};


class CTestStepNegative : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepNegative(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};


class CTestStepNegativeReduction : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepNegativeReduction(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};

class CTestStepUnsupExt : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepUnsupExt(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};

class CTestStepBigReduction : public CTestStepImgDisplayGeneric 
	{

public:
	CTestStepBigReduction(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};


class CTestStepPanicNotInit : public CTestStepNegative 
	{

public:
	CTestStepPanicNotInit(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};

class CTestStepPanicInitFailed : public CTestStepNegative 
	{

public:
	CTestStepPanicInitFailed(const TImageDisplayUnitTestParams& aTestParams);

private:
	virtual TVerdict TestL();
	};
	


#endif // TESTSTEPEXIFIMAGEDISPLAY_H

