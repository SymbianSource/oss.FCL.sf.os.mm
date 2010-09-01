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
// JpegPerformanceTest.cpp
// 
//

#include <iclexif.h>
#include <hal.h>
#include <dptest.h> // Required to check for demand paging.

#include <imageframe.h>

#include "TImage.h"

_LIT(KEncodeFile, ":\\encode_perf_dest_test.jpeg");
_LIT(KEncodeTestOutFile, "c:\\teststreamout.jpg");

/**
 This function is to scale execution time according to 
 the current hardware. 
 Currently supported: H2, H4, H6 and emulator
 Base machine is H2
*/
TInt64 ScaleTimeByMachineL(TInt64 aExecTime, TInt aDestMedia = TPerformanceTestParams::EDstMediaRamDrive)
	{
	TInt value;
	User::LeaveIfError(HAL::Get(HAL::EMachineUid, value));
	TReal mulCoef = 1.0;
	
	switch (value)
		{
		case HAL::EMachineUid_OmapH2:
			mulCoef = 1.0;
			break;
			
		case HAL::EMachineUid_OmapH4:
			mulCoef = 0.7; // H2/H4 coef is in range 0.55<->0.66, so use 0.7
			break;
			
		case HAL::EMachineUid_Win32Emulator:
			mulCoef = 1.0;
			break;
			
		case HAL::EMachineUid_X86PC:
			mulCoef = 1.0;
			break;
			
		case HAL::EMachineUid_NE1_TB:
			mulCoef = 0.7;
			break;			
		
		case HAL::EMachineUid_OmapH6:
			mulCoef = 1.0;
			break; 	
			
		default:
			{
			User::Leave(KErrGeneral);
			// we're running on unknown hardware, we can't scale execution time
			// this test must be adjusted for that new hardware
			}
		}

	// Demand paging affects decode speed so account for it when measuring performance.
	// Allow another 10% to the encode/decode time.
	// Also, tests that access the MMC card need even more time.
	TUint32 flags = DPTest::ERomPaging | DPTest::ECodePaging | DPTest::EDataPaging;
	TBool demandpaging = (DPTest::Attributes() & flags);
	if (demandpaging)
		{
		#ifdef SYMBIAN_WRITABLE_DATA_PAGING
		if (aDestMedia == TPerformanceTestParams::EDstMediaMMCDrive)
			{
			mulCoef *= 4.0;
			}
		else
			{
			mulCoef *= 2.2;
			}
		#else
		if (aDestMedia == TPerformanceTestParams::EDstMediaMMCDrive)
			{
			mulCoef *= 2.0;
			}
		else
			{
			mulCoef *= 1.1;
			}
		#endif
		}
	
	return TInt64(mulCoef * TReal(aExecTime));
	}

void TImage::JpegDecoderPerformanceTestL(const TExifDataType& /*aFormat*/, const TExifOperation& /*aOperation*/)
	{
	
	iDecoderOptions = iPerfTstParams->iDecoderOptions;
	CJPEGExifDecoder* decoder=CreateDecoderOnFileL(TPtrC(iPerfTstParams->iSrcFileName));
	CleanupStack::PushL(decoder);

	TSize destSize;
	// calculate destination size according to the test params
	User::LeaveIfError( decoder->ReducedSize(decoder->FrameInfo().iOverallSizeInPixels, 
												iPerfTstParams->iReductionFactor, 
												destSize
											)
						);
						
	// choose a display mode for the dest. bitmap, 
	// use one provided wby the decoder if none specified.
	TDisplayMode displMode= (ENone == iPerfTstParams->iDisplayMode ? 
											decoder->FrameInfo().iFrameDisplayMode :
											iPerfTstParams->iDisplayMode
						);

	CFbsBitmap* dest=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(dest);
	User::LeaveIfError( dest->Create(destSize, displMode) );
	
	CActiveDecodeListener* listener= new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(listener);
	
	listener->InitialiseActiveListener(decoder);
	
	TTime start;
	start.HomeTime();
	// get current time and initiate decoding
	
	decoder->Convert(& listener->iStatus, *dest);
	CActiveScheduler::Start();
	
	TTime end;
	end.HomeTime();
	
	if (listener->iStatus.Int() != KErrNone)
		{
		User::Leave( listener->iStatus.Int() );
		}
	const TInt64 microSecDecodeTime=end.Int64() - start.Int64();
	Printf(_L("--Image has been decoded in %d microseconds"), I64INT(microSecDecodeTime) );
	// compare results to test params
	const TInt64 KScaledTime = ScaleTimeByMachineL(iPerfTstParams->iMaxDecodeTime);
	
	if (iPerfTstParams->iMaxDecodeTime && KScaledTime < microSecDecodeTime)
		{
		Printf(_L("--Desired decode time exceeded by %d microseconds"), 
						I64INT(microSecDecodeTime - KScaledTime) );
						
		TInt err;
#if defined(__WINS__)
		Printf(_L("WARNING: That may mean JPEG performance degradation"));
		Printf(_L("         check this test on a target hardware!"));
		err = KErrNone;
#else
		Printf(_L("ERROR: JPEG performance degradation has been detected."));
		err = KErrGeneral;
#endif
		User::LeaveIfError( err );
		}
	
	if (iPerfTstParams->iRefFileName != NULL)
		{
		TBool r=CompareBitmapsL(*dest, TPtrC(iPerfTstParams->iRefFileName));
		if (!r)
			{
			User::Leave(KErrGeneral);
			}
		}
	
	CleanupStack::PopAndDestroy(listener);
	CleanupStack::PopAndDestroy(dest);
	CleanupStack::PopAndDestroy(decoder);
	}

