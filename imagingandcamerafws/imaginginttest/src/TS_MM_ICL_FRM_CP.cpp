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
// This file contains the test steps ICL testing
// 
//

// EPOC includes
#include <e32base.h>

// Test system includes
#include <testframework.h>

// Specific includes for this test suite
#include "TestSuite_TSI_ICL_FRM_00.h"

// Specific includes for these test steps
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "TS_MM_ICL_FRM_CP.h"
// --------------------------------------------

/**
 * 
 *
 * Create a RTest_MM_ICL_FRM_CP class
 *
 *	const TDesC& aTestName - step's name
 *	Each test step initialises its own name.
 */

	CTest_MM_ICL_FRM_CP::CTest_MM_ICL_FRM_CP(const TDesC& aTestName) :
	iActiveScheduler(NULL),
	iBitmap(NULL)
		{
		iImageEncoder = NULL;
		iImageDecoder = NULL;
		
		iTestStepName = aTestName;
		iHeapSize = 0x10000 * 4;
		}

/**
DoTestStepPreambleL is the preamble function for test steps
It creates an active scheduler and installs it. This is necessary
since we are multithreading and each test step runs in a separate thread.
@returns TVerdict which is either EPass or Efail
 
The function can leave if there is insufficient memeory

@lib "TSU_ICL_FRM_01.lib"
*/
TVerdict CTest_MM_ICL_FRM_CP::DoTestStepPreambleL(void)
	{

	ReadConfig();	

	//define drive
	RFs theFs;
	User::LeaveIfError(theFs.Connect());
	TBuf<KLenBuffer>	thePath;
	Directory(EOutput, thePath);
	TInt theErr = theFs.MkDirAll(thePath);
	theFs.Close();
	
	if((theErr != KErrAlreadyExists) && (theErr != KErrNone))
		{
		ERR_PRINTF2(_L("Cannot create output directory = %S"), &thePath ); 
		return EInconclusive;

		}

	iActiveScheduler = new(ELeave)CTestActiveScheduler;
    //[ push the scheduler on the stack ]
	CleanupStack::PushL( iActiveScheduler );
	
	//[install the active scheduler ]
	CActiveScheduler::Install( iActiveScheduler );
	
	// Pop the element from the stack
	CleanupStack::Pop(1);


	return EPass;
	}

/**
The post amble step cleans up the allocated memory in the preamble
@returns TVerdict EPass or EFail
@leaves can leave due to insufficient memory
@lib "TSU_ICL_FRM_01.lib"
*/

TVerdict CTest_MM_ICL_FRM_CP::DoTestStepPostambleL(void)
	{
	
	//[ Destroy the scheduler ]
	if(iActiveScheduler != NULL)
		{
		delete iActiveScheduler ;
		iActiveScheduler = NULL;	
		}
	
	if(iImageEncoder != NULL)
		{	
		delete	iImageEncoder;
		iImageEncoder = NULL;
		}
		
	if(iImageDecoder != NULL)
		{	
		delete  iImageDecoder;
		iImageDecoder = NULL; 
		}
		
	iFile.Close();
	iFs.Close();
	
	if(iBitmap != NULL)
		{	
		delete iBitmap;
		iBitmap = NULL;
		}

	return EPass;
	}


/* 
 *
 * Compare two bitmaps in memory 
 *
 * @param     "CFbsBitmap& aBitmap"
 *            A reference to the bitmap on which it will compare
 *			  "const TDesC& aFileName"
 *            A reference to the filename for compare
 * @return    "TInt"
 *			  Error
 */
