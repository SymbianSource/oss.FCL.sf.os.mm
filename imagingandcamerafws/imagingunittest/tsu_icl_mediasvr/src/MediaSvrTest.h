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
// This file contains the definition of the classes for each of the MediaSvrBC Unit test steps
// 
//

#ifndef __MEDIASVRTEST_H__
#define __MEDIASVRTEST_H__


#include <mdaimageconverter.h>
#include "TestImageParameter.h"

/**
 *
 * Class with an invalid format to test framework
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class TMdaInvalidClipFormat : public TMdaClipFormat 
	{
public:
	TMdaInvalidClipFormat();
	};

/**
 *
 * MM-ICL-MEDIASVR-U-001. Test loading bitmap images from various formats
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-002. Test loading a bitmap image, without specifying a file format
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest2 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest2* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest2();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-003. Store image in descriptor, and retrieve it from the descriptor
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest3 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest3* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
protected:
	CMediaSvrTest3();

	virtual TInt DescriptorSize() { return 0; } // 0 -> calculate properly
	virtual TInt ExpectedResult() { return KErrNone; }
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-004. Test saving an image to all supported encodings
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest4 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest4* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest4();
private:
	TTestImage iInitialImage;
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-005. Test streaming an image from a descriptor
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest5 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest5* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest5();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	TInt iTestInt;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-006. Test streaming an image from a file
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest6 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest6* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest6();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	TInt iTestInt;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-007. Test loading a bitmap image, specifying an different format from that of 
 * the image.
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest7 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest7* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest7();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-008. Test saving an image to an invalid file
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest8 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest8* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest8();
private:
	TTestImage iInitialImage;
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-009. Test loading a file specifying an invalid format
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest9 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest9* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest9();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	TMdaInvalidClipFormat iInvalidFormat;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-010. Test loading a file which is not an image file
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest10 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest10* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest10();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

class CMdaStressTest;

/**
 *
 * MM-ICL-MEDIASVR-U-011. Multiple thread stress testing
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest11 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest11* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest11();
private:
	CMdaStressTest* iStressTest;
	};

/**
 *
 *
 * MM-ICL-MEDIASVR-U-012. Load image testing failure of each individual memory allocation
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest12 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest12* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
	virtual TVerdict DoTestStepFbsL();
private: 
	CMediaSvrTest12();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-013. Save image testing failure of each individual memory allocation
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest13 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest13* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest13();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-014. Stream image testing failure of each individual memory allocation
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest14 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest14* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest14();
private:
	CArrayFixFlat<TTestImage>* iTestImages;	
	};

/**
 *
 * MM-ICL-MEDIASVR-U-015. Load image, cancelling and restarting decoding
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest15 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest15* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest15();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-016. Save image, cancelling and restarting decoding
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest16 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest16* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest16();
private:
	TTestImage iInitialImage;
	TTestImage iOutputImage;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-017. Check FrameInfo explicitly
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest17 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest17* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest17();
private:
	TTestImage iTestImage;
	TFrameInfo iExpectedFrameInfo;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-018. Check convert with mask
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest18 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest18* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest18();
private:
	TTestImage iTestImage;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-019. Check ConvertL fails correctly when the bitmaps passed to it 
 * are the incorrect format
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest19 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest19* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest19();
private:
	TTestImage iTestImage;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-020. 
 * Test re-using CMdaImageDescToBitmapUtility after a previous ConvertL()
 * returned KErrUnderflow (Defect INC011980).
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest20 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest20* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepPostambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest20();
private:
	CArrayFixFlat<TTestImage>* iTestImages;
	};

/**
 *
 * MM-ICL-MEDIASVR-U-021. 
 * Test CMdaImageDecoder::FrameCommentL()
 *
 * @lib "TS_MediaSvr.lib"
 *
 * @xxxx
 *
 */
class CMediaSvrTest21 : public CTestStepMediaSvr
	{
public:
	static CMediaSvrTest21* NewL();
	virtual TVerdict DoTestStepPreambleL();
	virtual TVerdict DoTestStepL();
private: 
	CMediaSvrTest21();
private:
	TFileName iSourceFile;
	};

/*
 * MM-ICL-MEDIASVR-U-22
 *   Variation on test step 3 that uses a small descriptor and expects KErrArgument result
 *
 */

class CMediaSvrTest22 : public CMediaSvrTest3
	{
public:
	static CMediaSvrTest22* NewL();
private: 
	CMediaSvrTest22();
protected:
	// from CMediaSvrTest3
	virtual TInt DescriptorSize() { return 10; } // use very small descriptor - bound to overflow
	virtual TInt ExpectedResult() { return KErrArgument; }
	};

#endif //__MEDIASVRTEST_H__
