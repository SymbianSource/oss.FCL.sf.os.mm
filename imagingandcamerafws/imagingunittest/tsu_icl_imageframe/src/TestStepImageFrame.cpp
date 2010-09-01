
// TestStepImageFrame.cpp

// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestStepImageFrame.h"
#include <imageframe.h>
#include <imageframeformats.hrh>
#include "jpgimageframeprocessor.h"
#include "JpegConsts.h"
#include <e32def.h>
#include <imageframeconst.hrh>

_LIT(KGlobalChunkName, "TestRChunkICLA");

const TInt KMaxBlocks 		  = 6;
const TInt KImageSizeInBytes  = 1024;
const TInt KUniformValue1     = 128;
const TInt KUniformValue2     = 0;
const TInt KRChunkDataOffset  = 10;
const TInt KRChunkRestrictionFlags = 0;
const TInt KMultiFrameNumber  = 2;


enum TFormatIndex
	{
	EUnknownFormat,
	EYCbCrMono,
	EYCbCr422Int,
	EYCbCr420Pla,
	EYCbCr420PlaRev
	};
	
enum TSizeIndex
	{
	EUnknownSize,
	EYCbCrMonoSize,
	EYCbCr422IntSize,
	EYCbCr420PlaSize
	};
	
const TInt KExpectedValuesIndx[][KMaxBlocks] = { 
									{0,-1,-1,-1,-1,-1},
									{0,1,2,3,-1,-1}, {0,0,2,3,-1,-1},
									{0,1,2,3,4,5}, {0,0,2,2,4,5}, {0,1,0,1,4,5}, {0,0,0,0,4,5}, {}};

const TInt KFrameSizeData[][2] = { {16,16}, {32,32}, {64,128},{640,480},{4,4}};
const TInt KFrameTestSizeData[][2] = {{32,32},{13,14}, {18,12},{18,18},{640,480},{4,4}};
const TInt KExpectedBlocksValuesIndx[][4] = { {-1}, {0,0,0,0}, {1,2,1,2},{3,4,5,6},{640,480},{4,4}};

const TUid KFrameFormatDefaults[] = 
	{ 
	{0},
	{KFormatYUVMonochromeUidValue},
	{KFormatYUV422InterleavedUidValue},
	{KFormatYUV420PlanarUidValue},
	{KFormatYUV420PlanarReversedUidValue}
	};

const TUid KFormatLists[][3] = 
	{ 
	  {{KFormatYUVMonochromeUidValue},{KColourSpaceYCbCrUidValue},{KSamplingMonochromeUidValue}},
	  {{KFormatYUV422InterleavedUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor422UidValue}},
	  {{KFormatYUV422InterleavedReversedUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor422UidValue}},
	  {{KFormatYYUV422InterleavedUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor422UidValue}},
	  {{KFormatYUV422PlanarUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor422UidValue}},
	  {{KFormatYUV420PlanarUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor420UidValue}},
	  {{KFormatYUV420PlanarReversedUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor420UidValue}},
	  {{KFormatYUV420SemiPlanarUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor420UidValue}},
	  {{KFormatYUV420InterleavedUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor420UidValue}},
	  {{KFormat16bitRGB444InterleavedUidValue},{KColourSpaceRGBUidValue},{KSamplingColor444UidValue}},
	  {{KFormat16BitRGB565InterleavedUidValue},{KColourSpaceRGBUidValue},{KSamplingColor444UidValue}},
	  {{KFormat32BitRGB888InterleavedUidValue},{KColourSpaceRGBUidValue},{KSamplingColor444UidValue}},
	  {{KFormatYUV444InterleavedUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor444UidValue}},
	  {{KFormatYUV444PlanarUidValue},{KColourSpaceYCbCrUidValue},{KSamplingColor444UidValue}},
	  {{0},{0},{0}}	
	};

const TInt KTestClipValues [][2] = {{0,0}, {255,255}, {10, 10}, {-1, 0}, {256, 255}, {128, 128}, {0x7FFF, 0xFF}, {-10, 0}};
	

//
// Various helper utility methods
//
// Method checking whether two formats are equal				
TBool RImageFrameUtil::FormatsEqual(const TFrameFormat& aFormat1, const TFrameFormat& aFormat2)
	{
	if ((aFormat1.FormatCode() == aFormat2.FormatCode()) &&
	    (aFormat1.ColourSpace() == aFormat2.ColourSpace()) )
		{
		return ETrue;
		}
	else 
		{
		return EFalse;
		}
	}
	
// Method checking whether two layouts are equal
TBool RImageFrameUtil::LayoutsEqual(const TFrameLayout& aLayout1, const TFrameLayout& aLayout2)
	{
	if ( aLayout1.Planes() != aLayout2.Planes())
		{
		return EFalse;
		}
	else
		{
		for (TInt plane = 0; plane < aLayout1.Planes(); plane++)
			{
		    if ((aLayout1.Start(plane) != aLayout2.Start(plane)) ||
			    (aLayout1.Length(plane) != aLayout2.Length(plane)) ||
			    (aLayout1.CurrentLength(plane) != aLayout2.CurrentLength(plane)) ||
			    (aLayout1.ScanLength(plane)  != aLayout2.ScanLength(plane)))
				{
				return EFalse;
				}
			}
		return ETrue;
		}
	}

// Method setting a buffer of specified size to a uniform value, either starting from start to end (forward) 
// or from end to front (backward) 
TBool RImageFrameUtil::CheckUniformAssignment(CImageFrame& aFrame, TInt aValue, 
											  TInt aBufferSize, TBool aForward)
	{
	TInt i = 0;
	TBool sucess = ETrue;
	const CImageFrame* frame1 = &aFrame;
	CImageFrame* frame = &aFrame;
	
	if (aForward)	
		{
		// access forward
		for (i = 0; i < aBufferSize; i++ )
			{
			frame->Data()[i] = aValue;
			if (frame1->Data()[i] != aValue)
				{
				sucess = EFail;
				break;
				}
			}
		}
	else 
		{
		// access backward
		for (i = aBufferSize-1; i >= 0; i-- )
			{
			frame->Data()[i] = aValue;
			if (frame1->Data()[i] != aValue)
				{
				sucess = EFail;
				break;
				}
			}
		}
	
	return sucess;
	}

// Initialises the value of a Data unit to a specific value.  	
void RImageFrameUtil::SetBlockToUniformValue(TDataUnit& aDataUnit, TInt aValue)
	{
	for (TInt i = 0; i < KJpgDCTBlockSize; i++)
		{
		aDataUnit.iCoeff[i] = aValue;
		}	
	}

// compares the values of two data units for the same index 
TBool RImageFrameUtil::CompareBlockValues(TDataUnit& aDataUnit1, 
										 TDataUnit& aDataUnit2)
	{
	TBool result = ETrue;
	for (TInt i=0; i < KJpgDCTBlockSize; i++)
		{
		if (aDataUnit1.iCoeff[i] != aDataUnit2.iCoeff[i])
			{
			result = EFalse;
			break;
			}
		}
	return result;	
	}

// assignes specific values to a array of pointers to TDataUnit
void RImageFrameUtil::InitPointerValuesL(TDataUnit* aDataUnit, RArray<const TDataUnit*> &aDataUnitPtr, TInt aBlocks)
	{
	aDataUnitPtr.Reset();
	for (TInt i=0; i < aBlocks; i++)
    	{
    	aDataUnitPtr.AppendL(&aDataUnit[i]);
    	}
	}