TInt CTest_MM_ICL_FRM_CP::CheckBitmapL(CFbsBitmap& aBitmap, const TDesC& aFileName)
	{
	TInt result = KErrNone ;

	CFbsBitmap *aRefBitmap;
	aRefBitmap = new(ELeave) CFbsBitmap;


	INFO_PRINTF2(_L("Comparing with file  = %S"), &aFileName);
	
	TRAP(result, result = aRefBitmap->Load(aFileName));
	if(result != KErrNone)
		{
		delete	aRefBitmap;
		aRefBitmap = NULL;

		INFO_PRINTF2(_L("Cannot open file  = %S"), &aFileName);
		return result;
		}
	
	const TSize size = aBitmap.SizeInPixels();
	const TSize size1 = aRefBitmap->SizeInPixels();


	if(size != size1)
		{
		INFO_PRINTF5(_L("The source and reference files have different size, (%d, %d), (%d, %d)"),
			size.iWidth, size.iHeight, size1.iWidth, size1.iHeight);
		delete aRefBitmap;
		return KErrNotIdentical;
		}

	//
	int nAllPixel = 0; // all pixel in picture
	int nIdentPixel = 0; // only identical

	for (TInt y = 0; y < size.iHeight; y++)
		{
		for (TInt x = 0; x < size.iWidth; x++)
			{
			TPoint pt(x,y);
			TRgb color1,color2;
			aBitmap.GetPixel(color1,pt);
			aRefBitmap->GetPixel(color2,pt);
			nAllPixel++;	
			if (color1 != color2)
				{
				//INFO_PRINTF1(_L("Colour difference detected!"));
				//delete aRefBitmap;
				//return KErrNotIdentical;
				}
			else 
				nIdentPixel++;
			}
		} // end of comparison for-loop

	if(nAllPixel == nIdentPixel)
		INFO_PRINTF1(_L("The bitmaps are identical"));
	else
		{
		result = KErrNotIdentical;
		if(nAllPixel != 0)
			INFO_PRINTF2(_L("The bitmaps are not identical, identical : %f"), (TReal32) nIdentPixel / nAllPixel);
		else
			INFO_PRINTF1(_L("The bitmaps are not identical"));
		}
	delete aRefBitmap;
	return result;
	}


/* 
 *
 * Bitwise compare two file with buffer 
 *
 * @param		"const TDesC& aFilename"
 *				A reference to the file on which it will compare
 *				"HBufC8	*theDestinationImage"
 *				pointer to image's buffer
 *				"TUid		aTypeImag"	
 *				codec   Id
 *				"TPlaceType			aDirectory"
 *				where file places
 *				
 * @return		"const TDesC& aFileName"
 *				A reference to the filename for compare
 */
TInt CTest_MM_ICL_FRM_CP::CompareFileL(const TDesC& aFilename, 
													HBufC8	*theDestinationImage,
													TUid		aTypeImage,
													TPlaceType			aDirectory)
	{
	TInt	theRes = KErrNone;

	if(theDestinationImage == NULL)
		return KErrPathNotFound;
	
	TBuf<KLenBufferExtension>			imageExtension;
	//

	TBuf<KLenBuffer> compFileName;
	
	Directory(aDirectory, compFileName);

	compFileName.Append(aFilename.Left(aFilename.Length()-3));
	GetImageExtension(aTypeImage, imageExtension);
	compFileName.Append(imageExtension);
	//	
	
	TInt	nLengthDest = theDestinationImage->Des().Length(); 
	TPtr8 imageDestinationPtr = theDestinationImage->Des();
	
	User::LeaveIfError(iFs.Connect());
	TInt fileSize;

	theRes = iFile.Open(iFs, compFileName, EFileRead);
	if(theRes != KErrNone)
		{
		INFO_PRINTF2(_L("Cannot open file: %S to compare"), &compFileName);
		iFs.Close();
		return theRes;
		}
	
	iFile.Size(fileSize);
	HBufC8 *theImageFromFile = HBufC8::NewMaxL(fileSize);

	TPtr8 imageFromFilePtr = theImageFromFile->Des();
	iFile.Read(imageFromFilePtr);
	iFile.Close();
	
	
	INFO_PRINTF2(_L("Comparing  with file : %S"), &compFileName);

	if(nLengthDest == fileSize)
		{	
	
	//
		theRes = Mem::Compare(imageDestinationPtr.Ptr(), 
						nLengthDest, imageFromFilePtr.Ptr(), fileSize);
		if(theRes != 0)
			theRes = KErrNotIdentical;
	//
		}
		else
			{
			theRes = KErrNotIdentical;
			/*RFile f;
			f.Create(iFs, _L("c:\\err"), EFileWrite);
			f.Write(*theDestinationImage);
			f.Close();*/
			}
	iFs.Close();		

	
	if(theRes == KErrNone)
		INFO_PRINTF1(_L("Files are identical"));
	else
		INFO_PRINTF1(_L("Files are not identical"));
		
	delete	theImageFromFile;
	theImageFromFile = NULL;

	return theRes;
	}

/* 
 *
 * Convert struct  TDisplayMode to descriptor
 *
 * @param		"TDisplayMode	aDisplayMode"
 *				display mode
 *				"TDes& aDisplayModeDes"
 *				Destination descriptor
 * @return		"TAny"
 *				
 */
