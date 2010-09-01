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

#ifndef IMAGEDECODERTEST_H
#define IMAGEDECODERTEST_H

const TInt KDefaultStepsBeforeCancel = 1;	
class RCImageDecoderDecodeTest;

class MCancelledInterface 
	{
public:
	virtual void SetCancelled() = 0;
	};

class CTestActiveScheduler : public CActiveScheduler
	{
public:
//	CTestActiveScheduler() : CActiveScheduler()	{}

	TAny	SetDecoder(
		CImageDecoder*	aDecoder, 
		CActiveListener* aListener, 
		CFbsBitmap* aTestBitmap,
		CFbsBitmap* aTestBitmapMask,
		TInt aStepBeforeCancel)
		{
		iDecoder = aDecoder;
		iListener = aListener;
		iTestBitmap = aTestBitmap,
		iTestBitmapMask = aTestBitmapMask,
		iNumStep = aStepBeforeCancel;
		}
	void	SetStep(MCancelledInterface *aStep) {iStep = aStep;}
	virtual void WaitForAnyRequest();

private:
	void DecodeCancelAndContinue(void);

protected:
	CImageDecoder *	iDecoder;
	CActiveListener* iListener;
	MCancelledInterface *iStep;
	TInt	iNumStep;
	TInt	iCurStep;
	CFbsBitmap* iTestBitmap;
	CFbsBitmap* iTestBitmapMask;
	};


class RCImageDecoderDecodeTest : public RAllocTest, public MCancelledInterface
	{
public:
	static RCImageDecoderDecodeTest* NewL(CTestSuite* aSuite, TContentMode aContentMode, TBool aMaskDecode, TBool aAllocTest,TBool aCancelTest=EFalse);
	void SetCancelled() {iCancelled = ETrue;}

protected:
	void IncrementBufferSize();
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCImageDecoderDecodeTest(CTestSuite* aSuite, TContentMode aContentMode, TBool aMaskDecode, TBool aAllocTest, TBool aCancelTest=EFalse);

private:
	TUid iImplementationUid;

	CImageDecoder* iImageDecoder;

	TContentMode iContentMode; 
	TBool iMimeTest; //ETrue use MIME type for FileNewL()/DataNewL()
	TBool iMaskDecode;
	TBool iCancelTest;	//ETrue if step should be cancelled
	TBool iCancelled;	//ETrue if step has been successfully cancelled (& restarted)

	TInt  iSourceLength;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	RFile		iSourceFile;
	TFileName	iSourceFilename;
	TBuf8<KDefaultBufferSize>	iMimeType;
	CFbsBitmap* iTestBitmap;
	CFbsBitmap* iTestBitmapMask;
	CFbsBitmap* iRefBitmap;
	RFs			iFileSession;

	CTestActiveScheduler* iScheduler ;	
	};


class RCImageDecoderIntentTest: public RAllocTest
	{
public:
	static RCImageDecoderIntentTest* NewL(CTestSuite* aSuite, TBool aMimeTest);

protected:
	void IncrementBufferSize();
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCImageDecoderIntentTest(CTestSuite* aSuite, TBool aMimeTest);

private:
	TUid iImplementationUid;

	TBool iMimeTest;

	CImageDecoder* iImageDecoder;

	TInt  iSourceLength;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	RFile		iSourceFile;
	TFileName	iSourceFilename;
	TBuf8<KDefaultBufferSize>	iMimeType;
	CFbsBitmap* iTestBitmap;
	CFbsBitmap* iRefBitmap;
	RFs			iFileSession;

	CTestActiveScheduler* iScheduler ;	
	};