TInt RImageFrameUtil::LayoutPlanes(const TFrameFormat& aFormat)
	{
	TInt32 code = aFormat.FormatCode().iUid;
	switch (code)
		{
		case KFormatYUVMonochromeUidValue:
		case KFormatYUV422InterleavedUidValue:
			{
			return 1;
			}
			
		case KFormatYUV420PlanarUidValue:
		case KFormatYUV420PlanarReversedUidValue:
			{
			return 3;	
			}		

		default: 
			{
			return -1;	
			}		
		}
	}

// method used to initialise a layout aon the basis of format code.
// these are recommended example settings and used for demonstration only 
void RImageFrameUtil::InitLayoutValues(TFrameLayout& aLayout,TFrameFormat& aFormat, TSize& aSize, TInt& aRecommendedSize)
	{
    TInt bufferSize; 
    
	switch (aFormat.FormatCode().iUid)
		{
		case KFormatYUVMonochromeUidValue:
			{
			bufferSize = aSize.iWidth * aSize.iHeight;
			aLayout.SetStart(0,0);
			aLayout.SetLength(0, bufferSize);
			aLayout.SetCurrentLength(0, bufferSize);
			aLayout.SetScanLength(0,aSize.iWidth);
			aRecommendedSize = bufferSize;
			break;
			}
		case KFormatYUV422InterleavedUidValue:
			{
			if (aSize.iWidth % 4) 
				{
				aSize.iWidth = ((aSize.iWidth >> 2) + 1) << 2;
				}
			bufferSize = aSize.iWidth * aSize.iHeight * 2;
			aLayout.SetStart(0,0);
			aLayout.SetLength(0, bufferSize);
			aLayout.SetCurrentLength(0, bufferSize);
			aLayout.SetScanLength(0, aSize.iWidth * 2);
			aRecommendedSize = bufferSize;
			break;
			}
		case KFormatYUV420PlanarUidValue:
		case KFormatYUV420PlanarReversedUidValue:
			{
			bufferSize = aSize.iWidth * aSize.iHeight*3/2;
			TInt bufferSizeY = aSize.iWidth * aSize.iHeight;
			TInt bufferSizeUV = aSize.iWidth * aSize.iHeight/4;

			aLayout.SetStart(0,0);
			aLayout.SetStart(1,bufferSizeY);
			aLayout.SetStart(2,bufferSizeY+bufferSizeUV);
			aLayout.SetLength(0, bufferSizeY);
			aLayout.SetLength(1, bufferSizeUV);
			aLayout.SetLength(2, bufferSizeUV);
			aLayout.SetCurrentLength(0, bufferSizeY);
			aLayout.SetCurrentLength(1, bufferSizeUV);
			aLayout.SetCurrentLength(2, bufferSizeUV);
			aLayout.SetScanLength(0, aSize.iWidth);
			aLayout.SetScanLength(1, aSize.iWidth/2);
			aLayout.SetScanLength(2, aSize.iWidth/2);
			
			aRecommendedSize = bufferSize;
			break;
			}		
		
		default: 
			{
			
			}
		}
	}

//
// Base class - embeds the alloc functionaliity test as well
//	
RImageFrameTest::RImageFrameTest(TBool aAllocTest): iAllocTest(aAllocTest)
	{
	}
	
TVerdict RImageFrameTest::ProbeAllocTestsL()
	{
	// see if alloc fail stuff is enabled - ask if next alloc test will fail
	__UHEAP_SETFAIL(RHeap::EFailNext, 1);
	TAny *testAlloc = User::Alloc(1); 
	TBool workingOk = (testAlloc == NULL);
	User::Free(testAlloc);
	__UHEAP_SETFAIL(RHeap::ENone, 0);

	if (!workingOk)
		{
		ERR_PRINTF1(_L("RImageFrameTest::ProbeAllocTestsL Alloc Testing does not work on this build"));
		return EInconclusive;
		}
	return EPass;	
	}

// Make sure that alloc tests are run only in debug mode	
TVerdict RImageFrameTest::DoTestStepL()
	{
	TVerdict verdict = EFail;
	
	if (iAllocTest)
		{

		TInt failCount = 1;
		TBool completed = EFalse;
		TBool badResult = EFalse; 
		TBool reachedEnd = EFalse; // Note: declare outside loop to help with debugging
		TInt err = KErrNone;
		
		INFO_PRINTF2(_L("RImageFrameTest::DoTestStepL Alloc test %S starts"), &iTestStepName);
		
		// first make sure the alloc does not fail.
		TRAP(err, verdict = ProbeAllocTestsL());
		
		if (verdict != EPass)
			{
			INFO_PRINTF2(_L("RImageFrameTest::DoTestStepL ProbeAllocTestsL returned %d error"), verdict);
			}
			
	    if (verdict == EPass)
	    	{
	    	for (;;)
				{
				__UHEAP_SETFAIL(RHeap::EFailNext, failCount);
				__MM_HEAP_MARK;

				TRAP(err, verdict = DoTestL());

				completed = EFalse;
				if (err == KErrNone)
					{
					TAny *testAlloc = User::Alloc(1); // when this fails, we passed through all allocs within test
					if (testAlloc == NULL)
						{
						reachedEnd = ETrue;
						failCount -= 1;
						}
					else
						{
						User::Free(testAlloc);			
						}
			
					// see if valid result and break if wrong - might be premature result
					if (verdict != EPass)
						{
						badResult = ETrue;
						}
						
					completed = reachedEnd || badResult;
					}
				else if (err != KErrNoMemory) // bad error code
					{
					badResult = ETrue;
					completed = ETrue;
					}

				__MM_HEAP_MARKEND;
				// reset the heap
				__UHEAP_SETFAIL(RHeap::ENone, 0);
				
				if (err != KErrNoMemory) 
					{
					ERR_PRINTF2(_L("RImageFrameTest::DoTestStepL Alloc Testing err %d"), err);
					User::Leave(err);
					}

				if (completed)
					{
					break; // exit loop
					}
		
				failCount += 1;
				}

			failCount -= 1; // Failcount of 1 equates to 0 successful allocs, etc.
			if (badResult)
				{
				verdict = EFail;
				}
	    	}
		}
    else
    	{
    	// just do the vanila tests; no alloc tests
    	verdict = DoTestL();
    	}	
    	
    return verdict;
	}
	
TVerdict RImageFrameTest::DoTestL()
	{
	ERR_PRINTF1(_L("RImageFrameTest::DoTestL This function should be everridden"));
	User::Leave(KErrAbort);
	return EFail;
	}

void RImageFrameTest::InitLayoutValues(TFrameLayout& aLayout,TFrameFormat& aFormat, TSize& aSize, TInt& aRecommendedSize)
	{
	RImageFrameUtil::InitLayoutValues(aLayout, aFormat, aSize, aRecommendedSize);
	}
	
TInt RImageFrameTest::LayoutPlanes(const TFrameFormat& aFormat)
	{
	return RImageFrameUtil::LayoutPlanes(aFormat);
	}
	
//
// RImageFrameDecDesc - testing creating and acessing for reading and writing a CImageFrame
//
RImageFrameDecDesc* RImageFrameDecDesc::NewL(TBool aAllocTest)
	{
	RImageFrameDecDesc* self = new (ELeave) RImageFrameDecDesc(aAllocTest);
	return self;	
	}
	
