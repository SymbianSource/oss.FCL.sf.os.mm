
// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ICL Codec Tests - see "V:\Dev\GT\GT 0137 Multimedia codec - Msrvr\Testing\Unit Test Specifications\Typhoon\"
// "SGL.GT0137.xxx Rev0.01 MMF ICL COD Unit Test Specification.xls"
// 
//

#ifndef __ICLCodecTestStep_H__
#define __ICLCodecTestStep_H__

#include <testframework.h>
// recognizer stuff:
#include <apgcli.h>
#include <apfrec.h>

#include "icl/ImageCodecData.h"
#include "PNGCodec.h"

//
// RPngWriteCodecTest

enum TPngWriteCodecTestId
	{
	// Positive tests
	ECOD_0201 = 0,
	ECOD_0202,
	ECOD_0203,
	ECOD_0204,
	ECOD_0205,
	ECOD_0206,
	ECOD_0207,
	ECOD_0208,
	ECOD_0209,
	ECOD_0210,
	ECOD_0211,
	ECOD_0212,
	// Negative tests
	ECOD_0401,
	ECOD_0402,
	ECOD_0403,
	ECOD_0404,
	ECOD_0501,
	ECOD_0502,
	ECOD_0503
	};

enum TCodecDataTestId
	{
	ECOD_2000,
	ECOD_2001,
	ECOD_2002,
	ECOD_2003,
	ECOD_2004,
	ECOD_2005,
	ECOD_4001
	};

/**
 * Base class providing comparison functions
 */
class RTestStepCompare : public RTestStep
	{
public:
	void SetVerdict(TVerdict aVerdict);
	void Compare(TInt aActual, TInt aExpect, const TDesC& aErrorMessage);
	void Compare(TUid aActual, TUid aExpect, const TDesC& aErrorMessage);
	void Compare(const TDesC& aActual, const TDesC& aExpect, const TDesC& aErrorMessage);
	void Compare(const TDesC8& aActual, const TDesC8& aExpect, const TDesC& aErrorMessage);
protected:	
	void DummyHalCallL();
	};

class RPngWriteCodecTest : public RTestStep
	{
public:
	static RPngWriteCodecTest* NewL(const TDesC& aTestName, TPngWriteCodecTestId aPngTestId, const TInt aHeapSize = 0);
protected:
	RPngWriteCodecTest(const TDesC& aTestName, TPngWriteCodecTestId aPngTestId, const TInt aHeapSize = 0);
	void ConstructL();
	// from RTestStep
protected:
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();
	// Tests
protected:
	// Positive tests
	TVerdict MM_ICL_COD_U_0201_L();
	TVerdict MM_ICL_COD_U_0202_L();
	TVerdict MM_ICL_COD_U_0203_L();
	TVerdict MM_ICL_COD_U_0204_L();
	TVerdict MM_ICL_COD_U_0205_L();
//	TVerdict MM_ICL_COD_U_0206_L();
	TVerdict MM_ICL_COD_U_0207_L();
	TVerdict MM_ICL_COD_U_0208_L();
	TVerdict MM_ICL_COD_U_0209_L();
	TVerdict MM_ICL_COD_U_0210_L();
	TVerdict MM_ICL_COD_U_0211_L();
	TVerdict MM_ICL_COD_U_0212_L();
	// Negative tests
	TVerdict MM_ICL_COD_U_0401_L();
	TVerdict MM_ICL_COD_U_0402_L();
	TVerdict MM_ICL_COD_U_0403_L();
	TVerdict MM_ICL_COD_U_0404_L();
	// Test for opening png files with invalid color type and/or invalid bit depth
	// Fix for DEF040352.
	TVerdict MM_ICL_COD_U_0501_L();
	// Test for cancelling decoder at the same time as it finishes with an error
	// Fix for DEF067367.
	TVerdict MM_ICL_COD_U_0502_L();
	// Test for cancelling encoder at the same time as it finishes with an error
	// Fix for DEF067367.
	TVerdict MM_ICL_COD_U_0503_L();
protected:
	CActiveScheduler* iScheduler;
	TPngWriteCodecTestId iPngTestId;
	};

//
// RPngWriteSubCodecTest

enum TPngWriteSubCodecTestId
	{
	ECOD_0230,
	ECOD_0231,
	ECOD_0232,
	ECOD_0233,
	ECOD_0234,
	ECOD_0235,
	ECOD_0236,
	ECOD_0237,
	ECOD_0238,
	ECOD_0239,
	ECOD_0240,
	ECOD_0241,
	ECOD_0242,
	ECOD_0243,
	ECOD_0244,
	ECOD_0245,
	ECOD_0250
	};

