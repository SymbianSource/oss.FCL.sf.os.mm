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

#ifndef ICLFRAMEWORKTESTSTEP_H
#define ICLFRAMEWORKTESTSTEP_H 

#include <testframework.h>
#include <caf/caf.h>
using namespace ContentAccess;
#include "icl/ImageCodecData.h"
#include "icl/ImageConstruct.h"
#include "icl/ImagePlugin.h"
#include "ImageConversionPriv.h"
#include "ImagePrivSupport.h"
#include "imageframe.h"
#include "icl/imageconversionextension.h"
#include "icl/imageconversionextensionintf.h"

class TImageDataBlock;
class CFrameImageData;
class CImageTypeDescription;
class CFileExtensionMIMEType;
class CImplementationInformationType;

const TInt KDefaultBufferSize = 40;

enum TContentMode 
	{
	EContentDescriptor,
	EContentFile,
	EContentFileHandle
	};

class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

// default implementation
class MImageDecoderPrivSupportImp : public MImageDecoderPrivSupport
	{
public:
	TBool MustUseBufferWithDescriptor() const;
	void CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize);
	TBool AmInThread() const;
	TBool ShouldAbort() const;
	void Lock();
	void Unlock();
	void SaveFrameCount(TInt aFrameCount);
	};

// default implementation
class MImageEncoderPrivSupportImp : public MImageEncoderPrivSupport
	{
public:
	TBool AmInThread() const;
	TBool ShouldAbort() const;
	};


class RICLFrameworkTestStep : public RTestStep
	{
protected:
	void GetTestFileNameL(TFileName& aFileName, const TFileName& aPathNameAndExtn) const;
	void SetVerdict(TVerdict aVerdict);
	void Compare(TInt aNumber1, TInt aNumber2, const TDesC& aErrorMessage);
	void Compare(TUid aUid1, TUid aUid2, const TDesC& aErrorMessage);
	void Compare(const TDesC& aDescriptor1, const TDesC& aDescriptor2, const TDesC& aErrorMessage);
	void Compare(const TDesC8& aDescriptor1, const TDesC8& aDescriptor2, const TDesC& aErrorMessage);
	void ComparePointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage);
	void CompareDiffPointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage);
	void Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2, const TDesC& aErrorMessage);
	void SetFail(const TDesC& aErrorMessage);
	TBool CheckForFile(const RFs& aFs, const TDesC& aFileName);
	};

class RAllocTest : public RICLFrameworkTestStep
	{
protected:
	RAllocTest(TBool aAllocTest);

	virtual void SetupL() = 0;
	virtual void TestStepL() = 0;
	virtual void CheckResultL() = 0;
	virtual void Cleanup() = 0;
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

protected:
	TBool iAllocTest; //Alloc or normal test
	CActiveListener* iListener;
	};

class RTImageParameterDataTest : public RICLFrameworkTestStep
	{
public:
	static RTImageParameterDataTest* NewL(CTestSuite* aSuite);
	RTImageParameterDataTest(CTestSuite* aSuite);
	// From RTestStep
	virtual TVerdict DoTestStepL();
	};

class RTBufPtr8Test : public RICLFrameworkTestStep
	{
public:
	static RTBufPtr8Test* NewL(CTestSuite* aSuite);
	RTBufPtr8Test(CTestSuite* aSuite);
	// From RTestStep
	virtual TVerdict DoTestStepL();

protected:
	void ComparePointers(const TDesC8& aPtr1, const TUint8* aPtr2);
	void CompareLengths(const TDesC8& aPtr1, TInt aLength);
	void CompareMaxLengths(const TDes8& aPtr1, TInt aMaxLength);
	void CompareBuffer(const TDes8& aBuf, const TUint8* aPtr, TInt aLength, TInt aMaxLength);
	};

