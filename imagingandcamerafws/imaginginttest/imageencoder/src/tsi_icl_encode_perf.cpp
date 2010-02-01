// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains test step class implementation for baseline performance integration tests for image encoding.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


// Test user includes
#include "tsi_icl_encode_perf.h"


// Read common input from the configuration file
TInt RStepBaseImageEncoderPerf::ReadCommonInputFromConfigFile()
    {
    // last 7 chars of the test case number is the name of section
    iSectionName = iTestStepName.Right(KSectionNameLen);
    
    TPtrC tmpFilename;
    if ((!GetStringFromConfig(iSectionName, _L("referenceimage"), tmpFilename)) ||
    	(!GetBoolFromConfig(iSectionName, _L("hwretainoutputimage"), iHwRetainOutputImage)) ||
    	(!GetBoolFromConfig(iSectionName, _L("writethumbnail"), iWriteThumbnail)) ||
    	(!GetBoolFromConfig(iSectionName, _L("alwaysthread"), iAlwaysThread)) ||
    	(!GetBoolFromConfig(iSectionName, _L("writeexif"), iWriteExif)) ||
    	(!GetIntFromConfig(iSectionName, _L("expectedtime"), iExpectedTime)))
        {
        return KErrNotFound;
        }
    iReferenceImage.Append(tmpFilename);

    if (!GetStringFromConfig(iSectionName, _L("outputimage"), tmpFilename))
		{
		return KErrNotFound;
		}
	iOutputImage.Append(tmpFilename);

	if (!GetStringFromConfig(_L("directory"), _L("outputpath"), iOutputPath))
		{
		return KErrNotFound;
		}
    if (!GetStringFromConfig(iSectionName, _L("inputimage"), tmpFilename))
        {
        return KErrNotFound;
        }
    iInputImage.Append(tmpFilename);
		
    return KErrNone;
    }

/*
 * Read and encode image from bitmap to file
 * 
 */
void RStepBaseImageEncoderPerf::EncodeImageFromBitmapToFileL()
	{
	TInt error = KErrNone;; 
	if (!iBitmaploaded )
		{
		error = iSrcBitmap->Load(iInputImage);
		if (error != KErrNone)
			{
			ERR_PRINTF2(_L("Cannot load input file into a bitmap, error = %d"), error);
			User::Leave(error);
			}
		}
	 
	// Prepare encode data
	PrepareEncodeDataL();

	// do conversion
	iActiveListener->InitialiseActiveListener();
	if (!iBitmaploaded) 
		{
		iStartTime.HomeTime(); // record start time for encoding			
		}
	iImageEncoder->Convert(&iActiveListener->iStatus, *iSrcBitmap, iFrameEnImageData);
	}

/* 
 * Read and encode image from bitmap to file with EXIF and thumbnail data
 * 
 */
void RStepBaseImageEncoderPerf::EncodeImageFromBitmapToFileWithExifDataL()
    {
	TInt res = KErrNone;;

	// Load the image
	if (!iBitmaploaded) 
		{
		res = iSrcBitmap->Load(iInputImage);
		if (res != KErrNone)
			{
			ERR_PRINTF2(_L("Cannot load input file into a bitmap, error = %d"), res);
			User::Leave(res);
			}
		}

	// Encode image without thumbnail
	// true ==> add thumbnail data, false ==> don't add thumbnail data
	if (!iWriteThumbnail)
		{
		iExifEncoder->SetThumbnail(EFalse);
		}

	// Switch encoder from JFIF (default) to EXIF
	iExifMetaData = iExifEncoder->ExifMetadata();

	// Add EXIF and thumbnail tags of each data type to the metadata
	res = SetExifAndThumbnailData(iExifMetaData);
	if (res != KErrNone)
		{
		ERR_PRINTF2(_L("Unable to set EXIF and/or thumbnail data to the image, error = %d"), res);
		User::Leave(res);
		}

	// Prepare encode data
	PrepareEncodeDataL();

	// do the conversion
	iActiveListener->InitialiseActiveListener();
	if (!iBitmaploaded) 
		{
		iStartTime.HomeTime(); // record start time for encoding			
		}
	iExifEncoder->Convert(&iActiveListener->iStatus, *iSrcBitmap, iFrameEnImageData);
	}