class RCImageDecoderStaticTest : public RAllocTest
	{
public:
	static RCImageDecoderStaticTest* NewL(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	// From RTestStep
	virtual void Close();

private:
	RCImageDecoderStaticTest(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest);

private:
	TInt iTestNumber;
	RFs iFileSession;
	TInt iError;

	RImageTypeDescriptionArray iImageTypeArray;
	RImageTypeDescriptionArray iImageSubTypeArray;
	RFileExtensionMIMETypeArray iFileExtensionArray;
	RUidDataArray iPropertiesArray;
	RUidDataArray iImplArray;

	HBufC8* iSourceData;
	TPtr8	iSourceDataPtr;
	TBuf8<KDefaultBufferSize> iFileMimeType;
	TBuf8<KDefaultBufferSize> iDataMimeType;
	CActiveScheduler* iScheduler;
	};

class RCImageDecoderResolverTest : public RAllocTest
	{
public:
	static RCImageDecoderResolverTest* NewL(CTestSuite* aSuite, TContentMode aContentMode, TBool aAllocTest);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	// From RTestStep
	virtual void Close();

private:
	RCImageDecoderResolverTest(CTestSuite* aSuite, TContentMode aContentMode, TBool aAllocTest);

private:
	RFs iFileSession;
	CActiveScheduler* iScheduler;

	TContentMode iContentMode;

	CImageDecoder* iMimeDecoder;
	CImageDecoder* iContentDecoder;

	TInt iError;
	TFileName iSourceFilename;
	HBufC8* iSourceData;
	TPtr8	iSourceDataPtr;
	TBuf8<KDefaultBufferSize> iMimeType;
	};

class RCImageDecoderMaskDecodeTest : public RICLFrameworkTestStep
	{
public:
	static RCImageDecoderMaskDecodeTest* NewL(CTestSuite* aSuite, TBool aAlphaTest);

protected:
	// From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RCImageDecoderMaskDecodeTest(CTestSuite* aSuite, TBool aAlphaTest);

private:
	RFs iFileSession;
	CActiveScheduler* iScheduler;
	TBool iAlphaTest;

	TSize iBitmapSize;
	TDisplayMode iBitmapDisplayMode;
	TFileName iSourceFilename;
	CImageDecoder* iDecoder;
	};

class RThreadedDecodeCancelTest : public RICLFrameworkTestStep
	{
public:
	static RThreadedDecodeCancelTest* NewL(CTestSuite* aSuite);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RThreadedDecodeCancelTest(CTestSuite* aSuite);

private:
	CImageDecoder* iDecoder;
	CFbsBitmap* iBitmap;
	
	RFs			iFileSession;
	TFileName	iSourceFilename;

	CActiveScheduler* iScheduler ;
	};

/**
*
* Open an OTB file and decode
* *
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTImageDecDRMViewInvalIdMthd1
*
*/
class RTImageOpenOTB : public RICLFrameworkTestStep
    {
public:
    static RTImageOpenOTB* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RTImageOpenOTB(CTestSuite* aSuite);
    
public:
    RFs iFileSession;
    TFileName iFileName;

private:
    CActiveScheduler* iActiveScheduler;
    };



/**
*
* Create an CImageDecoder with invalid Mime Type
* *
* @lib "TSU_ICL_FRM_01.lib"
*
*/
class RDecoderInvalidMimeTypeArg : public RICLFrameworkTestStep
    {
public:
    static RDecoderInvalidMimeTypeArg* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    RDecoderInvalidMimeTypeArg(CTestSuite* aSuite);
    
public:
    RFs iFileSession;

    };

/**
	PREQ1630: Test client API calls to framework extensions.
	Extend the CImageDecoder in order to access the CustomSync
	and CustomAsync calls to pass the TFwExtParamCheck class from
	the test to the codec plugin.
*/
class CFwExtImageDecoder : public CImageDecoder
	{
public:
	// From CImageDecoder
	void CustomSyncL(TInt aParam) { CImageDecoder::CustomSyncL(aParam); }
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam) { CImageDecoder::CustomAsync(aRequestStatus, aParam); }
	};

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvOperation interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CDecodeOperationExtension.
	
	This object is created on the test client-side and a pointer to it is
	passed to the CFwExtTestDecoderPlugin which takes ownership.  All calls
	to the CDecodeOperationExtension result in a call to this object which
	allows the test code to set and check values that are being passed via
	the Framework Extension mechanism.
*/
class CFwExtTestDecOperation : public CBase,
							   public MImageConvOperation
	{
public:
	static CFwExtTestDecOperation* NewL();
	~CFwExtTestDecOperation();

	// From MImageConvOperation
	TUid Uid() const;
	void IncrementRef();
	void Release();
	TUint Capabilities() const;
	void AddOperationL(TImageConvOperation::TOperation aOperation);
	void ClearOperationStack();
	
	// For test purposes
	void SetCapabilities(TUint aCaps);
	TUint Operations();
	
private:
	CFwExtTestDecOperation();
	
private:
	TInt iReferenceCount;
	TUint iOperationCaps;
	TUint iOperations;
	};
	