class RTImageDataTest : public RICLFrameworkTestStep
	{
protected:
	virtual void DoSetupL() = 0;
	virtual void DoCheckDuplicateL() = 0;

	// From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	void CheckDataType(TUid aDataType);

protected:
	TBool iImageDataTest;     // TImageDataBlock or TFrameDataBlock test;
	TImageDataBlock* iImageData;
	TFrameDataBlock* iFrameData;
	TImageDataBlock* iDuplicateImageData;
	TFrameDataBlock* iDuplicateFrameData;
	CFrameImageData* iFrameImageData;
	TUid iDataType;
	};

class RTBmpImageDataTest : public RTImageDataTest
	{
public:
	static RTBmpImageDataTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTBmpImageDataTest(CTestSuite* aSuite);

private:
	TInt iBitsPerPixel;
	};

class RTBmpCompressionTest : public RTImageDataTest
	{
public:
	static RTBmpCompressionTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTBmpCompressionTest(CTestSuite* aSuite);

private:
	TBmpCompression::TCompression iCompression;
	};

class RTJpegImageDataTest : public RTImageDataTest
	{
public:
	static RTJpegImageDataTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTJpegImageDataTest(CTestSuite* aSuite);

private:
	TJpegImageData::TColorSampling iSampleScheme;
	TInt iQualityFactor;
	};

class RTJpegQTableTest : public RTImageDataTest
	{
public:
	static RTJpegQTableTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTJpegQTableTest(CTestSuite* aSuite);

private:
	TInt iTableIndex;
	TBuf8<KJpgQTableEntries> iEntries;
	TBuf8<KDefaultBufferSize> iEntry;
	};

class RTJpegCommentTest : public RTImageDataTest
	{
public:
	static RTJpegCommentTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();
	virtual void Close();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTJpegCommentTest(CTestSuite* aSuite);

private:
	HBufC8* iComment;
	};

class RTGifBackgroundColorTest : public RTImageDataTest
	{
public:
	static RTGifBackgroundColorTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTGifBackgroundColorTest(CTestSuite* aSuite);

private:
	TInt iBackgroundColorIndex;
	TRgb iBackgroundColor;
	};

class RTGifImageDescriptorTest : public RTImageDataTest
	{
public:
	static RTGifImageDescriptorTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTGifImageDescriptorTest(CTestSuite* aSuite);

private:
	TBool iLocalColorMap;
	TBool iSortedLocalMap;
	TBool iInterlaced;
	};

/**
 * Test duplicating TGifColorTable
 *
 * @xxxx
 */
class RTGifColorTableTest : public RTImageDataTest
	{
public:
	static RTGifColorTableTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTGifColorTableTest(CTestSuite* aSuite);

private:
	TRgb iPalette[KGifColorTableMaxEntries];
	};

class RTGifLZWInfoTest : public RTImageDataTest
	{
public:
	static RTGifLZWInfoTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTGifLZWInfoTest(CTestSuite* aSuite);

private:
	TInt iInitialCodeLength;
	TInt iCompressedBytes;
	};

class RTGifImageControlTest : public RTImageDataTest
	{
public:
	static RTGifImageControlTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTGifImageControlTest(CTestSuite* aSuite);

private:
	TInt iDelayTimeInCentiseconds;
	TInt iTransparentColorIndex;
	TBool iUserInputFlag;
	TGifImageControl::TDisposalMethod iDisposalMethod;
	};

class RTGifCommentTest : public RTImageDataTest
	{
public:
	static RTGifCommentTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();
	virtual void Close();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTGifCommentTest(CTestSuite* aSuite);

private:
	HBufC8* iComment;
	};

class RTMbmDecodeDataTest : public RTImageDataTest
	{
public:
	static RTMbmDecodeDataTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTMbmDecodeDataTest(CTestSuite* aSuite);

private:
	TBool iCompressed;
	TInt iImageDataSizeInBytes;
	};

class RTMbmEncodeDataTest : public RTImageDataTest
	{
public:
	static RTMbmEncodeDataTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTMbmEncodeDataTest(CTestSuite* aSuite);

private:
	TDisplayMode iDisplayMode;
	};

