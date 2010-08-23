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

#include <ecom/ecomresolverparams.h>
#include <ecom/ecom.h>
#include <barsc.h>
#include <apgcli.h>
#include "ICLFrameWorkTestStep.h"
#include "ICLFrameworkTestSuite.h"
#include "icl/ImageCodec.h"
#include "icl/ImageCodecData.h"
#include "ImageConversion.h"
#include "ImageResolverAPI.h"
#include "icl/ICL_UIDS.hrh"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "TestUids.hrh"
#include "imgpositionprocessor.h"

_LIT(KTiffFile,"TIF2.TIF");
_LIT(KTiffFileRef,"TIF2.MBM");
_LIT(KGifFile,"TRANS_1.GIF");
_LIT(KGifFileRef,"TRANS_1.MBM");

// Encoder tables.
const TInt16 KSourcePixData[] = {
	0,  1,  2,  3,  4,  5,  6,  7,
	8,  9,  10,  11,  12,  13,  14,  15,
	16,  17,  18,  19,  20,  21,  22,  23};

const TInt16 KRefRotPixData[] = {
	16,  8,  0,
	17,  9,  1,  
	18,  10, 2,  
	19,  11, 3,  
	20,  12, 4,
	21,  13, 5,  
	22,  14, 6,
	23,  15, 7};

const TInt16 KRefHorFlipPixData[] = {
	16,  17,  18,  19,  20,  21,  22,  23,
	8,  9,  10,  11,  12,  13,  14,  15,
	0,  1,  2,  3,  4,  5,  6,  7};

const TInt16 KRefVerFlipPixData[] = {
	7,	6,	5,  4,  3,  2,	1,	0, 	
	15,	14, 13,	12, 11, 10, 9,	8,  
	23,	22, 21,	20, 19,	18,	17,	16};
	
const TInt16 KRefRotVerHorFlipPixData[] = {	
	7,	15,	23,
	6,	14,	22,
	5,	13,	21,
	4,	12,	20,
	3,	11,	19,
	2,	10,	18,
	1,	9, 	17,
	0,	8, 	16};

const TInt KEncoderPixCount  = 24;
const TInt KEncoderPixWidth  = 8;
const TInt KEncoderPixHeight = 3;

//RICLFrameworkTestStep
void RICLFrameworkTestStep::GetTestFileNameL(TFileName& aFileName, const TFileName& aPathNameAndExtn) const
	{
	TFileName defaultPath = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	TParse parse;
	User::LeaveIfError(parse.SetNoWild(aPathNameAndExtn, &defaultPath, NULL));
	aFileName = parse.FullName();
	}

void RICLFrameworkTestStep::SetVerdict(TVerdict aVerdict)
	{
	if(aVerdict > iTestStepResult)
		iTestStepResult = aVerdict;
	}

void RICLFrameworkTestStep::Compare(const TDesC& aDescriptor1, const TDesC& aDescriptor2, const TDesC& aErrorMessage)
	{
	TPtrC ptr1;
	ptr1.Set(aDescriptor1);

	TPtrC ptr2;
	ptr2.Set(aDescriptor2);

	if(aDescriptor1.Ptr() == aDescriptor2.Ptr())
		SetFail(_L("Comparing same Pointer"));

	if(aDescriptor1 != aDescriptor2)
		SetFail(aErrorMessage);
	}

void RICLFrameworkTestStep::Compare(const TDesC8& aDescriptor1, const TDesC8& aDescriptor2, const TDesC& aErrorMessage)
	{
	TPtrC8 ptr1;
	ptr1.Set(aDescriptor1);

	TPtrC8 ptr2;
	ptr2.Set(aDescriptor2);

	if(aDescriptor1.Ptr() == aDescriptor2.Ptr())
		SetFail(_L("Comparing same Pointer"));

	if(aDescriptor1 != aDescriptor2)
		SetFail(aErrorMessage);
	}

void RICLFrameworkTestStep::ComparePointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage)
	{
	if(aPointer1 != aPointer2)
		SetFail(aErrorMessage);
	}

void RICLFrameworkTestStep::CompareDiffPointers(const TAny* aPointer1, const TAny* aPointer2, const TDesC& aErrorMessage)
	{
	if(aPointer1 == aPointer2)
		SetFail(aErrorMessage);
	}

void RICLFrameworkTestStep::Compare(TInt aNumber1, TInt aNumber2, const TDesC& aErrorMessage)
	{
	if(aNumber1 != aNumber2)
		SetFail(aErrorMessage);
	}

void RICLFrameworkTestStep::Compare(TUid aUid1, TUid aUid2, const TDesC& aErrorMessage)
	{
	if(aUid1 != aUid2)
		SetFail(aErrorMessage);
	}

void RICLFrameworkTestStep::Compare(const CFbsBitmap& aBitmap1, const CFbsBitmap& aBitmap2, const TDesC& aErrorMessage)
	{

	if(aBitmap1.SizeInPixels() != aBitmap2.SizeInPixels())
		{
		SetFail(aErrorMessage);
		return;
		}

	const TSize size = aBitmap1.SizeInPixels();
	
	for (TInt y = 0; y < size.iHeight; y++)
		{
		for (TInt x = 0; x < size.iWidth; x++)
			{
			TPoint pt(x,y);
			TRgb color1,color2;
			aBitmap1.GetPixel(color1,pt);
			aBitmap2.GetPixel(color2,pt);
				
			if (color1 != color2)
				{
				SetFail(aErrorMessage);
				return;
				}
			}
		} // end of comparison for-loop
	}


void RICLFrameworkTestStep::SetFail(const TDesC& aErrorMessage)
	{
	ERR_PRINTF1(aErrorMessage);
	SetVerdict(EFail);
	}


TBool RICLFrameworkTestStep::CheckForFile(const RFs& aFs, const TDesC& aFileName)
	{
	TUint flags = 0;	
	TInt err = aFs.Att(aFileName, flags);
	
	if (err != KErrNone)
		{
		INFO_PRINTF3(_L("*** Error %d when checking for file %S"), err, &aFileName);
		return EFalse;
		}
	
	return ETrue;
	}

//RAllocTest
RAllocTest::RAllocTest(TBool aAllocTest) :
	iAllocTest(aAllocTest)
	{
	}

TVerdict RAllocTest::OpenL()
	{
	//Test if this build support alloc testing
	if(iAllocTest)
		{
		__UHEAP_SETFAIL(RHeap::EFailNext, 1);
		TAny *testAlloc = User::Alloc(1);
		if (testAlloc!=NULL)
			{
			delete testAlloc; testAlloc=NULL;
			ERR_PRINTF1(_L("This build does not support alloc testing"));
			return EInconclusive;
			}
		}

	SetupL();
	return iTestStepResult;
	}

TVerdict RAllocTest::DoTestStepL()
	{
	if(iAllocTest)
		{ // Alloc test
		TInt err;
		TInt failCount = 1;
		TBool completed = EFalse;
		TBool badResult = EFalse; 
		TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
		for (;;)
			{
			__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
			__MM_HEAP_MARK;

			TRAP(err,TestStepL());

			completed = EFalse;
			if (err == KErrNone)
				{
				TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
				if (testAlloc==NULL)
					{
					reachedEnd = ETrue;
					failCount -= 1;
					}
				else
					User::Free(testAlloc);
				// see if valid result and break if wrong - might be premature result
				TRAP_IGNORE(CheckResultL());
				if(iTestStepResult != EPass)
					badResult = ETrue;
				completed = reachedEnd || badResult;
				}
			else if (err != KErrNoMemory) // bad error code
				{
				__UHEAP_SETFAIL(RHeap::ENone, 0); // avoid problem in ERR_PRINTF
				completed = ETrue;
				ERR_PRINTF2(_L("Bad error code %d"),err);
				SetVerdict(EFail);
				}

			Cleanup();

			__MM_HEAP_MARKEND;
			__UHEAP_SETFAIL(RHeap::ENone, 0);

			if (completed)
				break; // exit loop

			failCount += 1;
			}
		INFO_PRINTF2(_L("Finish with %d allocation tests"),failCount);
		}
	else
		{ // Normal test
		TestStepL();
		CheckResultL();
		Cleanup();
		}

	return iTestStepResult;
	}

void RAllocTest::Close()
	{
	delete iListener;	
	}

// RTImageParameterDataTest
RTImageParameterDataTest* RTImageParameterDataTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTImageParameterDataTest(aSuite);
	}

RTImageParameterDataTest::RTImageParameterDataTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4001-HP");
	}

TVerdict RTImageParameterDataTest::DoTestStepL()
	{
	INFO_PRINTF1(_L("<<< TImageParameterData Test >>>"));

	TImageParameterData imageParameterData;	

	//TEST SetFilenamePtr()
	{
	TPtrC filename;
	const TDesC* filenamePtr;

	imageParameterData.SetFilenamePtr(&filename);

	TBool isFilename = imageParameterData.IsFilename();
	if(!isFilename)
		SetFail(_L("IsFilename() return EFalse after SetFilenamePtr() !"));

	filenamePtr = imageParameterData.SourceFilenamePtr();
	if(filenamePtr != &filename)
		SetFail(_L("SourceFilenamePtr() returned wrong pointer !"));
	}
	
	//TEST SetDataPtr()
	{
	TPtrC8 data;
	imageParameterData.SetDataPtr(&data);
	TBool isFilename = imageParameterData.IsFilename();
	if(isFilename)
		SetFail(_L("IsFilename() return ETrue after SetDataPtr() !"));

	const TDesC8* dataPtr = imageParameterData.SourceDataPtr();
	if(dataPtr != &data)
		SetFail(_L("SourceDataPtr() returned wrong pointer !"));
	}

	return iTestStepResult;
	}

// RTBufPtr8Test
RTBufPtr8Test* RTBufPtr8Test::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTBufPtr8Test(aSuite);
	}

RTBufPtr8Test::RTBufPtr8Test(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4002-HP");
	}

TVerdict RTBufPtr8Test::DoTestStepL()
	{
	const TInt maxLength    = 128;
	const TInt middleLength = maxLength / 2;
	const TInt shortLength  = middleLength / 2;

	INFO_PRINTF1(_L("<<< TBufPtr8 Test >>>"));

	TBufPtr8 tBufPtr;

	//Test with TBuf8
	TBuf8<maxLength> tBuf;
	const TUint8* origPtr = tBuf.Ptr();

	//Test Set()
	tBuf.SetLength(middleLength);
	tBufPtr.Set(tBuf);
	CompareBuffer(tBufPtr, origPtr, middleLength, maxLength);

	//Test SetLengthOnly()
	tBufPtr.SetLengthOnly(tBuf);
	CompareLengths(tBuf, middleLength);
	CompareBuffer(tBufPtr, origPtr, middleLength, middleLength);

	//Test Shift()
	tBuf.SetLength(middleLength + shortLength);
	tBufPtr.Set(tBuf);
	tBufPtr.Shift(middleLength);
	CompareLengths(tBuf, middleLength + shortLength);
	CompareBuffer(tBufPtr, (origPtr + middleLength), shortLength, (maxLength - middleLength));

	//Test with TPtrC8
	TPtrC8 tPtrC;
	tBuf.SetLength(middleLength);
	tPtrC.Set(tBuf);
	origPtr = tPtrC.Ptr();

	//Test Set()
	tBufPtr.Set(tPtrC);
	CompareBuffer(tBufPtr, origPtr, middleLength, middleLength);

	return iTestStepResult;
	}

void RTBufPtr8Test::ComparePointers(const TDesC8& aPtr1, const TUint8* aPtr2)
	{
	if(aPtr1.Ptr() != aPtr2)
		SetFail(_L("Ptr() wrong"));
	}

void RTBufPtr8Test::CompareLengths(const TDesC8& aPtr1, TInt aLength)
	{
	Compare(aPtr1.Length(), aLength, _L("Length() wrong"));
	}

void RTBufPtr8Test::CompareMaxLengths(const TDes8& aPtr1, TInt aMaxLength)
	{
	Compare(aPtr1.MaxLength(), aMaxLength, _L("MaxLength() wrong"));
	}

void RTBufPtr8Test::CompareBuffer(const TDes8& aBuf, const TUint8* aPtr, TInt aLength, TInt aMaxLength)
	{
	ComparePointers(aBuf, aPtr);
	CompareLengths(aBuf, aLength);
	CompareMaxLengths(aBuf, aMaxLength);
	}

// RCImageDataTest
TVerdict RTImageDataTest::OpenL()
	{
	return EPass;
	}

TVerdict RTImageDataTest::DoTestStepL()
	{
	TUid dataType;
	if(iImageDataTest)
		dataType = iImageData->DataType();
	else
		dataType = iFrameData->DataType();

	CheckDataType(dataType);

	DoSetupL();

	iFrameImageData = CFrameImageData::NewL();
	if(iImageDataTest)
		iDuplicateImageData = iImageData->DuplicateL(*iFrameImageData);
	else
		iDuplicateFrameData = iFrameData->DuplicateL(*iFrameImageData);

	DoCheckDuplicateL();

	if(iImageDataTest)
		dataType = iDuplicateImageData->DataType();
	else
		dataType = iDuplicateFrameData->DataType();

	CheckDataType(dataType);

	return iTestStepResult;
	}

void RTImageDataTest::CheckDataType(TUid aDataType)
	{
	Compare(aDataType, iDataType, _L("Data type wrong"));
	}

void RTImageDataTest::Close()
	{
	if(iImageDataTest)
		{
		delete iImageData; iImageData = NULL;
		delete iDuplicateImageData; iDuplicateImageData = NULL;
		}
	else
		{
		delete iFrameData; iFrameData = NULL;
		delete iDuplicateFrameData; iDuplicateFrameData = NULL;
		}
	delete iFrameImageData; iFrameImageData = NULL;
	}
	
//RTBmpImageDataTest
RTBmpImageDataTest* RTBmpImageDataTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTBmpImageDataTest(aSuite);
	}

TVerdict RTBmpImageDataTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TBmpImageData Test >>>"));

	iImageData = new (ELeave) TBmpImageData;
	iDataType = KBMPImageDataUid;
	iBitsPerPixel = 24;

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

RTBmpImageDataTest::RTBmpImageDataTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4003-HP");
	}

void RTBmpImageDataTest::DoSetupL()
	{
	TBmpImageData* imageData;
	imageData = REINTERPRET_CAST(TBmpImageData*,iImageData);

	imageData->iBitsPerPixel = iBitsPerPixel;
	}

void RTBmpImageDataTest::DoCheckDuplicateL()
	{

	TBmpImageData* duplicateImageData;

	duplicateImageData = REINTERPRET_CAST(TBmpImageData*,iDuplicateImageData);

	Compare(duplicateImageData->iBitsPerPixel, iBitsPerPixel, _L("Duplicated bits per pixel wrong"));
	}

//RTBmpCompressionTest
RTBmpCompressionTest* RTBmpCompressionTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTBmpCompressionTest(aSuite);
	}

TVerdict RTBmpCompressionTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TBmpCompression Test >>>"));

	iImageData = new (ELeave) TBmpCompression;
	iDataType = KBMPCompressionUid;
	iCompression = TBmpCompression::EFourBppRLE;

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

RTBmpCompressionTest::RTBmpCompressionTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4004-HP");
	}

void RTBmpCompressionTest::DoSetupL()
	{
	TBmpCompression* bmpCompression;
	bmpCompression = REINTERPRET_CAST(TBmpCompression*,iImageData);

	bmpCompression->iCompression = iCompression;
	}

void RTBmpCompressionTest::DoCheckDuplicateL()
	{

	TBmpCompression* duplicateBmpCompression;

	duplicateBmpCompression = REINTERPRET_CAST(TBmpCompression*,iDuplicateImageData);

	Compare(duplicateBmpCompression->iCompression, iCompression, _L("Duplicated Compression wrong"));
	}

//RTJpegImageDataTest
RTJpegImageDataTest* RTJpegImageDataTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTJpegImageDataTest(aSuite);
	}

TVerdict RTJpegImageDataTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TJpegImageData Test >>>"));

	iImageData = new (ELeave) TJpegImageData;
	iDataType = KJPGImageDataUid;

	iSampleScheme = TJpegImageData::EColor422; //Use any color
	iQualityFactor = 57; //Use any integer

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

RTJpegImageDataTest::RTJpegImageDataTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4005-HP");
	}

void RTJpegImageDataTest::DoSetupL()
	{
	TJpegImageData* imageData;
	imageData = REINTERPRET_CAST(TJpegImageData*,iImageData);

	imageData->iSampleScheme = iSampleScheme;
	imageData->iQualityFactor = iQualityFactor;

	}