/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvOperation interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CScalerExtension.
	
	This object is created on the test client-side and a pointer to it is
	passed to the CFwExtTestDecoderPlugin which takes ownership.  All calls
	to the CScalerExtension result in a call to this object which
	allows the test code to set and check values that are being passed via
	the Framework Extension mechanism.
*/
class CFwExtTestScaler : public CBase,
						 public MImageConvScaler
	{
public:
	static CFwExtTestScaler* NewL();
	~CFwExtTestScaler();

	// From MImageConvScaler
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetCapabilities(TScalerCaps& aCaps) const;
	void SetScalingL(const TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	void SetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);
	
	// For test purposes
	void SetCapabilitiesL(TInt aMaxUpscaleLimit, TInt aMaxDownscaleLimit, TBool aMustPreserveAspectRatio);
	void SetScaledSize(TSize& aOriginalSize, TSize& aDesiredSize, TBool aLockAspectRatio);
	TInt CheckSetScalingL(TSize& aDesiredSize, TImageConvScaler::TScalerQuality aQuality, TBool aLockAspectRatio);
	TInt CheckSetScalingL(TInt aScalingCoeff, TImageConvScaler::TScalerQuality aScalingQuality);
	
protected:
	CFwExtTestScaler();
	
private:
	TInt iReferenceCount;
	
	// Used by Get/SetCapabilities()
	TScalerCaps* iScalerCaps;

	// Used by Get/SetScaledSize()
	TSize iOriginalSize;
	TSize iDesiredSize;
	TBool iLockAspectRatio;
	
	// Used by Get/SetScalingL()
	TSize iDesiredSizeSetScalingL;
	TImageConvScaler::TScalerQuality iQuality;
	TBool iLockAspectRatioSetScalingL;
	
	// Used by Get/SetScalingL()
	TInt iScalingCoeff;
	TImageConvScaler::TScalerQuality iScalingQuality;
	};

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvStreamedDecode interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CStreamedDecodeExtension
	
	This object is created on the test client-side and a pointer to it is
	passed to the CFwExtTestDecoderPlugin which takes ownership.  All calls
	to the CStreamedDecodeExtension result in a call to this object which
	allows the test code to set and check values that are being passed via
	the Framework Extension mechanism.
*/
class CFwExtTestStreamedDecode : public CBase,
								 public MImageConvStreamedDecode
	{
public:
	static CFwExtTestStreamedDecode* NewL();
	~CFwExtTestStreamedDecode();
	
	// From MImageConvStreamedDecode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps) const;
	TInt GetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation);
	void GetBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt* aNumBlocksRead);
	void GetNextBlocks(TRequestStatus* aStatus, CImageFrame* aFrame, TInt aNumBlocksToGet, TInt* aNumBlocksRead, TBool* aHaveMoreBlocks);
	
	// For test purposes
	void SetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat);
	void SetCapabilities(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps& aCaps);
	void SetBufferSize(TUid aFormat, TSize& aBlockSizeInPixels, TInt aNumBlocks);
	TInt CheckInitFrameL(TUid aFormat, TInt aFrameNumber, TDecodeStreamCaps::TNavigation aNavigation);
	void SetBlocks(TRequestStatus aStatus, CImageFrame& aFrame, TInt aSeqPosition, TInt aNumBlocksToGet, TInt aNumBlocksRead);
	void SetNextBlocks(TRequestStatus aStatus, CImageFrame& aFrame, TInt aNumBlocksToGet, TInt aNumBlocksRead, TBool aHaveMoreBlocks);
			
protected:
	CFwExtTestStreamedDecode();
	