RImageFrameDecDesc::RImageFrameDecDesc(TBool aAllocTest):RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0001-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0101-HP");
		}
	}
	
TVerdict RImageFrameDecDesc::DoTestL()
	{
	TVerdict result = EPass;
	
	__MM_HEAP_MARK;	
	
	// create a desc for the data
	HBufC8* buffer = HBufC8::NewMaxL(KImageSizeInBytes);
	CleanupStack::PushL(buffer);

	// create a new frame 
	TPtr8 ptr = buffer->Des();
	
  	INFO_PRINTF4(_L("Test %S size %d max size %d"), &iTestStepName, ptr.Length(), ptr.MaxLength());
	CImageFrame* frame = CImageFrame::NewL(ptr, ptr.MaxLength());
	CleanupStack::PushL(frame);

	// verify frame type 
	if (frame->IsChunk())
		{
		INFO_PRINTF2(_L("Test %S The frame type is incorrect - expected descriptor"), &iTestStepName);	
		result = EFail;
		}
	else
		{
		INFO_PRINTF2(_L("Test %S The frame type is correct - descriptor"), &iTestStepName);		
		}
		
	// set the content of the frame to uniform value 1 
	// verify all cells have the same value
	// start to end
	TBool outcome = ETrue;
	outcome = RImageFrameUtil::CheckUniformAssignment(*frame, KUniformValue1, KImageSizeInBytes, ETrue);
    if (!outcome)
    	{
    	INFO_PRINTF2(_L("Test %S Uniform forward assignment failed"), &iTestStepName);	
    	result = EFail;
    	}

	// set the content of the frame to uniform value 2 
	// verify all cells have the same value
	// end to front
	outcome = RImageFrameUtil::CheckUniformAssignment(*frame, KUniformValue2, KImageSizeInBytes, EFalse);
    if (!outcome)
    	{
    	INFO_PRINTF2(_L("Test %S Uniform backward assignment failed"), &iTestStepName);
    	result = EFail;
    	}

	// release all
	CleanupStack::PopAndDestroy(2,buffer);
	__MM_HEAP_MARKEND;
	
	return result;
	}

//
// RImageFrameDecChunk - testing creating and acessing for reading and writing a CImageFrame
//	
RImageFrameDecChunk* RImageFrameDecChunk::NewL(TBool aAllocTest)
	{
	RImageFrameDecChunk* self = new (ELeave) RImageFrameDecChunk(aAllocTest);
	return self;	
	}
	
RImageFrameDecChunk::RImageFrameDecChunk(TBool aAllocTest):RImageFrameTest(aAllocTest)
	{
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0002-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0102-HP");
		}
	}
	
TVerdict RImageFrameDecChunk::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;
	// we create a RChunk
	RChunk chunk;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, KImageSizeInBytes, KImageSizeInBytes, EOwnerProcess);	
	User::LeaveIfError(res);
  	CleanupClosePushL(chunk);
  	
  	// check chunk information
  	INFO_PRINTF4(_L("Test %S size %d max size %d"), &iTestStepName, chunk.Size(), chunk.MaxSize());
  	INFO_PRINTF4(_L("Test %S chunk is readable %d is writable %d"), &iTestStepName, chunk.IsReadable(), chunk.IsWritable());

	TFullName t;
	t = chunk.FullName();
	if (t.Compare(KGlobalChunkName)==0)
		{
		INFO_PRINTF3(_L("Test %S chunk name correct %S"), &iTestStepName, &t);
		}
	else 
		{
		INFO_PRINTF4(_L("Test %S chunk name unexpected %S exp %S"), &iTestStepName, &t, KGlobalChunkName);
		}
		
	chunk.SetRestrictions(KRChunkRestrictionFlags);

	// create a new frame using chunk
	CImageFrame* frame = CImageFrame::NewL(&chunk, KImageSizeInBytes, KRChunkDataOffset);
	CleanupStack::PushL(frame);
	
	// verify frame type and retrieve chunk and its name
	if (frame->IsChunk())
		{
		RChunk& chunk1 = frame->DataChunk();	
		
		CImageFrame* frame1 = frame;
		const RChunk& chunk1c = frame1->DataChunk();
		
		t = chunk1.FullName();
		if (t.Compare(KGlobalChunkName)==0)
			{
			INFO_PRINTF2(_L("Test %S chunk obtained and name verified"), &iTestStepName);
			}
		
		t = chunk1c.FullName();
		if (t.Compare(KGlobalChunkName)==0)
			{
			INFO_PRINTF2(_L("Test %S const chunk obtained and name verified"), &iTestStepName);
			}	
		}
	else
		{
		INFO_PRINTF2(_L("Test %S  The frame type is incorrect- expected chunk"),&iTestStepName);	
		result = EFail;
		}
	
	// get the offset and compare with the set value
	if (frame->DataOffset()!=KRChunkDataOffset)
		{
		result = EFail;
    	INFO_PRINTF4(_L("Test %S CImageFrame offset is wrong expected %d actual %d"), &iTestStepName, KRChunkDataOffset,frame->DataOffset() );
		}	
		
	// set the content of the frame to uniform value 1 
	// verify all cells have the same value
	// start to end
	TBool outcome = ETrue;
	outcome = RImageFrameUtil::CheckUniformAssignment(*frame, KUniformValue1, KImageSizeInBytes, ETrue);
    if (!outcome)
    	{
    	INFO_PRINTF2(_L("Test %S Uniform forward assignment failed"), &iTestStepName);	
    	result = EFail;
    	}

	// set the content of the frame to uniform value 2 
	// verify all cells have the same value
	// end to front
	outcome = RImageFrameUtil::CheckUniformAssignment(*frame, KUniformValue2, KImageSizeInBytes, EFalse);
    if (!outcome)
    	{
    	INFO_PRINTF2(_L("Test %S Uniform backward assignment failed"), &iTestStepName);
    	result = EFail;
    	}

	CleanupStack::PopAndDestroy(2, &chunk);
	__MM_HEAP_MARKEND;
	return result;
	}	
  
//
// RImageFrameEncDesc - testing creating and acessing the functionality of a CImageFrame
//
RImageFrameEncDesc* RImageFrameEncDesc::NewL(TBool aAllocTest)
	{
	RImageFrameEncDesc* self = new (ELeave) RImageFrameEncDesc(aAllocTest);
	return self;	
	}
	
RImageFrameEncDesc::RImageFrameEncDesc(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0003-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0103-HP");
		}
	}
	