class RTTiffImageDataTest : public RTImageDataTest
	{
public:
	static RTTiffImageDataTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();
	virtual void Close();

	// From RTImageDataTest
	virtual void DoSetupL();
	virtual void DoCheckDuplicateL();

private:
	RTTiffImageDataTest(CTestSuite* aSuite);

private:
	TUint32		iNewSubfileType;
	TSize		iSizeInPixels;
	TInt		iBitsPerSample;
	TInt		iSamplesPerPixel;
	TInt		iCompression;
	TInt		iT4Options;
	TInt		iT6Options;
	TInt		iPhotometricInterpretation;
	TInt		iFillOrder;
	TReal		iWidthInTwips;
	TReal		iHeightInTwips;
	TInt		iRowsPerStrip;

	TInt		iNumStrips;
	HBufC8*		iStripInfo;
	};

class RCImageTypeDescriptionTest : public RAllocTest
	{
public:
	static RCImageTypeDescriptionTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCImageTypeDescriptionTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	CImageTypeDescription* iImageTypeDescription;
	TBufC<KDefaultBufferSize> iDescription;
	TUid iImageType;
	TUid iSubType;
	};

class RCFileExtensionMIMETypeTest : public RAllocTest
	{
public:
	static RCFileExtensionMIMETypeTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCFileExtensionMIMETypeTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	CFileExtensionMIMEType* iFileExtensionMIMEType;
	TBuf16<KDefaultBufferSize> iFileExtension16;
	TBuf8<KDefaultBufferSize> iFileExtension8;
	TBuf8<KDefaultBufferSize> iMIMEType;
	TBuf<KDefaultBufferSize> iDisplayName;
	TUid iImageType;
	TUid iImageSubType;
	TUid iImplementationUid;
	};

class RCImplementationInformationTypeTest : public RAllocTest
	{
public:
	static RCImplementationInformationTypeTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCImplementationInformationTypeTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	CImplementationInformationType* iImpInfo;
	TUid		iImplementationUid;
	TInt		iVersion;
	TBuf<KDefaultBufferSize>	iDisplayName;
	TBuf8<KDefaultBufferSize>	iDataType;
	TBuf8<KDefaultBufferSize>	iOpaqueData;
	TDriveUnit  iDriveUnit;
	};

class RCFrameImageDataTest : public RAllocTest
	{
public:
	static RCFrameImageDataTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCFrameImageDataTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	CFrameImageData* iFrameImageData;
	TBmpImageData iBmpImageData;
	TJpegImageData iJpegImageData;
	TGifImageControl iGifImageControl;
	TGifImageDescriptor iGifImageDescriptor;
	};

class RCFrameImageDataAppendImageBufferTest : public RAllocTest
	{
public:
	static RCFrameImageDataAppendImageBufferTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCFrameImageDataAppendImageBufferTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	CFrameImageData* iFrameImageData;
	RPointerArray<HBufC8> iBuffer;
	};

class RCFrameImageDataAllocLTest : public RAllocTest
	{
public:
	static RCFrameImageDataAllocLTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCFrameImageDataAllocLTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	CFrameImageData* iFrameImageData;
	CFrameImageData* iDuplicate;
	};

class RCFrameInfoStringsTest : public RAllocTest
	{
public:
	static RCFrameInfoStringsTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RTestStep
	virtual void Cleanup();
	//From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCFrameInfoStringsTest(CTestSuite* aSuite, TBool aAllocTest = EFalse);

private:
	enum TFrameInfoStringIndex
		{
		EDecoder	= 0,
		EFormat		= 1,
		EDimensions = 2,
		EDepth		= 3,
		EDetails	= 4
		};

	CFrameInfoStrings* iFrameInfoStrings;
	
	TBuf<KDefaultBufferSize> iData[5];
	};

class RTFrameInfoTest : public RICLFrameworkTestStep
	{
public:
	static RTFrameInfoTest* NewL(CTestSuite* aSuite);

protected:
	// From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();

private:
	RTFrameInfoTest(CTestSuite* aSuite);

private:
	TFrameInfo::TFrameInfoState iFrameInfoState;
	TInt iFrameDataOffset;
	};

class CTestDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CTestDecodeConstruct* NewL();
	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;