private:
	TInt iReferenceCount;
	
	// for Get/SetSupportformatsL
	RArray<TUid> iFormats;
	TUid iOptimalFormat;
	
	// for Get/SetCapabilities
	TUid iFormat;
	TInt iFrameNumber;
	TDecodeStreamCaps iDecodeStreamCaps;
	
	// for Get/SetBufferSize
	TUid iFormatGetBufferSizeL;
	TSize iBlockSizeInPixels;
	TInt iNumBlocks;
	
	// for InitFrameL/CheckInitFrameL
	TUid iFormatInitFrameL;
	TInt iFrameNumberInitFrameL;
	TDecodeStreamCaps::TNavigation iNavigation;
	
	// Get/SetBlocks
	TRequestStatus iStatus;
	CImageFrame* iFrame;
	TInt iSeqPosition;
	TInt iNumBlocksToGet;
	TInt iNumBlocksRead;
	
	// Get/SetNextBlocks	
	TRequestStatus iStatusGetNextBlocks;
	CImageFrame* iFrameGetNextBlocks;
	TInt iNumBlocksToGetGetNextBlocks;
	TInt iNumBlocksReadGetNextBlocks;
	TBool iHaveMoreBlocks;
	};	

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvPrepare interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CPrepareExtension
	
	This object is created on the test client-side and a pointer to it is
	passed to the CFwExtTestDecoderPlugin which takes ownership.  All calls
	to the CPrepareExtension result in a call to this object which
	allows the test code to set and check values that are being passed via
	the Framework Extension mechanism.
*/
class CFwExtTestPrepare : public CBase,
						  public MImageConvPrepare
	{
public:
	static CFwExtTestPrepare* NewL();
	~CFwExtTestPrepare();
	
	// From MImageConvPrepare
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void Prepare(TRequestStatus* aStatus);
	
	// For test purposes
	void SetPrepare(TBool aSetPrepareCalled);
			
protected:
	CFwExtTestPrepare();
	
private:
	TInt iReferenceCount;
	TBool iSetPrepareCalled; // just to check comms
	};	
    
/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the PNG Decoder.
	PNG does not support the framework extensions.
*/
class RFwExtPNGDecNotSupportedTest : public RICLFrameworkTestStep
    {
public:
    enum TFwExtMethod
    	{
    	EOperationL, // 0
    	EScalerL,
    	EBlockStreamerL,
    	EGetDestinationSize,
    	ESetClippingRectL
    	};    
    
public:
    static RFwExtPNGDecNotSupportedTest* NewL(CTestSuite* aSuite,
    										TBool aThreaded,
    										TFwExtMethod aMethod,
    										const TDesC& aTestStepName);

    // from RTestStep
    virtual TVerdict OpenL();
    virtual TVerdict DoTestStepL();
    virtual void Close();

private:
    RFwExtPNGDecNotSupportedTest(CTestSuite* aSuite,
								TBool aThreaded,
								TFwExtMethod aMethod,
								const TDesC& aTestStepName);
    
public:
    RFs iFileSession;
    
private:
	CActiveScheduler* iScheduler;
    CImageDecoder* iImageDecoder;
    TBool iThreaded;
    CImageDecoder::TOptions iOptions;
    TFwExtMethod iMethod;
    };

/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the FwExtTest Decoder.
	FwExtTest codec supports the framework extensions but
	no actual Convert() call.  These tests exercise the
	Framework and its related classes.
*/
class RFwExtDecSupportedTest : public RICLFrameworkTestStep
    {
public:
    enum TFwExtMethod
    	{
    	EOperationL, // 0
    	EScalerL,
    	EBlockStreamerLSetup,
    	EBlockStreamerLGetBlocks,
    	EBlockStreamerLGetNextBlocks,
    	EGetDestinationSize,
    	ESetClippingRectL,
    	EPrepare,
    	};    
    
public:
    static RFwExtDecSupportedTest* NewL(CTestSuite* aSuite,
    									TBool aThreaded,
    									TFwExtMethod aMethod,
    									const TDesC& aTestStepName);

    // from RTestStep
    virtual TVerdict OpenL();
    virtual TVerdict DoTestStepL();
    virtual void Close();

private:
    RFwExtDecSupportedTest(CTestSuite* aSuite,
							TBool aThreaded,
							TFwExtMethod aMethod,
							const TDesC& aTestStepName);
    
public:
    RFs iFileSession;
    
private:
	CActiveScheduler* iScheduler;
	CActiveListener* iActiveListener;
    CFwExtImageDecoder* iImageDecoder;
    TBool iThreaded;
    CImageDecoder::TOptions iOptions;
    TFwExtMethod iMethod;
	CFbsBitmap* iTestBitmap;
    };

#endif // IMAGEDECODERTEST_H