TVerdict RImageFrameEncDesc::DoTestL()
	{
	TVerdict result = EPass;
		
	// create a new frame 
	TInt index = 1;
	TSize frameSize(KFrameSizeData[index][0], KFrameSizeData[index][1]);
	TInt frameBufferSize = frameSize.iWidth * frameSize.iHeight;
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	__MM_HEAP_MARK;
	// Create a desc
	HBufC8* buffer = HBufC8::NewMaxL(frameBufferSize);
	CleanupStack::PushL(buffer);
	TPtr8 ptr = buffer->Des();
	CImageFrame* frame = CImageFrame::NewL(ptr, frameBufferSize, frameSize, frameFormat,frameLayout);
	CleanupStack::PushL(frame);
	
	// verify frame type
	if (frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S CImageFrame wrong type"), &iTestStepName);
		}
		
	//	size
	frame->SetFrameSizeInPixels(frameSize);
    
	// set values again 
	frame->SetFrameFormatL(frameFormat);

	frame->SetFrameLayoutL(frameLayout);
	
    TSize frameSize1 = frame->FrameSizeInPixels();
    if (frameSize != frameSize1)
    	{
    	result = EFail;
    	INFO_PRINTF6(_L("Test %S different sizes test set %d %d got %d %d"), &iTestStepName,
    	frameSize.iWidth, frameSize.iHeight, frameSize1.iWidth, frameSize1.iHeight );
    	}
    const TFrameFormat& frameFormat1 = static_cast<const TFrameFormat&>(frame->FrameFormat());

    if (!RImageFrameUtil::FormatsEqual(frameFormat, frameFormat1))
    	{
    	result = EFail;
    	INFO_PRINTF4(_L("Test %S  different formats test set %0x  got %0x"), &iTestStepName,
    	frameFormat.FormatCode().iUid, frameFormat1.FormatCode().iUid );
    	}
    	
   const TFrameLayout& frameLayout1 = static_cast<const TFrameLayout&>(frame->FrameLayout());
   
    if (!(RImageFrameUtil::LayoutsEqual(frameLayout, frameLayout1)))
    	{
    	result = EFail;
    	INFO_PRINTF2(_L("Test %S  different layouts "), &iTestStepName );
    	}
    	
	// release all
	CleanupStack::PopAndDestroy(2, buffer);
	__MM_HEAP_MARKEND;
	
	return result;
	}

//
// RImageFrameEncChunk - testing creating and acessing the functionality of a CImageFrame
//	
RImageFrameEncChunk* RImageFrameEncChunk::NewL(TBool aAllocTest)
	{
	RImageFrameEncChunk* self = new (ELeave) RImageFrameEncChunk(aAllocTest);
	return self;	
	}
	
RImageFrameEncChunk::RImageFrameEncChunk(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0004-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0104-HP");
		}
	}
	
TVerdict RImageFrameEncChunk::DoTestL()
	{
	TVerdict result = EPass;
	// create a new frame 
	TInt index = 1;
	TSize frameSize(KFrameSizeData[index][0], KFrameSizeData[index][1]);
	TInt frameBufferSize = frameSize.iWidth * frameSize.iHeight;
	
	__MM_HEAP_MARK;
	//  create a RChunk
	RChunk chunk;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, frameBufferSize, frameBufferSize, EOwnerProcess);	
	User::LeaveIfError(res);
	
  	CleanupClosePushL(chunk);
  	
  	// chunk information
  	INFO_PRINTF4(_L("Test %S %d %d"), &iTestStepName, chunk.Size(), chunk.MaxSize());
  	INFO_PRINTF4(_L("Test %S  RChunk:: is readable %d is writable %d"), &iTestStepName, chunk.IsReadable(), chunk.IsWritable());
	
	TFullName t = chunk.FullName();
	if (t.Compare(KGlobalChunkName)==0)
		{
		INFO_PRINTF3(_L("Test %S name present %S"), &iTestStepName, &t);
		}

	chunk.SetRestrictions(KRChunkRestrictionFlags);

	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));

	CImageFrame* frame = CImageFrame::NewL(&chunk, frameBufferSize, KRChunkDataOffset, frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (!frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S CImageFrame wrong type"), &iTestStepName);
		}
	// get the offset and compare with the set value
	if (frame->DataOffset()!=KRChunkDataOffset)
		{
		result = EFail;
    	INFO_PRINTF4(_L("Test %S CImageFrame offset is wrong expected %d actual %d"), &iTestStepName, KRChunkDataOffset,frame->DataOffset() );
		}	

	// set values again 
	frame->SetFrameFormatL(frameFormat);

	frame->SetFrameSizeInPixels(frameSize);
	
	frame->SetFrameLayoutL(frameLayout);

	//	size
    TSize frameSize1 = frame->FrameSizeInPixels();
    if ( frameSize != frameSize1)
    	{
    	result = EFail;
    	INFO_PRINTF6(_L("Test %S different sizes test set %d %d got %d %d"), &iTestStepName,
    	frameSize.iWidth, frameSize.iHeight,frameSize1.iWidth, frameSize1.iHeight );
    	}

    const TFrameFormat& frameFormat1 = static_cast<const TFrameFormat&>(frame->FrameFormat());
    if (!RImageFrameUtil::FormatsEqual(frameFormat, frameFormat1))
    	{
    	result = EFail;
    	INFO_PRINTF4(_L("Test %S  different formats test set %0x got %0x"), &iTestStepName,
    	frameFormat.FormatCode().iUid, frameFormat1.FormatCode().iUid );
    	}
    
   	const TFrameLayout& frameLayout1 = static_cast<const TFrameLayout&>(frame->FrameLayout());
   
    if (!(RImageFrameUtil::LayoutsEqual(frameLayout, frameLayout1)))
    	{
    	result = EFail;
    	INFO_PRINTF2(_L("Test %S  different layouts "), &iTestStepName );
    	}		
    
	CleanupStack::PopAndDestroy(2, &chunk);
	__MM_HEAP_MARKEND;
	return result;
	}	

//
// RFormatTest - testing functionality of TFrameFormat class
//	
RFormatTest* RFormatTest::NewL(TBool aAllocTest)
	{
	RFormatTest* self = new (ELeave) RFormatTest(aAllocTest);
	return self;	
	}
	
RFormatTest::RFormatTest(TBool /*aAllocTest*/) 
	{

	iTestStepName = _L("MM-ICL-IMGFRM-U-0005-HP");

	}
	
TVerdict RFormatTest::DoTestStepL()
	{
	TVerdict result = EPass;
	TInt elements = sizeof(KFormatLists)/(3 * sizeof(TUid));
	
	__MM_HEAP_MARK;	
	// check the types
	for (TInt i = 0; i < elements; i++ )
		{
		TFrameFormat format(KFormatLists[i][0]);
		if ((format.FormatCode()!=KFormatLists[i][0]) ||
			(format.Sampling()!=KFormatLists[i][2]) ||
			(format.ColourSpace()!=KFormatLists[i][1]))
			{
			result = EFail;
			}
		}
		
	// check duplication
	TFrameFormat format(KFormatLists[0][0]);
	TFrameFormat* format1 = static_cast<TFrameFormat*>(format.DuplicateL());
	CleanupStack::PushL(format1);
	
	// check equivalence
	if (((format1->FormatCode()!=KFormatLists[0][0]) ||
		(format1->Sampling()!=KFormatLists[0][2]) ||
		(format1->ColourSpace()!=KFormatLists[0][1])))
			{
			result = EFail;
			}
	// 
	format1->SetColourSpace(KUidColourSpaceYUV);
	if (KUidColourSpaceYUV != format1->ColourSpace())
		{
		result = EFail;
		}
	CleanupStack::PopAndDestroy(1);	
	__MM_HEAP_MARKEND;
	
	return result;	
	}
	
/**********************************************/
//
// RYuvMonoProcDesc - testing creating and acessing for reading and writing a Mono image processor
//
RYuvMonoProcDesc* RYuvMonoProcDesc::NewL(TBool aAllocTest)
	{
	RYuvMonoProcDesc* self = new (ELeave) RYuvMonoProcDesc(aAllocTest);
	return self;	
	}
	
RYuvMonoProcDesc::RYuvMonoProcDesc(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0010-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0110-HP");
		}
	}
	
