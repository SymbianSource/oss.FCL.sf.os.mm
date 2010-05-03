// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef ICLCODECDEFECTTESTSTEP_H
#define ICLCODECDEFECTTESTSTEP_H

#include <testframework.h>
#include <imageconversion.h>

class CFbsBitmap;
class CFileExtensionMIMEType;

/**
 * Listener that stop the ActiveScheduler when signalled
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};

/**
 * Scheduler that will cancel decoding at the set interval
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class CCancelScheduler : public CActiveScheduler
	{
public:
	void SetDecoder(CImageDecoder& aDecoder);
	void SetCancel(TInt aCount);
	TInt CurrentCount() const;

	//From CActiveScheduler
	void WaitForAnyRequest();
public:
	TInt iCurrentCount;
	TInt iCancelCount;
	CImageDecoder* iDecoder; //Not owned
	};

/**
 * Base class for test steps
 * Provide common comparison tests
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class CImageDecoder;
class RICLCodecDefectTestStep : public RTestStep
	{
protected:
	void GetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;

	void SetVerdict(TVerdict aVerdict);
	void SetFail(const TDesC& aErrorMessage);

	void Compare(TInt aNumber1, TInt aNumber2, const TDesC& aErrorMessage);
	void Compare(TUid aUid1, TUid aUid2, const TDesC& aErrorMessage);
	void Compare(const TDesC& aDescriptor1, const TDesC& aDescriptor2, const TDesC& aErrorMessage);
	void Compare(const TDesC8& aDescriptor1, const TDesC8& aDescriptor2, const TDesC& aErrorMessage);
	void ComparePointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage);
	void CompareDiffPointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage);
	void Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2, const TDesC& aErrorMessage);
	TInt Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2);

	CImageDecoder* DecodeFileL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmapconst, TInt aFrameNumber = 0,TInt aReductionFactor = 0, TBool aDecodeThumbnail = EFalse, CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);
	CImageDecoder* DecodeFileWithMaskL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmapconst, CFbsBitmap& aMaskBitmapconst, TInt aFrameNumber = 0, CImageDecoder::TOptions aOptions = CImageDecoder::EOptionNone);
	CImageDecoder* DecodeDescL(RFs& aFs, const TDesC8& aBufferDes, CFbsBitmap& aBitmap, const TInt aFrameNumber = 0, const TInt aZoomFactor = 0);

	TInt WaitForResult(const TRequestStatus* aStatus) const;
	};

/**
 * Test for defect DEF000707
 * PNG causes an access violation in the decoder. 
 * This is due to it having a palette of 1024 entries when the 
 * spec says that the maximum is 256
 *
 * The code was fixed to leave with KErrNotFound.
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RPngDecodeFailureTest : public RICLCodecDefectTestStep
	{
public:
	static RPngDecodeFailureTest* NewL(CTestSuite* aSuite);

	// From RTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RPngDecodeFailureTest(CTestSuite* aSuite);

private:
	RFile		iFile;
	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	RFs			iFileSession;

	CActiveScheduler* iScheduler ;
	};

/**
 * Test for defect DEF012043 - Zero size GIFs generate an 
 * unhandled exception.
 * 
 * Test 0xX and Xx0 size GIF images.
 * The decode is expected to return normally and the size set correctly.
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5002 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5002* NewL(CTestSuite* aSuite);

	// From RTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5002(CTestSuite* aSuite);

	/**
	 * Save information of test files
	 * @lib "TSU_ICL_COD_03.LIB"
	 * @xxxx
	 */
	class TTestFile
		{
	public:
		TTestFile(const TDesC& aFileName, TInt aWidth, TInt aHeight);
		TTestFile(const TTestFile& aTestFile);
		inline const TDesC& FileName() const;
		inline TInt Width() const;
		inline TInt Height() const;

	private:
		const TDesC* iFileName;
		TInt iWidth;
		TInt iHeight;
		};

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	RArray<TTestFile> iTestFiles;
	};
	