private:
	CTestDecodeConstruct();

public:
	mutable TInt iNewPluginLCount;

	};

class CTestReadCodec : public CImageReadCodec
	{
public:
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	};

class CTestDecoderPlugin : public CImageDecoderPlugin
	{
public:
	static CTestDecoderPlugin* NewL();

	//From CImageDecoderPlugin
	virtual void ImageType(int,class TUid& aImageType,class TUid &aImageSubType) const;
	virtual void ScanDataL();
	virtual CFrameInfoStrings* FrameInfoStringsL(class RFs& aFs,int);
	virtual void ReadFrameHeadersL();

private:
	CTestDecoderPlugin();

public:
	TInt  iScanDataLCount;
	TInt  iScanDataLLeave;
	TBool iFrameInfoStringsLCalled;
	TBool iReadFrameHeadersLCalled;
	RFs*  iFs; //Not owned
	};

class RCImageDecoderPrivTestBase : public RAllocTest, public MImageDecoderPrivSupportImp
	{
protected:
	RCImageDecoderPrivTestBase(TBool aAllocTest);

	//Methods to inspect CImageDecoderPriv 
	CImageDecodeConstruct* Construct();
	CImageDecoderPriv* PluginProperties();
	RFs* FileSession();
	CImageDecoder::TOptions Options();
	TImageParameterData* ImageParameterData();
	CData* Data();
	void SetImageReadCodec(CImageReadCodec* aReadCodec);
	TBool ProcessingFrameInfo();
	TInt CurrentFrame();
	HBufC8* ReadBuffer();
	void SetFileSession(RFs& aFs);

	// From RAllocTest
	virtual void SetupL();
	virtual void Cleanup();
	// From RTestStep
	virtual void Close();

protected:
	CImageDecoderPriv* iImageDecoderPriv;
	CTestDecodeConstruct* iConstruct;
	CActiveScheduler* iScheduler;
	RThread iMyThread;
	};

class RCImageDecoderPrivTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	// From RTestStep
	virtual void Close();

private:
	RCImageDecoderPrivTest(CTestSuite* aSuite, TBool aAllocTest);
	};

class RCImageDecoderPrivSetFileLTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivSetFileLTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCImageDecoderPrivSetFileLTest(CTestSuite* aSuite, TBool aAllocTest);

private:
	RFs iFileSession;
	TFileName iFileName;
	CImageDecoder::TOptions iOptions;
	};

class RCImageDecoderPrivSetDataLTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivSetDataLTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCImageDecoderPrivSetDataLTest(CTestSuite* aSuite, TBool aAllocTest);

private:
	RFs iFileSession;
	TBufC8<KDefaultBufferSize> iData;
	CImageDecoder::TOptions iOptions;
	};

class RCImageDecoderPrivHandleNewlyOpenedImageLTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivHandleNewlyOpenedImageLTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();

private:
	RCImageDecoderPrivHandleNewlyOpenedImageLTest(CTestSuite* aSuite, TBool aAllocTest);
	};

class RCImageDecoderPrivContinueProcessingHeaderLTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivContinueProcessingHeaderLTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();

private:
	RCImageDecoderPrivContinueProcessingHeaderLTest(CTestSuite* aSuite, TBool aAllocTest);
	};

class RCImageDecoderPrivDecodeTest : public RAllocTest, public MImageDecoderPrivSupportImp
	{
public:
	static RCImageDecoderPrivDecodeTest* NewL(CTestSuite* aSuite, TBool aFileTest = ETrue, TBool aMaskDecode = EFalse, TBool aAllocTest = EFalse);

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
	RCImageDecoderPrivDecodeTest(CTestSuite* aSuite, TBool aFileTest, TBool aMaskDecode, TBool aAllocTest);

private:
	CImageDecoderPriv* iImageDecoderPriv;
	CImageDecodeConstruct* iConstruct;

	TBool iFileTest; //ETrue use SetFileL() / EFalse use SetDataL()
	TBool iMaskDecode;

	TInt  iSourceLength;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	RFile		iSourceFile;
	TFileName	iSourceFilename;
	CFbsBitmap* iTestBitmap;
	CFbsBitmap* iTestBitmapMask;
	CFbsBitmap* iRefBitmap;
	RFs			iFileSession;
	CImageDecoder::TOptions iOptions;

	CActiveScheduler* iScheduler ;

	RThread iMyThread;
	};

class RCImageDecoderPrivReadDataLTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivReadDataLTest* NewL(CTestSuite* aSuite, TBool aFileTest = ETrue, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	virtual void Close();

private:
	RCImageDecoderPrivReadDataLTest(CTestSuite* aSuite, TBool aFileTest, TBool aAllocTest);

private:
	RFs		iFileSession;
	TInt	iDataSize;
	TInt	iStartPosition;
	HBufC8* iDataBuffer;
	TBool	iFileTest;
	};

class RCImageDecoderPrivFrameInfoStringsLCTest : public RCImageDecoderPrivTestBase
	{
public:
	static RCImageDecoderPrivFrameInfoStringsLCTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();

private:
	RCImageDecoderPrivFrameInfoStringsLCTest(CTestSuite* aSuite, TBool aAllocTest);
	};

class RCImageDecoderPrivCustomTest : public RAllocTest, public MImageDecoderPrivSupportImp
	{
public:
	static RCImageDecoderPrivCustomTest* NewL(CTestSuite* aSuite, TBool aAllocTest = EFalse);

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
	RCImageDecoderPrivCustomTest(CTestSuite* aSuite, TBool aAllocTest);

private:
	CImageDecoderPriv* iImageDecoderPriv;
	CImageDecodeConstruct* iConstruct;

	TInt  iSourceLength;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	TFileName	iSourceFilename;
	RFs			iFileSession;

	CActiveScheduler* iScheduler;
	RThread iMyThread;
	};

class RCImageDecodeConstructTest : public RAllocTest
	{
public:
	static RCImageDecodeConstructTest* NewL(CTestSuite* aSuite, TBool aAllocTest);
	CImageDecodeConstruct* CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid);

protected:

	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();

private:
	RCImageDecodeConstructTest(CTestSuite* aSuite, TBool aAllocTest);
	};

class RCImageEncodeConstructTest : public RAllocTest
	{
public:
	static RCImageEncodeConstructTest* NewL(CTestSuite* aSuite, TBool aAllocTest);
	CImageEncodeConstruct* CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid);

protected:

	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();

private:
	RCImageEncodeConstructTest(CTestSuite* aSuite, TBool aAllocTest);
	};

class RBadVerDecoderTestStep : public RICLFrameworkTestStep
	{
public:
	static RBadVerDecoderTestStep* NewL(CTestSuite* aSuite);
	RBadVerDecoderTestStep(CTestSuite* aSuite);
	// From RTestStep
	virtual TVerdict DoTestStepL();
	TVerdict OpenL();
	void Close();
protected:
	
	CActiveScheduler* iScheduler ;
	};

class RBadVerEncoderTestStep : public RICLFrameworkTestStep
	{
public:
	static RBadVerEncoderTestStep* NewL(CTestSuite* aSuite);
	RBadVerEncoderTestStep(CTestSuite* aSuite);
	// From RTestStep
	virtual TVerdict DoTestStepL();
	TVerdict OpenL();
	void Close();	
protected:
	
	CActiveScheduler* iScheduler ;	
	};



// CAsyncTestDecodeConstruct
// responsible for creating a CAsyncTestDecoderPlugin object
class CAsyncTestDecodeConstruct : public CImageDecodeConstruct
	{
public:
	static CAsyncTestDecodeConstruct* NewL();
	// from CImageDecodeConstruct
	CImageDecoderPlugin* NewPluginL() const;

private:
	CAsyncTestDecodeConstruct();
	};


// CAsyncTestReadCodec - a dummy codec which reports EFrameComplete
// after a fixed number of iterations
class CAsyncTestReadCodec : public CImageReadCodec
	{
public:
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
private:
	};