// Rotate and or mirror a bitmap
void RStepBaseImageEncoderPerf::RotateABitmapL()
	{
	// Get the mirror axis
	User::LeaveIfError(GetMirrorAngle(iMirrorAxis));
		
	if (!iBitmaploaded)
		{
		// Create srcbmp and rotator
		TInt err = iSrcBitmap->Load(iInputImage);
		CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Could not load the image to bitmap"));
		iStartTime.HomeTime();
		iBitmaploaded = ETrue;
		// Mirror the bitmap through the requisite axis
		iActiveListener->InitialiseActiveListener();
		iBmpRotator->Rotate(&iActiveListener->iStatus,*iSrcBitmap, iMirrorAxis);
		}
	else
		{
		// Mirror the bitmap through the requisite axis
		iActiveListener->InitialiseActiveListener();
		iBmpRotator->Rotate(&iActiveListener->iStatus,*iSrcBitmap, iMirrorAxis);
		}
	}

void RStepBaseImageEncoderPerf::DoKickoffTestL()
    {
    TInt error = KErrNone;
    // Read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());
    
    // Create output directory
	User::LeaveIfError(CreateOutputDirectory());

	TFileName tmpname;
	tmpname.Append(iOutputPath);
	tmpname.Append(iOutputImage);
	iOutputImage.Copy(tmpname);

	iActiveListener = new (ELeave) CActiveListener(this);
	iSrcBitmap = new (ELeave) CFbsBitmap;
	iRefBitmap = new (ELeave) CFbsBitmap;
    iBmpRotator = CBitmapRotator::NewL();
    
    iOptions = static_cast<CImageEncoder::TOptions> (CImageEncoder::EOptionNone);

    SetThreadForEncoding(iOptions); // set the option if encoding is performed in a separate thread
    
    iMimeType.Copy(KMimeTypeJpeg);
   
	//Log the test case description based on the test case scenario
	LogTestCaseDescription();
    TRAP(error, FsmL());
    if (error != KErrNone)
    	{
        StopTest(error, EFail);
        return;
    	}
    }

/*
 * Baseline performance tests to check Encoding with exif data 
 * and/orthumbnail data
 */
RStepEncodeImageWithExifdata* RStepEncodeImageWithExifdata::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepEncodeImageWithExifdata(aStepName, aTestCaseDescription));
    }

RStepEncodeImageWithExifdata::RStepEncodeImageWithExifdata(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageEncoderPerf(aStepName, aTestCaseDescription)
    {
    }

void RStepEncodeImageWithExifdata::FsmL()
    {
    TInt err = KErrNone;
    switch (iState)
        {
        case 0:// Encoding image with/without exif data
            CreateImageEncoderL();
       		if (iWriteExif == EFalse && iWriteThumbnail == EFalse)
				{
				EncodeImageFromBitmapToFileL();                 // encode image from bitmap to file
				}
            else
                {
                EncodeImageFromBitmapToFileWithExifDataL();     // encode image from bitmap to file with exif data
                }
            StateChange();
            break;

        case 1:// check status of encoding
            {
            iEndTime.HomeTime(); // record end time for encoding
            err = iActiveListener->iStatus.Int();            
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Encode unsuccessful"));
			INFO_PRINTF1(_L("Encode successful!"));
			CompareFileL();
            CheckPerformanceOutputAndSaveL();                   // Check performance based on time and log the result 
            StopTest(EPass);
            break;
            }
            
        default:
                ERR_PRINTF2(_L("Invalid state, %d"), iState);
                User::Leave(KErrGeneral);
        }
    }

/*
 * Baseline performance tests to check Mirror then Encoding
 * with exif data and /or thumbnail data
 */
RStepMirrorThenEncodeToImage* RStepMirrorThenEncodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepMirrorThenEncodeToImage(aStepName, aTestCaseDescription));
    }

RStepMirrorThenEncodeToImage::RStepMirrorThenEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageEncoderPerf(aStepName, aTestCaseDescription)
    {
    }

// Read input from the configuration file
TInt RStepMirrorThenEncodeToImage::ReadInputFromConfigFile()
	{
	if (!GetIntFromConfig(iSectionName, _L("mirroringindex"), iMirroringIndex))
		{
		return KErrNotFound;
		}
	return KErrNone;
	}

void RStepMirrorThenEncodeToImage::FsmL()
    {
    TInt err = KErrNone;;
    switch (iState)
        {
        case 0:// rotate/mirror a bitmap
			User::LeaveIfError(ReadInputFromConfigFile());
            CreateImageEncoderL();
			iIndex = iMirroringIndex;
            RotateABitmapL();                               // Rotate and or mirror a bitmap
            StateChange();
            break;

        case 1:// check status of rotate/mirror then encode
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Rotation and/or Mirror during Encode unsuccessful"));           
			
            INFO_PRINTF1(_L("Rotate and /or Mirror during Encode successful!"));
			if (iWriteExif == EFalse && iWriteThumbnail == EFalse)
				{
				EncodeImageFromBitmapToFileL();             // encode image from bitmap to file
				}
			else
				{
				EncodeImageFromBitmapToFileWithExifDataL(); // encode image from bitmap to file with exif data 
				}
			StateChange();
			break;
			               
        case 2:// check status and time taken for encoding
            {
            iEndTime.HomeTime(); // record end time for encoding
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err, _L("Encode unsuccessful"));
			INFO_PRINTF1(_L("Encode successful!"));
			CompareFileL();
            CheckPerformanceOutputAndSaveL();               // Check performance based on time and log the result 
            StopTest(EPass);
            break;
            }
        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }

/*
 * Baseline performance tests to check Rotate and/or Mirror then Encoding
 *
 */
RStepRotateMirrorThenEncodeToImage* RStepRotateMirrorThenEncodeToImage::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepRotateMirrorThenEncodeToImage(aStepName, aTestCaseDescription));
    }

RStepRotateMirrorThenEncodeToImage::RStepRotateMirrorThenEncodeToImage(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageEncoderPerf(aStepName, aTestCaseDescription)
    {
    }

// Read input from the configuration file
TInt RStepRotateMirrorThenEncodeToImage::ReadInputFromConfigFile()
	{
	if (!GetIntFromConfig(iSectionName, _L("operationorder"), iOperationIndex) ||
		!GetIntFromConfig(iSectionName, _L("rotationindex"), iRotationIndex))
		{
		return KErrNotFound;
		}	
	if (iOperationIndex == 1) //0 if rotateonly , 1 if rotate and mirror order, 2 if rotate , mirrorvertical and/or  mirror horizontal order
		{
		if (!GetIntFromConfig(iSectionName, _L("mirroringindex"), iMirroringIndex))
			{
			return KErrNotFound;
			}
		}
	else if (iOperationIndex == 2)
		{
		if (!GetIntFromConfig(iSectionName, _L("mirroringindex1"), iMirrorIndex1) ||
			!GetIntFromConfig(iSectionName, _L("mirroringindex2"), iMirrorIndex2))
			{
			return KErrNotFound;
			}
		}
	return KErrNone;
	}

void RStepRotateMirrorThenEncodeToImage::FsmL()
    {
    TInt err = KErrNone;;
    switch (iState)
        {
        case 0:// rotate/mirror a bitmap
			User::LeaveIfError(ReadInputFromConfigFile());
            CreateImageEncoderL();
			iIndex = iRotationIndex;
            RotateABitmapL();                               // Rotate and or mirror a bitmap
            StateChange();
            break;

        case 1:// check status and apply rotate/mirror to bitmap
			{
			err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Rotation and/or Mirror during Encode unsuccessful"));
            // apply rotate to bitmap
            if (iOperationIndex == 1)
				{
				iIndex = iMirroringIndex;
				iState = 3;
				}
            // apply rotate and mirror to bitmap				
			else if (iOperationIndex == 2)
				{
				iIndex = iMirrorIndex1;
				StateChange();
				}
			RotateABitmapL();                               // Rotate and or mirror a bitmap
			break;								
    		}
    		
        case 2:// check status and apply rotate/mirror to bitmap
        	err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Rotation and/or Mirror during Encode unsuccessful"));
			iIndex = iMirrorIndex2;
			RotateABitmapL();                               // Rotate and or mirror a bitmap
			StateChange();
			break;
			
        case 3:// check status of rotate/mirror then encoding
    		err = iActiveListener->iStatus.Int();
    		CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Rotation and/or Mirror during Encode unsuccessful"));
            INFO_PRINTF1(_L("Rotation and/or Mirror during Encode successful!"));
			if (iWriteExif == EFalse && iWriteThumbnail == EFalse)
				{
				EncodeImageFromBitmapToFileL();             // encode image from bitmap to file
				}
			else
				{
				EncodeImageFromBitmapToFileWithExifDataL(); // encode image from bitmap to file with exif data
				}
			StateChange();
			break;
			
        case 4:// check status and time taken for encoding
            {
            iEndTime.HomeTime(); // record end time for encoding
            err = iActiveListener->iStatus.Int();
            CHECK_STATUS_LEAVE_IF_ERROR(err,_L("Encode unsuccessful"));
			INFO_PRINTF1(_L("Encode successful!"));
			CompareFileL();
			CheckPerformanceOutputAndSaveL();               // Check performance based on time and log the result 
            StopTest(EPass);
            break;
            }
            
        default:
            ERR_PRINTF2(_L("Invalid state, %d"), iState);
            User::Leave(KErrGeneral);
        }
    }