/**
 * Test for defect DEF012242 ICL GetFileTypesL() not according to specification 
 * Tests and verifies that the returned information is correct by inspecting the
 * the following entries:
 * 1) FailingTestDecoder for <.tst><image/test>
 * 2) TestDecoder for <.tst><image/tst>
 * 3) ExtendedTestDecoder <.tst><image/tst> should not be found since it
 *    has a lower version no than TestDecoder
 * 4) TestEncoder <.tst><image/test>
 * 5) ExtendedTestEncoder <.tst><image/test> should not be found since it
 *    has a lower version no than TestEncoder
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5004 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5004* NewL(CTestSuite* aSuite);

	// From RTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5004(CTestSuite* aSuite);
	void VerifyPlugin(CFileExtensionMIMEType& aPlugin);
	void VerifyResults(void);

	class TFileMimeType
		{
	public:
		TFileMimeType(const TDesC& aExtn, 
					  const TDesC8& aMimeType,
					  const TDesC& aDisplayName,
					  const TUid aImageType,
					  const TUid aImageSubType,
					  const TUid aImplementationUid,
					  TBool aShouldBeFound);

		const TDesC& iExtn;
		const TDesC8& iMimeType;
		const TDesC& iDisplayName;
		const TUid iImageType;
		const TUid iImageSubType;
		const TUid iImplementationUid;
		TBool iShouldBeFound;
		TBool iFound;
		TBool iVerified;
		};

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	RArray<TFileMimeType> iPluginData;
	};

/**
 * Test for defect INC012846 GIF decoder use wrong palette for
 * multi-frame images with mixed global/local color tables (palettes).
 * The first and last frame in the selected image use local color
 * tables while the rest use the global color table. All frame headers
 * are read, and the second frame is then decoded. (The bug caused this
 * frame to be decoded with the local color table of the last frame.)
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5006 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5006* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5006(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;

	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect INC011852 - Animated gif crash the emulator
 * Decode a frame from an interlaced GIF with an offset and
 * compare the decoded frame with a reference image.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5008 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5008* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5008(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect DEF018286
 * - ICL does not allow the Logical Information for an image to be read
 * The first frame of an animated GIF should be the same size as the 
 * complete image.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5010 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5010* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5010(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect DEF018361 - Regression in converting animated image
 * Application/Comment Extension blocks not interpreted correctly
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5012 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5012* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5012(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;

	RPointerArray<TDesC> iReferenceComments;
	};

/**
 * Test for defect DEF021187  jpeg decoder reports particular jpeg files as corrupt. 
 * JPEG restart interval of 0 causes decoder to leave with KErrNotFound
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5014 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5014* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5014(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect DEF021196 - Interlaced GIF leave blank lines when streaming resized image. 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5016 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5016* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5016(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	
	HBufC8* iSource;

	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect INC021620 - GIF Graphic Control Extension is not correctly extracted.
 * Test that the "user input required" field is parsed from the header.
 * 
 * Test for defect INC112911: Opera: Indefinitely looped animated Gif only animates once
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5018 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5018* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5018(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iSourceFile2;	
	};

/**
 * Test for defect INC021621 - 
 *
 * this tests :
 *
 * - Opening a BMP file with an invalid length in image header (0xFFFFFFFF)
 *   this should leave with KErrNotFound
 *
 * - Opening a truncated BMP file with only the image header present (ie no frame data)
 * this should leave with KErrUnderflow unless EOptionAllowZeroFrameOpen is set
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5019 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5019* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5019(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFileCorrupt;	
	TFileName iSourceFileTruncated;	
	TFileName iSourceFileCompressionType3;	
	};

/**
 * Test for defect  INC021911  CImageDecoder:ConvertL() is working incorrectly
 * when called again after Cancel()
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5020 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5020* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5020(CTestSuite* aSuite);

private:
	CCancelScheduler* iScheduler;
	CActiveListener* iListener;
	TRequestStatus* iStatus; //not owned
	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect DEF024748  Jpeg decoder no londer interprets comment blocks correctly 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5021 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5021* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5021(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;

	RPointerArray<TDesC> iReferenceComments;
	};

/**
 * Test for defect DEF024756 Mono Adobe Jpeg images should be supported by ICL 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5022 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5022* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5022(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

inline const TDesC& RTestStep_5002::TTestFile::FileName() const { return *iFileName;}
inline TInt RTestStep_5002::TTestFile::Width() const { return iWidth; }
inline TInt RTestStep_5002::TTestFile::Height() const { return iHeight; }


/**
 * Test for defect CR Test for CR PHAR-5J5LP4 
 * Add support for generating mask information.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5023 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5023* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5023(CTestSuite* aSuite);
	void GetTestDirNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFiles;

	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iSourceMaskBitmap;

	CFbsBitmap* iReferenceBitmap;
	CFbsBitmap* iReferenceMaskBitmap;
	CFbsBitmap* iEmptyMaskBitmap;

	CDir *iDir;
	TBool iIsNaviEngine;
	TBool iIsH6;
	};

/**
 * Alloc test for defect CR Test for CR PHAR-5J5LP4 
 * Add support for generating mask information.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5024 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5024* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5024(CTestSuite* aSuite);
	void GetTestDirNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFiles;

	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iSourceMaskBitmap;

	CFbsBitmap* iReferenceBitmap;
	CFbsBitmap* iReferenceMaskBitmap;
	CFbsBitmap* iEmptyMaskBitmap;

	CDir *iDir;
	TBool iIsNaviEngine;
	TBool iIsH6;
	};

/**
 * Test for defect DEF037251 - CMdaJfifClipFormat cannot decode a near-JFIF file 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5025 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5025* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5025(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for:
 * INC039757- 2 Color bitmap decoding problem in ICL BMPCodec
 * PDEF112752: V&V:IOP:Content: The phone is not able to display .bmp files (TE Triage comme... 	 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5026 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5026* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5026(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	RArray<TFileName> iSourceFiles;
	RArray<TFileName> iRefFiles;

	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for defect DEF039297/DEF042393 - Background color is not extracted correctly for monochrome PNG images
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */

// Test case settings.
class TTest5027_TestCase
	{	
public:
	const TText* iTestName; // Name of this test case.
	const TText* iFileName; // Image file to load.
	const TText* iRefName; // Image reference file to load.
	TInt iBackgroundColor; // Expected background color.
	};

// Array of test cases for testing.
const TInt KTest5027_NumTestCases = 3;
const TTest5027_TestCase KTest5027_TestCase[KTest5027_NumTestCases] =
	{
	{ _S("MM-ICL-COD-U-5027-HP"), _S("png8bpp_g0_b.png"), _S("png8bpp_g0_b.mbm"), 0x555555 },
	{ _S("MM-ICL-COD-U-5055-HP"), _S("png4bpp_g0_b.png"), _S("png4bpp_g0_b.mbm"), 0xBBBBBB },
	{ _S("MM-ICL-COD-U-5056-HP"), _S("png2bpp_g0_b.png"), _S("png2bpp_g0_b.mbm"), 0xFFFFFF },
	};

class RTestStep_5027 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5027* NewL(CTestSuite* aSuite, TInt aTestCase);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5027(CTestSuite* aSuite, TInt aTestCase);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;

	const TTest5027_TestCase& iTestCase;
	};

/**
 * Test for defect INC041474 - Multiple vulnerabilities in TIFF decoder
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */

// Test case settings.
class TTest5028_TestCase
	{	
public:
	const TText* iTestName; // Name of this test case.
	const TText* iFileName; // Image file to load.
	TInt iErr; // Expected error.
	};

// Array of test cases for testing.
const TInt KTest5028_NumTestCases = 27;
const TTest5028_TestCase KTest5028_TestCase[KTest5028_NumTestCases] =
	{
	{ _S("MM-ICL-COD-U-5028-HP"), _S("00000023.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5029-HP"), _S("00000026.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5030-HP"), _S("00000048.tif"), KErrCorrupt },
	{ _S("MM-ICL-COD-U-5031-HP"), _S("00000052.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5032-HP"), _S("00000185.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5033-HP"), _S("00000187.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5034-HP"), _S("00000188.tif"), KErrNoMemory },
	{ _S("MM-ICL-COD-U-5035-HP"), _S("00000342.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5036-HP"), _S("00000343.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5037-HP"), _S("00000345.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5038-HP"), _S("00000366.tif"), KErrNoMemory },
	{ _S("MM-ICL-COD-U-5039-HP"), _S("00000371.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5040-HP"), _S("00000372.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5041-HP"), _S("00000374.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5042-HP"), _S("00000376.tif"), KErrNoMemory },
	{ _S("MM-ICL-COD-U-5043-HP"), _S("00000527.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5044-HP"), _S("00000530.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5045-HP"), _S("00000532.tif"), KErrNoMemory },
	{ _S("MM-ICL-COD-U-5046-HP"), _S("00000565.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5047-HP"), _S("00000568.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5048-HP"), _S("00000570.tif"), KErrNoMemory },
	{ _S("MM-ICL-COD-U-5049-HP"), _S("00000603.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5050-HP"), _S("00000606.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5051-HP"), _S("00000608.tif"), KErrNoMemory },
	{ _S("MM-ICL-COD-U-5052-HP"), _S("00000641.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5053-HP"), _S("00000644.tif"), KErrNotFound },
	{ _S("MM-ICL-COD-U-5054-HP"), _S("00000646.tif"), KErrNoMemory }
	};