void RTJpegImageDataTest::DoCheckDuplicateL()
	{

	TJpegImageData* duplicateImageData;

	duplicateImageData = REINTERPRET_CAST(TJpegImageData*,iDuplicateImageData);

	Compare(duplicateImageData->iSampleScheme, iSampleScheme, _L("Duplicated sample scheme wrong"));
	Compare(duplicateImageData->iQualityFactor, iQualityFactor, _L("Duplicated quality factor wrong"));
	}

//RTJpegQTableTest
RTJpegQTableTest* RTJpegQTableTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTJpegQTableTest(aSuite);
	}

TVerdict RTJpegQTableTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TJpegQTable Test >>>"));

	iImageData = new (ELeave) TJpegQTable;
	iDataType = KJPGQTableUid;

	iTableIndex = 3; //Use any index
	iEntry.Copy(_L("Hallo World")); //Use any buffer

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

RTJpegQTableTest::RTJpegQTableTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4006-HP");
	}

void RTJpegQTableTest::DoSetupL()
	{
	TJpegQTable* imageData;
	imageData = REINTERPRET_CAST(TJpegQTable*,iImageData);

	imageData->iTableIndex = iTableIndex;
	imageData->iEntries.Copy(iEntry);
	}

void RTJpegQTableTest::DoCheckDuplicateL()
	{
	TJpegQTable* duplicateImageData;

	duplicateImageData = REINTERPRET_CAST(TJpegQTable*,iDuplicateImageData);

	Compare(duplicateImageData->iTableIndex, iTableIndex, _L("Duplicated table index wrong"));
	Compare(duplicateImageData->iEntries, iEntry, _L("Duplicated entry wrong"));
	}

//RTJpegCommentTest
RTJpegCommentTest* RTJpegCommentTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTJpegCommentTest(aSuite);
	}

TVerdict RTJpegCommentTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TJpegComment Test >>>"));

	iImageData = new (ELeave) TJpegComment;
	iDataType = KJPGCommentUid;

	iComment = HBufC8::NewL(20);
	TPtr8 commentPtr(iComment->Des());
	commentPtr.Copy(_L("Hello World!"));

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

void RTJpegCommentTest::Close()
	{
	delete iComment; iComment = NULL;

	RTImageDataTest::Close();
	}

RTJpegCommentTest::RTJpegCommentTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4007-HP");
	}

void RTJpegCommentTest::DoSetupL()
	{
	TJpegComment* imageData;
	imageData = REINTERPRET_CAST(TJpegComment*,iImageData);

	imageData->iComment = iComment;
	}

void RTJpegCommentTest::DoCheckDuplicateL()
	{
	TJpegComment* duplicateImageData;

	duplicateImageData = REINTERPRET_CAST(TJpegComment*,iDuplicateImageData);

	// Make sure we are comparing different pointers
	if(iComment == duplicateImageData->iComment)
		{
		ERR_PRINTF1(_L("Comparing same pointer"));
		SetVerdict(EFail);
		}

	TInt comparison;
	comparison = iComment->Compare(duplicateImageData->iComment->Des());
	if(comparison)
		{
		ERR_PRINTF1(_L("Duplicated comment wrong"));
		SetVerdict(EFail);
		}
	}

//RTGifBackgroundColorTest
RTGifBackgroundColorTest* RTGifBackgroundColorTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTGifBackgroundColorTest(aSuite);
	}

TVerdict RTGifBackgroundColorTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TGifBackgroundColor Test >>>"));

	iImageData = new (ELeave) TGifBackgroundColor;

	iDataType = KGIFBackgroundColorUid;
	iBackgroundColorIndex = 53; //Use any index
	iBackgroundColor = KRgbDarkMagenta; //Use any color

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

RTGifBackgroundColorTest::RTGifBackgroundColorTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4008-HP");
	}

void RTGifBackgroundColorTest::DoSetupL()
	{
	TGifBackgroundColor* imageData;
	imageData = REINTERPRET_CAST(TGifBackgroundColor*,iImageData);

	imageData->iBackgroundColorIndex = iBackgroundColorIndex;
	imageData->iBackgroundColor = iBackgroundColor;
	}

void RTGifBackgroundColorTest::DoCheckDuplicateL()
	{
	TGifBackgroundColor* duplicateImageData;

	duplicateImageData = REINTERPRET_CAST(TGifBackgroundColor*,iDuplicateImageData);

	if(duplicateImageData->iBackgroundColorIndex != iBackgroundColorIndex)
		{
		ERR_PRINTF1(_L("Duplicated background color index wrong"));
		SetVerdict(EFail);
		}

	if(duplicateImageData->iBackgroundColor != iBackgroundColor)
		{
		ERR_PRINTF1(_L("Duplicated background color wrong"));
		SetVerdict(EFail);
		}
	}

//RTGifImageDescriptorTest
RTGifImageDescriptorTest* RTGifImageDescriptorTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTGifImageDescriptorTest(aSuite);
	}

TVerdict RTGifImageDescriptorTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TGifImageDescriptor Test >>>"));

	iFrameData = new (ELeave) TGifImageDescriptor;
	iDataType = KGIFImageDescriptorUid;

	iLocalColorMap = ETrue;
	iSortedLocalMap = ETrue;
	iInterlaced = ETrue;

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

RTGifImageDescriptorTest::RTGifImageDescriptorTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4009-HP");
	}

void RTGifImageDescriptorTest::DoSetupL()
	{
	TGifImageDescriptor* frameData;
	frameData = REINTERPRET_CAST(TGifImageDescriptor*,iFrameData);

	frameData->iLocalColorMap = iLocalColorMap;
	frameData->iSortedLocalMap = iSortedLocalMap;
	frameData->iInterlaced = iInterlaced;
	}

