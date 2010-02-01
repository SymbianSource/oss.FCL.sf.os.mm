
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
// ICL Codec Tests - see ../UnitTest/ICL/Documentation/ICL Codecs Unit Test Specification.xls
// 
//

#ifndef __ICLCodecTestStep_H__
#define __ICLCodecTestStep_H__

#include <testframework.h>
#include "TGPSImage.h"

const TInt KSmallBufferLen = 33;
const TInt KSectionNameLen = 7;         // last seven characters of the test case number is the name of the section in the configuration file

class TPerformanceTestParams;

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
	TExifDataType	 iDataType;
	TExifOperation iOperation;

	CActiveScheduler* iScheduler;
	
	TInt iDecUid;
	
	TVerdict	iTestResult;
	TBool iIsAllocTest;
	TBuf<KSmallBufferLen> iSectionName;
	};

#endif //__ICLCodecTestStep_H__

