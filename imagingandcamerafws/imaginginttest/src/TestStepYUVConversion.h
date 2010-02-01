// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the test steps ICL YUV testing  for Bravo - PREQ 747.
// 
//


#ifndef	__TESTSTEPYUVCONVERSION__
#define	__TESTSTEPYUVCONVERSION__

// EPOC includes
#include "TestStepConversion.h"
//#include <imageframe.h>
#include <iclexifimageframe.h>
//#include <JPEGImageFramePlugin.h>

/**
 *
 * @class CTestStepYUVConversion 
 * Base Class for YUV Tests
 *
 */
class CTestStepYUVConversion : public CTestStepConversion
{
protected:
	CTestStepYUVConversion(const TDesC& aTestName); 
	void EnableLogs(TBool aEnable);
	
 	// MM-ICL-FRM-I-1001-CP, MM-ICL-FRM-I-1008-HP 
	TInt DecodeToRChunkAndEncodeToFileL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1002-CP,  MM-ICL-FRM-I-1009-HP
	TInt DecodeToRChunkAndEncodeToDescL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1003-CP,  MM-ICL-FRM-I-1010-HP
	TInt DecodeToDescAndEncodeToFileL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1004-CP,  MM-ICL-FRM-I-1011-HP
	TInt DecodeToDescAndEncodeToDescL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1005-LP
	TInt DecodeToRChunkAndEncodeToFileXtraParamsL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1006-LP
	TInt EnquireYUVDecoderPluginUIDL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1006-LP
	TInt EnquireYUVEncoderPluginUIDL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1007-CP
	TInt ListYUVEncoderSupportedFormats();
	
	// MM-ICL-FRM-I-1007-CP
	TInt ListYUVDecoderSupportedFormats();
	
	// MM-ICL-FRM-I-1111-CP
   	TInt YUVEncodeUnsupportedImageL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1112-CP
	TInt YUVEncodeUnsupportedFrameL(const TDesC& aFileName);
	
	// MM-ICL-FRM-I-1113-CP
	TInt YUVEncodeCorruptRChunkL(const TDesC& aFileName);
    
    // MM-ICL-FRM-I-1114-CP
    TInt YUVEncodeCorruptDescriptorL(const TDesC& aFileName);

    // MM-ICL-FRM-I-1117-CP
    TInt JPEGDecodeUnsupportedYUVL(const TDesC& aFileName);
    
    // MM-ICL-FRM-I-1118-CP
    TInt CorruptJPEGDecodeYUVL(const TDesC& aFileName);
    
    // MM-ICL-FRM-I-1119-HP
    TInt CorruptJPEGDecodeDescYUVL(const TDesC& aFileName);
    
    // MM-ICL-FRM-I-1120-HP
    TInt GIFDecodeYUVL(const TDesC& aFileName);
    
    TFileName	iFileNameDest;
    
    TBool iEnableLogs;	

};

/**
 *
 * @class CTestConvertYUVRChunk
 * For YUV Conversion tests from RChunk
 * MM-ICL-FRM-I-1001-CP, MM-ICL-FRM-I-1002-CP, MM-ICL-FRM-I-1008-HP & MM-ICL-FRM-I-1009-HP
 *
 */
class CTestConvertYUVRChunk : public CTestStepYUVConversion
{
public:
	static CTestConvertYUVRChunk* NewL(const TDesC& aTestName);
	virtual TVerdict DoTestStepL();
	
protected:
	TVerdict TestStepL();

private:
	CTestConvertYUVRChunk(const TDesC& aTestName);
};

/**
 *
 * CTestConvertYUVDesc
 * For YUV Conversion tests from Descriptor 
 * MM-ICL-FRM-I-1003-CP, MM-ICL-FRM-I-1004-CP, MM-ICL-FRM-I-1010-HP & MM-ICL-FRM-I-1011-HP
 *
 */
class CTestConvertYUVDesc : public CTestStepYUVConversion
{
public:
	static CTestConvertYUVDesc* NewL(const TDesC& aTestName);
    virtual TVerdict DoTestStepL();
    
protected:    
	TVerdict TestStepL();

private:
	CTestConvertYUVDesc(const TDesC& aTestName);
};

/**
 *
 * For YUV Conversion tests from Descriptor 
 * CTestCSEncodeYUVExtraParams
 * MM-ICL-FRM-I-1005-LP
 *
 */

class CTestCSEncodeYUVExtraParams : public CTestStepYUVConversion
{
	public:
	static CTestCSEncodeYUVExtraParams* NewL(const TDesC& aTestName);

protected:
	CTestCSEncodeYUVExtraParams(const TDesC& aTestName);
	virtual TVerdict DoTestStepL();
};

/**
* 
* CTestCSGetPluginUID
* For correct UID for the YUVJPEG encoder and decoder plug-ins 
* MM-ICL-FRM-I-1006-LP
*
*/

class CTestCSGetPluginUID : public CTestStepYUVConversion
{
	public:
	static CTestCSGetPluginUID* NewL(const TDesC& aTestName);

protected:
	CTestCSGetPluginUID(const TDesC& aTestName);
	virtual TVerdict DoTestStepL();
};

/**
* 
* CTestCSGetSupportedYUVFormats
* For correct list of supported YUV formats for the YUVJPEG encoder and decoder plug-ins
* MM-ICL-FRM-I-1007-LP
*
*/

class CTestCSGetSupportedYUVFormats : public CTestStepYUVConversion
{
	public:
	static CTestCSGetSupportedYUVFormats* NewL(const TDesC& aTestName);

protected:
	CTestCSGetSupportedYUVFormats(const TDesC& aTestName);
	virtual TVerdict DoTestStepL();		
};

/**
* 
* CTestCSYUVEncoderNegative
* For YUV Encoder Negative tests
* MM-ICL-FRM-I-1111-CP, MM-ICL-FRM-I-1112-CP, MM-ICL-FRM-I-1113-HP & MM-ICL-FRM-I-1114-HP
*
*/

class CTestCSYUVEncoderNegative : public CTestStepYUVConversion
{
	public:
	static 	CTestCSYUVEncoderNegative* NewL(const TDesC& aTestName);

protected:
	CTestCSYUVEncoderNegative(const TDesC& aTestName);
	virtual TVerdict DoTestStepL();		
};

/**
* 
* CTestCSYUVDecoderNegative
* For YUV Decoder Negative tests
* MM-ICL-FRM-I-1117-CP, MM-ICL-FRM-I-1118-CP, MM-ICL-FRM-I-1119-HP & MM-ICL-FRM-I-1120-HP
*
*/
class CTestCSYUVDecoderNegative: public CTestStepYUVConversion
{
	public:
	static 	CTestCSYUVDecoderNegative* NewL(const TDesC& aTestName);

protected:
	CTestCSYUVDecoderNegative(const TDesC& aTestName);
	virtual TVerdict DoTestStepL();		
};


#endif
