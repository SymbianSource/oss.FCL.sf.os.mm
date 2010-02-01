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

#if (!defined __IMAGE_ENCODER_TEST_H__)
#define __IMAGE_ENCODER_TEST_H__  

#include "ICLFrameWorkTestStep.h"

class RCImageEncoderEncodeTest : public RAllocTest
	{
public:
	static RCImageEncoderEncodeTest* NewL(CTestSuite* aSuite, TContentMode aContentMode, TBool aMimeTest, TBool aAllocTest);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCImageEncoderEncodeTest(CTestSuite* aSuite, TContentMode aContentMode, TBool aMimeTest, TBool aAllocTest);

private:
	CImageEncoder* iImageEncoder;

	CFbsBitmap* iSourceBitmap;
	HBufC8*		iImageData;
	HBufC8*		iRefImageData;
	TBuf8<20>	iMimeType;

	TContentMode iContentMode;
	TBool iMimeTest;

	RFs iFileSession;
	TFileName iDestinationFilename;
	CActiveScheduler* iScheduler ;
	};

class RCImageEncoderStaticTest : public RAllocTest
	{
public:
	static RCImageEncoderStaticTest* NewL(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();

private:
	RCImageEncoderStaticTest(CTestSuite* aSuite, TInt aTestNumber, TBool aAllocTest);

private:
	RFs iFileSession;
	TInt iTestNumber;
	TInt iError;

	RImageTypeDescriptionArray iImageTypeArray;
	RImageTypeDescriptionArray iImageSubTypeArray;
	RFileExtensionMIMETypeArray iFileExtensionArray;	
	RUidDataArray iPropertiesArray;
	RUidDataArray iImplArray;
	CActiveScheduler* iScheduler ;
	};

class RThreadedEncodeCancelTest : public RICLFrameworkTestStep
	{
public:
	static RThreadedEncodeCancelTest* NewL(CTestSuite* aSuite);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RThreadedEncodeCancelTest(CTestSuite* aSuite);

private:
	CImageEncoder* iEncoder;
	CFbsBitmap* iBitmap;
	
	RFs			iFileSession;
	CActiveScheduler* iScheduler ;
	HBufC8* iDestinationData;
	};


/**
*
* Create an CImageEncoder with invalid Mime Type
* *
* @lib "TSU_ICL_FRM_01.lib"
*
*/
class REncoderInvalidMimeTypeArg : public RICLFrameworkTestStep
    {
public:
    static REncoderInvalidMimeTypeArg* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict DoTestStepL();

private:
    REncoderInvalidMimeTypeArg(CTestSuite* aSuite);
    
public:
    RFs iFileSession;

    };
/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the PNG Encoder.
	PNG does not support the framework extensions.
*/
class RFwExtPNGEncNotSupportedTest : public RICLFrameworkTestStep
    {
public:
    enum TFwExtMethod
    	{
    	EOperationL, // 0
    	EBlockStreamerL,
    	};    
    
public:
    static RFwExtPNGEncNotSupportedTest* NewL(CTestSuite* aSuite,
    										TBool aThreaded,
    										TFwExtMethod aMethod,
    										const TDesC& aTestStepName);

    // from RTestStep
    virtual TVerdict OpenL();
    virtual TVerdict DoTestStepL();
    virtual void Close();

private:
    RFwExtPNGEncNotSupportedTest(CTestSuite* aSuite,
								TBool aThreaded,
								TFwExtMethod aMethod,
								const TDesC& aTestStepName);
    
public:
    RFs iFileSession;
    
private:
	CActiveScheduler* iScheduler;
    CImageEncoder* iImageEncoder;
    TBool iThreaded;
    CImageEncoder::TOptions iOptions;
    TFwExtMethod iMethod;
    };

/**
	PREQ1630: Test client API calls to framework extensions.
	Extend the CImageEncoder in order to access the CustomSync
	and CustomAsync calls to pass the TFwExtParamCheck class from
	the test to the codec plugin.
*/
class CFwExtImageEncoder : public CImageEncoder
	{
public:
	// From CImageEncoder
	void CustomSyncL(TInt aParam) { CImageEncoder::CustomSyncL(aParam); }
	void CustomAsync(TRequestStatus* aRequestStatus, TInt aParam) { CImageEncoder::CustomAsync(aRequestStatus, aParam); }
	};

/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvOperation interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CEncodeOperationExtension.
	
	This object is created on the test client-side and a pointer to it is
	passed to the CFwExtTestDecoderPlugin which takes ownership.  All calls
	to the CStreamedEncodeExtension result in a call to this object which
	allows the test code to set and check values that are being passed via
	the Framework Extension mechanism.
*/
class CFwExtTestEncOperation : public CBase,
							   public MImageConvOperation
	{
public:
	static CFwExtTestEncOperation* NewL();
	~CFwExtTestEncOperation();

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
	CFwExtTestEncOperation();
	
private:
	TInt iReferenceCount;
	TUint iOperationCaps;
	TUint iOperations;
	};
	
/**
	PREQ1630: Test client API calls to framework extensions.
	Implement the MImageConvStreamedEncode interface to allow callbacks
	from the actual plugin extension.
	
	Effectively a mirror of the test codec extension CStreamedEncodeExtension.
	
	This object is created on the test client-side and a pointer to it is
	passed to the CFwExtTestDecoderPlugin which takes ownership.  All calls
	to the CStreamedEncodeExtension result in a call to this object which
	allows the test code to set and check values that are being passed via
	the Framework Extension mechanism.
*/
class CFwExtTestStreamedEncode : public CBase,
								 public MImageConvStreamedEncode
	{
public:
	static CFwExtTestStreamedEncode* NewL();
	~CFwExtTestStreamedEncode();

	// From MImageConvStreamedEncode
	TUid Uid() const;
	void IncrementRef();
	void Release();
	void GetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat) const;
	void GetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps) const;
	void InitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	void AppendBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	void AddBlocks(TRequestStatus* aStatus, const CImageFrame& aBlocks, const TInt& aSeqPosition);
	void Complete(TRequestStatus* aStatus);
	
	// For test purposes
	void SetSupportedFormatsL(RArray<TUid>& aFormats, TUid& aOptimalFormat);
	void SetCapabilities(TUid aFormat, TEncodeStreamCaps& aCaps);
	TInt CheckInitFrameL(TUid aFormat, TInt aFrameNumber, const TSize& aFrameSizeInPixels, const TSize& aBlockSizeInPixels, TEncodeStreamCaps::TNavigation aNavigation, const CFrameImageData* aFrameImageData);
	TInt CheckAppendBlocks(const CImageFrame& aBlocks, TInt aNumBlocksToAdd);
	TInt CheckAddBlocks(const CImageFrame& aBlocks, const TInt& aSeqPosition);
	TInt CheckComplete();