class RTestStep_5028 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5028* NewL(CTestSuite* aSuite, TInt aTestCase);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5028(CTestSuite* aSuite, TInt aTestCase);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;

	const TTest5028_TestCase& iTestCase;
	};


/**
 Test for defect INC042385
 
 this tests :
 Opening a GIF file with an invalid length in the block terminator
 this should leave with KErrNotFound
   
 @lib "TSU_ICL_COD_03.LIB"

 */
class RTestStep_5029 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5029* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5029(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFileCorrupt;	
	};

/**
 * Test for defect DEF042382-CGifDecoder::ReadFormatL doesn't set the background color field of frame info.
 * 
 * This tests for setting a correct background colour for
 * GIF images.
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5058 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5058* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5058(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};


/**
 * Test for defect INC043752 - CImageDecoder can not open certain AGIF and TIFF files.
 * 
 * This tests for opening non-standard GIF image of ICL and checks it decodes correctly.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5059 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5059* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5059(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};
	
/**
 *Test for INC045490 - AV21 PR3 Corrupted GIF images crash image decoder.
 * 
 * This tests Gif file with wrong ClearCode leaves with KErrCorrupt
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5060 : public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5060* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5060(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	};	
	
/**
 * Test for 
 * INC045490 - AV21 PR3 Corrupted GIF images crash image decoder.
 * PDEF095166: RF Incorrect offset of displayed gif on TImageViewer
 *
 * This tests for correct .gif files gets decoded with KErrNone.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5061 : public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5061* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5061(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iReferenceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};	

/**
 * Test for:
 * INC046305, INC046308 - 16 bpp grayscale PNG not supported; invalid bpp for colour-indexed images
 * INC099484: PNG image file crashes Browser
 * PDEF107407: PNG decoding error when it has IEND tag missing at the end
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
NONSHARABLE_CLASS( RTestStep_5062 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5062* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5062(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CActiveListener* iListener;
	TRequestStatus* iStatus; //not owned	
	};	

/**
 *Test for INC047703 - PNG-decoder panics when decoding a corrupt PNG-file
 * 
 * This tests surviving the corrupted PNG image
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
NONSHARABLE_CLASS( RTestStep_5063 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5063* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5063(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	};


/**
 * DEF051731 - ICL CImageDecoder and inconsistent reduced frame sizes 
 * This test checks if the Size returned by ReducedSize() is same as the 
 * size of the output bitmap returned by convert()
 * (Added newly ReductionFactor() & ReducedSize() to CImageDecoder)
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
 
NONSHARABLE_CLASS(RTestStep_5064): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5064* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5064(CTestSuite* aSuite);

private:
	CActiveScheduler *iAS;
	CActiveListener* iListener;
	TRequestStatus* iStatus; //not owned
	RFs iFileSession;
	TFileName iSourceFile;
	};


NONSHARABLE_CLASS(RTestStep_5065): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5065* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5065(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
	RFs iFileSession;
	};
	

/**
 * Test for INC062084 - NTT - Multiple vulnerabilities in JPEG decoding
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
NONSHARABLE_CLASS( RTestStep_5066 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5066* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5066(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	};		
	

/**
 * Test for DEF063125 - JPEG decoder makes white block to certain images
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
NONSHARABLE_CLASS( RTestStep_5067 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5067* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5067(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iReferenceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};
	
/**
 * Test for defect DEF065298 - GIF decoder offset defect
 * 
 * Test would decode the image and compare resulting bitmaps with one that is frame contests 
 	and another one that is Mask image.
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5068 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5068* NewL(CTestSuite* aSuite);

	// From RTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
	CImageDecoder* DecodeFileL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, CFbsBitmap& aBitmapMask, const TInt aFrameNumber);
private:
	RTestStep_5068(CTestSuite* aSuite);
	
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceImage;
	CFbsBitmap* iSourceMask;
	CFbsBitmap* iReference;
	};

/**
 * Test for DEF067576 - Animated GIF file returns KErrCorrupt
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5069 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5069* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5069(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iReferenceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};	

/**
 * Test for DEF068359: Symbian's ICL JPEG encoder produces incorrect JFIF format.
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5070 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5070* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5070(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iSavedFileName;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};
	
/**
 * DEF066190 - ReducedSize() for WMF images returns incorrect values 
 * 
 * (Added newly ReducedSize() and Reduction factor to CWmfReadCodec)
 * 
 * @return	"TVerdict"
 *			EPass if the test succeeded
 * @xxxx
 */
 