void CTest_MM_ICL_FRM_CP::FrameDisplayMode(TDisplayMode	aDisplayMode, 
										   TDes& aDisplayModeDes)
	{
	switch(aDisplayMode)
		{
		case ENone:
			{
			aDisplayModeDes.Copy(_L("none mode"));
			break;
			}
		case EGray2:
			{
			aDisplayModeDes.Copy(_L("monohrom mode, 1 bpp"));
			break;
			}
		case EGray4:
			{
			aDisplayModeDes.Copy(_L("grayscales mode, 2 bpp"));
			break;
			}
		case EGray16:
			{
			aDisplayModeDes.Copy(_L("grayscales mode, 4 bpp"));
			break;
			}
		case EGray256:
			{
			aDisplayModeDes.Copy(_L("grayscales mode, 8 bpp"));
			break;
			}
		case EColor16:
			{
			aDisplayModeDes.Copy(_L("colour mode 4 bpp"));
			break;
			}
		case EColor256:
			{
			aDisplayModeDes.Copy(_L("colour mode 8 bpp"));
			break;
			}
		case EColor64K:
			{
			aDisplayModeDes.Copy(_L("colour mode 16 bpp"));
			break;
			}
		case EColor16M:
			{
			aDisplayModeDes.Copy(_L("colour mode 24 bpp"));
			break;
			}
		default:
		case ERgb:
			{
			aDisplayModeDes.Copy(_L("Not a known display mode"));
			break;
			}
		case EColor4K:
			{
			aDisplayModeDes.Copy(_L("colour mode 12 bpp"));
			break;
			}

		}
	}


/* 
 *
 * Convert codec Uid to file's extension
 *
 * @param		"TUid	aTypeImageUid"
 *				codec Uid 
 *				"TDes& imageExtension"
 *				Destination descriptor
 * @return		"TAny"
 *				
 */
void CTest_MM_ICL_FRM_CP::GetImageExtension(TUid	aTypeImageUid, 
											TDes& imageExtension)
	{
	if(aTypeImageUid == KImageTypeOTAUid)
			imageExtension.Copy(_L("ota"));
	else if(aTypeImageUid == KImageTypeWBMPUid)
			imageExtension.Copy(_L("wbmp"));
	else if(aTypeImageUid == KImageTypePNGUid)
			imageExtension.Copy(_L("png"));
	else if(aTypeImageUid == KImageTypeWMFUid)
			imageExtension.Copy(_L("wmf"));
	else if(aTypeImageUid == KImageTypeMBMUid)
			imageExtension.Copy(_L("mbm"));
	else if(aTypeImageUid == KImageTypeBMPUid)
			imageExtension.Copy(_L("bmp"));
	else if(aTypeImageUid == KImageTypeGIFUid)
			imageExtension.Copy(_L("gif"));
	else if(aTypeImageUid == KImageTypeJPGUid)
			imageExtension.Copy(_L("jpg"));
	else if(aTypeImageUid == KImageTypeTIFFUid)
			imageExtension.Copy(_L("tif"));
	else if(aTypeImageUid == KImageTypeICOUid)
			imageExtension.Copy(_L("ico"));
	else if(aTypeImageUid == KImageTypePPMUid)
			{
			imageExtension.Copy(_L("ppm"));
			}		
	}


/* 
 *
 * Read configuration file
 *				
 */
TAny	CTest_MM_ICL_FRM_CP::ReadConfig()
	{
	GetStringFromConfig(_L("Directory"), _L("input"), iInputDir);
	GetStringFromConfig(_L("Directory"), _L("output"), iOutputDir);
	GetStringFromConfig(_L("Directory"), _L("reference"), iRefDir);

	TBuf<KLenBuffer>	nameSection;
	
	TPtrC ptr = iTestStepName;
	nameSection = ptr.Right(KLenNameInSection);
	
	GetStringFromConfig(nameSection, _L("file"), iFileNameSrc);
	
	TPtrC	codec;
	GetStringFromConfig(nameSection, _L("codec"), codec);
	
	iImageTypeUid.Null();
	GetImageTypeUid(codec);
	}

/* 
 *
 * Read configuration file
 *				
 */
void CTest_MM_ICL_FRM_CP::ReadMimeTypeFromConfig(TDes8& aMimeType)
	{
	TBuf<KLenBuffer> nameSection;
	
	TPtrC ptr = iTestStepName;
	nameSection = ptr.Right(KLenNameInSection);
	
	TPtrC mimeType;
	GetStringFromConfig(nameSection, _L("mime"), mimeType);
	aMimeType.Copy(mimeType);
	}