TVerdict RYuvMonoProcDesc::DoTestL()
	{
	TVerdict result = EPass;
	
	__MM_HEAP_MARK;		

	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCrMonoSize][0],KFrameTestSizeData[EYCbCrMonoSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);
	
	// create a desc
	HBufC8* buffer = HBufC8::NewMaxL(bufferSize);
	CleanupStack::PushL(buffer);
	
	// create a new frame 
	TPtr8 ptr = buffer->Des();
	CImageFrame* frame = CImageFrame::NewL(ptr, bufferSize, frameSize, frameFormat,frameLayout);
	CleanupStack::PushL(frame);
	
	// verify the type
	if (frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}
	
	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	CImageFrame& frame1 = processor->ImageFrame();
	// create a TDataUnit
    const TInt blocks = 1;
    TDataUnit testUnit[blocks];
    TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;

 	while (processor->MoreData())
 		{
 		// init test unit
 		j++;
		RImageFrameUtil::SetBlockToUniformValue(testUnit[0], ((j*10)&0xFF));
		
		processor->GetCurrentPosition(startPos);
		processor->WriteBlockL(testUnitPtr);
		
		processor->SetCurrentPosition(startPos);
		testUnit2[0] = processor->ReadBlockL();
	
		if (!RImageFrameUtil::CompareBlockValues(testUnit[0], (*testUnit2)[0]))
			{
			INFO_PRINTF2(_L("Test %S  Block values different"), &iTestStepName);
		    result = EFail;
			}
			
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName,
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

  	// release all
	CleanupStack::PopAndDestroy(4, buffer);
	
	__MM_HEAP_MARKEND;
	return result;
	}	
	
//
// RYuvMonoProcChunk - testing creating and acessing for reading and writing a Mono image processor
//	
RYuvMonoProcChunk* RYuvMonoProcChunk::NewL(TBool aAllocTest)
	{
	RYuvMonoProcChunk* self = new (ELeave) RYuvMonoProcChunk(aAllocTest);
	return self;	
	}
RYuvMonoProcChunk::RYuvMonoProcChunk(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0011-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0111-HP");
		}
	}
	
TVerdict RYuvMonoProcChunk::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
	
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCrMonoSize][0],KFrameTestSizeData[EYCbCrMonoSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	RChunk chunk;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, bufferSize, bufferSize, EOwnerProcess);	
	User::LeaveIfError(res);
	
  	CleanupClosePushL(chunk);
	// create a new frame 
	
	CImageFrame* frame = CImageFrame::NewL(&chunk, bufferSize,0, frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (!frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}

	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
    const TInt blocks = 1;
    TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;
   	res = KErrNone;
   	
 	while (processor->MoreData())
 		{
	 	// init test unit
	 	j++;
		RImageFrameUtil::SetBlockToUniformValue(testUnit[0], ((j*10)&0xFF));
		
		processor->GetCurrentPosition(startPos);
		processor->WriteBlockL(testUnitPtr);
	
		processor->SetCurrentPosition(startPos);
		testUnit2[0] = processor->ReadBlockL();
	
		if (!RImageFrameUtil::CompareBlockValues(testUnit[0], (*testUnit2)[0]))
			{
			INFO_PRINTF2(_L("Test %S  Block values different"), &iTestStepName);
		    result = EFail;
			}
			
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S  start position %d %d next %d %d"),  &iTestStepName,
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

    // release all
	CleanupStack::PopAndDestroy(4, &chunk);
	__MM_HEAP_MARKEND;
	return result;
	}	

//
// RYuv422IntlProcDesc - testing creating and acessing for reading and writing a YCbCr422 image processor
//
RYuv422IntlProcDesc* RYuv422IntlProcDesc::NewL(TBool aAllocTest)
	{
	RYuv422IntlProcDesc* self = new (ELeave) RYuv422IntlProcDesc(aAllocTest);
	return self;	
	}
	
RYuv422IntlProcDesc::RYuv422IntlProcDesc(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0012-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0112-HP");
		}
	}
	
TVerdict RYuv422IntlProcDesc::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
	
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCr422Int]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCr422IntSize][0],KFrameTestSizeData[EYCbCr422IntSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);
	

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	// we create a desc
	HBufC8* buffer = HBufC8::NewMaxL(bufferSize);
	CleanupStack::PushL(buffer);
	// create a new frame 
	
	TPtr8 ptr = buffer->Des();
	CImageFrame* frame = CImageFrame::NewL(ptr, bufferSize,frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}
		
 	const TInt blocks = 4;
	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
	// create a TDataUnit
	TDataUnit testUnit[blocks];
    TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	    
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;
   
   	while (processor->MoreData())
 		{
	 	// init test unit
	 	j++;
		for (TInt m=0; m < blocks; m++)
			{
			RImageFrameUtil::SetBlockToUniformValue(testUnit[m], (j*10+m)&0xFF);	
			}
			
		processor->GetCurrentPosition(startPos);
		
		processor->WriteBlockL(testUnitPtr);
		
		processor->SetCurrentPosition(startPos);
		
		testUnit2[0] = processor->ReadBlockL();
	
		for (TInt m = 0; m < blocks; m++)
			{
			TInt g = KExpectedBlocksValuesIndx[EYCbCr422IntSize][j-1];
			TInt f = KExpectedValuesIndx[g][m];
			if (!RImageFrameUtil::CompareBlockValues(testUnit[f], (*testUnit2)[m]))
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
			    result = EFail;
				}
			}
			
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName, 
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

	// release all
	CleanupStack::PopAndDestroy(4, buffer);
	__MM_HEAP_MARKEND;
	return result;
	}
		
//
// RYuv422IntlProcChunk - testing creating and acessing for reading and writing a YCbCr422 image processor
//	
RYuv422IntlProcChunk* RYuv422IntlProcChunk::NewL(TBool aAllocTest)
	{
	RYuv422IntlProcChunk* self = new (ELeave) RYuv422IntlProcChunk(aAllocTest);
	return self;	
	}
RYuv422IntlProcChunk::RYuv422IntlProcChunk(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0013-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0113-HP");
		}
	}
	
TVerdict RYuv422IntlProcChunk::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
	
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCr422Int]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCr422IntSize][0],KFrameTestSizeData[EYCbCr422IntSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);
	
	RChunk chunk;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, bufferSize, bufferSize, EOwnerProcess);	
	
	User::LeaveIfError(res);
	
  	CleanupClosePushL(chunk);
  	
	// create a new frame 
	CImageFrame* frame = CImageFrame::NewL(&chunk, bufferSize, 0, frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (!frame->IsChunk() )
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}
		
	const TInt blocks = 4;
	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
	// create a TDataUnit
	TDataUnit testUnit[blocks];
    TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;
   	
 	while (processor->MoreData())
 		{
 		j++;
		for (TInt m=0; m < blocks; m++)
			{
			RImageFrameUtil::SetBlockToUniformValue(testUnit[m], ((j*10+m)&0xFF));		
			}
	 	// init test unit
		processor->GetCurrentPosition(startPos);
		
		processor->WriteBlockL(testUnitPtr);

		processor->SetCurrentPosition(startPos);
 
 		testUnit2[0] = processor->ReadBlockL();
	
		for (TInt m = 0; m < blocks; m++)
			{
			TInt g = KExpectedBlocksValuesIndx[EYCbCr422IntSize][j-1];
			TInt f = KExpectedValuesIndx[g][m];

			if (!RImageFrameUtil::CompareBlockValues(testUnit[f], (*testUnit2)[m]))
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
			    result = EFail;
				}
			}
			
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName, 
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}


	// release all
	CleanupStack::PopAndDestroy(4, &chunk);
	__MM_HEAP_MARKEND;
	return result;
	}	