LOCAL_C
TChar GetDriveLetterL(RFs& aFs, TPerformanceTestParams::TDriveType aDriveType)
    {
    for (TInt drive=EDriveA;  drive < EDriveZ ; drive++)
        {
        TDriveInfo driveInfo;
        User::LeaveIfError( aFs.Drive(driveInfo, drive) );

        switch (aDriveType)
            {
            case TPerformanceTestParams::EDstMediaRamDrive:
                if ( driveInfo.iType == EMediaRam && (driveInfo.iDriveAtt & KDriveAttInternal))
                    {
                    return TChar( drive + 'A' );    
                    }
                break;
            case TPerformanceTestParams::EDstMediaMMCDrive:
                if ( driveInfo.iType == EMediaHardDisk && (driveInfo.iDriveAtt & KDriveAttRemovable))
                    {
                    return TChar( drive + 'A' );    
                    }            
            }
        
        }
    const TInt KDriveNotFound = -1004;
    User::Leave( KDriveNotFound );
    return TChar(0);
    }

void TImage::JpegEncoderPerformanceTestL(const TExifDataType& /*aFormat*/, const TExifOperation& /*aOperation*/)
	{
	
	iDecoderOptions = iPerfTstParams->iDecoderOptions;
	
	CActiveDecodeListener* listener= new (ELeave) CActiveDecodeListener();
	CleanupStack::PushL(listener);
	
	CJPEGExifDecoder* decoder=CreateDecoderOnFileL(TPtrC(iPerfTstParams->iSrcFileName));
	CleanupStack::PushL(decoder);

	TSize destSize;
	// calculate destination size according to the test params
	User::LeaveIfError( decoder->ReducedSize(decoder->FrameInfo().iOverallSizeInPixels, 
												iPerfTstParams->iReductionFactor, 
												destSize
											)
						);
						
	// choose a display mode for the dest. bitmap, 
	// use one provided wby the decoder if none specified.
	TDisplayMode displMode= (ENone == iPerfTstParams->iDisplayMode ? 
											decoder->FrameInfo().iFrameDisplayMode :
											iPerfTstParams->iDisplayMode
						);

	CFbsBitmap* dest=new (ELeave) CFbsBitmap();
	CleanupStack::PushL(dest);
	User::LeaveIfError( dest->Create(destSize, displMode) );
	
	listener->InitialiseActiveListener(decoder);
	decoder->Convert(&listener->iStatus, *dest);
	CActiveScheduler::Start();
	
	if (listener->iStatus.Int() != KErrNone)
	    {
	    Printf(_L("---ERROR: failed to decode a file err=%d"),listener->iStatus.Int());
	    User::Leave( listener->iStatus.Int() );
	    }
	    
    CFrameImageData* imgData = CFrameImageData::NewL();
    CleanupStack::PushL( imgData );
   
    TJpegImageData* jpegData = new (ELeave) TJpegImageData;
    CleanupDeletePushL(jpegData);
    jpegData->iSampleScheme = TJpegImageData::TColorSampling( iPerfTstParams->iEncoderFormat );
    jpegData->iQualityFactor= iPerfTstParams->iEncoderQuality;

    User::LeaveIfError( imgData->AppendImageData( jpegData ) );
    CleanupStack::Pop( jpegData );
    
    
    TFileName destFileName;
    destFileName.Append( GetDriveLetterL(iFs, TPerformanceTestParams::TDriveType(iPerfTstParams->iDstMediaType)  ) );
    destFileName.Append( KEncodeFile );
    
    Printf(_L("---Encode destination is '%S'"), &destFileName);
    iFs.Delete( destFileName ); // we ignore errors here

    CImageEncoder* encoder=CImageEncoder::FileNewL(iFs, destFileName, CImageEncoder::TOptions(iPerfTstParams->iEncoderOptions), KImageTypeJPGUid);
    CleanupStack::PushL(encoder);
    
	CActiveEncodeListener* encodeListener = new (ELeave) CActiveEncodeListener();
	CleanupStack::PushL(encodeListener);
    
    encodeListener->InitialiseActiveListener(encoder);
    
    TTime start;
    start.HomeTime();    

    encoder->Convert(&encodeListener->iStatus, *dest, imgData);

    CActiveScheduler::Start();
      
    TTime end;
    end.HomeTime();          

	if (iPerfTstParams->iRefFileName != NULL)
		{
		VerifyAgainstReferenceFile(destFileName, TFileName(iPerfTstParams->iRefFileName));
		}
    
	iFs.Delete( destFileName ); // we ignore errors here
	
	if (encodeListener->iStatus.Int() != KErrNone)
	    {
	    Printf(_L("---ERROR: failed to encode a file err=%d"),encodeListener->iStatus.Int());
	    User::Leave( encodeListener->iStatus.Int() );
	    }    
	
	const TInt64 microSecDecodeTime = end.Int64() - start.Int64();
	Printf(_L("--Image has been encoded in %d microseconds"), I64INT(microSecDecodeTime) );
	// compare results to test params
	const TInt64 KScaledTime = ScaleTimeByMachineL(iPerfTstParams->iMaxDecodeTime, iPerfTstParams->iDstMediaType);
	
	if (iPerfTstParams->iMaxDecodeTime && KScaledTime < microSecDecodeTime)
		{
		Printf(_L("--Desired encode time exceeded by %d microseconds"), 
						I64INT(microSecDecodeTime - KScaledTime) );
						
		TInt err;
#if defined(__WINS__)
		Printf(_L("WARNING: That may mean JPEG performance degradation"));
		Printf(_L("         check this test on a target hardware!"));
		err = KErrNone;
#else
		Printf(_L("ERROR: JPEG performance degradation has been detected."));
		err = KErrGeneral;
#endif
		User::LeaveIfError( err );
		}	
	
	CleanupStack::PopAndDestroy(6, listener);
	}