NONSHARABLE_CLASS(RTestStep_5071): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5071* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5071(CTestSuite* aSuite);
	void GetTestDirNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;
private:
	CActiveScheduler *iAS;
	CActiveListener* iListener;
	TRequestStatus* iStatus; //not owned
	RFs iFileSession;
	TFileName iSourceFile;
	};


/**
 * Test for DEF069034 Animated GIF returns KErrArgument
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5072 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5072* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5072(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	};	

/**
 * Test for DEF069034 Animated GIF returns KErrArgument
 * 
 * 
 *
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
NONSHARABLE_CLASS( RTestStep_5073 ): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5073* NewL(CTestSuite* aSuite);

	// From RTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
	CImageDecoder* DecodeFileL(RFs& aFs,const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber);
private:
	RTestStep_5073(CTestSuite* aSuite);
	
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceImage;
	CFbsBitmap* iReference;
	};

/**
 * Test for DEF070867: Some more animated GIFs fail
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5074 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5074* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5074(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	};	
	
/**
 * Test for:
 * INC070821 Image viewer is unable to open jpg images 
 * INC089954 Image Viewer crash on some jpeg with User 47. JPEG decoding problem
 * PDEF088079: Exif - Comment marker in exif header causes panic 
 * PDEF088084: Exif - Invalid JPEGInterchangeFormat/Length tags cause Panic
 * PDEF096485: JPEGs cause "Not Found" error and do not display
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5075 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5075* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5075(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	RArray<TFileName> iSourceFiles;
	CFbsBitmap* iSourceBitmap;
	};	

/**
 * Test for INC069900: Problems with memory usage when opening corrupted gif images
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5076 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5076* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5076(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	};	

/**
 * Test for INC076787: ICL cannot decode JPEG with incorrect comment block length
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5077 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5077* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5077(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	};

/**
 * Test for   DEF083440: CImageDecoder incorrect behaviour on finding no codec
 * 			  PDEF100237: WBMP image doesn't open
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5078 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5078* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5078(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	};
	
/**
 Test for defect DEF082926: GIFcodec unable to parse Waterdrop.gif
 
 Opening a GIF file with corrupt LZW stream
   
 @lib "TSU_ICL_COD_03.LIB"

 */
NONSHARABLE_CLASS( RTestStep_5079 ): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5079* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5079(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	TFileName iSourceFileCorrupt;	
	};
	
/**
 * Test for  DEF086789: P: Can't resolve GIF's first frame size using TFrameInfo
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5080 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5080* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5080(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};
	
/**
 * Test 1 for INC086953: WCDMA - OTC - The display position of AnimatedGIF file is incorrect.
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5081 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5081* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5081(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap1;
	CFbsBitmap* iReferenceBitmap2;
	};
	
/**
 * Test 2 for INC086953: WCDMA - OTC - The display position of AnimatedGIF file is incorrect.
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5082 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5082* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5082(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap1;
	CFbsBitmap* iReferenceBitmap2;
	CFbsBitmap* iReferenceBitmap3;
	};
		

/**
 * INC091941  16x16 pixel BMP image loads 15-20 seconds and is displayed incorrectly 
 *
 */