class RPngWriteSubCodecTest : public RTestStep
	{
public:
	static RPngWriteSubCodecTest* NewL(CTestSuite* aSuite, const TDesC& aTestName, TPngWriteSubCodecTestId aTestId);
	
protected:
	RPngWriteSubCodecTest(CTestSuite* aSuite, const TDesC& aTestName, TPngWriteSubCodecTestId aTestId);
	void ConstructL();
	// from RTestStep
protected:
	void SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();
	// Tests
protected:
	TVerdict TestSubCodecL(const TInt aBitDepth, TPngImageInformation::TColorType aColorType,
						   const TSize& aSize, TDisplayMode aMode, const TInt aScanlineBufferSize);
	TVerdict TestSubCodecDecodeL(const TInt aBitDepth, TPngImageInformation::TColorType aColorType,
								 const TDesC& aFileName);

	TVerdict MM_ICL_COD_U_0230_L();
	TVerdict MM_ICL_COD_U_0231_L();
	TVerdict MM_ICL_COD_U_0232_L();
	TVerdict MM_ICL_COD_U_0233_L();
	TVerdict MM_ICL_COD_U_0234_L();
	TVerdict MM_ICL_COD_U_0235_L();
	TVerdict MM_ICL_COD_U_0236_L();
	TVerdict MM_ICL_COD_U_0237_L();
	TVerdict MM_ICL_COD_U_0238_L();
	TVerdict MM_ICL_COD_U_0239_L();
	TVerdict MM_ICL_COD_U_0240_L();
	TVerdict MM_ICL_COD_U_0241_L();
	TVerdict MM_ICL_COD_U_0242_L();
	TVerdict MM_ICL_COD_U_0243_L();
	TVerdict MM_ICL_COD_U_0244_L();
	TVerdict MM_ICL_COD_U_0245_L();
	TVerdict MM_ICL_COD_U_0250_L();
protected:
	CActiveScheduler* iScheduler;
	TPngWriteSubCodecTestId iTestId;
	};

class RCodecDataTest : public RTestStepCompare
	{
public:	
	enum TMimeTest
		{
		EMIME_TEST_DATA,
		EMIME_TEST_FILE,
		EMIME_TEST_RECOG,
		EMIME_TEST_RECOG_DIRECT
		};

public:
	static RCodecDataTest* NewL(CTestSuite* aSuite, const TDesC& aTestName, TCodecDataTestId aTestId);

protected:
	RCodecDataTest(CTestSuite* aSuite, const TDesC& aTestName, TCodecDataTestId aTestId);
	
	// from RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();

private:
	class TCodecDataType
		{
	public:
		TCodecDataType(const TDesC& aFileName, const TDesC8& aMimeType, TUid aImageType, TUid aImageSubType, CApaDataRecognizerType::TRecognitionConfidence aConfidence);
		TInt AddInfoString(const TDesC& aInfoString);
		const TDesC& iFileName;
		const TDesC8& iMimeType;
		const TUid iImageType;
		const TUid iImageSubType;
		const CApaDataRecognizerType::TRecognitionConfidence iConfidence;
		RArray<TPtrC> iInfoStrings;
		};

	void VerifyFrameInfoStringsL(const TCodecDataType& aCodecData);
	void VerifyImageTypesL(const TCodecDataType& aCodecData);
	void VerifyMimeTypesL(const TCodecDataType& aCodecData, TMimeTest aTestType/*TBool aFileTest*/);
	void SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;

private:
	TCodecDataTestId iTestId;
	RArray<TCodecDataType> iPluginData;
	RFs iFs;

protected:
	CActiveScheduler* iScheduler;
	};

class RRecogAllocTest : public RTestStepCompare
	{
public:
	static RRecogAllocTest* NewL(const TDesC& aTestName, TCodecDataTestId aTestId);

protected:
	RRecogAllocTest(const TDesC& aTestName, TCodecDataTestId aTestId);
	
	// from RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();

private:
	void VerifyMimeTypesL(const TDesC& aFileName, TDes8& aMimeType, TInt& aConfidenceLevel);

private:
	TCodecDataTestId iTestId;
	RFs iFs;
	TPtrC iFileName;
	TPtrC8 iMimeType;


protected:
	CActiveScheduler* iScheduler;
	};


class RRecogNegTest : public RTestStepCompare
	{
public:
	static RRecogNegTest* NewL(CTestSuite* aSuite, const TDesC& aTestName);

protected:
	RRecogNegTest(CTestSuite* aSuite, const TDesC& aTestName);
	
	// from RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();

private:
	void VerifyMimeTypesL(const TDesC& aFileName);
	void SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;

private:
	RPointerArray<TDesC> iFileNames;
	RFs iFs;

protected:
	CActiveScheduler* iScheduler;
	};

// Test for "INC093688 - JPEG recognizer and mime types"
class RRecogNonStandardMimeTest : public RTestStepCompare
	{
public:
	static RRecogNonStandardMimeTest* NewL(CTestSuite* aSuite, const TDesC& aTestName);

protected:
	RRecogNonStandardMimeTest(CTestSuite* aSuite, const TDesC& aTestName);
	
	// from RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();
	
private:	
	void VerifyNonStandardMimeTypeL();
	void SetTestFileNameL(TDes& aFileName, const TDesC& aPathNameAndExtn) const;
	
protected:
	CActiveScheduler* iScheduler;

private:
	RFs iFs;
	};
	
// Test for PDEF111744: WMF plugin should not rely on RFbsBitmap::Connect() having been called 
class RRecogNonStandardWMFTest : public RTestStepCompare
	{
public:
	static RRecogNonStandardWMFTest* NewL(CTestSuite* aSuite, const TDesC& aTestName);
protected:
	RRecogNonStandardWMFTest(CTestSuite* aSuite, const TDesC& aTestName);
	// from RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();
	CActiveScheduler* iScheduler;
private:	
	void VerifyWMFWithoutServer();
	RFs iFs;
	};

// Test for  PDEF112277:V&V:IOP:CONTENT: .wbm not recognized as image file  
class RRecogNonStandardWBMTest : public RTestStepCompare
	{
public:
 	static RRecogNonStandardWBMTest* NewL(CTestSuite* aSuite, const TDesC& aTestName);
protected:
	RRecogNonStandardWBMTest(CTestSuite* aSuite, const TDesC& aTestName);
	// from RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();
	CActiveScheduler* iScheduler;
private:
	void VerifyWBM();
	RFs iFs;
 	};

#endif //__ICLCodecTestStep_H__

