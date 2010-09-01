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

#if (!defined __IMAGE_CONVERT_TESTSTEP_H__)
#define __IMAGE_CONVERT_TESTSTEP_H__

#include <imagetransform.h>
#include <e32base.h>


enum TImageTestType
	{
	ENormal,
	EAlloc
	};

class TImageTransformUnitTestParams
	{	
public:
	const TText*  iTestName;		// name of the test
	const TText*  iTestDescription;	// description
	const TText*  iInputFilename;	// source filename
	TInt iDestWidth;				// dest width
	TInt iDestHeight;				// dest height

	TBool iMaintainAspectRatio;		// maintain aspect ratio
	TBool iSourceIsFilename;		// Source is a filename
	TBool iDestIsFilename;			// Destination is a filename
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

class CActiveListener : public CActive
	{
public:
	CActiveListener(CImageTransform& aImageTransform);
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
	CImageTransform& iImageTransform;
	};

class CTestSuite;
class CTestSuiteImageTransform;

/**
 *
 * CTestStepImageTransform, which is the base class for all 
 * the ImageTransform suite test steps, providing common functionality.
 *
 * @lib "TSU_ICL_ImageTransform.lib"
 *
 */
class CTestStepImageTransform : public CTestStep
	{
public:
	CTestStepImageTransform();
	~CTestStepImageTransform();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
	void SetSuite(CTestSuiteImageTransform* aPtr );
 	CTestSuiteImageTransform* Suite();

protected:
    void SetSourceFilenameL(const TDesC& aPathNameAndExtn);
    void SetDestFilenameL(const TDesC& aPathNameAndExtn);
    void SetRefFilenameL(const TDesC& aPathNameAndExtn);
	void DeleteDestFilenameL();
	TVerdict CompareFilesL(const TDesC& aFile1,const TDesC& aFile2) const;
	TVerdict CompareDescWithFileL(const TDesC8& aDesc,const TDesC& aFile) const;
	TFileName& SourceFilename();
	TFileName& DestFilename();
	TFileName& RefFilename();

protected:
	RFs iFs;

private:
  	 CTestSuiteImageTransform* iImageTransformSuite;
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
 * CTestStepTransformGeneric
 */
class CTestStepTransformGeneric : public CTestStepImageTransform
	{
public:
	CTestStepTransformGeneric(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformGeneric();
	// from CTestStep
	virtual TVerdict DoTestStepL();

protected:
	void SetFilenamesL(const TDesC& aSourceFilename);
	void ConfigureL(CImageTransform& aImageTransform);
	TVerdict CheckResult(TInt aListenerError);
	virtual TVerdict TestL();
	void Cleanup();
	void DummyHalCallL();

private:
	TVerdict DoTestStepFbsL();


protected:
	// pointer to current test data
	const TImageTransformUnitTestParams* iTestParams;

	// pointer to previous test data (for API re-use tests)
	const TImageTransformUnitTestParams* iTestParamsPrev;

	HBufC8* iSourceBuf;
	TPtr8 iSourceBufPtr;
	HBufC8* iDestBuf;
	TPtrC8 iMimeType;
	};

/**
 * CTestStepTransformSpecific1
 */
class CTestStepTransformSpecific1 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformSpecific1(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformSpecific1();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformSpecific2
 */
class CTestStepTransformSpecific2 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformSpecific2(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformSpecific2();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformSpecific3
 */
class CTestStepTransformSpecific3 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformSpecific3(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformSpecific3();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformCancel1
 */
class CTestStepTransformCancel1 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformCancel1(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformCancel1();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformCancel2
 */
class CTestStepTransformCancel2 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformCancel2(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformCancel2();
private:
	virtual TVerdict TestL();
	};


/**
 * CTestStepTransformNegative1
 */
class CTestStepTransformNegative1 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformNegative1(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformNegative1();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformNegative6
 */
class CTestStepTransformNegative6 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformNegative6(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformNegative6();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformNegative7
 */
class CTestStepTransformNegative7 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformNegative7(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformNegative7();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformNegative8
 */
class CTestStepTransformNegative8 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformNegative8(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformNegative8();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformNegative9
 */
class CTestStepTransformNegative9 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformNegative9(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformNegative9();
private:
	virtual TVerdict TestL();
	};



/**
 * CTestStepTransformPanic1
 */
class CTestStepTransformPanic1 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformPanic1(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformPanic1();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformPanic2
 */
class CTestStepTransformPanic2 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformPanic2(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformPanic2();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformPanic3
 */
class CTestStepTransformPanic3 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformPanic3(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformPanic3();
private:
	virtual TVerdict TestL();
	};

/**
 * CTestStepTransformPanic4
 */
class CTestStepTransformPanic4 : public CTestStepTransformGeneric
	{
public:
	CTestStepTransformPanic4(const TImageTransformUnitTestParams& aTestParams);
	~CTestStepTransformPanic4();
private:
	virtual TVerdict TestL();
	};

#endif // __IMAGE_CONVERT_TESTSTEP_H__