NONSHARABLE_CLASS( RTestStep_5083 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5083* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5083(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iDestBitmap;
	};

	
/**
 *Test for defect  DEF091243: T: ICL decode GIF with sub-block data length zero
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
class RTestStep_5086 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5086* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5086(CTestSuite* aSuite);

private:
	CCancelScheduler* iScheduler;
	CActiveListener* iListener;
	TRequestStatus* iStatus; //not owned
	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iReferenceBitmap;
	};

/**
 * Test for INC097489: SWSV_633_91_R6A01_Image: GIF 256 color picture "Damaged" after editing in Pic...
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
class MImageDecoderEncoder_5088_Observer
	{
public:
	virtual void DecodeEncodeFinished(TInt aErrorCode) = 0;
	};
	
class CImageDecoderEncoder_5088;	
NONSHARABLE_CLASS( RTestStep_5088 ): public RAsyncTestStep, public MImageDecoderEncoder_5088_Observer
 	{
public:
 	static RTestStep_5088* NewL(CTestSuite* aSuite);
 	
 	void Fsm();
 
 	// From RAsyncTestStep
	void KickoffTestL();
	void CloseTest();
	
	// from MImageDecoderEncoder_5082_Observer
	void DecodeEncodeFinished(TInt aErrorCode);
private:
 	RTestStep_5088(CTestSuite* aSuite);
 	TBool CompareBitmaps(const CFbsBitmap* aBitmap1, const CFbsBitmap* aBitmap2, CPalette* aPalette = NULL);
private:
 	RFs iFileSession;
	HBufC* iSourceFileName;
	HBufC* iSavedFileName;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iSavedBitmap;
 	CImageDecoderEncoder_5088* iDecoderEncoder;
 	enum TTestState
		{
		EStateLoadSourceImage,
		EStateSaveImageWithAdaptivePalette,
		EStateLoadSavedImage1,
		EStateSaveImageWithForcedPalette,
		EStateLoadSavedImage2
		};
	TTestState iTestState;
	CFrameImageData* iFrameImageData;
	CPalette* iPalette;
	};

NONSHARABLE_CLASS( CImageDecoderEncoder_5088 ): public CActive
	{
public:
	static CImageDecoderEncoder_5088* NewL(RFs& aFs, MImageDecoderEncoder_5088_Observer& aObserver);
	virtual ~CImageDecoderEncoder_5088();
	
	void EncodeToFile(const CFbsBitmap& aSourceBitmap, const TDesC& aTargetFileName, const CImageEncoder::TOptions aOptions, const TUid aImageTypeUid, const CFrameImageData* aFrameImageData = NULL);
	void DecodeFromFile(const TDesC& aSourceFileName, CFbsBitmap& aTargetBitmap, const CImageDecoder::TOptions aOptions);
	// From CActive
	virtual void RunL();
	virtual void DoCancel();
protected:
	CImageDecoderEncoder_5088(RFs& aFs, MImageDecoderEncoder_5088_Observer& aObserver);
	void ConstructL();
	void SelfComplete(TInt aError);
private:
	RFs& iFs;
	MImageDecoderEncoder_5088_Observer& iObserver;
 	CImageDecoder* iDecoder;
 	CImageEncoder* iEncoder;
	};
	
class CImageTransform;
/**
 *Test for defect  DEF096720  Exif ImageTransform should withstand some discrepances in JPEGs as decoder does.
 * 
 */
class RTestStep_5089 : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5089* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	explicit RTestStep_5089(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
	CImageTransform* iTransformer;
	TBool iFbsConnected;
	RFs iFileSession;
	TFileName iSourceFile;
	HBufC8* iDestBuf;
	};

/**
 * Test for:
 * PDEF102815: JPEG with 0xFF 0xFF bytes sequence in image byte stream not decoded fully
 * 
 *
 */
NONSHARABLE_CLASS( RTestStep_5090 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5090* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5090(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* iSourceBitmap;
	};	

/**
 * Test for:
 * PDEF102719: CImageDecoder::ReducedSize() does not check upper boundary
 * 
 *
 */
NONSHARABLE_CLASS( RTestStep_5091 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5091* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
 	TBool ApplyRf(const TDesC& aSourceFileName, 
 				 const TDesC& aRefFileName, 
 				 TInt aRf, 
 				 const TDesC& aDecoderName);
protected:
//override to deal with wmf specially; to use same reference file across platforms.
CImageDecoder* DecodeFileL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmapconst, TInt aFrameNumber = 0, TInt aReductionFactor = 0);
private:
 	RTestStep_5091(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iDestnFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iDestnBitmap;
	};	

/**
 * Test for:
 * PDEF103195: PNGs BitBlt:ed from Ecolor16MU to EColor16MA becomes transparent
 * 
 *
 */
NONSHARABLE_CLASS( RTestStep_5092 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5092* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5092(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* i16MuBitmap;
	CFbsBitmap* i16MaBitmap;
	};
	
/**
 * Test for:
 * PDEF104507: CGifCodec::DoProcessExtensionL() assumes that the block is not zero length
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5093 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5093* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5093(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	};

/**
 * Test for:
 * CR1111: PNGs behaviour with Mask and 16M/16MU/16MA/16MAP bitmaps
 * 
 *
 */
NONSHARABLE_CLASS( RTestStep_5094 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5094* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5094(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CFbsBitmap* i16MxBitmap;
	CFbsBitmap* iMaskBitmap;
	};

/**
 * Test for:	
 * PDEF102796: WMF rendering problem
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5095 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5095* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5095(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	};

/**
 * Test for PDEF111303 AQP: Coverity tool for ICL - FORWARD NULL Reference in ICL
 * 
 */
NONSHARABLE_CLASS( RTestStep_5096 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5096* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5096(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	CActiveListener* iListener;
	};

/**
* Test for PDEF106475: Jpeg encoding in rect 100*75 from res 220*294 grey scale
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5097 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5097* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
private:
 	RTestStep_5097(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
 	CActiveListener* iActiveListener;
	CImageEncoder*	 iEncoder;
	};
	
/**
 * Test for:PDEF108785:Symbian JPEG decoder cannot decode multi-scan JPEG images 
 * 
 */
NONSHARABLE_CLASS( RTestStep_5098 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5098* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
 	TBool CheckDecoding(const TDesC& aSourceFileName, 
 				 		const TDesC& aRefFileName, 
 				 		TInt aRf, 
 				 		TDisplayMode aDisplayMode);
protected:
CImageDecoder* DecodeFileL(RFs& aFs, const TFileName& aFileName, CFbsBitmap& aBitmap, const TInt aFrameNumber, const TInt aReductionFactor, TDisplayMode aDisplayMode);
private:
 	RTestStep_5098(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iDestnFile;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iDestnBitmap;
	};

NONSHARABLE_CLASS( RTestStep_5100 ): public RICLCodecDefectTestStep
	{
public:
 	static RTestStep_5100* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
 	TUint32 ElaspedCounterTicks(TUint32 aStart, TUint32 aEnd);
 	
private:
 	RTestStep_5100(CTestSuite* aSuite);
private:
  	RFs iFileSession;
	TFileName iSourceFile;
	};

/**
 * Test for PDEF112298: Mbm Bitmap is not encoded to JPEG correctly using CImageEncoder
 * 
 */
NONSHARABLE_CLASS( RTestStep_5101 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5101* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
 	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2);
private:
 	RTestStep_5101(CTestSuite* aSuite);
private:
	CActiveScheduler* iScheduler;
 	RFs iFs;
	TFileName iSourceFile;
	CActiveListener* iListener;
	};

/**
 * Test for defect DEF109410. 
 * Some WMF images are shown cropped when zoomed out.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 * @xxxx
 */
NONSHARABLE_CLASS(RTestStep_5110) : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5110* NewL(CTestSuite* aSuite);

	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RTestStep_5110(CTestSuite* aSuite);

private:
	CActiveScheduler* iScheduler;
	RFs iFileSession;

	TFileName iSourceFiles;

	CFbsBitmap* iSourceBitmap;

	CFbsBitmap* iReferenceBitmap;

	CDir *iDir;
	};

/**
 * For reproducing PDEF113475 Thumbnail is not displayed for DRM image of mime type image/x-png 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS(RTestStep_5111) : public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5111* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
	TBool CheckForDecoder(const TDesC8 &aMIMEType);
private:
	RTestStep_5111(CTestSuite* aSuite);
	CActiveScheduler* iScheduler;
	RFs iFileSession;
	HBufC8* iSource;
	TFileName iFileName;
	};

/**
 * Test for:
 * PREQ1630: ICL Enhancements for memory and performance optimisation.
 * Tests loading a decoder that satisfies a TOptions bitfield.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS(RTestStep_5102): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5102* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
	
private:
	RTestStep_5102(CTestSuite* aSuite);
	
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	};

/**
 * Test for:
 * PREQ1630: ICL Enhancements for memory and performance optimisation.
 * Tests loading an encoder that satisfies a TOptions bitfield.
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS(RTestStep_5099): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5099* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
	
private:
	RTestStep_5099(CTestSuite* aSuite);
	
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	};
	
/**
 * Test for DEF113733: Encoding a color display mode bitmap to a jpeg monochrome thumbnail 
 * 
 * 
 * @lib "TSU_ICL_COD_03.LIB"
 *
 */
NONSHARABLE_CLASS( RTestStep_5103 ): public RICLCodecDefectTestStep
 	{
public:
 	static RTestStep_5103* NewL(CTestSuite* aSuite);
 	// From RICLCodecDefectTestStep
 	TVerdict OpenL();
 	TVerdict DoTestStepL();
 	void Close();
 	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2);
private:
 	RTestStep_5103(CTestSuite* aSuite);
private:
 	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
 	RFs iFileSession;
	TFileName iSourceFile;
	TFileName iSavedFileName;
	CFbsBitmap* iSourceBitmap;
	CFbsBitmap* iReferenceBitmap;
	};


	
/**
 * DEF114840: Panic observed while saving an image
 */
NONSHARABLE_CLASS(RTestStep_5104): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5104* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
		
private:
	RTestStep_5104(CTestSuite* aSuite);
		
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	};
	
/**
 * INC116806: Media files: Open image folder containing gif89 images results in a Main crash
 */
NONSHARABLE_CLASS(RTestStep_5112): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5112* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
			
private:
	RTestStep_5112(CTestSuite* aSuite);
		
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	CActiveListener* iListener;
	TRequestStatus* iStatus; 
	TFileName iSourceFile;
	
	CFbsBitmap* iReferenceBitmapMask;
	};	

/**
 * PDEF118662: 5.0 LWS: Browser crashes while loading the page 
 */
NONSHARABLE_CLASS(RTestStep_5113): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5113* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
			
private:
	RTestStep_5113(CTestSuite* aSuite);
		
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	TFileName iSourceFile;
	};	