//
// RYuv420PlnProcDesc - testing creating and acessing for reading and writing a YCbCr420Planar Reversed image processor
//
RYuv420PlnProcDesc* RYuv420PlnProcDesc::NewL(TBool aAllocTest)
	{
	RYuv420PlnProcDesc* self = new (ELeave) RYuv420PlnProcDesc(aAllocTest);
	return self;	
	}
	
RYuv420PlnProcDesc::RYuv420PlnProcDesc(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0014-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0114-HP");
		}
	}
	
TVerdict RYuv420PlnProcDesc::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
	
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCr420PlaRev]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCr420PlaSize][0],KFrameTestSizeData[EYCbCr420PlaSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	// we create a desc
	HBufC8* buffer = HBufC8::NewMaxL(bufferSize);
	CleanupStack::PushL(buffer);
	// create a new frame 
	
	TPtr8 ptr = buffer->Des();
	CImageFrame* frame = CImageFrame::NewL(ptr, bufferSize,frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (frame->IsChunk())
		{
		result = EFail;
     	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}

	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
	// create a TDataUnit
	const TInt blocks = 6;
	TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);

    for(TInt i=0; i < blocks; i++)
    	{
    	testUnitPtr[i]=&testUnit[i];
    	}
    TInt j=0;

	TPoint startPos;
   	TPoint endPos;
   	
 	while (processor->MoreData())
 		{
		j++; 
	 	// init test unit
		processor->GetCurrentPosition(startPos);
	
		for (TInt m=0; m < blocks; m++)
			{
			RImageFrameUtil::SetBlockToUniformValue(testUnit[m], ((j*10+m)&0xFF));			
			}
			
		processor->WriteBlockL(testUnitPtr);
		
		processor->SetCurrentPosition(startPos);

		testUnit2[0] = processor->ReadBlockL();
		
		for (TInt m=0; m < blocks; m++)
			{
			TInt g = KExpectedBlocksValuesIndx[EYCbCr420PlaSize][j-1];
			TInt f = KExpectedValuesIndx[g][m];

			if (!RImageFrameUtil::CompareBlockValues(testUnit[f], (*testUnit2)[m]))
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
			    result = EFail;
				}
			}	
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName, 
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

	// release all
	CleanupStack::PopAndDestroy(4, buffer);
	
	__MM_HEAP_MARKEND;
	return result;
	}	
	
//
// RYuv420PlnProcChunk - testing creating and acessing for reading and writing a YCbCr420Planar Reversed image processor
//
RYuv420PlnProcChunk* RYuv420PlnProcChunk::NewL(TBool aAllocTest)
	{
	RYuv420PlnProcChunk* self = new (ELeave) RYuv420PlnProcChunk(aAllocTest);
	return self;	
	}
RYuv420PlnProcChunk::RYuv420PlnProcChunk(TBool aAllocTest) : RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0015-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0115-HP");
		}
	}
	
TVerdict RYuv420PlnProcChunk::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
	
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCr420PlaRev]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCr420PlaSize][0],KFrameTestSizeData[EYCbCr420PlaSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

   	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	RChunk chunk;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, bufferSize, bufferSize, EOwnerProcess);	

	User::LeaveIfError(res);
	
  	CleanupClosePushL(chunk);
	// create a new frame 
	
	CImageFrame* frame = CImageFrame::NewL(&chunk, bufferSize, 0, frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (!frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}

	const TInt blocks = 6;

	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
	// create a TDataUnit
	TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks]; 
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);   
      
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;
   
   	while (processor->MoreData())
 		{
	 	// init test unit
	 	j++;
		for (TInt m=0; m < blocks; m++)
			{
			RImageFrameUtil::SetBlockToUniformValue(testUnit[m], (j*10+m)&0xFF);			
			}
	 	// init test unit
		processor->GetCurrentPosition(startPos);

		processor->WriteBlockL(testUnitPtr);
		
		processor->SetCurrentPosition(startPos);
		 
		testUnit2[0] = processor->ReadBlockL();
		 
		for (TInt m=0; m < blocks; m++)
			{
			TInt g = KExpectedBlocksValuesIndx[EYCbCr420PlaSize][j-1];
			TInt f = KExpectedValuesIndx[g][m];

			if (!RImageFrameUtil::CompareBlockValues(testUnit[f], (*testUnit2)[m]))
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
			    result = EFail;
				}
			}	
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName, 
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

	// release all
	CleanupStack::PopAndDestroy(4, &chunk);

	__MM_HEAP_MARKEND;
	return result;
	}
//
// RYuv420PlnRevProcDesc - testing creating and acessing for reading and writing a YCbCr420Planar image processor
//
RYuv420PlnRevProcDesc* RYuv420PlnRevProcDesc::NewL(TBool aAllocTest)
	{
	RYuv420PlnRevProcDesc* self = new (ELeave) RYuv420PlnRevProcDesc(aAllocTest);
	return self;	
	}
	
RYuv420PlnRevProcDesc::RYuv420PlnRevProcDesc(TBool aAllocTest):RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0016-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0116-HP");
		}
	}
	
TVerdict RYuv420PlnRevProcDesc::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;
		
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCr420Pla]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCr420PlaSize][0],KFrameTestSizeData[EYCbCr420PlaSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	// we create a desc
	HBufC8* buffer = HBufC8::NewMaxL(bufferSize);
	CleanupStack::PushL(buffer);
	
	// create a new frame 
	TPtr8 ptr = buffer->Des();
	CImageFrame* frame = CImageFrame::NewL(ptr, bufferSize, frameSize, frameFormat,frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}

	const TInt blocks = 6;
	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
	// create a TDataUnit
	TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	    
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;
 	
 	while (processor->MoreData())
 		{
 		j++;
		for (TInt m=0; m < blocks; m++)
			{
			RImageFrameUtil::SetBlockToUniformValue(testUnit[m], ((j*10+m)&0xFF));			
			}

	 	// init test unit
		processor->GetCurrentPosition(startPos);
		 
		processor->WriteBlockL(testUnitPtr);

		processor->SetCurrentPosition(startPos);
 
 		testUnit2[0] = processor->ReadBlockL();
 		
 		for (TInt m=0; m < blocks; m++)
			{
			TInt g = KExpectedBlocksValuesIndx[EYCbCr420PlaSize][j-1];
			TInt f = KExpectedValuesIndx[g][m];

			if (!RImageFrameUtil::CompareBlockValues(testUnit[f], (*testUnit2)[m]))
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
			    result = EFail;
				}
			}
 
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName, 
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

    // release all
	CleanupStack::PopAndDestroy(4,buffer);
	
	__MM_HEAP_MARKEND;
	return result;
	}	

//
// RYuv420PlnRevProcChunk - testing creating and acessing for reading and writing a YCbCr420Planar(reversed) image processor
//
RYuv420PlnRevProcChunk* RYuv420PlnRevProcChunk::NewL(TBool aAllocTest)
	{
	RYuv420PlnRevProcChunk* self = new (ELeave) RYuv420PlnRevProcChunk(aAllocTest);
	return self;	
	}