protected:
	CFwExtTestStreamedEncode();
	
private:
	TInt iReferenceCount;
	
	// for Get/SetSupportedFormats
	RArray<TUid> iFormats;
	TUid iOptimalFormat;
	
	// for Get/SetCapabilities
	TUid iFormat;
	TEncodeStreamCaps iEncodeStreamCaps;
	
	// for InitFrameL/CheckInitFrameL
	TUid iFormatInitFrameL;
	TInt iFrameNumber;
	TSize iFrameSizeInPixels;
	TSize iBlockSizeInPixels;
	TEncodeStreamCaps::TNavigation iNavigation;
	const CFrameImageData* iFrameImageData;
	
	// for AppendBlocks/CheckAppendBlocks
	TRequestStatus* iReqAppendBlocks;
	const CImageFrame* iBlocksAppendBlocks;
	TInt iNumBlocksToAddAppendBlocks;
	
	// for AddBlocks/CheckAddBlocks
	TRequestStatus* iReqAddBlocks;
	const CImageFrame* iBlocksAddBlocks;
	TInt iSeqPosition;
	
	// for Complete/CheckComplete
	TRequestStatus* iReqComplete;
	};
	
/**
	PREQ1630: Test client API calls to framework extensions.
	Uses the FwExtTest Encoder.
	FwExtTest codec supports the framework extensions.
*/
class RFwExtEncSupportedTest : public RICLFrameworkTestStep
    {
public:
    enum TFwExtMethod
    	{
    	EOperationL, // 0
    	EPrepare,
    	EBlockStreamerLSetup,
    	EBlockStreamerLAppendBlocks,
    	EBlockStreamerLAddBlocks,
    	};    
    
public:
    static RFwExtEncSupportedTest* NewL(CTestSuite* aSuite,
    									TBool aThreaded,
    									TFwExtMethod aMethod,
    									const TDesC& aTestStepName);

    // from RTestStep
    virtual TVerdict OpenL();
    virtual TVerdict DoTestStepL();
    virtual void Close();

private:
    RFwExtEncSupportedTest(CTestSuite* aSuite,
							TBool aThreaded,
							TFwExtMethod aMethod,
							const TDesC& aTestStepName);
    
public:
    RFs iFileSession;
    
private:
	CActiveScheduler* iScheduler;
	CActiveListener* iActiveListener;
    CFwExtImageEncoder* iImageEncoder;
    TBool iThreaded;
    CImageEncoder::TOptions iOptions;
    TFwExtMethod iMethod;
    CFbsBitmap* iTestBitmap;
    HBufC8 *iDestinationImage;
    };

// DEF119087: After call CImageEncoder::Cancel, uncompleted file still remains on disk	
class RThreadedFileEncodeCancelAfterConvertTest : public RICLFrameworkTestStep
	{
public:
	static RThreadedFileEncodeCancelAfterConvertTest* NewL(CTestSuite* aSuite);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RThreadedFileEncodeCancelAfterConvertTest(CTestSuite* aSuite);

private:
	CImageEncoder* iEncoder;
	CFbsBitmap* iBitmap;
	
	RFs			iFileSession;
	CActiveScheduler* iScheduler;
	};
	
class RThreadedFileHandleEncodeCancelAfterConvertTest : public RICLFrameworkTestStep
	{
public:
	static RThreadedFileHandleEncodeCancelAfterConvertTest* NewL(CTestSuite* aSuite);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RThreadedFileHandleEncodeCancelAfterConvertTest(CTestSuite* aSuite);

private:
	CImageEncoder* iEncoder;
	CFbsBitmap* iBitmap;
	
	RFs			iFileSession;
	CActiveScheduler* iScheduler;
	};

class RThreadedFileEncodeDeleteTest : public RICLFrameworkTestStep
	{
public:
	static RThreadedFileEncodeDeleteTest* NewL(CTestSuite* aSuite);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RThreadedFileEncodeDeleteTest(CTestSuite* aSuite);

private:
	CImageEncoder* iEncoder;
	CFbsBitmap* iBitmap;
	
	RFs			iFileSession;
	CActiveScheduler* iScheduler;
	};

class RThreadedFileHandleEncodeDeleteTest : public RICLFrameworkTestStep
	{
public:
	static RThreadedFileHandleEncodeDeleteTest* NewL(CTestSuite* aSuite);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RThreadedFileHandleEncodeDeleteTest(CTestSuite* aSuite);

private:
	CImageEncoder* iEncoder;
	CFbsBitmap* iBitmap;
	
	RFs			iFileSession;
	CActiveScheduler* iScheduler;
	};

#endif // __IMAGE_ENCODER_TEST_H__