// MAsyncSchedulerObserver
// mixin class to handle callbacks from an CAsyncScheduler object
class MAsyncSchedulerObserver
	{
public:
	virtual void ProcessL(TInt error) = 0;
	};

class CAsyncScheduler : public CActive
	{
public:
	CAsyncScheduler(MAsyncSchedulerObserver& aSchedulerObserver);
	void ScheduleDecode();

	// From CActive
	virtual void RunL();
	virtual void DoCancel() {};

private:
	MAsyncSchedulerObserver& iSchedulerObserver;
};

class CAsyncTestDecoderPlugin : public CImageDecoderPlugin, public MAsyncSchedulerObserver
	{
public:
	~CAsyncTestDecoderPlugin();
	static CAsyncTestDecoderPlugin* NewL();

	// From CImageDecoderPlugin
	virtual void ImageType(int,class TUid& aImageType,class TUid &aImageSubType) const;
	virtual void ScanDataL();
	virtual void DoConvert();
	virtual CFrameInfoStrings* FrameInfoStringsL(class RFs& aFs,int);
	virtual void NotifyComplete();
	virtual void Cleanup();

	// from MAsyncSchedulerObserver
	virtual void ProcessL(TInt aError);

private:
	CAsyncTestDecoderPlugin();
	void ConstructL();

private:
	enum TDecodeState
	{
	EDecodeInit,
	EDecodeInProgress,
	EDecodeFinished
	};
	TDecodeState iDecodeState;
	
	CAsyncTestReadCodec* iImageReadCodec;	// not owned
	TInt iDecodeIterations;

	RFs*  iFs; //Not owned
	TBool iReadHeaders;
	TBool iAsynchronous;
	CAsyncScheduler* iAsyncDecoder;
	TFrameState iCodecState;
	TInt iError;
	};


// Async decode unit test
class RCImageDecoderPrivAsyncDecodeTest : public RAllocTest, public MImageDecoderPrivSupportImp
	{
public:
	static RCImageDecoderPrivAsyncDecodeTest* NewL(CTestSuite* aSuite);

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
	RCImageDecoderPrivAsyncDecodeTest(CTestSuite* aSuite);

private:
	CImageDecoderPriv* iImageDecoderPriv;
	CAsyncTestDecodeConstruct* iConstruct;

	TInt  iSourceLength;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	CFbsBitmap* iTestBitmap;
	RFs			iFileSession;
	CImageDecoder::TOptions iOptions;

	CActiveScheduler* iScheduler;

	RThread iMyThread;
	TInt iStatus;
	};

// MTimerObserver
// mixin class to handle callbacks from a CUtilityTimer object
class MTimerObserver
	{
public:
	virtual void TimerExpired() = 0;
	};

class CUtilityTimer : public CTimer
	{
public:
	static CUtilityTimer* NewL(TTimeIntervalMicroSeconds32& aDelay, MTimerObserver& aObserver);
	~CUtilityTimer();

	void InitializeTimer();

private:
    CUtilityTimer(MTimerObserver& aObserver);
    void ConstructL(TTimeIntervalMicroSeconds32& aDelay);

	// from CActive
    void RunL();
	void DoCancel();

private:
    TTimeIntervalMicroSeconds32 iDelay;
	MTimerObserver& iObserver;
    };


// Async decode plugin test
class RCImageDecoderAsyncDecodeTest : public RAllocTest, public MTimerObserver
	{
public:
	static RCImageDecoderAsyncDecodeTest* NewL(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest);

	// from MTimerObserver
	virtual void TimerExpired();

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
	RCImageDecoderAsyncDecodeTest(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest);

private:
	RFs	iFileSession;
	TInt iStatus;
	TBool iCancelTest;

	CImageDecoder* iImageDecoder;
	CImageDecoder::TOptions iOptions;
	CActiveScheduler* iScheduler;

	TInt  iSourceLength;
	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	CFbsBitmap* iTestBitmap;
	
	CUtilityTimer* iTimer;
	TBool iCancelled;
	};