void CTest_MM_ICL_FRM_CP::ReadDecoderUidFromConfig(TUid& aUid)
	{
	TBuf<KLenBuffer> nameSection;
	
	TPtrC ptr = iTestStepName;
	nameSection = ptr.Right(KLenNameInSection);
	
	TPtrC decoder;
	GetStringFromConfig(nameSection, _L("decoder"), decoder);

	if(decoder.Compare(_L("OTA")) == 0)
		aUid = TUid::Uid(KOTADecoderImplementationUidValue);
	else
		ASSERT(EFalse);
	}

/* 
 *
 * Convert codec name to codec Uid, and set internal member iImageTypeUid
 *
 * @param		"const TPtrC	aCodecName"
 *				codec name 
 *				
 * @return		"TAny"
 *				
 */
TAny	CTest_MM_ICL_FRM_CP::GetImageTypeUid(const TPtrC	aCodecName)
	{
	if(aCodecName.Compare(_L("JPEG")) == 0)
		iImageTypeUid = KImageTypeJPGUid;
	else if(aCodecName.Compare(_L("BMP")) == 0)
		iImageTypeUid = KImageTypeBMPUid;
	else if(aCodecName.Compare(_L("GIF")) == 0)
			iImageTypeUid = KImageTypeGIFUid;
	else if(aCodecName.Compare(_L("WMF")) == 0)
		iImageTypeUid = KImageTypeWMFUid;
	else if(aCodecName.Compare(_L("TIFF")) == 0)
		iImageTypeUid = KImageTypeTIFFUid;
	else if(aCodecName.Compare(_L("PNG")) == 0)
		iImageTypeUid = KImageTypePNGUid;
	else if(aCodecName.Compare(_L("MBM")) == 0)
		iImageTypeUid = KImageTypeMBMUid;
	else if(aCodecName.Compare(_L("WBMP")) == 0)
		iImageTypeUid = KImageTypeWBMPUid;
	else if(aCodecName.Compare(_L("OTA")) == 0)
		iImageTypeUid = KImageTypeOTAUid;
	else if(aCodecName.Compare(_L("ICO")) == 0)
		iImageTypeUid = KImageTypeICOUid;
	else if(aCodecName.Compare(_L("PPM")) == 0)
		{
		iImageTypeUid = KImageTypePPMUid;
		}
	}


/* 
 *
 * Get directory according struct TPlaceType
 *
 * @param		"TPlaceType	aPlace"
 *				"TDes& imagePlace"
 *				
 * @return		"TAny"
 *				
 */
TAny	CTest_MM_ICL_FRM_CP::Directory( TPlaceType	aPlace, TDes& imagePlace)
	{


	switch(aPlace)
		{
		case EInput:
#ifdef __WINS__
	imagePlace = _L("c:");
#elif defined (__MARM__) || defined(__X86GCC__)
	#ifdef __TEST_IMAGES_ON_C_DRIVE__
		imagePlace = _L("c:");
	#else
		imagePlace = _L("z:");
	#endif // __TEST_IMAGES_ON_C_DRIVE__
#endif	// __MARM__,__X86GCC__

		imagePlace.Append(iInputDir); 
		break;
	
		case EOutput:
		imagePlace = _L("c:");

		imagePlace.Append(iOutputDir);

		break;
		
		case EReference:
#ifdef __WINS__
	imagePlace = _L("c:");
#elif defined(__MARM__) || defined (__X86GCC__)
	#ifdef __TEST_IMAGES_ON_C_DRIVE__
		imagePlace = _L("c:");
	#else
		imagePlace = _L("z:");
	#endif // __TEST_IMAGES_ON_C_DRIVE__
#endif	// __MARM__,__X86GCC__	
		imagePlace.Append(iRefDir);
		break;
		}

	}


TAny	CTest_MM_ICL_FRM_CP::PrintBusyOrFreeImageConversion(TBool aBusy)
	{
	if(aBusy == (TBool) aBusy)
			INFO_PRINTF1(_L("Encoder image convert is busy"));
	else
			INFO_PRINTF1(_L("Encoder image convert is free"));
	}


/* 
 *
 *	we will wait  iNumStep times until cancel decoding
 *
 * @return		"TAny"
 *					
 */
TAny CTestActiveScheduler::WaitForAnyRequest()
{
	User::WaitForAnyRequest();

	if(iDecoder)
		{
		
		iCurStep++;
		if(iCurStep > iNumStep && iListener->IsActive())
			{
			iDecoder->Cancel();	
			iDecoder = NULL;

			}
		}
	
	if(iEncoder)
		{

		if(iListener->IsActive())
			{
			iStep->PrintBusyOrFreeImageConversion(ETrue);
			}
		else
			{
			iStep->PrintBusyOrFreeImageConversion(EFalse);
			}
		}

}