RYuv420PlnRevProcChunk::RYuv420PlnRevProcChunk(TBool aAllocTest):RImageFrameTest(aAllocTest)
	{
	
	if (!iAllocTest)
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0017-HP");
		}
	else
		{
		iTestStepName = _L("MM-ICL-IMGFRM-U-0117-HP");
		}
	}
	
TVerdict RYuv420PlnRevProcChunk::DoTestL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
		
	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCr420Pla]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCr420PlaSize][0],KFrameTestSizeData[EYCbCr420PlaSize][1]);
	TFrameLayout frameLayout(LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	RChunk chunk;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, bufferSize, bufferSize, EOwnerProcess);	

	User::LeaveIfError(res);
	
  	CleanupClosePushL(chunk);
	// create a new frame 
	
	CImageFrame* frame = CImageFrame::NewL(&chunk, bufferSize, 0, frameSize, frameFormat,frameLayout);
	CleanupStack::PushL(frame);
	
	// get the type
	if (!frame->IsChunk())
		{
		result = EFail;
    	INFO_PRINTF2(_L("Test %S image frame wrong type "), &iTestStepName);
		}
 
 	const TInt blocks = 6;

	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	// create a TDataUnit
	TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks]; 
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
    
    TInt j=0;
	TPoint startPos;
   	TPoint endPos;
   	
 	while (processor->MoreData())
 		{
 		j++;
		for (TInt m=0; m < blocks; m++)
			{
			RImageFrameUtil::SetBlockToUniformValue(testUnit[m], ((j*10+m)&0xFF));		
			}
	 	// init test unit
		processor->GetCurrentPosition(startPos);
		processor->WriteBlockL(testUnitPtr);
		processor->SetCurrentPosition(startPos);
 
		testUnit2[0] = processor->ReadBlockL();
		
		for (TInt m = 0; m < blocks; m++)
			{
			TInt g = KExpectedBlocksValuesIndx[EYCbCr420PlaSize][j-1];
			TInt f = KExpectedValuesIndx[g][m];

			if (!RImageFrameUtil::CompareBlockValues(testUnit[f], (*testUnit2)[m]))
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
			    result = EFail;
				}
			}
		processor->GetCurrentPosition(endPos);
		INFO_PRINTF6(_L("Test %S start position %d %d next %d %d"), &iTestStepName, 
	    		startPos.iX, startPos.iY, endPos.iX, endPos.iY );
 		}

    
	// release all
	CleanupStack::PopAndDestroy(4,&chunk);
	__MM_HEAP_MARKEND;
	return result;
	}

/***************************************************************************/
//
// RImageFrameClip - testing creating and acessing for reading and writing with a Mono image processor but with 
// values some of which require clipping
//
RImageFrameClip* RImageFrameClip::NewL(TBool aAllocTest)
	{
	RImageFrameClip* self = new (ELeave) RImageFrameClip(aAllocTest);
	return self;	
	}
	
RImageFrameClip::RImageFrameClip(TBool /*aAllocTest*/) 
	{
	iTestStepName = _L("MM-ICL-IMGFRM-U-0018-HP");
	}

TVerdict RImageFrameClip::DoTestStepL()	
	{
	TVerdict result = EPass;
	
	__MM_HEAP_MARK;		

	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]); 
	TSize frameSize = TSize(8,8);
	TFrameLayout frameLayout(RImageFrameUtil::LayoutPlanes(frameFormat));
	
	TInt bufferSize;
	RImageFrameUtil::InitLayoutValues(frameLayout,frameFormat, frameSize, bufferSize);

 	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);
	
	// create a desc
	HBufC8* buffer = HBufC8::NewMaxL(bufferSize);
	CleanupStack::PushL(buffer);
	
	// create a new frame 
	TPtr8 ptr = buffer->Des();
	CImageFrame* frame = CImageFrame::NewL(ptr, bufferSize, frameSize, frameFormat, frameLayout);
	CleanupStack::PushL(frame);
	
	// create processor
	CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frame);
	CleanupStack::PushL(processor);
	
	// create a TDataUnit
    const TInt blocks = 1;
    TDataUnit testUnit[blocks];
    TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	    
	TPoint startPos;
   	TPoint endPos;

   	TInt elements = sizeof(KTestClipValues)/(sizeof(TInt)*2);
 	while (processor->MoreData())
 		{
 		// init test unit
		for (TInt j = 0; j < 64; j++)
			{
			testUnit[0].iCoeff[j] = KTestClipValues[j%elements][0];  
			}
			
		processor->GetCurrentPosition(startPos);
		
	    processor->WriteBlockL(testUnitPtr);
	    
		processor->SetCurrentPosition(startPos);

		testUnit2[0] = processor->ReadBlockL();
		for (TInt j = 0; j < 64; j++)
			{
			if ((*testUnit2[0]).iCoeff[j] != KTestClipValues[j%elements][1])
				{
				INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, j);
			    result = EFail;
				}
			
			}
 		}

  	// release all
	CleanupStack::PopAndDestroy(4, buffer);
	__MM_HEAP_MARKEND;
	return result;
	}	
	
/***************************************************************************/
//
// RImageFrameDescMultiFrame testing the creation and access of two CImageFrame objects sharing the 
// same descriptor.
//
RImageFrameDescMultiFrame* RImageFrameDescMultiFrame::NewL(TBool aAllocTest)
	{
	RImageFrameDescMultiFrame* self = new (ELeave) RImageFrameDescMultiFrame(aAllocTest);
	return self;	
	}
	
RImageFrameDescMultiFrame::RImageFrameDescMultiFrame(TBool /*aAllocTest*/)
	{
	iTestStepName = _L("MM-ICL-IMGFRM-U-0040-HP");
	}
	