// Async Encode plugin test
class RCImageEncoderAsyncEncodeTest : public RAllocTest, public MTimerObserver
	{
public:
	static RCImageEncoderAsyncEncodeTest* NewL(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest);

	// from MTimerObserver
	virtual void TimerExpired();

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
	RCImageEncoderAsyncEncodeTest(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest);

private:
	RFs	iFileSession;
	TInt iStatus;
	TBool iCancelTest;

	CImageEncoder* iImageEncoder;
	CImageEncoder::TOptions iOptions;
	CActiveScheduler* iScheduler;

	HBufC8*		iDestData;
	CFbsBitmap* iTestBitmap;
	
	CUtilityTimer* iTimer;
	TBool iCancelled;
	};

/**
*
* CR0711 Configurable ICL worker thread priority
* *
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RSetThreadPriorityTestStep
*
*/
class RSetThreadPriorityTestStep: public RICLFrameworkTestStep
    {
public:
    static RSetThreadPriorityTestStep* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict OpenL();
    virtual void Cleanup();
    virtual TVerdict DoTestStepL();

private:
    explicit RSetThreadPriorityTestStep(CTestSuite* aSuite);
    
    TThreadPriority IclThreadPriotityL();
    
public:
    RFs iFileSession;
    TFileName iFileName;

private:
    CActiveScheduler* iActiveScheduler;
    };    

// Mime type test
class RTImageMimeTypeTest : public RICLFrameworkTestStep
    {
public:
	static RTImageMimeTypeTest* NewL(CTestSuite* aSuite);
	
	// From RTestStep
	virtual TVerdict DoTestStepL();
    // from RTestStep
    virtual TVerdict OpenL();
	virtual void Close();

private:
	RTImageMimeTypeTest(CTestSuite* aSuite);

    CActiveScheduler* iActiveScheduler;
    };

/**
*
* CR0793 Jpeg encoder performance: 
* *
* @lib "TSU_ICL_FRM_01.lib"
*
* @class REmulateSlowMediaTestStep
*
*/
class REmulateSlowMediaTestStep; // declared here
NONSHARABLE_CLASS( REmulateSlowMediaTestStep ): public RICLFrameworkTestStep
    {
public:
    static REmulateSlowMediaTestStep* NewL(CTestSuite* aSuite);

    // from RTestStep
    virtual TVerdict OpenL();
    virtual void Cleanup();
    virtual TVerdict DoTestStepL();

private:
    explicit REmulateSlowMediaTestStep(CTestSuite* aSuite);
    
    
public:
    RFs iFileSession;
    TFileName iFileName;

private:
    CActiveScheduler* iActiveScheduler;
    };    

/**
*
* PREQ1630 Encoder Rotation and Flip Transformation: 
* that test case relates to the REQ7844, 
* and verfies proper functioning of the TPosition Processor Algos for Rotation and Flips
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTPositionProcessorTest

@param	aSuite
			This suite.
@param	aMethod
			The method to be run (TTransformMethod).
@param	aTestStepName
			The name of this test step.
@return RTPositionProcessorTest*
			Constructed RTPositionProcessorTest object
*/

class RTPositionProcessorTest : public RICLFrameworkTestStep
	{
public:
    enum TTransformMethod
    	{
    	ETransformRotate, // 0
    	ETransformHorizontalFlip,
    	ETransformVerticalFlip,
    	ETransformRotVerHorFlip
    	};
public:
	static RTPositionProcessorTest* NewL(CTestSuite* aSuite,
    										TTransformMethod aMethod,
    										const TDesC& aTestStepName);

	// From RTestStep
	virtual TVerdict OpenL();
    virtual TVerdict DoTestStepL();
    virtual void Close();

private:
	RTPositionProcessorTest(CTestSuite* aSuite,
    								TTransformMethod aMethod,
    								const TDesC& aTestStepName);
	void Compare(const TInt16* aDstPtr, const TInt16* aRefPtr, TInt count);
    								
private:
	TTransformMethod iMethod;
	};

#endif (ICLFRAMEWORKTESTSTEP_H) 
