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

#if (!defined __IMAGE_PROCESSOR_TEST_H__)
#define __IMAGE_PROCESSOR_TEST_H__  

#include "ICLFrameWorkTestStep.h"

/*
 * CImageProcessor test
 * Use CImageProcessor to write a sequence of standard pixels
 * to a bitmap for every display mode. Verify that the result
 * is as expected.
 *
 */
class RCImageProcessorUtilityTest : public RICLFrameworkTestStep
	{
private:
	class TTestCase
		{
		public:
			TTestCase(TDisplayMode aDisplayMode, const TDesC8& aResultBuffer, const TDesC& aDisplayName);
		public:
			TDisplayMode iDisplayMode;
			TPtrC8 iResultBuffer;
			TPtrC iDisplayName;
		};

public:
	RCImageProcessorUtilityTest(const TDesC& aTestName);

protected:
	// From RTestStep
	TVerdict OpenL();
	TVerdict DoTestStepL();
	void Close();

private:
	RArray<TTestCase> iTestData;
	};

#endif // __IMAGE_PROCESSOR_TEST_H__