TVerdict RImageFrameDescMultiFrame::DoTestStepL()
	{
	TVerdict result = EPass;
	// make sure mo memory leaks
	__MM_HEAP_MARK;	
	// image is predefined - we have several images at const offset.
	// very much akin to a raw video stream feed 
	// init general stuff
	TInt numFrames = KMultiFrameNumber;
	TInt frameOffsets = KRChunkDataOffset;

    TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCrMonoSize][0], KFrameTestSizeData[EYCbCrMonoSize][1]);
	TFrameLayout frameLayout(RImageFrameUtil::LayoutPlanes(frameFormat));
	
	TInt frameBufferSize;
	RImageFrameUtil::InitLayoutValues(frameLayout,frameFormat, frameSize, frameBufferSize);
   	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);
	// total size
	TInt bufferTSize = (frameOffsets + frameBufferSize) * numFrames;
	
	HBufC8* buffer = HBufC8::NewMaxL(bufferTSize);
	CleanupStack::PushL(buffer);
	
	// create a new frame 
	TPtr8 ptr = buffer->Des();
	
    // wrappers within the descriptor
	CImageFrame* frameArray[KMultiFrameNumber];
	TInt startingOffset = frameOffsets;
	
	for (TInt i = 0; i < numFrames; i++)
		{
		TPtr8 ptr1 = (ptr.MidTPtr(startingOffset));
		// maybe we want to have the offsets as well?
		CImageFrame* frame = CImageFrame::NewL(ptr1, frameBufferSize, frameSize, frameFormat, frameLayout);
		CleanupStack::PushL(frame);
		
		// check the type
		if (frame->IsChunk())
			{
			result = EFail;
	    	INFO_PRINTF2(_L("Test %S wrong type "), &iTestStepName);
			}
		startingOffset += frameBufferSize; 
		frameArray[i] = frame;
		}
	
	const TInt blocks = 1;

	// create a TDataUnit
	TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks]; 
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	
	// initialise to prevent warnings 
	testUnit2[0] = NULL;    
	TPoint startPos;
   	TPoint endPos;
   	
   	for (TInt frameIndx = 0; frameIndx < numFrames; frameIndx++)
   		{
   		startPos.SetXY(0,0);
   		// create processor	
   		CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frameArray[frameIndx]);
		CleanupStack::PushL(processor);

   		while (processor->MoreData())
	 		{
		 	// init test unit
			processor->GetCurrentPosition(startPos);
			
			for (TInt m=0; m < blocks; m++)
				{
				RImageFrameUtil::SetBlockToUniformValue(testUnit[m], 2+m);			
				}
			TInt res;	
		 	TRAP(res, processor->WriteBlockL(testUnitPtr));
		 	if (res!=KErrNone)
		 		{
		 		INFO_PRINTF3(_L("Test %S failed to write MCU result %d "), &iTestStepName, res);
		 		result = EFail;
		 		}
			processor->SetCurrentPosition(startPos);
			TRAP(res, testUnit2[0] = processor->ReadBlockL());
			if (res!=KErrNone)
		 		{
		 		INFO_PRINTF3(_L("Test %S failed to read MCU result %d "), &iTestStepName, res);
		 		result = EFail;
		 		}
		 		
		 	for (TInt m = 0; m < blocks; m++)
				{
				if (!RImageFrameUtil::CompareBlockValues(testUnit[m], (*testUnit2)[m]))
					{
					INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
				    result = EFail;
					}
				}
			
			processor->GetCurrentPosition(endPos);

	 		}
	 	INFO_PRINTF3(_L("Test %S Frame %d completed"), &iTestStepName, frameIndx);
	 	CleanupStack::PopAndDestroy(1);// the processor
   		}
 	
    // release all
	CleanupStack::PopAndDestroy(2 + numFrames, buffer);
	__MM_HEAP_MARKEND;
	return result;
	}

//
// RImageFrameRChunkMultiFrame testing the creation and access of two CImageFrame objects sharing the 
// same RChunk.
//
RImageFrameRChunkMultiFrame* RImageFrameRChunkMultiFrame::NewL(TBool aAllocTest)
	{
	RImageFrameRChunkMultiFrame* self = new (ELeave) RImageFrameRChunkMultiFrame(aAllocTest);
	return self;	
	}
	
RImageFrameRChunkMultiFrame::RImageFrameRChunkMultiFrame(TBool /*aAllocTest*/)
	{
	iTestStepName = _L("MM-ICL-IMGFRM-U-0041-HP");
	}

TVerdict RImageFrameRChunkMultiFrame::DoTestStepL()
	{
	TVerdict result = EPass;
	__MM_HEAP_MARK;	
	// image is predefined - we have several images at const offset.
	// very much akin to a raw video stream feed 
	// init general stuff
	TInt numFrames = KMultiFrameNumber;
	TInt frameOffsets = KRChunkDataOffset;

	TFrameFormat frameFormat(KFrameFormatDefaults[EYCbCrMono]); 
	TSize frameSize = TSize(KFrameTestSizeData[EYCbCrMonoSize][0],KFrameTestSizeData[EYCbCrMonoSize][1]);
	TFrameLayout frameLayout(RImageFrameUtil::LayoutPlanes(frameFormat));
	
	TInt frameBufferSize;
	RImageFrameUtil::InitLayoutValues(frameLayout,frameFormat, frameSize, frameBufferSize);

	INFO_PRINTF4(_L("Test %S Image size width %d height %d"), &iTestStepName, frameSize.iWidth, frameSize.iHeight);

	// chunk allocation
	RChunk chunk;
	TInt chunkSize = (frameOffsets + frameBufferSize) * numFrames;
	TInt res = chunk.CreateGlobal(KGlobalChunkName, chunkSize, chunkSize, EOwnerProcess);	
	if (res!=KErrNone)
		{
		INFO_PRINTF3(_L("Test %S failed to create an RChunk %d"), &iTestStepName, res);
		User::LeaveIfError(res);
		}

  	CleanupClosePushL(chunk);

    // wrappers within the chunk
	CImageFrame* frameArray[KMultiFrameNumber];
	TInt startingOffset = frameOffsets;
	
	for (TInt i = 0; i < numFrames; i++)
		{
		// create  frames 
		CImageFrame* frame = CImageFrame::NewL(&chunk, frameBufferSize, startingOffset, frameSize, frameFormat, frameLayout);
		CleanupStack::PushL(frame);
		
		// check the type
		if (!frame->IsChunk())
			{
			result = EFail;
	    	INFO_PRINTF1(_L("RImageFrameRChunkMultiFrame::wrong type "));
			}
		startingOffset += frameBufferSize;
		frameLayout.SetStart(0, startingOffset);
		frameArray[i] = frame;
		}
	
	const TInt blocks = 1;

	// create a TDataUnit
	TDataUnit testUnit[blocks];
	TDataUnit* testUnit2[blocks];
    RArray<const TDataUnit*> testUnitPtr;
	CleanupClosePushL(testUnitPtr);
	RImageFrameUtil::InitPointerValuesL(testUnit, testUnitPtr, blocks);
	
	// initialise to prevent warnings 
	testUnit2[0] = NULL;  
	TPoint startPos;
   	TPoint endPos;
   
   	for (TInt frameIndx = 0; frameIndx < numFrames; frameIndx++)
   		{
   		startPos.SetXY(0,0);	
		// create processor
   		CJpgImageFrameProcessor* processor = CJpgImageFrameProcessorUtility::NewL(*frameArray[frameIndx]);
		CleanupStack::PushL(processor);

   		while (processor->MoreData())
	 		{
		 	// init test unit
			processor->GetCurrentPosition(startPos);
			
			for (TInt m=0; m < blocks; m++)
				{
				RImageFrameUtil::SetBlockToUniformValue(testUnit[m], 2+m);			
				}
			TInt res;	
		 	TRAP(res, processor->WriteBlockL(testUnitPtr));
		 	if (res!=KErrNone)
		 		{
		 		INFO_PRINTF3(_L("Test %S failed to write MCU result %d "), &iTestStepName, res);
		 		result = EFail;
		 		}
			processor->SetCurrentPosition(startPos);
			
			TRAP(res,testUnit2[0] = processor->ReadBlockL());
			if (res!=KErrNone)
		 		{
		 		INFO_PRINTF3(_L("Test %S failed to read MCU result %d "), &iTestStepName, res);
		 		result = EFail;
		 		}
		 		
		 	for (TInt m = 0; m < blocks; m++)
				{
				if (!RImageFrameUtil::CompareBlockValues(testUnit[m], (*testUnit2)[m]))
					{
					INFO_PRINTF3(_L("Test %S Block values different unit %d"), &iTestStepName, m);
				    result = EFail;
					}
				}
		 		
			processor->GetCurrentPosition(endPos);
	 		}
	 	INFO_PRINTF3(_L("Test %S  Frame  %d completed"), &iTestStepName, frameIndx);
	 	CleanupStack::PopAndDestroy(1);// the processor
   		}
 	
	// release all
	CleanupStack::PopAndDestroy(2+numFrames,&chunk);
	__MM_HEAP_MARKEND;
	return result;
	}