void RTGifImageDescriptorTest::DoCheckDuplicateL()
	{
	TGifImageDescriptor* duplicateFrameData;

	duplicateFrameData = REINTERPRET_CAST(TGifImageDescriptor*,iDuplicateFrameData);

	if(duplicateFrameData->iLocalColorMap != iLocalColorMap)
		{
		ERR_PRINTF1(_L("Duplicated local color map wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iSortedLocalMap != iSortedLocalMap)
		{
		ERR_PRINTF1(_L("Duplicated sorted local map wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iInterlaced != iInterlaced)
		{
		ERR_PRINTF1(_L("Duplicated interlace wrong"));
		SetVerdict(EFail);
		}
	}

//
//RTGifColorTableTest
//
/**
 * Factory function for RTGifColorTableTest
 *
 * @return	"RTGifColorTableTest*"
 *			A pointer to the newly created RTGifColorTableTest
 * @xxxx
 */
RTGifColorTableTest* RTGifColorTableTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTGifColorTableTest(aSuite);
	}

/**
 * Constructor
 * Initialise the test step name
 *
 * @xxxx
 */
RTGifColorTableTest::RTGifColorTableTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4032-HP");
	}

/**
 * Create a TGifColorTable set the test values.
 *
 * @return	"TVerdict"
 *			EPass if the setup is successful.
 * @xxxx
 */
TVerdict RTGifColorTableTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TGifColorTable Test >>>"));

	iFrameData = new (ELeave) TGifColorTable;
	iDataType = KGIFColorTableUid;

	// Initialise the test palette with values
	for(TInt entry=0; entry<KGifColorTableMaxEntries; entry++)
		iPalette[entry] = TRgb(entry);

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

/**
 * Initialise the palette with the local values
 *
 * @xxxx
 */
void RTGifColorTableTest::DoSetupL()
	{
	TGifColorTable* frameData;
	frameData = REINTERPRET_CAST(TGifColorTable*,iFrameData);
	
	// Initialise the palette from our local values
	for(TInt entry=0; entry<KGifColorTableMaxEntries; entry++)
		frameData->iPalette[entry] = iPalette[entry];
	}

/**
 * Verify the duplicate against original (local) values
 *
 * @xxxx
 */
void RTGifColorTableTest::DoCheckDuplicateL()
	{
	TGifColorTable* duplicateFrameData;
	duplicateFrameData = REINTERPRET_CAST(TGifColorTable*,iDuplicateFrameData);

	// Compare the duplicate with the local values
	for(TInt entry=0; entry<KGifColorTableMaxEntries; entry++)
		Compare(duplicateFrameData->iPalette[entry].Value(),iPalette[entry].Value(),_L("Palette entry wrong"));

	}

//RTGifLZWInfoTest
RTGifLZWInfoTest* RTGifLZWInfoTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTGifLZWInfoTest(aSuite);
	}

TVerdict RTGifLZWInfoTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TGifLZWInfo Test >>>"));

	iFrameData = new (ELeave) TGifLZWInfo;
	iDataType = KGIFLZWInfoUid;

	iInitialCodeLength = 123;
	iCompressedBytes = 55;

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

RTGifLZWInfoTest::RTGifLZWInfoTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4010-HP");
	}

void RTGifLZWInfoTest::DoSetupL()
	{
	TGifLZWInfo* frameData;
	frameData = REINTERPRET_CAST(TGifLZWInfo*,iFrameData);

	frameData->iInitialCodeLength = iInitialCodeLength;
	frameData->iCompressedBytes = iCompressedBytes;
	}

void RTGifLZWInfoTest::DoCheckDuplicateL()
	{

	TGifLZWInfo* duplicateFrameData;

	duplicateFrameData = REINTERPRET_CAST(TGifLZWInfo*,iDuplicateFrameData);

	if(duplicateFrameData->iInitialCodeLength != iInitialCodeLength)
		{
		ERR_PRINTF1(_L("Duplicated initial code length wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iCompressedBytes != iCompressedBytes)
		{
		ERR_PRINTF1(_L("Duplicated compresses bytes wrong"));
		SetVerdict(EFail);
		}
	}

//RTGifImageControlTest
RTGifImageControlTest* RTGifImageControlTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTGifImageControlTest(aSuite);
	}

TVerdict RTGifImageControlTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TGifImageControl Test >>>"));

	iFrameData = new (ELeave) TGifImageControl;
	iDataType = KGIFImageControlUid;

	iDelayTimeInCentiseconds = 24;
	iTransparentColorIndex = 9;
	iUserInputFlag = ETrue;
	iDisposalMethod = TGifImageControl::ERestoreToPrevious;

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

RTGifImageControlTest::RTGifImageControlTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4011-HP");
	}

void RTGifImageControlTest::DoSetupL()
	{
	TGifImageControl* frameData;
	frameData = REINTERPRET_CAST(TGifImageControl*,iFrameData);

	frameData->iDelayTimeInCentiseconds = iDelayTimeInCentiseconds;
	frameData->iTransparentColorIndex = iTransparentColorIndex;
	frameData->iUserInputFlag = iUserInputFlag;
	frameData->iDisposalMethod = iDisposalMethod;
	}

void RTGifImageControlTest::DoCheckDuplicateL()
	{

	TGifImageControl* duplicateFrameData;

	duplicateFrameData = REINTERPRET_CAST(TGifImageControl*,iDuplicateFrameData);

	if(duplicateFrameData->iDelayTimeInCentiseconds != iDelayTimeInCentiseconds)
		{
		ERR_PRINTF1(_L("Duplicated delay time wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iTransparentColorIndex != iTransparentColorIndex)
		{
		ERR_PRINTF1(_L("Duplicated transparent color index wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iUserInputFlag != iUserInputFlag)
		{
		ERR_PRINTF1(_L("Duplicated user input flag wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iDisposalMethod != iDisposalMethod)
		{
		ERR_PRINTF1(_L("Duplicated disposal method wrong"));
		SetVerdict(EFail);
		}
	}

//RTGifCommentTest
RTGifCommentTest* RTGifCommentTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTGifCommentTest(aSuite);
	}

TVerdict RTGifCommentTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TGifComment Test >>>"));

	iImageData = new (ELeave) TGifComment;
	iDataType = KGIFCommentUid;

	iComment = HBufC8::NewL(20);
	TPtr8 commentPtr(iComment->Des());
	commentPtr.Copy(_L("Hello World!"));

	iImageDataTest = ETrue;

	return iTestStepResult;
	}

void RTGifCommentTest::Close()
	{
	delete iComment; iComment = NULL;
	RTImageDataTest::Close();
	}

RTGifCommentTest::RTGifCommentTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4012-HP");
	}

void RTGifCommentTest::DoSetupL()
	{
	TGifComment* imageData;
	imageData = REINTERPRET_CAST(TGifComment*,iImageData);

	imageData->iComment = iComment;
	}

void RTGifCommentTest::DoCheckDuplicateL()
	{

	TGifComment* duplicateImageData;

	duplicateImageData = REINTERPRET_CAST(TGifComment*,iDuplicateImageData);

	// Make sure we are comparing different pointers
	if(iComment == duplicateImageData->iComment)
		{
		ERR_PRINTF1(_L("Comparing same pointer"));
		SetVerdict(EFail);
		}

	TInt comparison;
	comparison = iComment->Compare(duplicateImageData->iComment->Des());
	if(comparison)
		{
		ERR_PRINTF1(_L("Duplicated comment wrong"));
		SetVerdict(EFail);
		}
	}

//RTMbmDecodeDataTest
RTMbmDecodeDataTest* RTMbmDecodeDataTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTMbmDecodeDataTest(aSuite);
	}

TVerdict RTMbmDecodeDataTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TMbmDecodeData Test >>>"));

	iFrameData = new (ELeave) TMbmDecodeData;
	iDataType = KMBMDecodeDataUid;

	iCompressed = ETrue;
	iImageDataSizeInBytes = 43;

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

RTMbmDecodeDataTest::RTMbmDecodeDataTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4013-HP");
	}

void RTMbmDecodeDataTest::DoSetupL()
	{
	TMbmDecodeData* frameData;
	frameData = REINTERPRET_CAST(TMbmDecodeData*,iFrameData);

	frameData->iCompressed = iCompressed;
	frameData->iImageDataSizeInBytes = iImageDataSizeInBytes;
	}

void RTMbmDecodeDataTest::DoCheckDuplicateL()
	{

	TMbmDecodeData* duplicateFrameData;

	duplicateFrameData = REINTERPRET_CAST(TMbmDecodeData*,iDuplicateFrameData);

	if(duplicateFrameData->iCompressed != iCompressed)
		{
		ERR_PRINTF1(_L("Duplicated compression flag wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iImageDataSizeInBytes != iImageDataSizeInBytes)
		{
		ERR_PRINTF1(_L("Duplicated image data size wrong"));
		SetVerdict(EFail);
		}

	}

//RTMbmEncodeDataTest
RTMbmEncodeDataTest* RTMbmEncodeDataTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTMbmEncodeDataTest(aSuite);
	}

TVerdict RTMbmEncodeDataTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TMbmEncodeData Test >>>"));

	iFrameData = new (ELeave) TMbmEncodeData;
	iDataType = KMBMEncodeDataUid;

	iDisplayMode = EColor256;

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

RTMbmEncodeDataTest::RTMbmEncodeDataTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4014-HP");
	}

void RTMbmEncodeDataTest::DoSetupL()
	{
	TMbmEncodeData* frameData;
	frameData = REINTERPRET_CAST(TMbmEncodeData*,iFrameData);

	frameData->iDisplayMode = iDisplayMode;
	}

void RTMbmEncodeDataTest::DoCheckDuplicateL()
	{

	TMbmEncodeData* duplicateFrameData;

	duplicateFrameData = REINTERPRET_CAST(TMbmEncodeData*,iDuplicateFrameData);

	if(duplicateFrameData->iDisplayMode != iDisplayMode)
		{
		ERR_PRINTF1(_L("Duplicated display mode wrong"));
		SetVerdict(EFail);
		}
	}

//RTTiffImageDataTest
RTTiffImageDataTest* RTTiffImageDataTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTTiffImageDataTest(aSuite);
	}

TVerdict RTTiffImageDataTest::OpenL()
	{
	RTImageDataTest::OpenL();

	INFO_PRINTF1(_L("<<< TTiffImageData Test >>>"));

	iFrameData = new (ELeave) TTiffImageData;
	iDataType = KTIFFImageDataUid;

	iNewSubfileType = 55;
	iSizeInPixels = TSize(640,480);
	iBitsPerSample = 8;
	iSamplesPerPixel = 4;
	iCompression = 1;
	iT4Options = 4;
	iT6Options = 8;
	iPhotometricInterpretation = 16;
	iFillOrder = 32;
	iWidthInTwips = 6400.5;
	iHeightInTwips = 4800.5;
	iRowsPerStrip = 5;
	iNumStrips = 9;

	iStripInfo = HBufC8::NewL(20);
	TPtr8 stripInfoPtr(iStripInfo->Des());
	stripInfoPtr.Copy(_L("Hello World!"));

	iImageDataTest = EFalse;

	return iTestStepResult;
	}

void RTTiffImageDataTest::Close()
	{
	delete iStripInfo; iStripInfo = NULL;
	RTImageDataTest::Close();
	}

RTTiffImageDataTest::RTTiffImageDataTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4015-HP");
	}

void RTTiffImageDataTest::DoSetupL()
	{
	TTiffImageData* frameData;
	frameData = REINTERPRET_CAST(TTiffImageData*,iFrameData);

	frameData->iNewSubfileType = iNewSubfileType;
	frameData->iSizeInPixels = iSizeInPixels;
	frameData->iBitsPerSample = iBitsPerSample;
	frameData->iSamplesPerPixel = iSamplesPerPixel;
	frameData->iCompression = iCompression;
	frameData->iT4Options = iT4Options;
	frameData->iT6Options = iT6Options;
	frameData->iPhotometricInterpretation = iPhotometricInterpretation;
	frameData->iFillOrder = iFillOrder;
	frameData->iWidthInTwips = iWidthInTwips;
	frameData->iHeightInTwips = iHeightInTwips;
	frameData->iRowsPerStrip = iRowsPerStrip;
	frameData->iNumStrips = iNumStrips;
	frameData->iStripInfo = iStripInfo;

	}

void RTTiffImageDataTest::DoCheckDuplicateL()
	{

	TTiffImageData* duplicateFrameData;

	duplicateFrameData = REINTERPRET_CAST(TTiffImageData*,iDuplicateFrameData);

	if(duplicateFrameData->iNewSubfileType != iNewSubfileType)
		{
		ERR_PRINTF1(_L("Duplicated new sub file type wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iSizeInPixels != iSizeInPixels)
		{
		ERR_PRINTF1(_L("Duplicated size wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iBitsPerSample != iBitsPerSample)
		{
		ERR_PRINTF1(_L("Duplicated bits per sample wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iSamplesPerPixel != iSamplesPerPixel)
		{
		ERR_PRINTF1(_L("Duplicated samples per pixel wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iCompression != iCompression)
		{
		ERR_PRINTF1(_L("Duplicated compression wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iT4Options != iT4Options)
		{
		ERR_PRINTF1(_L("Duplicated T4 options wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iT6Options != iT6Options)
		{
		ERR_PRINTF1(_L("Duplicated T6 options wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iPhotometricInterpretation != iPhotometricInterpretation)
		{
		ERR_PRINTF1(_L("Duplicated photometric interpretation wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iFillOrder != iFillOrder)
		{
		ERR_PRINTF1(_L("Duplicated fill order wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iWidthInTwips != iWidthInTwips)
		{
		ERR_PRINTF1(_L("Duplicated width in twips wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iHeightInTwips != iHeightInTwips)
		{
		ERR_PRINTF1(_L("Duplicated height in twips wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iRowsPerStrip != iRowsPerStrip)
		{
		ERR_PRINTF1(_L("Duplicated rows per strip wrong"));
		SetVerdict(EFail);
		}

	if(duplicateFrameData->iNumStrips != iNumStrips)
		{
		ERR_PRINTF1(_L("Duplicated number of strips wrong"));
		SetVerdict(EFail);
		}

	// Make sure we are comparing different pointers
	if(iStripInfo == duplicateFrameData->iStripInfo)
		{
		ERR_PRINTF1(_L("Comparing same pointer"));
		SetVerdict(EFail);
		}

	TInt comparison;
	comparison = iStripInfo->Compare(duplicateFrameData->iStripInfo->Des());
	if(comparison)
		{
		ERR_PRINTF1(_L("Duplicated strip info wrong"));
		SetVerdict(EFail);
		}
	}

//RCImageTypeDescriptionTest
RCImageTypeDescriptionTest* RCImageTypeDescriptionTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageTypeDescriptionTest(aSuite, aAllocTest);
	}

RCImageTypeDescriptionTest::RCImageTypeDescriptionTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4016-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4017-HP");		
	}

void RCImageTypeDescriptionTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CImageTypeDescription Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CImageTypeDescription Alloc Test >>>"));

	iDescription = _L("Hallo World"); //Use any buffer
	iImageType.iUid = 1234; //Use any UID
	iSubType.iUid = 5678;   //Use any UID

	iImageTypeDescription = NULL;
	}

void RCImageTypeDescriptionTest::TestStepL()
	{
	iImageTypeDescription = CImageTypeDescription::NewL(iDescription, iImageType, iSubType);
	}

void RCImageTypeDescriptionTest::CheckResultL()
	{
	const HBufC* description = STATIC_CAST(const HBufC*, &iImageTypeDescription->Description());

	if(description->Ptr() == iDescription.Ptr())
		{
		ERR_PRINTF1(_L("Comparing same Pointer"));
		SetVerdict(EFail);
		}

	TInt comparison = description->Compare(iDescription);
	if(comparison)
		{
		ERR_PRINTF1(_L("Description wrong"));
		SetVerdict(EFail);
		}

	TUid imageType = iImageTypeDescription->ImageType();
	if(imageType != iImageType)
		{
		ERR_PRINTF1(_L("Image type wrong"));
		SetVerdict(EFail);
		}

	TUid subType = iImageTypeDescription->SubType();
	if(subType != iSubType)
		{
		ERR_PRINTF1(_L("Sub type wrong"));
		SetVerdict(EFail);
		}

	}

void RCImageTypeDescriptionTest::Cleanup()
	{
	delete iImageTypeDescription; iImageTypeDescription = NULL;
	}

//RCFileExtensionMIMETypeTest
RCFileExtensionMIMETypeTest* RCFileExtensionMIMETypeTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCFileExtensionMIMETypeTest(aSuite, aAllocTest);
	}

RCFileExtensionMIMETypeTest::RCFileExtensionMIMETypeTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4018-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4019-HP");		
	}

void RCFileExtensionMIMETypeTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CFileExtensionMIMEType Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CFileExtensionMIMEType Alloc Test >>>"));

	iFileExtension16 = _L("jpg");
	iFileExtension8.Copy(iFileExtension16);
	iMIMEType.Copy(_L("image/jpeg"));
	iDisplayName = _L("TEST");
	iImageType.iUid = 1;
	iImageSubType.iUid = 2;
	iImplementationUid.iUid = 3;

	iFileExtensionMIMEType = NULL;
	}

void RCFileExtensionMIMETypeTest::TestStepL()
	{
	iFileExtensionMIMEType = CFileExtensionMIMEType::NewL(iFileExtension8, iMIMEType, iDisplayName, iImageType, iImageSubType, iImplementationUid);
	}

void RCFileExtensionMIMETypeTest::CheckResultL()
	{

	//Check file extension
	Compare(iFileExtension16,iFileExtensionMIMEType->FileExtension(),_L("File extension wrong"));
	//Check mime type
	Compare(iMIMEType,iFileExtensionMIMEType->MIMEType(),_L("Mime type wrong"));
	//Check display name
	Compare(iDisplayName,iFileExtensionMIMEType->DisplayName(),_L("Display name wrong"));
	//Check image type
	Compare(iImageType,iFileExtensionMIMEType->ImageType(),_L("Image type wrong"));
	//Check image sub-type
	Compare(iImageSubType,iFileExtensionMIMEType->ImageSubType(),_L("Image sub-type wrong"));
	//Check implementation uid
	Compare(iImplementationUid,iFileExtensionMIMEType->ImplementationUid(),_L("ImplementationUid wrong"));
	}

void RCFileExtensionMIMETypeTest::Cleanup()
	{
	delete iFileExtensionMIMEType; iFileExtensionMIMEType = NULL;
	}

//RCImplementationInformationTypeTest
RCImplementationInformationTypeTest* RCImplementationInformationTypeTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImplementationInformationTypeTest(aSuite, aAllocTest);
	}

RCImplementationInformationTypeTest::RCImplementationInformationTypeTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4020-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4021-HP");		
	}

void RCImplementationInformationTypeTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CImplementationInformationType Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CImplementationInformationType Alloc Test >>>"));

	iImplementationUid.iUid = 1234;
	iVersion = 4;
	iDisplayName = _L("JPEG");
	iDataType.Copy(_L("Test data type"));
	iOpaqueData.Copy(_L("Test opaque data"));
	iDriveUnit = _L("C:");

	iImpInfo = NULL;
	}

void RCImplementationInformationTypeTest::TestStepL()
	{
	iImpInfo = CImplementationInformationType::NewL();
	iImpInfo->SetDataL(iImplementationUid, iVersion, iDisplayName, iDataType, iOpaqueData, iDriveUnit);
	}

void RCImplementationInformationTypeTest::CheckResultL()
	{
	Compare(iImpInfo->ImplementationUid(), iImplementationUid, _L("Implementation uid wrong"));
	Compare(iImpInfo->Version(), iVersion, _L("Version wrong"));
	Compare(iImpInfo->DisplayName(), iDisplayName, _L("Display name wrong"));
	Compare(iImpInfo->DataType(), iDataType, _L("Data type wrong"));
	Compare(iImpInfo->OpaqueData(), iOpaqueData, _L("Opaque data wrong"));
	Compare(iImpInfo->Drive(), iDriveUnit , _L("Drive unit wrong"));
	}

void RCImplementationInformationTypeTest::Cleanup()
	{
	delete iImpInfo; iImpInfo = NULL;
	}

//RCFrameImageDataTest
RCFrameImageDataTest* RCFrameImageDataTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCFrameImageDataTest(aSuite, aAllocTest);
	}

RCFrameImageDataTest::RCFrameImageDataTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4022-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4023-HP");		
	}

void RCFrameImageDataTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CFrameImageData Basic Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CFrameImageData Basic Alloc Test >>>"));

	iFrameImageData = NULL;
	}

void RCFrameImageDataTest::TestStepL()
	{
	iFrameImageData = CFrameImageData::NewL();

	//Test ImageData
	{
	User::LeaveIfError(iFrameImageData->AppendImageData(&iBmpImageData));
	User::LeaveIfError(iFrameImageData->InsertImageData(&iJpegImageData, 0));

	//Count should now be two
	TInt count = iFrameImageData->ImageDataCount();
	Compare(count, 2, _L("Image data count wrong after append/insert"));

	const TImageDataBlock* imageData;
	//JPG should be first
	imageData = iFrameImageData->GetImageData(0);
	ComparePointers(imageData, &iJpegImageData, _L("Inserted image data wrong"));

	//BMP should be second
	imageData = iFrameImageData->GetImageData(1);
	ComparePointers(imageData, &iBmpImageData, _L("Appended image data wrong"));
	
	//Remove the JPG
	iFrameImageData->RemoveImageData(0);

	//Count should now be one
	count = iFrameImageData->ImageDataCount();
	Compare(count, 1, _L("Image data count wrong after delete"));

	//BMP should be left
	imageData = iFrameImageData->GetImageData(0);
	ComparePointers(imageData, &iBmpImageData, _L("Remove image data wrong"));

	//Remove the BMP
	iFrameImageData->RemoveImageData(0);

	//Count should now be none
	count = iFrameImageData->ImageDataCount();
	Compare(count, 0, _L("Image data count wrong after second delete"));
	}

	//Test FrameData
	{
	User::LeaveIfError(iFrameImageData->AppendFrameData(&iGifImageControl));
	User::LeaveIfError(iFrameImageData->InsertFrameData(&iGifImageDescriptor, 0));

	//Count should now be two
	TInt count = iFrameImageData->FrameDataCount();
	Compare(count, 2, _L("Frame data count wrong after append/insert"));

	const TFrameDataBlock* frameData;
	//GIF image descriptor should be first
	frameData = iFrameImageData->GetFrameData(0);
	ComparePointers(frameData, &iGifImageDescriptor, _L("Inserted frame data wrong"));

	//GIF image control should be second
	frameData = iFrameImageData->GetFrameData(1);
	ComparePointers(frameData, &iGifImageControl, _L("Appended frame data wrong"));
	
	//Remove the GIF image descriptor
	iFrameImageData->RemoveFrameData(0);

	//Count should now be one
	count = iFrameImageData->FrameDataCount();
	Compare(count, 1, _L("Frame data count wrong after delete"));

	//GIF image control should be left
	frameData = iFrameImageData->GetFrameData(0);
	ComparePointers(frameData, &iGifImageControl, _L("Remove frame data wrong"));

	//Remove the GIF image control
	iFrameImageData->RemoveFrameData(0);

	//Count should now be none
	count = iFrameImageData->FrameDataCount();
	Compare(count, 0, _L("Frame data count wrong after second delete"));
	}

	//Test cleanup behaviour (__MM_HEAP_MARK / __MM_HEAP_MARKEND RAllocTest::DoTestStep())
	{
	CFrameImageData* frameImageData = CFrameImageData::NewL();
	CleanupStack::PushL(frameImageData);

	TBmpImageData* bmpImageData = new (ELeave) TBmpImageData;
	//Ownership passed to CFrameImageData
	User::LeaveIfError(iFrameImageData->AppendImageData(bmpImageData)); 

	TGifImageControl* gifImageControl = new (ELeave) TGifImageControl;
	//Ownership passed to CFrameImageData
	User::LeaveIfError(iFrameImageData->AppendFrameData(gifImageControl));
	
	CleanupStack::PopAndDestroy(frameImageData);
	}

	}

void RCFrameImageDataTest::CheckResultL()
	{
	}

void RCFrameImageDataTest::Cleanup()
	{
	delete iFrameImageData; iFrameImageData = NULL;
	}

//RCFrameImageDataAppendImageBufferTest
RCFrameImageDataAppendImageBufferTest* RCFrameImageDataAppendImageBufferTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCFrameImageDataAppendImageBufferTest(aSuite, aAllocTest);
	}

RCFrameImageDataAppendImageBufferTest::RCFrameImageDataAppendImageBufferTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4024-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4025-HP");		
	}

void RCFrameImageDataAppendImageBufferTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CFrameImageData::AppendImageBuffer() Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CFrameImageData::AppendImageBuffer() Alloc Test >>>"));

	iFrameImageData = NULL;
	iBuffer.Reset();
	}

void RCFrameImageDataAppendImageBufferTest::TestStepL()
	{
	iFrameImageData = CFrameImageData::NewL();

	TInt cnt;
	for(cnt = 0; cnt < 3; cnt++)
		{
		HBufC8* buffer = HBufC8::NewLC(20);
		User::LeaveIfError(iBuffer.Append(buffer));
		CleanupStack::Pop(); //buffer
		}

	TInt noOfEntries = iBuffer.Count();
	for(cnt = 0; cnt < noOfEntries; cnt++)
		{
		User::LeaveIfError(iFrameImageData->AppendImageBuffer(iBuffer[0]));
		//iFrameImageData owns the data;
		iBuffer.Remove(0);
		}
	}

void RCFrameImageDataAppendImageBufferTest::CheckResultL()
	{
	}

void RCFrameImageDataAppendImageBufferTest::Cleanup()
	{
	iBuffer.ResetAndDestroy();
	delete iFrameImageData; iFrameImageData = NULL;
	}

//RCFrameImageDataAllocLTest
RCFrameImageDataAllocLTest* RCFrameImageDataAllocLTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCFrameImageDataAllocLTest(aSuite, aAllocTest);
	}

RCFrameImageDataAllocLTest::RCFrameImageDataAllocLTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4026-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4027-HP");		
	}

void RCFrameImageDataAllocLTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CFrameImageDataAllocL Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CFrameImageDataAllocL Alloc Test >>>"));

	iFrameImageData = NULL;
	iDuplicate = NULL;
	}

void RCFrameImageDataAllocLTest::TestStepL()
	{
	iFrameImageData = CFrameImageData::NewL();

	TBmpImageData* bmpImageData = new (ELeave) TBmpImageData;
	CleanupDeletePushL(bmpImageData);
	User::LeaveIfError(iFrameImageData->AppendImageData(bmpImageData));
	CleanupStack::Pop(); //bmpImageData;

	TJpegImageData* jpegImageData = new (ELeave) TJpegImageData;
	CleanupDeletePushL(jpegImageData);
	User::LeaveIfError(iFrameImageData->AppendImageData(jpegImageData));
	CleanupStack::Pop(); //jpegImageData

	TGifImageControl* gifImageControl = new (ELeave) TGifImageControl;
	CleanupDeletePushL(gifImageControl);
	User::LeaveIfError(iFrameImageData->AppendFrameData(gifImageControl));
	CleanupStack::Pop(); //gifImageControl

	iDuplicate = iFrameImageData->AllocL();
	}

void RCFrameImageDataAllocLTest::CheckResultL()
	{
	TInt noOfEntries = iFrameImageData->ImageDataCount();
	Compare(noOfEntries, 2, _L("Duplicated image data count wrong"));

	TImageDataBlock* imageData;
	TImageDataBlock* duplicateImageData;

	TInt entry;
	for(entry = 0; entry < noOfEntries; entry++)
		{
		imageData = iFrameImageData->GetImageData(entry);
		duplicateImageData = iDuplicate->GetImageData(entry);
		CompareDiffPointers(imageData, duplicateImageData, _L("Duplicated image data at same memory location"));
		}

	noOfEntries = iFrameImageData->FrameDataCount();
	Compare(noOfEntries, 1, _L("Duplicated frame data count wrong"));

	TFrameDataBlock* frameData;
	TFrameDataBlock* duplicateFrameData;

	for(entry = 0; entry < noOfEntries; entry++)
		{
		frameData = iFrameImageData->GetFrameData(entry);
		duplicateFrameData = iDuplicate->GetFrameData(entry);
		CompareDiffPointers(frameData, duplicateFrameData, _L("Duplicated frame data at same memory location"));
		}
	}

void RCFrameImageDataAllocLTest::Cleanup()
	{
	delete iDuplicate; iDuplicate = NULL;
	delete iFrameImageData; iFrameImageData = NULL;
	}

//RCFrameInfoStringsTest
RCFrameInfoStringsTest* RCFrameInfoStringsTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCFrameInfoStringsTest(aSuite, aAllocTest);
	}

RCFrameInfoStringsTest::RCFrameInfoStringsTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-4028-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-4029-HP");		
	}

void RCFrameInfoStringsTest::SetupL()
	{
	if(!iAllocTest)
		INFO_PRINTF1(_L("<<< CFrameInfoStrings Test >>>"));
	else
		INFO_PRINTF1(_L("<<< CFrameInfoStrings Alloc Test >>>"));

	iData[EDecoder] = _L("Decoder");
	iData[EFormat] = _L("Format");
	iData[EDimensions] = _L("Dimensions");
	iData[EDepth] = _L("Depth");
	iData[EDetails] = _L("Details");

	iFrameInfoStrings = NULL;
	}

void RCFrameInfoStringsTest::TestStepL()
	{
	iFrameInfoStrings = CFrameInfoStrings::NewL();

	iFrameInfoStrings->SetDecoderL(iData[EDecoder]);
	iFrameInfoStrings->SetFormatL(iData[EFormat]);
	iFrameInfoStrings->SetDimensionsL(iData[EDimensions]);
	iFrameInfoStrings->SetDepthL(iData[EDepth]);
	iFrameInfoStrings->SetDetailsL(iData[EDetails]);
	}

void RCFrameInfoStringsTest::CheckResultL()
	{

	TInt count = iFrameInfoStrings->Count();
	Compare(count, 5, _L("Number of frame info strings wrong"));

	TPtrC string;
	for(TInt cnt = 0; cnt < count; cnt++)
		{
		string.Set(iFrameInfoStrings->String(cnt));
		Compare(string, iData[cnt], _L("Returned string wrong"));
		}

	string.Set(iFrameInfoStrings->Decoder());
	Compare(string, iData[EDecoder], _L("Returned decoder wrong"));

	string.Set(iFrameInfoStrings->Format());
	Compare(string, iData[EFormat], _L("Returned format wrong"));

	string.Set(iFrameInfoStrings->Dimensions());
	Compare(string, iData[EDimensions], _L("Returned dimensions wrong"));

	string.Set(iFrameInfoStrings->Depth());
	Compare(string, iData[EDepth], _L("Returned depth wrong"));

	string.Set(iFrameInfoStrings->Details());
	Compare(string, iData[EDetails], _L("Returned details wrong"));

	}

void RCFrameInfoStringsTest::Cleanup()
	{
	delete iFrameInfoStrings; iFrameInfoStrings = NULL;
	}

//RTFrameInfoTest
RTFrameInfoTest* RTFrameInfoTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTFrameInfoTest(aSuite);
	}

RTFrameInfoTest::RTFrameInfoTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-4030-HP");
	}

TVerdict RTFrameInfoTest::OpenL()
	{
	INFO_PRINTF1(_L("<<< TFrameInfo Test >>>"));

	iFrameInfoState = TFrameInfo::EFrameInfoProcessingFrameHeader;
	iFrameDataOffset = 123;

	return iTestStepResult;
	}

TVerdict RTFrameInfoTest::DoTestStepL()
	{
	TFrameInfo* frameInfo = new (ELeave) TFrameInfo;
	CleanupDeletePushL(frameInfo);

	frameInfo->SetCurrentFrameState(iFrameInfoState);
	frameInfo->SetFrameDataOffset(iFrameDataOffset);

	TFrameInfo::TFrameInfoState frameInfoState;
	frameInfoState = frameInfo->CurrentFrameState();
	Compare(frameInfoState, iFrameInfoState, _L("Current frame state wrong"));

	TInt frameDataOffset;
	frameDataOffset = frameInfo->FrameDataOffset();
	Compare(frameDataOffset, iFrameDataOffset, _L("Frame data offset wrong"));

	CleanupStack::PopAndDestroy(frameInfo);

	return iTestStepResult;
	}

//CTestDecoderPlugin
CTestDecoderPlugin* CTestDecoderPlugin::NewL()
	{
	return new (ELeave) CTestDecoderPlugin;
	}

CTestDecoderPlugin::CTestDecoderPlugin()
	{
	}

void CTestDecoderPlugin::ImageType(int,class TUid& aImageType,class TUid &aImageSubType) const
	{
	aImageType = KImageTypeGIFUid;
	aImageSubType = KNullUid;
	}

void CTestDecoderPlugin::ScanDataL()
	{
	iScanDataLCount++;
	User::Leave(iScanDataLLeave);
	}

CFrameInfoStrings* CTestDecoderPlugin::FrameInfoStringsL(class RFs& aFs,int)
	{
	iFs = &aFs;
	iFrameInfoStringsLCalled = ETrue;
	return NULL;
	}

void CTestDecoderPlugin::ReadFrameHeadersL()
	{
	iReadFrameHeadersLCalled = ETrue;
	}

//CTestDecodeConstruct
CTestDecodeConstruct* CTestDecodeConstruct::NewL()
	{
	return new (ELeave) CTestDecodeConstruct;
	}

CTestDecodeConstruct::CTestDecodeConstruct()
	{
	}

CImageDecoderPlugin* CTestDecodeConstruct::NewPluginL() const
	{
	iNewPluginLCount++;
	return CTestDecoderPlugin::NewL();
	}

void CTestReadCodec::InitFrameL(TFrameInfo&, CFrameImageData&, TBool, CFbsBitmap&, CFbsBitmap*)
	{
	}

TFrameState CTestReadCodec::ProcessFrameL(TBufPtr8&)
	{
	return EFrameIncomplete;
	}

//RCImageDecoderPrivTest
RCImageDecoderPrivTest* RCImageDecoderPrivTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivTest::RCImageDecoderPrivTest(CTestSuite* aSuite, TBool aAllocTest) :
	RCImageDecoderPrivTestBase(aAllocTest)
	{
	iSuite = aSuite;
	if(!iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5001-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5002-HP");
	}

//Override RCImageDecoderPrivBase::SetupL() to test the construction
void RCImageDecoderPrivTest::SetupL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	iImageDecoderPriv = NULL;
	iConstruct = NULL;

	if(!iAllocTest)
		INFO_PRINTF1(_L("<<<   CImageDecoderPriv Construction Test   >>>"));
	else
		INFO_PRINTF1(_L("<<<   CImageDecoderPriv Construction Alloc Test   >>>"));
	}

void RCImageDecoderPrivTest::TestStepL()
	{
	iConstruct = CTestDecodeConstruct::NewL();

	iImageDecoderPriv = CImageDecoderPriv::NewL(iConstruct, this);

	iImageDecoderPriv->CreatePluginL();	
	}

void RCImageDecoderPrivTest::CheckResultL()
	{
	Compare(iImageDecoderPriv->IsAdded(), ETrue, _L("AO not added to scheduler"));
	ComparePointers(iConstruct, Construct(), _L("iConstruct not correctly set"));
	if(!iImageDecoderPriv->Plugin())
		{
		SetFail(_L("iPlugin not created"));
		return;
		}

	Compare(iConstruct->iNewPluginLCount, 1, _L("CImageDecodeConstruct::NewPluginL() not used to create iPlugin"));
	ComparePointers(PluginProperties(), iImageDecoderPriv, _L("Plugin properties not set correctly"));
	}

void RCImageDecoderPrivTest::Cleanup()
	{
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	}

//Override RCImageDecoderPrivBase::Close() to test the construction
void RCImageDecoderPrivTest::Close()
	{
	delete iScheduler; iScheduler = NULL;
	RAllocTest::Close();
	}

//RCImageDecoderPrivBase
RCImageDecoderPrivTestBase::RCImageDecoderPrivTestBase(TBool aAllocTest):
	RAllocTest(aAllocTest)
	{
	}

CImageDecodeConstruct* RCImageDecoderPrivTestBase::Construct()
	{
	return iImageDecoderPriv->iConstruct;
	}

CImageDecoderPriv* RCImageDecoderPrivTestBase::PluginProperties()
	{
	return iImageDecoderPriv->iPlugin->iProperties;
	}

RFs* RCImageDecoderPrivTestBase::FileSession()
	{
	return iImageDecoderPriv->iFs;
	}

CImageDecoder::TOptions RCImageDecoderPrivTestBase::Options()
	{
	return iImageDecoderPriv->iOptions;
	}

TImageParameterData* RCImageDecoderPrivTestBase::ImageParameterData()
	{
	return &iImageDecoderPriv->iImageParameterData;
	}

CData* RCImageDecoderPrivTestBase::Data()
	{
	return iImageDecoderPriv->iData;
	}

void RCImageDecoderPrivTestBase::SetImageReadCodec(CImageReadCodec* aReadCodec)
	{
	iImageDecoderPriv->Plugin()->SetImageReadCodec(aReadCodec);
	}

TBool RCImageDecoderPrivTestBase::ProcessingFrameInfo()
	{
	return iImageDecoderPriv->iProcessingFrameInfo;
	}

TInt RCImageDecoderPrivTestBase::CurrentFrame()
	{
	return iImageDecoderPriv->iCurrentFrame;
	}

HBufC8* RCImageDecoderPrivTestBase::ReadBuffer()
	{
	return iImageDecoderPriv->iReadBuffer;
	}

void RCImageDecoderPrivTestBase::SetFileSession(RFs& aFs)
	{
	iImageDecoderPriv->iFs = &aFs;
	}

void RCImageDecoderPrivTestBase::SetupL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );

	iConstruct = CTestDecodeConstruct::NewL();

	iImageDecoderPriv = CImageDecoderPriv::NewL(iConstruct, this);

	iImageDecoderPriv->CreatePluginL();
	}

void RCImageDecoderPrivTestBase::Cleanup()
	{
	}

void RCImageDecoderPrivTestBase::Close()
	{
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	delete iScheduler; iScheduler = NULL;
	RAllocTest::Close();
	}

//RCImageDecoderPrivSetFileLTest
RCImageDecoderPrivSetFileLTest* RCImageDecoderPrivSetFileLTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivSetFileLTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivSetFileLTest::RCImageDecoderPrivSetFileLTest(CTestSuite* aSuite, TBool aAllocTest):
	RCImageDecoderPrivTestBase(aAllocTest)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5003-HP");
	}

void RCImageDecoderPrivSetFileLTest::SetupL()
	{
	INFO_PRINTF1(_L("<<<   CImageDecoderPriv::SetFileL() Test   >>>"));

	RCImageDecoderPrivTestBase::SetupL();

	User::LeaveIfError(iFileSession.Connect());

	iFileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	iFileName.Append(KGifFile);
	iOptions = static_cast<CImageDecoder::TOptions>
		(CImageDecoder::EOptionNoDither |CImageDecoder::EOptionAllowZeroFrameOpen);
	}

void RCImageDecoderPrivSetFileLTest::TestStepL()
	{
	TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(_L("FileThatDoesNotExist"));
	//Test with a file that does not exist this should leave with KErrNotFound
	TRAPD(error,iImageDecoderPriv->SetFileL(iFileSession, fileName));
	Compare(error, KErrNotFound, _L("SetFileL() did not leave with KErrNotFound on missing file"));
	iImageDecoderPriv->SetFileL(iFileSession, iFileName, iOptions);
	}

void RCImageDecoderPrivSetFileLTest::CheckResultL()
	{
	ComparePointers(&iFileSession, FileSession(), _L("iFs set incorrect"));
	Compare(iOptions, Options(), _L("iOptions set incorrect"));
	Compare(ImageParameterData()->IsFilename(), ETrue, _L("IsFilename() set incorrect"));
	ComparePointers(&iFileName, ImageParameterData()->SourceFilenamePtr(), _L("SourceFilenamePtr() set incorrect"));

	TBuf8<KDefaultBufferSize> data;
	Compare(Data()->Read(data), KErrNone, _L("Could not read from file"));

	}

//RCImageDecoderPrivSetDataLTest
RCImageDecoderPrivSetDataLTest* RCImageDecoderPrivSetDataLTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivSetDataLTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivSetDataLTest::RCImageDecoderPrivSetDataLTest(CTestSuite* aSuite, TBool aAllocTest):
	RCImageDecoderPrivTestBase(aAllocTest)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5004-HP");
	}

void RCImageDecoderPrivSetDataLTest::SetupL()
	{
	INFO_PRINTF1(_L("<<<   CImageDecoderPriv::SetDataL() Test   >>>"));

	RCImageDecoderPrivTestBase::SetupL();

	User::LeaveIfError(iFileSession.Connect());

	iOptions = static_cast<CImageDecoder::TOptions>
		(CImageDecoder::EOptionNoDither |
		CImageDecoder::EOptionAllowZeroFrameOpen);
	}

void RCImageDecoderPrivSetDataLTest::TestStepL()
	{
	iImageDecoderPriv->SetDataL(iFileSession, iData, iOptions);
	}

void RCImageDecoderPrivSetDataLTest::CheckResultL()
	{
	ComparePointers(&iFileSession, FileSession(), _L("iFs set incorrect"));
	Compare(iOptions, Options(), _L("iOptions set incorrect"));
	Compare(ImageParameterData()->IsFilename(), EFalse, _L("IsFilename() set incorrect"));
	ComparePointers(&iData, ImageParameterData()->SourceDataPtr(), _L("SourceDataPtr set incorrect"));
	}

//RCImageDecoderPrivHandleNewlyOpenedImageLTest
RCImageDecoderPrivHandleNewlyOpenedImageLTest* RCImageDecoderPrivHandleNewlyOpenedImageLTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivHandleNewlyOpenedImageLTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivHandleNewlyOpenedImageLTest::RCImageDecoderPrivHandleNewlyOpenedImageLTest(CTestSuite* aSuite, TBool aAllocTest):
	RCImageDecoderPrivTestBase(aAllocTest)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5005-HP");
	}

void RCImageDecoderPrivHandleNewlyOpenedImageLTest::SetupL()
	{
	INFO_PRINTF1(_L("<<<   CImageDecoderPriv::HandleNewlyOpenedImageL() Test   >>>"));

	RCImageDecoderPrivTestBase::SetupL();
	}

void RCImageDecoderPrivHandleNewlyOpenedImageLTest::TestStepL()
	{
	CTestDecoderPlugin* decoderPlugin = NULL;
	decoderPlugin = REINTERPRET_CAST(CTestDecoderPlugin*, iImageDecoderPriv->Plugin());

	//Should return normally
	decoderPlugin->iScanDataLLeave = KErrNone;
	TRAPD(error, iImageDecoderPriv->HandleNewlyOpenedImageL());
	Compare(decoderPlugin->iScanDataLCount, 1, _L("CImageDecoderPlugin::ScanDataL() not called for KErrNone test"));
	Compare(error, KErrNone, _L("KErrNone leave incorrect"));

	//Should return normally
	decoderPlugin->iScanDataLLeave = KErrUnderflow;
	TRAP(error, iImageDecoderPriv->HandleNewlyOpenedImageL());
	Compare(decoderPlugin->iScanDataLCount, 2, _L("Second CImageDecoderPlugin::ScanDataL() not called for KErrUnderflow test"));
	Compare(error, KErrUnderflow, _L("KErrUnderflow leave incorrect"));

	//Should leave with KErrCorrupt
	decoderPlugin->iScanDataLLeave = KErrCorrupt;
	TRAP(error, iImageDecoderPriv->HandleNewlyOpenedImageL());
	Compare(decoderPlugin->iScanDataLCount, 3, _L("CImageDecoderPlugin::ScanDataL() not called for KErrCorrupt test"));
	Compare(error, KErrCorrupt, _L("KErrCorrupt leave incorrect"));
	}

void RCImageDecoderPrivHandleNewlyOpenedImageLTest::CheckResultL()
	{
	}

//RCImageDecoderPrivContinueProcessingHeaderLTest
RCImageDecoderPrivContinueProcessingHeaderLTest* RCImageDecoderPrivContinueProcessingHeaderLTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivContinueProcessingHeaderLTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivContinueProcessingHeaderLTest::RCImageDecoderPrivContinueProcessingHeaderLTest(CTestSuite* aSuite, TBool aAllocTest):
	RCImageDecoderPrivTestBase(aAllocTest)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5006-HP");
	}

void RCImageDecoderPrivContinueProcessingHeaderLTest::SetupL()
	{
	INFO_PRINTF1(_L("<<<   CImageDecoderPriv::ContinueProcessingHeaderL() Test   >>>"));

	RCImageDecoderPrivTestBase::SetupL();
	}

void RCImageDecoderPrivContinueProcessingHeaderLTest::TestStepL()
	{
	CTestDecoderPlugin* decoderPlugin = NULL;
	decoderPlugin = REINTERPRET_CAST(CTestDecoderPlugin*, iImageDecoderPriv->Plugin());

	//Should return normally
	decoderPlugin->iScanDataLLeave = KErrNone;
	TRAPD(error, iImageDecoderPriv->ContinueProcessingHeaderL());
	Compare(CurrentFrame(), 0 , _L("iCurrentFrame not 0"));
	Compare(ProcessingFrameInfo(), EFalse, _L("iProcessingFrameInfo not EFalse"));
	Compare(iImageDecoderPriv->Position(), 0, _L("iPosition not correct"));
	Compare(decoderPlugin->iScanDataLCount, 1, _L("CImageDecoderPlugin::ScanDataL() not called"));
	Compare(error, KErrNone, _L("KErrNone leave incorrect"));

	//Should return normally
	decoderPlugin->iScanDataLLeave = KErrUnderflow;
	TRAP(error, iImageDecoderPriv->ContinueProcessingHeaderL());
	Compare(error, KErrNone, _L("KErrUnderflow leave incorrect"));

	//Should leave with KErrCorrupt
	decoderPlugin->iScanDataLLeave = KErrCorrupt;
	TRAP(error, iImageDecoderPriv->ContinueProcessingHeaderL());
	Compare(error, KErrCorrupt, _L("KErrCorrupt leave incorrect"));

	const TInt currentDataOffset = 123;
	TFrameInfo::TFrameInfoState frameInfoState = TFrameInfo::EFrameInfoUninitialised;
	CTestReadCodec* imageReadCodec = new (ELeave) CTestReadCodec;
	SetImageReadCodec(imageReadCodec);

	TFrameInfo* newFrameInfo = new(ELeave) TFrameInfo;
	CleanupStack::PushL(newFrameInfo); // push in case insert fails
	newFrameInfo->SetCurrentDataOffset(currentDataOffset);
	newFrameInfo->SetCurrentFrameState(frameInfoState);
	User::LeaveIfError(iImageDecoderPriv->iFrameInfo.Insert(newFrameInfo, 0));
	CleanupStack::Pop(newFrameInfo); 
	
	TRAP(error, iImageDecoderPriv->ContinueProcessingHeaderL());
	Compare(CurrentFrame(), 1, _L("iCurrentFrame not 1"));
	Compare(ProcessingFrameInfo(), EFalse, _L("iProcessingFrameInfo not ETrue"));
	Compare(iImageDecoderPriv->Position(), currentDataOffset, _L("iPosition not correct"));
	Compare(decoderPlugin->iReadFrameHeadersLCalled, ETrue, _L("CImageDecoderPlugin::ReadFrameHeadersL() not called"));
	Compare(error, KErrNone, _L("KErrNone leave incorrect"));

	iImageDecoderPriv->iFrameInfo[0]->SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingFrame);
	TRAP(error, iImageDecoderPriv->ContinueProcessingHeaderL());
	Compare(CurrentFrame(), 0 , _L("iCurrentFrame not 0"));
	Compare(ProcessingFrameInfo(), ETrue, _L("iProcessingFrameInfo not ETrue"));
	Compare(error, KErrNone, _L("KErrNone leave incorrect"));

	}

void RCImageDecoderPrivContinueProcessingHeaderLTest::CheckResultL()
	{
	}

void RCImageDecoderPrivContinueProcessingHeaderLTest::Cleanup()
	{
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	}


//RCImageDecoderPrivDecodeTest
RCImageDecoderPrivDecodeTest* RCImageDecoderPrivDecodeTest::NewL(CTestSuite* aSuite, TBool aFileTest, TBool aMaskDecode, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivDecodeTest(aSuite, aFileTest, aMaskDecode, aAllocTest);
	}

RCImageDecoderPrivDecodeTest::RCImageDecoderPrivDecodeTest(CTestSuite* aSuite, TBool aFileTest, TBool aMaskDecode, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iFileTest(aFileTest),
	iMaskDecode(aMaskDecode),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	// Increase the heap size for this test
	iHeapSize = 0x20000;
	if(iFileTest)
		{
		//Use SetFileL()
		if(!iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5100-HP");
		if(!iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5101-HP");
		if(iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5102-HP");
		if(iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5103-HP");
		}
	else
		{
		//Use SetDataL()
		if(!iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5104-HP");
		if(!iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5105-HP");
		if(iMaskDecode && !iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5106-HP");
		if(iMaskDecode && iAllocTest)		
			iTestStepName = _L("MM-ICL-FRM-U-5107-HP");
		}
	}

void RCImageDecoderPrivDecodeTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iConstruct = NULL;

	if(iFileTest)
		{
		//Use SetFileL()
		if(!iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetFileL() Decode Test   >>>"));
		if(!iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetFileL() Decode Alloc Test   >>>"));
		if(iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetFileL() Mask Decode Test   >>>"));
		if(iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetFileL() Mask Decode Alloc Test   >>>"));

		User::LeaveIfError(iSourceFile.Temp(iFileSession, _L("C:\\"), iSourceFilename, EFileWrite|EFileShareAny));
		}
	else
		{
		//Use SetDataL()
		if(!iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetDataL() Decode Test   >>>"));
		if(!iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetDataL() Decode Alloc Test   >>>"));
		if(iMaskDecode && !iAllocTest)
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetDataL() Mask Decode Test   >>>"));
		if(iMaskDecode && iAllocTest)		
			INFO_PRINTF1(_L("<<<   CImageDecoderPriv SetDataL() Mask Decode Alloc Test   >>>"));

		}


	RFile file;
	if(iMaskDecode)
		{
	    TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
		fileName.Append(KGifFile);
		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
		}
	else
		{
	    TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
		fileName.Append(KTiffFile);
		User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
		}
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);
	iSourceLength = iSourceData->Length();

	TFileName fileName;
	if(iMaskDecode)
		{
		TFileName refFileName(KGifFileRef);
		GetTestFileNameL(fileName, refFileName);
		}
	else
		{
		TFileName refFileName(KTiffFileRef);
		GetTestFileNameL(fileName, refFileName);
		}

	iRefBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iRefBitmap->Load(fileName));

	iTestBitmap = NULL;
	iTestBitmapMask = NULL;
	iImageDecoderPriv = NULL;
	iOptions = CImageDecoder::EOptionAllowZeroFrameOpen;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageDecoderPrivDecodeTest::TestStepL()
	{
	iSourceDataPtr.SetLength(0);

	if(iImageDecoderPriv == NULL)
		{
		TUid imageTypeUid;
		TUid implementationUid;
		if(iMaskDecode)
			{
			imageTypeUid = KImageTypeGIFUid;
			implementationUid.iUid = KGIFDecoderImplementationUidValue;
			}
		else
			{
			imageTypeUid = KImageTypeTIFFUid;
			implementationUid.iUid = KTIFFDecoderImplementationUidValue;
			}

		//Use test class which is a friend to CImageDecodeConstruct 
		RCImageDecodeConstructTest* decodeConstruct = RCImageDecodeConstructTest::NewL(iSuite, EFalse);
		CleanupStack::PushL(decodeConstruct);
		iConstruct = decodeConstruct->CreatePluginL(imageTypeUid, implementationUid);
		CleanupStack::PopAndDestroy(decodeConstruct);

		iImageDecoderPriv = CImageDecoderPriv::NewL(iConstruct, this);

		if(iFileTest)
			iImageDecoderPriv->SetFileL(iFileSession, iSourceFilename, iOptions);
		else
			iImageDecoderPriv->SetDataL(iFileSession, *iSourceData, iOptions);

		iImageDecoderPriv->CreatePluginL();
		iImageDecoderPriv->HandleNewlyOpenedImageL();
		}

	while(!iImageDecoderPriv->IsImageHeaderProcessingComplete())
		{
		iImageDecoderPriv->ContinueProcessingHeaderL();
		IncrementBufferSize();
		}

	const TFrameInfo frameInfo = iImageDecoderPriv->FrameInfo(0);

	if(iMaskDecode)
		{
		if(!(frameInfo.iFlags & TFrameInfo::ETransparencyPossible))
			{
			SetFail(_L("Image does not support masks"));
			iMaskDecode = EFalse;
			}
		}
	
	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iTestBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	if(iMaskDecode)
		{
		iTestBitmapMask = new (ELeave) CFbsBitmap;
		User::LeaveIfError(iTestBitmapMask->Create(frameInfo.iOverallSizeInPixels, EGray2));
		}

	iListener->InitialiseActiveListener();
	if(iMaskDecode)
		iImageDecoderPriv->Convert(iMyThread, &iListener->iStatus, *iTestBitmap, *iTestBitmapMask, 0);
	else
		iImageDecoderPriv->Convert(iMyThread, &iListener->iStatus, *iTestBitmap, 0);
	CActiveScheduler::Start();

	TInt status;
	status = iListener->iStatus.Int();
	if((status != KErrNone) && (status != KErrUnderflow))
		User::Leave(status);

	while(status == KErrUnderflow)
		{
		IncrementBufferSize();
		iListener->InitialiseActiveListener();
		iImageDecoderPriv->ContinueConvert(iMyThread, &iListener->iStatus);
		CActiveScheduler::Start();
		status = iListener->iStatus.Int();
		if((status != KErrNone) && (status != KErrUnderflow))
			User::Leave(status);
		}
	}

void RCImageDecoderPrivDecodeTest::CheckResultL()
	{
	Compare(*iTestBitmap, *iRefBitmap, _L("Bitmaps are not the same"));
	//The image and mask look the same
	if(iMaskDecode)
		Compare(*iTestBitmapMask, *iRefBitmap, _L("Mask bitmaps are not the same"));
	}

void RCImageDecoderPrivDecodeTest::Cleanup()
	{
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	if(iTestBitmap)
		{delete iTestBitmap; iTestBitmap = NULL;}
	if(iTestBitmapMask)
		{delete iTestBitmapMask; iTestBitmapMask = NULL;}
	if(iFileTest)
		iSourceFile.SetSize(0);
	}

void RCImageDecoderPrivDecodeTest::Close()
	{
	delete iRefBitmap; iRefBitmap = NULL;
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iSourceFile.Close();
	if(iFileTest)
		if(iFileSession.Delete(iSourceFilename) != KErrNone)
			User::Panic(_L("FileDeleteFail"),0);
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

void RCImageDecoderPrivDecodeTest::IncrementBufferSize()
	{
	TInt position(iSourceDataPtr.Length());

	if (position < iSourceLength)
		{
		iSourceDataPtr.SetLength(position + 1);

		if(iFileTest)
			{
			iSourceFile.Write(iSourceDataPtr.Mid(position, 1));
			}
		}
	}

//RCImageDecoderPrivReadDataLTest
RCImageDecoderPrivReadDataLTest* RCImageDecoderPrivReadDataLTest::NewL(CTestSuite* aSuite, TBool aFileTest, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivReadDataLTest(aSuite, aFileTest, aAllocTest);
	}

RCImageDecoderPrivReadDataLTest::RCImageDecoderPrivReadDataLTest(CTestSuite* aSuite, TBool aFileTest, TBool aAllocTest):
	RCImageDecoderPrivTestBase(aAllocTest),
	iFileTest(aFileTest)
	{
	iSuite = aSuite;
	if(iFileTest)
		{
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5007-HP");
		else
			iTestStepName = _L("MM-ICL-FRM-U-5008-HP");
		}
	else
		{
		if(!iAllocTest)
			iTestStepName = _L("MM-ICL-FRM-U-5009-HP");		
		else
			iTestStepName = _L("MM-ICL-FRM-U-5010-HP");		
		}
	}

void RCImageDecoderPrivReadDataLTest::SetupL()
	{
	if(iFileTest)
		{
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<< CImageDecoderPriv::ReadDataL() File Test >>>"));
		else
			INFO_PRINTF1(_L("<<< CImageDecoderPriv::ReadDataL() File Alloc Test >>>"));
		}
	else
		{
		if(!iAllocTest)
			INFO_PRINTF1(_L("<<< CImageDecoderPriv::ReadDataL() Descriptor Test >>>"));
		else
			INFO_PRINTF1(_L("<<< CImageDecoderPriv::ReadDataL() Descriptor Alloc Test >>>"));
		}

	RCImageDecoderPrivTestBase::SetupL();

	User::LeaveIfError(iFileSession.Connect());
	
	RFile file;
	TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(KGifFile);
	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Size(iDataSize));
	iDataBuffer = HBufC8::NewL(iDataSize);
	TPtr8 dataPtr(iDataBuffer->Des());
	User::LeaveIfError(file.Read(dataPtr));
	CleanupStack::PopAndDestroy(&file);

	iStartPosition = iDataSize / 2;
	}

void RCImageDecoderPrivReadDataLTest::TestStepL()
	{
	// Don't use member variable iImageDecoderPriv, as SetFileL allocates memory, which isn't
	// deallocated until the TestStep destructor
	CImageDecoderPriv* imageDecoderPriv = CImageDecoderPriv::NewL(iConstruct, this);
	CleanupStack::PushL(imageDecoderPriv);
	if(iFileTest)
	{
		TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
		fileName.Append(KGifFile);
		imageDecoderPriv->SetFileL(iFileSession, fileName);
	}
	else
		imageDecoderPriv->SetDataL(iFileSession, *iDataBuffer);

	TPtrC8 buffer;
	TPtrC8 dataPtr;
	//Start reading past EOF, this should return normally
	imageDecoderPriv->ReadDataL(iDataSize + 1, buffer, 10);
	Compare(buffer.Length(), 0, _L("Reading past EOF length should be 0"));

	//Start reading in middle of file and continue past EOF
	TInt dataLeft = iDataSize - iStartPosition;
	imageDecoderPriv->ReadDataL(iStartPosition, buffer, iDataSize);
	dataPtr.Set(iDataBuffer->Mid(iStartPosition));
	if(iFileTest)
		Compare(buffer, dataPtr, _L("Read data not correct"));
	else
		{
		ComparePointers(buffer.Ptr(), dataPtr.Ptr(), _L("Pointers not correct"));
		Compare(buffer.Length(), dataLeft, _L("Data length wrong"));
		}


	//Read file from start
	imageDecoderPriv->ReadDataL(0, buffer, iDataSize);
	dataPtr.Set(*iDataBuffer);
	if(iFileTest)
		Compare(buffer, dataPtr, _L("Read data not correct"));
	else
		{
		ComparePointers(buffer.Ptr(), dataPtr.Ptr(), _L("Pointers not correct"));
		Compare(buffer.Length(), iDataSize, _L("Data length wrong"));
		}
	imageDecoderPriv->Cleanup();
	CleanupStack::PopAndDestroy(imageDecoderPriv);
	}

void RCImageDecoderPrivReadDataLTest::CheckResultL()
	{
	
	}

void RCImageDecoderPrivReadDataLTest::Cleanup()
	{
	iImageDecoderPriv->Cleanup();
	}

void RCImageDecoderPrivReadDataLTest::Close()
	{
	delete iDataBuffer; iDataBuffer = NULL;
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	iFileSession.Close();
	RCImageDecoderPrivTestBase::Close();
	}

//RCImageDecoderPrivCustomTest
RCImageDecoderPrivCustomTest* RCImageDecoderPrivCustomTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivCustomTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivCustomTest::RCImageDecoderPrivCustomTest(CTestSuite* aSuite, TBool aAllocTest):
	RAllocTest(aAllocTest),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	ASSERT(!aAllocTest); // we're using this structure, but don't use it
	iTestStepName = _L("MM-ICL-FRM-U-5110-HP");
	}

void RCImageDecoderPrivCustomTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );	

	INFO_PRINTF1(_L("<<<   CImageDecoderPriv Custom Test   >>>"));

	RFile file;
	TFileName fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(KGifFile);
	User::LeaveIfError(file.Open(iFileSession, fileName, EFileRead|EFileShareAny));
	CleanupClosePushL(file);

	TInt size;
	User::LeaveIfError(file.Size(size));
	iSourceData = HBufC8::NewL(size);
	iSourceDataPtr.Set(iSourceData->Des());
	User::LeaveIfError(file.Read(iSourceDataPtr));
	CleanupStack::PopAndDestroy(&file);
	iSourceLength = iSourceData->Length();

	TFileName refFileName(KGifFileRef);
	GetTestFileNameL(fileName, refFileName);

	iImageDecoderPriv = NULL;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageDecoderPrivCustomTest::TestStepL()
	{
	iSourceDataPtr.SetLength(0);

	if(iImageDecoderPriv == NULL)
		{
		TUid imageTypeUid;
		TUid implementationUid;
		imageTypeUid = KImageTypeGIFUid;
		implementationUid.iUid = KGIFDecoderImplementationUidValue;

		//Use test class which is a friend to CImageDecodeConstruct 
		RCImageDecodeConstructTest* decodeConstruct = RCImageDecodeConstructTest::NewL(iSuite, EFalse);
		CleanupStack::PushL(decodeConstruct);
		iConstruct = decodeConstruct->CreatePluginL(imageTypeUid, implementationUid);
		CleanupStack::PopAndDestroy(decodeConstruct);

		iImageDecoderPriv = CImageDecoderPriv::NewL(iConstruct, this);

		iImageDecoderPriv->SetDataL(iFileSession, *iSourceData, CImageDecoder::EOptionAllowZeroFrameOpen);

		iImageDecoderPriv->CreatePluginL();
		iImageDecoderPriv->HandleNewlyOpenedImageL();
		}

	TRAPD(error, iImageDecoderPriv->CustomSyncL(0));
	Compare(error, KErrNotSupported, _L("Wrong error value returned from sync call"));

	iListener->InitialiseActiveListener();
	iImageDecoderPriv->CustomAsync(iMyThread, &iListener->iStatus, 10);
	CActiveScheduler::Start();

	TInt status = iListener->iStatus.Int();
	Compare(status, KErrNotSupported, _L("Wrong error value returned from async call"));
	}

void RCImageDecoderPrivCustomTest::CheckResultL()
	{
	}

void RCImageDecoderPrivCustomTest::Cleanup()
	{
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	}

void RCImageDecoderPrivCustomTest::Close()
	{
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

//RCImageDecoderPrivFrameInfoStringsLCTest
RCImageDecoderPrivFrameInfoStringsLCTest* RCImageDecoderPrivFrameInfoStringsLCTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecoderPrivFrameInfoStringsLCTest(aSuite, aAllocTest);
	}

RCImageDecoderPrivFrameInfoStringsLCTest::RCImageDecoderPrivFrameInfoStringsLCTest(CTestSuite* aSuite, TBool aAllocTest):
	RCImageDecoderPrivTestBase(aAllocTest)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5011-HP");
	}

void RCImageDecoderPrivFrameInfoStringsLCTest::SetupL()
	{
	INFO_PRINTF1(_L("<<< CImageDecoderPriv::FrameInfoStringsLC() Test >>>"));

	RCImageDecoderPrivTestBase::SetupL();
	}

void RCImageDecoderPrivFrameInfoStringsLCTest::TestStepL()
	{
	CFrameInfoStrings* frameInfoStrings = NULL;

	//Call without RFs
	frameInfoStrings = iImageDecoderPriv->FrameInfoStringsLC(0);
	CleanupStack::Pop(frameInfoStrings);
	CTestDecoderPlugin* plugin;
	plugin = REINTERPRET_CAST(CTestDecoderPlugin*, iImageDecoderPriv->Plugin());
	Compare(plugin->iFrameInfoStringsLCalled, ETrue, _L("CImageDecoderPlugin::FrameInfoStringsL() not called"));
	CompareDiffPointers(plugin->iFs, NULL, _L("FrameInfoStringsLC did not create a RFs"));

	//Call with RFs
	RFs fileSession;
	SetFileSession(fileSession); 
	frameInfoStrings = iImageDecoderPriv->FrameInfoStringsLC(0);
	CleanupStack::Pop(frameInfoStrings);
	ComparePointers(plugin->iFs, &fileSession, _L("Wrong file session used"));
	}

void RCImageDecoderPrivFrameInfoStringsLCTest::CheckResultL()
	{
	
	}

void RCImageDecoderPrivFrameInfoStringsLCTest::Cleanup()
	{
	}

//RCImageDecodeConstructTest
RCImageDecodeConstructTest* RCImageDecodeConstructTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageDecodeConstructTest(aSuite, aAllocTest);
	}

RCImageDecodeConstructTest::RCImageDecodeConstructTest(CTestSuite* aSuite, TBool aAllocTest) :
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	}

void RCImageDecodeConstructTest::SetupL()
	{
	}

void RCImageDecodeConstructTest::TestStepL()
	{
	}

void RCImageDecodeConstructTest::CheckResultL()
	{
	}

void RCImageDecodeConstructTest::Cleanup()
	{
	}

CImageDecodeConstruct* RCImageDecodeConstructTest::CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid)
	{
	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetBaseType(aImageTypeUid);
	customMatchData->SetSubType(KNullUid);
	customMatchData->SetImplementationType(aImplementationUid);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	resolverParams.SetDataType(packageDes);
	CImageDecodeConstruct* construct = NULL;

	const TUid KImageDecoderInterfaceUid = {0x101F45AD};
	const TUid KImageConvertResolverUid = {0x101F45DC};

	construct = STATIC_CAST(CImageDecodeConstruct*,
					REComSession::CreateImplementationL(KImageDecoderInterfaceUid, 
														_FOFF(CImageDecodeConstruct, iDtorIDKey),
														resolverParams, 
														KImageConvertResolverUid));
	if (construct == NULL)
		{ // We didn't get a match - leave
		User::Leave(KErrNotFound); 
		}
	CleanupStack::PopAndDestroy(2); // package, customMatchData

	return construct;
	}

RCImageEncodeConstructTest* RCImageEncodeConstructTest::NewL(CTestSuite* aSuite, TBool aAllocTest)
	{
	return new (ELeave) RCImageEncodeConstructTest(aSuite, aAllocTest);
	}

RCImageEncodeConstructTest::RCImageEncodeConstructTest(CTestSuite* aSuite, TBool aAllocTest) :
	RAllocTest(aAllocTest)
	{
	iSuite = aSuite;
	}

CImageEncodeConstruct* RCImageEncodeConstructTest::CreatePluginL(TUid aImageTypeUid, TUid aImplementationUid)
	{
	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetBaseType(aImageTypeUid);
	customMatchData->SetSubType(KNullUid);
	customMatchData->SetImplementationType(aImplementationUid);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	resolverParams.SetDataType(packageDes);
	CImageEncodeConstruct* construct = NULL;

	const TUid KImageEncoderInterfaceUid = {0x101F45B4};
	const TUid KImageConvertResolverUid = {0x101F45DC};

	construct = STATIC_CAST(CImageEncodeConstruct*,
					REComSession::CreateImplementationL(KImageEncoderInterfaceUid, 
														_FOFF(CImageEncodeConstruct, iDtorIDKey),
														resolverParams, 
														KImageConvertResolverUid));													
	if (construct == NULL)
		{ // We didn't get a match - leave
		User::Leave(KErrNotFound); 
		}
	CleanupStack::PopAndDestroy(2); // package, customMatchData

	return construct;
	}

void RCImageEncodeConstructTest::SetupL()
	{
	}

void RCImageEncodeConstructTest::TestStepL()
	{
	}

void RCImageEncodeConstructTest::CheckResultL()
	{
	}

void RCImageEncodeConstructTest::Cleanup()
	{
	}

// virtual support functions copied from CImageDecoderStraightRelay

TBool MImageDecoderPrivSupportImp::MustUseBufferWithDescriptor() const
	{
	return EFalse;
	}

void MImageDecoderPrivSupportImp::CopyBufferToDescriptor(TDes8& aBuffer, const TDesC8& aSource, TInt aPosition, TInt aSize)
	{
	ASSERT(EFalse); // should never get here
	aBuffer = aSource.Mid(aPosition, aSize); // but this is the required behaviour if we were to
	}

TBool MImageDecoderPrivSupportImp::AmInThread() const
	{
	return EFalse;
	}

TBool MImageDecoderPrivSupportImp::ShouldAbort() const
	{
	return EFalse;
	}

void MImageDecoderPrivSupportImp::Lock()
	{
	}

void MImageDecoderPrivSupportImp::Unlock()
	{
	}

void MImageDecoderPrivSupportImp::SaveFrameCount(TInt /*aFrameCount*/)
	{
	}

TBool MImageEncoderPrivSupportImp::AmInThread() const
	{
	return EFalse;
	}

TBool MImageEncoderPrivSupportImp::ShouldAbort() const
	{
	return EFalse;
	}


// RBadVerDecoderTestStep
RBadVerDecoderTestStep* RBadVerDecoderTestStep::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RBadVerDecoderTestStep(aSuite);
	}

RBadVerDecoderTestStep::RBadVerDecoderTestStep(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5610-HP");
	}
	
TVerdict RBadVerDecoderTestStep::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	return EPass;
	}

TVerdict RBadVerDecoderTestStep::DoTestStepL()
	{
	INFO_PRINTF1(_L("<<< BadVerDecoder Test >>>"));
	RFs fs;
	User::LeaveIfError(fs.Connect());

	CleanupClosePushL(fs);

	TBuf8<100> tempDes;
	tempDes = _L8("TT100"); // would match if

	CImageDecoder* decoder = NULL;
	CImageDecoder::TOptions options = CImageDecoder::EOptionAllowZeroFrameOpen;
	TUid badUid = {KBadVerTestDecoderImplementationUidValue};
	TRAPD(error, decoder = CImageDecoder::DataNewL(fs, tempDes, options, KNullUid, KNullUid, badUid));

	TVerdict result = EPass;

	if (error!=KErrNotFound)
		{
		INFO_PRINTF2(_L("Expected error -1 got %d"), error);
		delete decoder;
		result = EFail;
		}

	CleanupStack::PopAndDestroy(&fs);

	return result;
	}

void RBadVerDecoderTestStep::Close()
	{
	delete iScheduler; iScheduler = NULL;
	}

// RBadVerEncoderTestStep
RBadVerEncoderTestStep* RBadVerEncoderTestStep::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RBadVerEncoderTestStep(aSuite);
	}

RBadVerEncoderTestStep::RBadVerEncoderTestStep(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5611-HP");
	}

TVerdict RBadVerEncoderTestStep::OpenL()
	{
	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	return EPass;
	}
	
void RBadVerEncoderTestStep::Close()
	{
	delete iScheduler; iScheduler = NULL;
	}

TVerdict RBadVerEncoderTestStep::DoTestStepL()
	{
	INFO_PRINTF1(_L("<<< BadVerEncoder Test >>>"));

	HBufC8* tempDes = NULL;

	CImageEncoder::TOptions options = CImageEncoder::EOptionNone;
	TUid badUid = {KBadVerTestEncoderImplementationUidValue};
	TRAPD(error, CImageEncoder::DataNewL(tempDes, options, KNullUid, KNullUid, badUid));	

	TVerdict result = EPass;

	if (error!=KErrNotFound)
		{
		INFO_PRINTF2(_L("Expected error -1 got %d"), error);
		delete tempDes; // even if descriptor created internally, should still be owned by encoder - but you never know
		result = EFail;
		}

	return result;
	}


const TInt KDummyFileSize = 100;
const TInt KDummyHeaderSize = 10;
const TInt KDummyDataSize = KDummyFileSize - KDummyHeaderSize;

// CAsyncTestReadCodec
void CAsyncTestReadCodec::InitFrameL(TFrameInfo&, CFrameImageData&, TBool, CFbsBitmap&, CFbsBitmap*)
	{
	}

TFrameState CAsyncTestReadCodec::ProcessFrameHeaderL(TBufPtr8& /*aData*/)
	{
	return EFrameComplete;
	}

TFrameState CAsyncTestReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	return EFrameComplete;
	}


// CAsyncTestDecoderPlugin
void CAsyncTestDecoderPlugin::ConstructL()
	{
	}

CAsyncTestDecoderPlugin* CAsyncTestDecoderPlugin::NewL()
	{
	CAsyncTestDecoderPlugin* self = new (ELeave) CAsyncTestDecoderPlugin;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CAsyncTestDecoderPlugin::CAsyncTestDecoderPlugin()
	{
	iAsynchronous = ETrue;
	}

CAsyncTestDecoderPlugin::~CAsyncTestDecoderPlugin()
	{
	delete iAsyncDecoder;
	}

void CAsyncTestDecoderPlugin::ImageType(int,class TUid& aImageType,class TUid &aImageSubType) const
	{
	aImageType = KImageTypeGIFUid;
	aImageSubType = KNullUid;
	}

void CAsyncTestDecoderPlugin::ScanDataL()
	{
	if (!iReadHeaders)
		{
		TPtrC8 bufferDes;
		ReadDataL(0, bufferDes, (KDummyHeaderSize));
		if (bufferDes.Length() < KDummyHeaderSize)
			User::Leave(KErrUnderflow);

		iReadHeaders = ETrue;
		SetDataLength(KDummyDataSize);


		iDecodeIterations = 0;

		// create the codec
		delete iImageReadCodec;
		iImageReadCodec = NULL;
		iImageReadCodec = new (ELeave) CAsyncTestReadCodec;
		SetImageReadCodec(iImageReadCodec);
	
		// create the async decoder
		delete iAsyncDecoder;
		iAsyncDecoder = NULL;
		iAsyncDecoder = new (ELeave) CAsyncScheduler(*this);


		// create a dummy TFrameInfo
		TFrameInfo imageInfo;
		imageInfo = ImageInfo();
		imageInfo.iFrameSizeInTwips.iWidth = 0;
		imageInfo.iFrameSizeInTwips.iHeight = 0;
		imageInfo.iFrameCoordsInPixels.SetRect(TPoint(0, 0), TPoint(256, 256));
		imageInfo.iOverallSizeInPixels = TSize(256, 256);
		imageInfo.iBitsPerPixel = 24;
		imageInfo.iDelay = 0;
		imageInfo.iFlags = TFrameInfo::ECanDither;
		imageInfo.iFrameDisplayMode = EColor16M;

		SetImageInfo(imageInfo);

		SetStartPosition(KDummyHeaderSize);
		
		ReadFrameHeadersL();
		}
	}


void CAsyncTestDecoderPlugin::ProcessL(TInt aError)
	{
	iError = aError;

	if (aError != KErrNone)
		{
		SelfComplete(iError);
		return;
		}

	switch(iDecodeState)
		{
		case EDecodeInit:
			iDecodeState = EDecodeInProgress;
			iAsyncDecoder->ScheduleDecode();
			break;
		case EDecodeInProgress:
			TRAP(iError, iImageReadCodec->ProcessFrameL(SourceData()));
			if (iError != KErrNone)
				{
				SelfComplete(iError);
				break;
				}

			iDecodeState = EDecodeFinished;
			iAsyncDecoder->ScheduleDecode();
			break;
		case EDecodeFinished:
			iCodecState = EFrameComplete;
			SelfComplete(iError);
			break;
		}

	}


void CAsyncTestDecoderPlugin::DoConvert()
	{
	const TInt KDecodeIterations = 10;

	TRAPD(errCode, PrepareForProcessFrameL());

	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}

	if (iAsynchronous)
		{
		if (iError != KErrNone)
			{
			RequestComplete(iError);
			}
		// need to get more data from client app ?
		else if (SourceData().Length() < KDummyDataSize)
			{
			RequestComplete(KErrUnderflow);
			}
		else if (iDecodeIterations++ > KDecodeIterations)
			{
			RequestComplete(iError);
			}
		else 
			{
			iDecodeState = EDecodeInit;
			iAsyncDecoder->ScheduleDecode();
			SetSelfPending();
			}
		}
	else
		{
		TRAP(errCode, iCodecState = iImageReadCodec->ProcessFrameL(SourceData()));
		HandleProcessFrameResult(errCode, iCodecState);
		}
	}

CFrameInfoStrings* CAsyncTestDecoderPlugin::FrameInfoStringsL(class RFs& aFs,int)
	{
	iFs = &aFs;
	return NULL;
	}

void CAsyncTestDecoderPlugin::NotifyComplete()
	{
	// initialize the little state machine
	iDecodeState = EDecodeInit; 
	iError = KErrNone;

	CImageDecoderPlugin::NotifyComplete();
	}


void CAsyncTestDecoderPlugin::Cleanup()
	{
	if (iAsyncDecoder)
		iAsyncDecoder->Cancel();
	}

// CAsyncScheduler
CAsyncScheduler::CAsyncScheduler(MAsyncSchedulerObserver& aSchedulerObserver) : 
	CActive(CActive::EPriorityIdle), iSchedulerObserver(aSchedulerObserver)
	{
	CActiveScheduler::Add(this); 
	}


void CAsyncScheduler::ScheduleDecode()
	{ 
	SetActive();
	TRequestStatus* status = &iStatus; 
	User::RequestComplete(status, KErrNone);
	}

void CAsyncScheduler::RunL()
	{
	TInt error = iStatus.Int();

	iSchedulerObserver.ProcessL(error);
	}


//CAsyncTestDecodeConstruct
CAsyncTestDecodeConstruct* CAsyncTestDecodeConstruct::NewL()
	{
	return new (ELeave) CAsyncTestDecodeConstruct;
	}

CAsyncTestDecodeConstruct::CAsyncTestDecodeConstruct()
	{
	}

CImageDecoderPlugin* CAsyncTestDecodeConstruct::NewPluginL() const
	{
	return CAsyncTestDecoderPlugin::NewL();
	}


//RCImageDecoderPrivAsyncDecodeTest
RCImageDecoderPrivAsyncDecodeTest* RCImageDecoderPrivAsyncDecodeTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RCImageDecoderPrivAsyncDecodeTest(aSuite);
	}

RCImageDecoderPrivAsyncDecodeTest::RCImageDecoderPrivAsyncDecodeTest(CTestSuite* aSuite):
	RAllocTest(EFalse),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-5620-HP");
	}

void RCImageDecoderPrivAsyncDecodeTest::SetupL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(iFileSession.Connect());
	User::LeaveIfError(RFbsSession::Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iConstruct = NULL;

	iSourceData = HBufC8::NewL(KDummyFileSize);
	iSourceDataPtr.Set(iSourceData->Des());
	iSourceDataPtr.SetLength(KDummyFileSize);

	iSourceLength = KDummyFileSize;

	iTestBitmap = NULL;
	iImageDecoderPriv = NULL;
	iOptions = CImageDecoder::EOptionAllowZeroFrameOpen;

	iListener = new (ELeave) CActiveListener;
	}

void RCImageDecoderPrivAsyncDecodeTest::TestStepL()
	{
	iSourceDataPtr.SetLength(0);

	if(iImageDecoderPriv == NULL)
		{
		iConstruct = CAsyncTestDecodeConstruct::NewL();
		iImageDecoderPriv = CImageDecoderPriv::NewL(iConstruct, this);
		iImageDecoderPriv->CreatePluginL();	
		iImageDecoderPriv->SetDataL(iFileSession, iSourceDataPtr, iOptions);

		iImageDecoderPriv->CreatePluginL();
		iImageDecoderPriv->HandleNewlyOpenedImageL();
		}

	while(!iImageDecoderPriv->IsImageHeaderProcessingComplete())
		{
		iImageDecoderPriv->ContinueProcessingHeaderL();
		IncrementBufferSize();
		}

	const TFrameInfo frameInfo = iImageDecoderPriv->FrameInfo(0);

	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iTestBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	iListener->InitialiseActiveListener();
	iImageDecoderPriv->Convert(iMyThread, &iListener->iStatus, *iTestBitmap, 0);
	CActiveScheduler::Start();

	iStatus = iListener->iStatus.Int();
	if((iStatus != KErrNone) && (iStatus != KErrUnderflow))
		User::Leave(iStatus);

	while(iStatus == KErrUnderflow)
		{
		IncrementBufferSize();
		iListener->InitialiseActiveListener();
		iImageDecoderPriv->ContinueConvert(iMyThread, &iListener->iStatus);
		CActiveScheduler::Start();
		iStatus = iListener->iStatus.Int();
		if((iStatus != KErrNone) && (iStatus != KErrUnderflow))
			User::Leave(iStatus);
		}
	}

void RCImageDecoderPrivAsyncDecodeTest::CheckResultL()
	{
	Compare(iStatus, KErrNone, _L("Decode error"));
	}

void RCImageDecoderPrivAsyncDecodeTest::Cleanup()
	{
	delete iImageDecoderPriv; iImageDecoderPriv = NULL;
	delete iConstruct; iConstruct = NULL;
	if(iTestBitmap)
		{
		delete iTestBitmap; 
		iTestBitmap = NULL;
		}
	}

void RCImageDecoderPrivAsyncDecodeTest::Close()
	{
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RFbsSession::Disconnect();
	RAllocTest::Close();
	}

void RCImageDecoderPrivAsyncDecodeTest::IncrementBufferSize()
	{
	TInt position = iSourceDataPtr.Length();

	if(position < iSourceLength)
		{
		iSourceDataPtr.SetLength(position+1);
		}
	}


// CUtilityTimer
CUtilityTimer* CUtilityTimer::NewL(TTimeIntervalMicroSeconds32& aDelay, MTimerObserver& aObserver)
    {
    CUtilityTimer* self = new (ELeave) CUtilityTimer(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL(aDelay);
    CleanupStack::Pop(self);
    return self;
    }

void CUtilityTimer::ConstructL(TTimeIntervalMicroSeconds32& aDelay)
    {
    CTimer::ConstructL();

    iDelay = aDelay;
    CActiveScheduler::Add(this);
    }

CUtilityTimer::~CUtilityTimer()
    {
    Cancel();
    }

void CUtilityTimer::InitializeTimer()
    {
	// Request another wait - assume not active
	CTimer::After(iDelay);
    }

void CUtilityTimer::RunL()
    {
	if (iStatus.Int() == KErrNone)
		iObserver.TimerExpired();
    }

void CUtilityTimer::DoCancel()
	{
	}


CUtilityTimer::CUtilityTimer(MTimerObserver& aObserver) :
	CTimer(CActive::EPriorityUserInput),
    iObserver(aObserver)
	{
    }

// RCImageDecoderAsyncDecodeTest 
RCImageDecoderAsyncDecodeTest* RCImageDecoderAsyncDecodeTest::NewL(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest)
	{
	return new (ELeave) RCImageDecoderAsyncDecodeTest(aSuite, aAllocTest, aCancelTest);
	}

RCImageDecoderAsyncDecodeTest::RCImageDecoderAsyncDecodeTest(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest):
	RAllocTest(aAllocTest),
	iCancelTest(aCancelTest),
	iSourceDataPtr(NULL,0,0)
	{
	iSuite = aSuite;
	if (iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5623-HP");
	else if (iCancelTest)
		iTestStepName = _L("MM-ICL-FRM-U-5622-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5621-HP");
	}

void RCImageDecoderAsyncDecodeTest::SetupL()
	{
	if (iAllocTest)
		INFO_PRINTF1(_L("Async decoder alloc Test"));
	else if (iCancelTest)
		INFO_PRINTF1(_L("Async decoder cancel Test"));
	else
		INFO_PRINTF1(_L("Async decoder straight Test"));

	TTimeIntervalMicroSeconds32 KOneSecond = 1000000;

	User::LeaveIfError(iFileSession.Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iSourceData = HBufC8::NewL(KDummyFileSize);
	iSourceDataPtr.Set(iSourceData->Des());
	iSourceDataPtr.SetLength(KDummyFileSize);

	iSourceLength = KDummyFileSize;

	iTestBitmap = NULL;
	iImageDecoder = NULL;
	iOptions = CImageDecoder::EOptionAllowZeroFrameOpen;

	iTimer = CUtilityTimer::NewL(KOneSecond, *this);

	iListener = new (ELeave) CActiveListener;
	}

void RCImageDecoderAsyncDecodeTest::TimerExpired()
	{
	if (iImageDecoder)
		{
		iImageDecoder->Cancel();
		iCancelled = ETrue;
		}
	}


void RCImageDecoderAsyncDecodeTest::TestStepL()
	{
	// don't need to push this to cleanup stack because we should
	// always call Cleanup() if this function leaves
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	iSourceDataPtr.SetLength(0);

	iImageDecoder = CImageDecoder::DataNewL(
		iFileSession, 
		iSourceDataPtr,
		iOptions,
		KNullUid,				// const TUid aImageType
		KNullUid,				// const TUid aImageSubType
		TUid::Uid(KTestAsyncDecoderImplementationUidValue));	// const TUid aDecoderUid

	
	while(!iImageDecoder->IsImageHeaderProcessingComplete())
		{
		iImageDecoder->ContinueProcessingHeaderL();
		IncrementBufferSize();
		}

	const TFrameInfo frameInfo = iImageDecoder->FrameInfo(0);

	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(iTestBitmap->Create(frameInfo.iOverallSizeInPixels, frameInfo.iFrameDisplayMode));

	iListener->InitialiseActiveListener();
	iImageDecoder->Convert(&iListener->iStatus, *iTestBitmap, 0);
	CActiveScheduler::Start();

	iStatus = iListener->iStatus.Int();
	if((iStatus != KErrNone) && (iStatus != KErrUnderflow))
		User::Leave(iStatus);

	if (iCancelTest)
		iTimer->InitializeTimer();

	while(iStatus == KErrUnderflow)
		{
		IncrementBufferSize();
		iListener->InitialiseActiveListener();
		iImageDecoder->ContinueConvert(&iListener->iStatus);
		CActiveScheduler::Start();
		iStatus = iListener->iStatus.Int();
		if((iStatus != KErrNone) && (iStatus != KErrUnderflow) && (iStatus != KErrCancel))
			User::Leave(iStatus);
		}
	}

void RCImageDecoderAsyncDecodeTest::CheckResultL()
	{
	if (iCancelTest)
		{
		Compare(iStatus, KErrCancel, _L("Decode error - expected KErrCancel"));
		Compare(iCancelled, ETrue, _L("Decoder was not cancelled"));
		}
	else
		{
		Compare(iStatus, KErrNone, _L("Decode error"));
		Compare(iCancelled, EFalse, _L("Decoder was unexpectedly cancelled"));
		}
	}

void RCImageDecoderAsyncDecodeTest::Cleanup()
	{
	delete iImageDecoder; iImageDecoder = NULL;
	if(iTestBitmap)
		{
		delete iTestBitmap; 
		iTestBitmap = NULL;
		}
	RFbsSession::Disconnect();
	}

void RCImageDecoderAsyncDecodeTest::Close()
	{
	delete iTimer; iTimer = NULL;
	delete iSourceData; iSourceData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RAllocTest::Close();
	}

void RCImageDecoderAsyncDecodeTest::IncrementBufferSize()
	{
	TInt position = iSourceDataPtr.Length();
	if(position < iSourceLength)
		{
		iSourceDataPtr.SetLength(position+1);
		}
	}



// RCImageEncoderAsyncEncodeTest 
RCImageEncoderAsyncEncodeTest* RCImageEncoderAsyncEncodeTest::NewL(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest)
	{
	return new (ELeave) RCImageEncoderAsyncEncodeTest(aSuite, aAllocTest, aCancelTest);
	}

RCImageEncoderAsyncEncodeTest::RCImageEncoderAsyncEncodeTest(CTestSuite* aSuite, TBool aAllocTest, TBool aCancelTest):
	RAllocTest(aAllocTest),
	iCancelTest(aCancelTest)
	{
	iSuite = aSuite;
	if (iAllocTest)
		iTestStepName = _L("MM-ICL-FRM-U-5626-HP");
	else if (iCancelTest)
		iTestStepName = _L("MM-ICL-FRM-U-5625-HP");
	else
		iTestStepName = _L("MM-ICL-FRM-U-5624-HP");
	}

void RCImageEncoderAsyncEncodeTest::SetupL()
	{
	if (iAllocTest)
		INFO_PRINTF1(_L("Async encoder alloc Test"));
	else if (iCancelTest)
		INFO_PRINTF1(_L("Async encoder cancel Test"));
	else
		INFO_PRINTF1(_L("Async encoder straight Test"));

	TTimeIntervalMicroSeconds32 KOneSecond = 1000000;

	User::LeaveIfError(iFileSession.Connect());

	iScheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install( iScheduler );
	
	iTestBitmap = NULL;
	iImageEncoder = NULL;
	iOptions = CImageEncoder::EOptionNone;

	iTimer = CUtilityTimer::NewL(KOneSecond, *this);

	iListener = new (ELeave) CActiveListener;
	}

void RCImageEncoderAsyncEncodeTest::TimerExpired()
	{
	if (iImageEncoder)
		{
		iImageEncoder->Cancel();
		iCancelled = ETrue;
		}
	}


void RCImageEncoderAsyncEncodeTest::TestStepL()
	{
    User::LeaveIfError(FbsStartup());
	User::LeaveIfError(RFbsSession::Connect());

	iImageEncoder = CImageEncoder::DataNewL(
		iDestData,
		iOptions,
		KNullUid,				// const TUid aImageType
		KNullUid,				// const TUid aImageSubType
		TUid::Uid(KTestAsyncEncoderImplementationUidValue));

	
	iTestBitmap = new (ELeave) CFbsBitmap;
	User::LeaveIfError(
		iTestBitmap->Create(
		TSize(256, 256),	// OverallSizeInPixels
		EColor256));		// FrameDisplayMode

	if (iCancelTest)
		iTimer->InitializeTimer();

	iListener->InitialiseActiveListener();
	iImageEncoder->Convert(&iListener->iStatus, *iTestBitmap, 0);
	CActiveScheduler::Start();

	iStatus = iListener->iStatus.Int();
	if((iStatus != KErrNone) && (iStatus != KErrUnderflow) && (iStatus != KErrCancel))
		User::Leave(iStatus);
	}

void RCImageEncoderAsyncEncodeTest::CheckResultL()
	{
	if (iCancelTest)
		{
		Compare(iStatus, KErrCancel, _L("Encode error - expected KErrCancel"));
		Compare(iCancelled, ETrue, _L("Encoder was not cancelled"));
		}
	else
		{
		Compare(iStatus, KErrNone, _L("Encode error"));
		Compare(iCancelled, EFalse, _L("Encoder was unexpectedly cancelled"));
		}
	}

void RCImageEncoderAsyncEncodeTest::Cleanup()
	{
	delete iImageEncoder; iImageEncoder = NULL;
	if(iTestBitmap)
		{
		delete iTestBitmap; 
		iTestBitmap = NULL;
		}
	RFbsSession::Disconnect();
	}

void RCImageEncoderAsyncEncodeTest::Close()
	{
	delete iTimer; iTimer = NULL;
	delete iDestData; iDestData = NULL;
	delete iScheduler; iScheduler = NULL;
	iFileSession.Close();
	RAllocTest::Close();
	}

/*static*/
RTImageMimeTypeTest* RTImageMimeTypeTest::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RTImageMimeTypeTest(aSuite);
	}
	
RTImageMimeTypeTest::RTImageMimeTypeTest(CTestSuite* aSuite)
	{
	iSuite = aSuite;
	iTestStepName = _L("MM-ICL-FRM-U-7021-HP");	
	}
	
TVerdict RTImageMimeTypeTest::DoTestStepL()
	{
	_LIT(KVideoFile, "video.mp4");
	TFileName  fileName = ((CICLFrameworkTestSuite*)iSuite)->DefaultPath();
	fileName.Append(KVideoFile);
	
	RApaLsSession lsSession; 
	User::LeaveIfError( lsSession.Connect() );
	
	TDataRecognitionResult  result;
	TInt error=lsSession.RecognizeData(fileName, KNullDesC8(), result);
	
	lsSession.Close();
	
	User::LeaveIfError( error );
	
	if (result.iDataType.Uid() == KImageTypeWBMPUid)
		{
		INFO_PRINTF1(_L("--- ERROR: Got wrong MIME type for mp4 video"));		
		return EFail;
		}
	
	return EPass;
	}

TVerdict RTImageMimeTypeTest::OpenL()
	{
	return EPass;
	}
	
void RTImageMimeTypeTest::Close()
	{
	}


/*static*/
RSetThreadPriorityTestStep* RSetThreadPriorityTestStep::NewL(CTestSuite* aSuite)
	{
	return new (ELeave) RSetThreadPriorityTestStep(aSuite);
	}

RSetThreadPriorityTestStep::RSetThreadPriorityTestStep(CTestSuite* aSuite)
	{
	iSuite = aSuite;

	iTestStepName = _L("MM-ICL-FRM-U-7022-HP");	
	}

TVerdict RSetThreadPriorityTestStep::OpenL()
	{
	iActiveScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install( iActiveScheduler );
	
	User::LeaveIfError( iFileSession.Connect() );
	
	return EPass;
	}
	
void RSetThreadPriorityTestStep::Cleanup()
	{
	iFileSession.Close();
	delete iActiveScheduler;
	iActiveScheduler = NULL;
	}
// from RTestStep
TVerdict RSetThreadPriorityTestStep::DoTestStepL()
	{
	TVerdict result = EPass;
	iFileName = static_cast<CICLFrameworkTestSuite*>(iSuite)->DefaultPath();
	iFileName.Append(KGifFile);
	
	CImageDecoder* decoder = CImageDecoder::FileNewL(iFileSession, iFileName, CImageDecoder::EOptionAlwaysThread);
	CleanupStack::PushL(decoder);
	TThreadPriority initialPriority = IclThreadPriotityL();
	TThreadPriority newPriority = ( initialPriority == EPriorityAbsoluteBackground? EPriorityAbsoluteLow : EPriorityAbsoluteBackground );
	TInt error = decoder->SetDecoderThreadPriority( newPriority );
	if (error != KErrNone)
		{
		result = EFail;
		INFO_PRINTF2(_L("-- Failed to set the ICL decoder thread priority error=%d"),error);
		}
	TThreadPriority current = IclThreadPriotityL();
	if ( newPriority != current )
		{
		INFO_PRINTF3(_L("-- Failed to set the ICL decoder thread priority: new priority is %d but must be %d"),current, newPriority);
		result = EFail;		
		}
	CleanupStack::PopAndDestroy(decoder);
	decoder = NULL;
	
	decoder = CImageDecoder::FileNewL(iFileSession, iFileName, CImageDecoder::EOptionNone);
	error = decoder->SetDecoderThreadPriority( EPriorityAbsoluteLow );
	delete decoder;
	decoder = NULL;
	if (error != KErrNotSupported )
	    {
	    INFO_PRINTF2(_L("-- Wrong error code: error=%d"),error);
		result = EFail;
	    }
	
	// encoder //
	
	HBufC8* destBuffer=NULL;
	CImageEncoder* encoder = CImageEncoder::DataNewL(destBuffer, CImageEncoder::EOptionAlwaysThread, KImageTypeGIFUid);
	CleanupStack::PushL( encoder );
	
	initialPriority = IclThreadPriotityL();
	newPriority = ( initialPriority == EPriorityAbsoluteBackground? EPriorityAbsoluteLow : EPriorityAbsoluteBackground );
	error = encoder->SetEncoderThreadPriority( newPriority );
	if (error != KErrNone)
		{
		INFO_PRINTF2(_L("-- Failed to set ICL encoder thread priority: error=%d"),error);
		result = EFail;
		}	
	if ( newPriority != (current = IclThreadPriotityL()) )
		{
		INFO_PRINTF3(_L("-- Failed to set ICL encoder thread priority: new priority is %d but must be %d"),current, newPriority);
		result = EFail;		
		}	
	
	CleanupStack::PopAndDestroy( encoder );
	encoder = NULL;
	
	encoder = CImageEncoder::DataNewL(destBuffer, CImageEncoder::EOptionNone, KImageTypeGIFUid); 
	error = encoder->SetEncoderThreadPriority( EPriorityAbsoluteLow );
	delete encoder;
	encoder = NULL;
	if (error != KErrNotSupported )
	    {
	    INFO_PRINTF2(_L("-- Wrong error code: error=%d"),error);
		result = EFail;
	    }	
	
	return result;
	}

TThreadPriority RSetThreadPriorityTestStep::IclThreadPriotityL()
	{
	TFindThread finder(_L("*"));
	TFullName threadName;
	TThreadPriority priority(EPriorityNull);
	TBool found=EFalse;
	while (KErrNone == finder.Next(threadName) && !found)
		{
		if ( threadName.Find(_L("ICLThread")) == KErrNotFound)
			{
			continue;
			}
		RThread thread;
		if (KErrNone == thread.Open(threadName) )
			{
			RProcess me, that;
			if (KErrNone == thread.Process( that ))
				{
				if (that.Id() == me.Id())
					{
					priority = thread.Priority();
					found = ETrue;
					}
				that.Close();
				}
			thread.Close();
			}
		}
	//
	return priority;
	}


/**
*
* CR0793 Jpeg encoder performance: 
* that test case relates to the REQ6095, 
* and verfies proper functioning of the write-back cache
* by emulating writting onto slow media
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class REmulateSlowMediaTestStep
*
*/
/*static*/
REmulateSlowMediaTestStep* REmulateSlowMediaTestStep::NewL(CTestSuite* aSuite)
    {
	return new (ELeave) REmulateSlowMediaTestStep(aSuite);        
    }

REmulateSlowMediaTestStep::REmulateSlowMediaTestStep(CTestSuite* aSuite)
    {
	iSuite = aSuite;
	iHeapSize = 1024*1024; // 1 MB

	iTestStepName = _L("MM-ICL-FRM-U-7035-HP");	        
    }

NONSHARABLE_CLASS( CImageEnc ): public CImageEncoder
    {
public:
    inline void EmulateSlowMediaWritingL()
        {
        CustomSyncL(0x10273823);
        }
    };    

// from RTestStep
TVerdict REmulateSlowMediaTestStep::OpenL()
    {
	iActiveScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install( iActiveScheduler );
	
	User::LeaveIfError( iFileSession.Connect() );
	
    User::LeaveIfError( RFbsSession::Connect(iFileSession) );
	
    return EPass;
    }
    
void REmulateSlowMediaTestStep::Cleanup()
    {
    RFbsSession::Disconnect();
	iFileSession.Close();
    
	delete iActiveScheduler;
	iActiveScheduler = NULL;        
    }
    
LOCAL_C
TChar GetExternalDriveLetterL(RFs& aFs)
    {
    for (TInt drive=EDriveA;  drive < EDriveZ ; drive++)
        {
        TDriveInfo driveInfo;
        User::LeaveIfError( aFs.Drive(driveInfo, drive) );

        #ifdef __X86GCC__
        // removable drive presently not supported on X86. So checking only for Hard Disk media
        if ( driveInfo.iType == EMediaHardDisk)
        #else
        if ( driveInfo.iType == EMediaHardDisk && (driveInfo.iDriveAtt & KDriveAttRemovable))
        #endif
            {
            return TChar( drive + 'A' );    
            }            
        
        }
    const TInt KDriveNotFound = -1004;
    User::Leave( KDriveNotFound );
    return TChar(0);
    }    
    
TVerdict REmulateSlowMediaTestStep::DoTestStepL()
    {
    typedef const TUid* TPCUid; 
    static const TPCUid KImageTypes[]= {&KImageTypePNGUid, &KImageTypeJPGUid, NULL};
    TFileName encodeDestination(_L("c:\\tsu_icl_frm_enc_dest"));
    encodeDestination[0] = GetExternalDriveLetterL(iFileSession);
    
    _LIT(KSrcMbmFile, "tif2.mbm");
    
    TFileName srcFile1(KSrcMbmFile);
    TFileName srcFile;
    GetTestFileNameL(srcFile, srcFile1);
    
    CFbsBitmap* src = new (ELeave) CFbsBitmap();
    CleanupStack::PushL(src);
    User::LeaveIfError( src->Create(TSize(1,1), EColor16M) );
    User::LeaveIfError( src->Load(srcFile) );

    CFrameImageData* imgData = CFrameImageData::NewL();
    CleanupStack::PushL( imgData );
   
    for (const TPCUid* imgId=KImageTypes; *imgId!=NULL; ++imgId)
        {
        INFO_PRINTF2(_L("---Image Type Id=%x"), (*imgId)->iUid);
// initialize codec-specific encoding parameters //        
        if (**imgId == KImageTypeJPGUid)
            {
            TJpegImageData* jpegData = new (ELeave) TJpegImageData;
            CleanupDeletePushL(jpegData);
            jpegData->iSampleScheme = TJpegImageData::TColorSampling( TJpegImageData::EColor444 );
            jpegData->iQualityFactor= 100; // maximum quality

            User::LeaveIfError( imgData->AppendImageData( jpegData ) );
            CleanupStack::Pop( jpegData );            
            }
        else if (**imgId == KImageTypePNGUid)
            {
            TPngEncodeData* imageData = new (ELeave) TPngEncodeData;
            CleanupDeletePushL(imageData);
            imageData->iBitsPerPixel  = 24;
            imageData->iColor         = ETrue;
            imageData->iPaletted      = EFalse;
            imageData->iLevel         = TPngEncodeData::EBestSpeed;

            User::LeaveIfError( imgData->AppendFrameData( imageData ) );
            CleanupStack::Pop( imageData );                        
            }
        else
            {
            ASSERT(EFalse);
            }
        
       
        iFileSession.Delete( encodeDestination ); // we ignore errors here
// encode a file with slow media emulation ON  //
        CImageEncoder* encoder=CImageEncoder::FileNewL(iFileSession, encodeDestination, CImageEncoder::EOptionAlwaysThread, **imgId );
        CleanupStack::PushL(encoder);
        
        reinterpret_cast<CImageEnc*>(encoder)->EmulateSlowMediaWritingL();
        
        TRequestStatus req;
        encoder->Convert(&req, *src, imgData);
        User::WaitForRequest(req);
        CleanupStack::PopAndDestroy(encoder);
        encoder=NULL;

        if (req.Int() != KErrNone)
    	    {
    	    INFO_PRINTF2(_L("---ERROR: failed to encode a file err=%d"), req.Int());
    	    User::Leave( req.Int() );
    	    }
// now encode to a memory buffer //    	    
        HBufC8* imageBuffer=NULL;
        encoder=CImageEncoder::DataNewL(imageBuffer, CImageEncoder::EOptionAlwaysThread, **imgId );

        
        encoder->Convert(&req, *src, imgData);
        User::WaitForRequest(req);
        delete encoder;

        CleanupStack::PushL(imageBuffer);
        if (req.Int() != KErrNone)
    	    {
    	    INFO_PRINTF2(_L("---ERROR: failed to encode a buffer err=%d"), req.Int());
    	    
    	    User::Leave( req.Int() );
    	    }
        
        for (TInt i=imgData->ImageDataCount(); i > 0; --i )
            {
            imgData->RemoveImageData( i-1 );    
            }
        for (TInt i=imgData->FrameDataCount(); i > 0; --i )
            {
            imgData->RemoveFrameData( i-1 );    
            }            
// compare file and memory buffer // 
        RFile file;
        User::LeaveIfError( file.Open(iFileSession, encodeDestination, EFileRead) );
        CleanupClosePushL(file);
    	
    	TInt fileSize=0;
    	User::LeaveIfError( file.Size(fileSize) );
    	if (fileSize != imageBuffer->Length())
    	    {
    	    INFO_PRINTF1(_L("---ERROR: file and buffer sizes are different"));
    	    User::Leave(KErrGeneral);
    	    }
    	HBufC8* buffer=HBufC8::NewMaxLC(fileSize);
    	
    	TPtr8 ptr(buffer->Des());
    	User::LeaveIfError( file.Read(ptr) );
    	
    	if (KErrNone != buffer->Compare(*imageBuffer))
    	    {
    	    INFO_PRINTF1(_L("---ERROR: file and buffer content are different"));
    	    User::Leave(KErrGeneral);    	    
    	    }
    	
    	CleanupStack::PopAndDestroy(3, imageBuffer);
    	iFileSession.Delete( encodeDestination ); // we ignore errors here
    	
        }
        
    CleanupStack::PopAndDestroy(2, src);
    
    return EPass;
    }


/**
*
* PREQ1630 Encoder Rotation and Flip Transformation: 
* that test case relates to the REQ7844, 
* and verfies proper functioning of the TPosition Processor Algos for Rotation and Flips
*
* @lib "TSU_ICL_FRM_01.lib"
*
* @class RTPositionProcessorTest
*
*/

RTPositionProcessorTest* RTPositionProcessorTest::NewL(CTestSuite* aSuite,
    													TTransformMethod aMethod,
    													const TDesC& aTestStepName)
	{
	return new (ELeave) RTPositionProcessorTest(aSuite,
													aMethod,
													aTestStepName);
	}
	
RTPositionProcessorTest::RTPositionProcessorTest(CTestSuite* aSuite,
    												TTransformMethod aMethod,
    												const TDesC& aTestStepName)
	{
	iSuite = aSuite;
	iMethod = aMethod;
	iTestStepName = aTestStepName;
	}
	
TVerdict RTPositionProcessorTest::OpenL()
	{
	return iTestStepResult;	
	}
	
TVerdict RTPositionProcessorTest::DoTestStepL()
	{
	switch(iMethod)
		{
		case ETransformRotate:
			{
			INFO_PRINTF1(_L("Processing Transform Rotation"));
			
			TPositionProcessor posProcess;
			TPoint pos;	
			TInt16 destData[KEncoderPixCount];	
			
			const TInt16* srcPtr = KSourcePixData;
			TInt16* dstPtr = destData;

			posProcess.SetDimensions(TRect(TPoint(0,0),TPoint(KEncoderPixWidth,KEncoderPixHeight)), TSize(1,1));
			posProcess.RotateCoordinates();
			posProcess.MoveFirst();
			
			do
				{		
				posProcess.GetCurrentPosition(pos);					
				TInt elementPos = ((pos.iY * 8) + pos.iX);
				*dstPtr++ = TInt16(srcPtr[elementPos]);		
				posProcess.MoveNext();		
				}while(!posProcess.IsEndOfImage());
			
			Compare(destData, KRefRotPixData, KEncoderPixCount);
			
			break;
			}
		case ETransformVerticalFlip:
			{
			INFO_PRINTF1(_L("Processing Transform Vertical Flip"));
			
			TPositionProcessor posProcess;
			TPoint pos;	
			TInt16 destData[KEncoderPixCount];	
			
			const TInt16* srcPtr = KSourcePixData;
			TInt16* dstPtr = destData;

			posProcess.SetDimensions(TRect(TPoint(0,0),TPoint(KEncoderPixWidth,KEncoderPixHeight)), TSize(1,1));
			posProcess.VerFlipCoordinates();
			posProcess.MoveFirst();
			
			do
				{		
				posProcess.GetCurrentPosition(pos);					
				TInt elementPos = ((pos.iY * 8) + pos.iX);
				*dstPtr++ = TInt16(srcPtr[elementPos]);		
				posProcess.MoveNext();		
				}while(!posProcess.IsEndOfImage());
			
			Compare(destData, KRefVerFlipPixData, KEncoderPixCount);
			
			break;
			}
		case ETransformHorizontalFlip:
			{
			INFO_PRINTF1(_L("Processing Transform Horizontal Flip"));
			
			TPositionProcessor posProcess;
			TPoint pos;	
			TInt16 destData[KEncoderPixCount];	
			
			const TInt16* srcPtr = KSourcePixData;
			TInt16* dstPtr = destData;

			posProcess.SetDimensions(TRect(TPoint(0,0),TPoint(KEncoderPixWidth,KEncoderPixHeight)), TSize(1,1));
			posProcess.HorFlipCoordinates();
			posProcess.MoveFirst();
			
			do
				{		
				posProcess.GetCurrentPosition(pos);					
				TInt elementPos = ((pos.iY * 8) + pos.iX);
				*dstPtr++ = TInt16(srcPtr[elementPos]);		
				posProcess.MoveNext();		
				}while(!posProcess.IsEndOfImage());
			
			Compare(destData, KRefHorFlipPixData, KEncoderPixCount);
			
			break;			
			}			
		case ETransformRotVerHorFlip:
			{
			INFO_PRINTF1(_L("Processing Transform Rotate Vertical & Horizontal Flip"));
			
			TPositionProcessor posProcess;
			TPoint pos;	
			TInt16 destData[KEncoderPixCount];	
			
			const TInt16* srcPtr = KSourcePixData;
			TInt16* dstPtr = destData;

			posProcess.SetDimensions(TRect(TPoint(0,0),TPoint(KEncoderPixWidth,KEncoderPixHeight)), TSize(1,1));			
			posProcess.RotateCoordinates();
			posProcess.VerFlipCoordinates();
			posProcess.HorFlipCoordinates();
			posProcess.MoveFirst();
			
			do
				{		
				posProcess.GetCurrentPosition(pos);					
				TInt elementPos = ((pos.iY * 8) + pos.iX);
				*dstPtr++ = TInt16(srcPtr[elementPos]);		
				posProcess.MoveNext();		
				}while(!posProcess.IsEndOfImage());
			
			Compare(destData, KRefRotVerHorFlipPixData, KEncoderPixCount);
			
			break;			
			}						
		default:
			{
			SetFail(_L("Called unsupported function"));
			}
		}
		
	return iTestStepResult;
	}

void RTPositionProcessorTest::Compare(const TInt16* aDstPtr, const TInt16* aRefPtr, TInt count)
	{
	for(TInt i=0; i<= count - 1 ;i++)
		{
		if(*aDstPtr++ != *aRefPtr++)
			{
			SetFail(_L("Transformation Failed"));
			return;
			}			
		}
	}

void RTPositionProcessorTest::Close()
	{
	//delete iComment; iComment = NULL;
	}