/*
 * Tests for the ImageFrame Plugin block wise sequential streaming with different block sizes.
 * Supported formats are YUV420,YUV422 and mono.
 */
void TImage::TestJpgSequentialStreamDecodeEncodePerformanceL(const TExifDataType& /*aDataType*/, const TExifOperation& /*aOperation*/)
	{

	Printf(_L("\n"));
	Printf(_L("--- DEF122727: Jpeg block stream: Missing EOI (End of Image) ---"));

    for (TInt i=1; i<=26; i++ )
		{
		RChunk chunk;
		TSize streamBlockSizeInPixels;
		TDecodeStreamCaps decodeCaps;
		TInt numBlocksRead = 0;
		TBool haveMoreBlocks = ETrue;
		
		//decode
		TPtrC fileName(iPerfTstParams->iSrcFileName); 
		CJPEGExifDecoder* jpegImageDecoder =CreateDecoderOnFileL(fileName);
		CleanupStack::PushL(jpegImageDecoder);

		//encode
		CImageEncoder* jpegImageEncoder = CImageEncoder::FileNewL(iFs, KEncodeTestOutFile, CImageEncoder::EOptionNone, KImageTypeJPGUid);
		CleanupStack::PushL(jpegImageEncoder);
		
		//initialize decode block streamer
		TImageConvStreamedDecode* streamDecode = jpegImageDecoder->BlockStreamerL();	
		
		TFrameInfo frameInfo = jpegImageDecoder->FrameInfo();
		TSize frameSizeInPixels(frameInfo.iOverallSizeInPixels);
		
		//get decoder capabilities
		TDecodeStreamCaps::TNavigation decodeNavigation = TDecodeStreamCaps::ENavigationSequentialForward;
		
		RArray<TUid> formats;
		TUid optimalFormat;

		CleanupClosePushL(formats);
		streamDecode->GetSupportedFormatsL(formats, optimalFormat);
		CleanupStack::PopAndDestroy();

		streamDecode->GetCapabilities(optimalFormat, 0, decodeCaps);
		
	    const CFrameImageData& frameDecImageData = jpegImageDecoder->FrameData(0);

	    const TImageDataBlock* imageDecData = frameDecImageData.GetImageData(0);
		const TJpegImageData* jpegDecImageData = static_cast<const TJpegImageData*>(imageDecData);

		TInt streamSizeInBlocks = 0;
		streamSizeInBlocks = frameInfo.iOverallSizeInPixels.iWidth * frameInfo.iOverallSizeInPixels.iHeight;

		switch (jpegDecImageData->iSampleScheme)
			{
			case TJpegImageData::EMonochrome:
				// 8 x 8
				streamSizeInBlocks /= 8 * 8;
				break;
			case TJpegImageData::EColor420:
				// 16 x 16
				streamSizeInBlocks /= 16 * 16;
				break;
			case TJpegImageData::EColor422:
				// 8 x 16
				streamSizeInBlocks /= 8 * 16;
				break;
			default:
				User::Leave(KErrNotSupported);
				break;
			}
		
		if ( i == 1 )
			Printf(_L("streamSizeInBlocks: %d"),streamSizeInBlocks);
			
		TInt numBlocksSoFar = 0;
		
		Printf(_L("OptimalBlocksPerRequest %d"),decodeCaps.OptimalBlocksPerRequest());
			
		//check that the decoder supports sequential navigation capabilities
		if(decodeCaps.Navigation() & TDecodeStreamCaps::ENavigationSequentialForward != decodeNavigation)
			{
			User::Leave(KErrNotSupported);
			}
		
		//set the navigation mode initialize decoder frame
		streamDecode->InitFrameL(optimalFormat, 0, decodeNavigation);
		
		TInt decodeBlockSizeInBytes = streamDecode->GetBufferSize(optimalFormat, streamBlockSizeInPixels, i);
				
		User::LeaveIfError(chunk.CreateLocal(decodeBlockSizeInBytes, decodeBlockSizeInBytes));
		CleanupClosePushL(chunk);
			
		//create an empty imageframe   
		CImageFrame* imageFrame = CImageFrame::NewL(&chunk, decodeBlockSizeInBytes, 0);
		CleanupStack::PushL(imageFrame);

		imageFrame->SetFrameSizeInPixels(streamBlockSizeInPixels);

		//initialize encode block streamer
		TImageConvStreamedEncode* streamEncode = jpegImageEncoder->BlockStreamerL();
		TEncodeStreamCaps encodeCaps;

		//get encode capabilities
		streamEncode->GetCapabilities(optimalFormat,encodeCaps);
		
		//create frame image data
		CFrameImageData* frameImageData = CFrameImageData::NewL();
		CleanupStack::PushL(frameImageData);
		
		TJpegImageData* jpegImageData = new(ELeave) TJpegImageData;		
		frameImageData->AppendImageData(jpegImageData);
		
		//initialize encoder frame
		TEncodeStreamCaps::TNavigation encodeNavigation = TEncodeStreamCaps::ENavigationSequentialForward;
		
		//check that the encoder supports sequential navigation capabilities
		if(encodeCaps.Navigation() & TEncodeStreamCaps::ENavigationSequentialForward != encodeNavigation)
			{
			User::Leave(KErrNotSupported);
			}

		streamEncode->InitFrameL(optimalFormat, 0, frameSizeInPixels, streamBlockSizeInPixels, encodeNavigation, NULL);
		
		CActiveDecodeListener* activeListener= new (ELeave) CActiveDecodeListener();
		CleanupStack::PushL(activeListener);

		CActiveEncodeListener* encodeListener= new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListener);

		TTime startStream;
		startStream.HomeTime();

		while(haveMoreBlocks)
			{

			activeListener->InitialiseActiveListener(jpegImageDecoder);
			
			// decoder get blocks
			streamDecode->GetNextBlocks(activeListener->iStatus, *imageFrame, i, numBlocksRead, haveMoreBlocks);

			CActiveScheduler::Start();
			User::LeaveIfError(activeListener->iStatus.Int()); // decode complete.
		
			encodeListener->InitialiseActiveListener(jpegImageEncoder);
			
			// encoder append blocks
			streamEncode->AppendBlocks(encodeListener->iStatus, *imageFrame, numBlocksRead);

			CActiveScheduler::Start();
			User::LeaveIfError(encodeListener->iStatus.Int()); // encode complete.

			numBlocksSoFar += numBlocksRead;
			
			}

		CleanupStack::PopAndDestroy(encodeListener);
		CleanupStack::PopAndDestroy(activeListener);
			
		CActiveEncodeListener* encodeListenerComplete= new (ELeave) CActiveEncodeListener();
		CleanupStack::PushL(encodeListenerComplete);
		
		encodeListenerComplete->InitialiseActiveListener(jpegImageEncoder);
		streamEncode->Complete(encodeListenerComplete->iStatus);
		CActiveScheduler::Start();
		User::LeaveIfError(encodeListenerComplete->iStatus.Int());
		
		TTime endStream;
		endStream.HomeTime();
		const TInt64 microSecDecodeTime=endStream.Int64() - startStream.Int64();
		Printf(_L("%S,Stream, %S, %d, microsec, %d, numblockstoget, %d,membuffsize"),&iTestName,&fileName,I64INT(microSecDecodeTime),i,decodeBlockSizeInBytes);

		CleanupStack::PopAndDestroy(encodeListenerComplete);  
		
		CleanupStack::PopAndDestroy(5, jpegImageDecoder); // frameImageData, imageFrame, chunk, jpegImageEncoder and jpegImageDecoder
		}
	}

