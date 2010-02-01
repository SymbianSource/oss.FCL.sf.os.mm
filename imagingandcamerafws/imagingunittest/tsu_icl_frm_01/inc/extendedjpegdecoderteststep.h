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

#if (!defined __EXTENDED_JPEG_DECODER_TEST_H__)
#define __EXTENDED_JPEG_DECODER_TEST_H__  

#include "ICLFrameWorkTestStep.h"

class CExtendedDecoder;

/**
 * Test for DEF110209 CJpegDecoder::HandleCustomSyncL() problem 
 * 
 */

class RCExtendedJpegDecoderTest : public RICLFrameworkTestStep
	{
public:
	static RCExtendedJpegDecoderTest* NewL(CTestSuite* aSuite, TBool aUseThread);

protected:
	//From RTestStep
	virtual TVerdict OpenL();
	virtual TVerdict DoTestStepL();
	virtual void Close();

private:
	RCExtendedJpegDecoderTest(CTestSuite* aSuite, TBool aUseThread);

private:
	CExtendedDecoder* iDecoder;	
	RFs			iFileSession;
	const TBool	iUseThread;
	CActiveScheduler* iScheduler ;
	};


#endif // __EXTENDED_JPEG_DECODER_TEST_H__
