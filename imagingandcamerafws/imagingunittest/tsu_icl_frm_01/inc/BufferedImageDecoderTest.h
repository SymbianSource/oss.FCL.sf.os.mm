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

#if (!defined __BUFFERED_IMAGE_DECODER_TEST_H__)
#define __BUFFERED_IMAGE_DECODER_TEST_H__  


class RCBufferedImageDecoderDecodeTest : public RAllocTest
	{
public:
	static RCBufferedImageDecoderDecodeTest* NewL(CTestSuite* aSuite, TBool aMaskDecode, TBool aAllocTest);

protected:
	void ResetPointer();
	void IncrementPointer();
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCBufferedImageDecoderDecodeTest(CTestSuite* aSuite, TBool aMaskDecode, TBool aAllocTest);

private:
	TUid iImplementationUid;

	CBufferedImageDecoder* iImageDecoder;

	TBool iMimeTest; //ETrue use MIME type for FileNewL()/DataNewL()
	TBool iMaskDecode;

	TInt  iSourceLength;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;
	TInt		iPosition;
	RFile		iSourceFile;
	TFileName	iSourceFilename;
	TBuf8<KDefaultBufferSize>	iMimeType;
	CFbsBitmap* iTestBitmap;
	CFbsBitmap* iTestBitmapMask;
	CFbsBitmap* iRefBitmap;
	RFs			iFileSession;

	CActiveScheduler* iScheduler ;
	};

class RCBufferedImageDecoderFunctionTest : public RAllocTest
	{
public:
	static RCBufferedImageDecoderFunctionTest* NewL(CTestSuite* aSuite, TBool aAllocTest);

protected:
	// From RAllocTest
	virtual void SetupL();
	virtual void TestStepL();
	virtual void CheckResultL();
	virtual void Cleanup();
	//From RTestStep
	virtual void Close();

private:
	RCBufferedImageDecoderFunctionTest(CTestSuite* aSuite, TBool aAllocTest);

private:
	CBufferedImageDecoder* iImageDecoder;

	HBufC8*		iSourceData;
	TPtr8		iSourceDataPtr;

	HBufC* iImageComment;
	HBufC* iFrameComment;
	TInt   iImageCommentCount;
	TInt   iFrameCommentCount;

	CFrameInfoStrings* iFrameInfoStrings;
	TBuf<KDefaultBufferSize> iDecoder;
	TBuf<KDefaultBufferSize> iFormat;
	TBuf<KDefaultBufferSize> iDimensions;
	TBuf<KDefaultBufferSize> iDepth;

	TUid iImplementationUid;

	RFs iFileSession;
	CActiveScheduler* iScheduler ;
	};


#endif // __BUFFERED_IMAGE_DECODER_TEST_H__