/**
 * PDEF123012: 5.0 :Browser crashes while loading the homepage for www.refdesk.com
 */
NONSHARABLE_CLASS(RTestStep_5114): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5114* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
			
private:
	RTestStep_5114(CTestSuite* aSuite);
		
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	TFileName iSourceFile;
	};	

/**
 * PDEF127500: Error decoding a specific .gif
 */
NONSHARABLE_CLASS(RTestStep_5106): public RICLCodecDefectTestStep
	{
public:
	static RTestStep_5106* NewL(CTestSuite* aSuite);
	// From RICLCodecDefectTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();
		
private:
	RTestStep_5106(CTestSuite* aSuite);
		
private:
	RFs iFs;
	CActiveScheduler* iScheduler;
	};

/**
 * PDEF138486: Error decoding a specific .gif
 */
NONSHARABLE_CLASS(RTestStep_5107): public RICLCodecDefectTestStep
    {
public:
    static RTestStep_5107* NewL(CTestSuite* aSuite);
    // From RICLCodecDefectTestStep
    TVerdict OpenL();
    TVerdict DoTestStepL();
    void Close();
        
private:
    RTestStep_5107(CTestSuite* aSuite);
        
private:
    RFs iFs;
    CActiveScheduler* iScheduler;
    };

//AKAL-7YHLPR : Crash found in Themes Thread.
//memory corruption for Gif image secret.gif 
NONSHARABLE_CLASS(RTestStep_5108): public RICLCodecDefectTestStep
    {
public:
    static RTestStep_5108* NewL(CTestSuite* aSuite);
    // From RICLCodecDefectTestStep
    TVerdict OpenL();
    TVerdict DoTestStepL();
    void Close();
        
private:
    RTestStep_5108(CTestSuite* aSuite);
        
private:
    RFs iFs;
    CActiveScheduler* iScheduler;
    };

//DEF144646 : Genius Case #144754: Nokia N97-1: Israel: N97 TIFF files not displayed properly. 
    NONSHARABLE_CLASS(RTestStep_5109): public RICLCodecDefectTestStep 
        { 
    public: 
        static RTestStep_5109* NewL(CTestSuite* aSuite); 
        // From RICLCodecDefectTestStep 
        TVerdict OpenL(); 
        TVerdict DoTestStepL(); 
        void Close(); 
             
    private: 
        RTestStep_5109(CTestSuite* aSuite); 
             
    private: 
        RFs iFs; 
        CActiveScheduler* iScheduler; 
        }; 
	
#endif // ICLCODECDEFECTTESTSTEP_H
