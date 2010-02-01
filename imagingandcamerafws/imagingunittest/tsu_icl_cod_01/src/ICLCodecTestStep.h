
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
// ICL Codec Tests - see ../UnitTest/ICL/Documentation/ICL Codecs Unit Test Specification.xls
// 
//

#ifndef __ICLCodecTestStep_H__
#define __ICLCodecTestStep_H__

#include <testframework.h>
#include "TImage.h"

const TInt KSmallBufferLen = 33;
const TInt KSectionNameLen = 7;         // last seven characters of the test case number is the name of the section in the configuration file

class TImage;

/*
 * Base class for all test steps - wraps call into old TImage code. 
 * This class does all of these test steps, depending on the parameters read from table on construction
 */
class RICLCodecStep : public RTestStep
	{
public:
	static RICLCodecStep* NewL(TInt aIndex);
protected:
	RICLCodecStep(TInt aIndex);
	// From RTestStep
	TVerdict OpenL();
	void Close();
	TVerdict DoTestStepL();

protected:
	TInt		 iIndex;		// not required but useful for debugging
	TImage*		 iTImage;		
	TImageCall	 iCallL;		// read from table
	TImageFormat iFormat;		// ditto
	const TText* iTestFileName; // ditto
	TBool		 iIsAllocTest;  // ditto
	TBool		 iUseThread;    // ditto

	CActiveScheduler* iScheduler;
	TBuf<KSmallBufferLen> iSectionName;
	TVerdict	iTestResult;
	};

/*
 * Image Codec Panic Generic Test Step
 */
NONSHARABLE_CLASS( CTestStepICLCodecPanic ): public CTestStep
	{
public:
	CTestStepICLCodecPanic();
	~CTestStepICLCodecPanic();
	
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	
protected:
	RFs iFs;
	};

/*
 * Panic test class for DEF091559: T: Documentation error on CImageDecoder::FileNewL(), no EIllegalImageType panic.
 * EIllegalImageSubType panic generated when trying to call CImageDecoder::FileNewL() with an image sub-type but no image base type.
 */
NONSHARABLE_CLASS( CTestStepICLCodecPanic_1 ): public CTestStepICLCodecPanic
	{
public:
		CTestStepICLCodecPanic_1();
		~CTestStepICLCodecPanic_1();
		virtual TVerdict DoTestStepL();
	};

/*
 * Panic test class for DEF091559: T: Documentation error on CImageDecoder::FileNewL(), no EIllegalImageType panic.
 * EIllegalImageType panic generated when trying to call CImageEncoder::FileNewL() with no image type and no encoder uid.
 */
NONSHARABLE_CLASS( CTestStepICLCodecPanic_2 ): public CTestStepICLCodecPanic
	{
public:
		CTestStepICLCodecPanic_2();
		~CTestStepICLCodecPanic_2();
		virtual TVerdict DoTestStepL();
	};

/*
 * Panic test class for DEF091559: T: Documentation error on CImageDecoder::FileNewL(), no EIllegalImageType panic.
 * EIllegalImageSubType panic generated when trying to call CImageEncoder::FileNewL() with an image sub-type but no image base type.
 */
NONSHARABLE_CLASS( CTestStepICLCodecPanic_3 ): public CTestStepICLCodecPanic
	{
public:
		CTestStepICLCodecPanic_3();
		~CTestStepICLCodecPanic_3();
		virtual TVerdict DoTestStepL();
	};

#endif //__ICLCodecTestStep_H__

