
//Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains test step class implementation for confirming which extension interface(s) are supported.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageDecoder.xls
// 
//


// Test user includes
#include "tsi_icl_decode_extinterface.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif

// Read common input from the configuration file
TInt RStepBaseImageDecoderExtensionInterface::ReadCommonInputFromConfigFile()
    {
    // only for extension interface tests, last 14 chars of the test case number is the name of the section
    iSectionName = iTestStepName.Right(KSectionNameLen + 7);
    // iUM->iDeOptionsIndex corresponds to extension interface(s)
    if ((!GetIntFromConfig(iSectionName, _L("optionsindex"), iUM->iDeOptionsIndex)) || 
        (!GetIntFromConfig(iSectionName, _L("implementationuidindex"), iUM->iImplementationUidIndex)))
        {
        return KErrNotFound;
        }
    return ReadCommonInputImageFromConfigFile();
    }


// Get the ImplementationUidValue for extension interface(s)
TUid RStepBaseImageDecoderExtensionInterface::DecoderImplementationUidValue()
    {
    TUid implementationUid;
    implementationUid.iUid = 0;

    switch (iUM->iImplementationUidIndex)
        {
        case 1:
            // Supports extension interfaces
            implementationUid.iUid = KBMPDecoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: BMP Decoder"));
            break;

        case 2:
            implementationUid.iUid = KJPGDecoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: JPG Decoder"));
            break;

        case 3:
            implementationUid.iUid = KGIFDecoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: GIF Decoder"));
            break;

        case 4:
            implementationUid.iUid = KMBMDecoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: MBM Decoder"));
            break;

        case 5:
	        implementationUid.iUid = KTIFFDecoderImplementationUidValue;
	        INFO_PRINTF1(_L("Codec: TIFF Decoder"));
	        break;

        case 6:
	        implementationUid.iUid = KWMFDecoderImplementationUidValue;
	        INFO_PRINTF1(_L("Codec: WMF Decoder"));
	        break;

        case 7:
	        implementationUid.iUid = KPNGDecoderImplementationUidValue;
	        INFO_PRINTF1(_L("Codec: PNG Decoder"));
	        break;

        case 8:
	        implementationUid.iUid = KWBMPDecoderImplementationUidValue;
	        INFO_PRINTF1(_L("Codec: WBMP Decoder"));
	        break;

        case 9:
	        implementationUid.iUid = KOTADecoderImplementationUidValue;
	        INFO_PRINTF1(_L("Codec: OTA Decoder"));
	        break;

        case 10:
	        implementationUid.iUid = KICODecoderImplementationUidValue;
	        INFO_PRINTF1(_L("Codec: ICO Decoder"));
	        break;

        case 11:
	        implementationUid.iUid = KJPGDecoderImplementation2UidValue;
	        INFO_PRINTF1(_L("Codec: JPG Decoder 2"));
	        break;

        default:
            // in this case, implementationUid.iUid = 0;
            ERR_PRINTF2(_L("Invalid value for implementation Uid index, %d"), iUM->iImplementationUidIndex);
        }
    return implementationUid;
    }


void RStepBaseImageDecoderExtensionInterface::DoKickoffTestL()
    {
    INFO_PRINTF2(_L("%S"), &iTestCaseDescription);

    // read common input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());

    // Get implementation uid
    iUM->iDecoderImplementationUidValue = DecoderImplementationUidValue();
    if (iUM->iDecoderImplementationUidValue.iUid == 0)
        {
        ERR_PRINTF2(_L("Implementation Uid %d not supported"), iUM->iDecoderImplementationUidValue.iUid);
        StopTest(EFail);
        return;
        }

    User::LeaveIfError(GetDecodeOptions(iUM->iDeOptions));

    switch (iUM->iDeOptionsIndex)
        {
        // 0 - 8 correspond to flags in CImageDecoder::TOptions on controlling how the image is decoded, 
        // 9 onwards correspond to extension interface(s)
        case 9:  INFO_PRINTF1(_L("Extension: Crop during decoding")); break;
        case 10: INFO_PRINTF1(_L("Extension: Streamed decoding"));    break;
        case 11: INFO_PRINTF1(_L("Extension: Rotation during decoding")); break;
        case 12: INFO_PRINTF1(_L("Extension: Mirror during decoding along horizontal axis")); break;
        case 13: INFO_PRINTF1(_L("Extension: Mirror during decoding along vertical axis"));   break;
        case 14: INFO_PRINTF1(_L("Extension: Scale during decoding"));  break;
        case 15: INFO_PRINTF1(_L("Extension: Reserved7")); break;
        case 16: INFO_PRINTF1(_L("Extension: Reserved8")); break;
        case 17: INFO_PRINTF1(_L("Extension: AutoRotate")); break;

        // Multiple CImageDecoder::TOptions
        case 51: INFO_PRINTF1(_L("Extensions: Crop during decoding and Streamed decoding")); break;
        case 52: INFO_PRINTF1(_L("Extensions: Rotation and MirrorHorizontalAxis during decoding")); break;
        case 53: INFO_PRINTF1(_L("Extensions: Rotation and MirrorVerticalAxis during decoding"));   break;
        case 54: INFO_PRINTF1(_L("Extensions: Rotation and MirrorHorizontalAxis & MirrorVerticalAxis during decoding")); break;
        case 55: INFO_PRINTF1(_L("Extensions: Crop, Rotation, MirrorHorizontalAxis & MirrorVerticalAxis during decoding")); break;
        case 56: INFO_PRINTF1(_L("Extensions: Streamed decoding and Rotation during decoding")); break;
        case 57: INFO_PRINTF1(_L("Extensions: Streamed decoding and MirrorHorizontalAxis & MirrorVerticalAxis during decoding")); break;
        case 58: INFO_PRINTF1(_L("Extensions: Streamed decoding and Scale during decoding")); break;
        case 59: INFO_PRINTF1(_L("Extensions: Crop and Scale during decoding")); break;
        case 60: INFO_PRINTF1(_L("Extensions: Crop, Scale, Rotation, MirrorHorizontalAxis & MirrorVerticalAxis during decoding")); break;
        case 61: INFO_PRINTF1(_L("Extensions: Autorotate and MirrorHorizontalAxis during decoding")); break;
        
        default:
            ERR_PRINTF2(_L("Invalid value for CImageDecoder::TOptions, %d"), iUM->iDeOptionsIndex);
            StopTest(EFail);
            return;
        }

    DoTestL();
    }


// Confirm that a codec, which supports extension interface during decoding, 
// correctly reports support (or no support) of the feature through 'plug-in properties'
// (codec's resource file)
RStepImageDecodeExtInterfacePluginProp* RStepImageDecodeExtInterfacePluginProp::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodeExtInterfacePluginProp(aStepName, aTestCaseDescription));
    }

RStepImageDecodeExtInterfacePluginProp::RStepImageDecodeExtInterfacePluginProp(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderExtensionInterface(aStepName, aTestCaseDescription)
    {
    }

void RStepImageDecodeExtInterfacePluginProp::DoTestL()
    {
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0020-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0025-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-SCAL-I-0040-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-SCAL-I-0045-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0020-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0025-HP")) == 0) ||
        (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0180-HP")) == 0))
        {
        iUM->iNegativeTest = ETrue;
        }

    iUM->iPropertiesArray.Reset();

    // Extract plug-in properties
    TRAPD(err, CImageDecoder::GetPluginPropertiesL(iUM->iDecoderImplementationUidValue, iUM->iPropertiesArray));
    if (err != KErrNone)
        {
        ERR_PRINTF2(_L("GetPluginPropertiesL() left, error = %d"), err);
        StopTest(err, EFail);
        return;
        }

    if (iUM->iPropertiesArray.Count() > 0)
        {
        INFO_PRINTF1(_L("GetPluginPropertiesL(): Codec may support the extension interface during decoding, check further"));
        }
    else
        {
        ERR_PRINTF1(_L("GetPluginPropertiesL() returned 0 entries"));
        StopTest(err, EFail);
        return;
        }

    TBool found = EFalse;
    for (TInt i = 0; i < iUM->iPropertiesArray.Count() - 1; i++)
        {
        if (iUM->iPropertiesArray[i] == KUidJPGAutoRotateSupportValue || iUM->iPropertiesArray[i] == KICLExtensionUidValue)
            {
            if (iUM->iPropertiesArray[i] == KUidJPGAutoRotateSupportValue)
                {
                INFO_PRINTF2(_L("AutoRotate option is supported by the codec UID: %d"), iUM->iPropertiesArray[i].iUid);
                StopTest(EPass);
                found = ETrue;
                break;
                }
            else if (iUM->iPropertiesArray[i] == KICLExtensionUidValue) 
                {
                if ((iUM->iPropertiesArray[i+1].iUid & iUM->iDeOptions) == iUM->iDeOptions)
                    {
                    INFO_PRINTF2(_L("Extension interface Uid : %d"), iUM->iPropertiesArray[i+1].iUid);
                    INFO_PRINTF1(_L("Extension interface is supported by the codec"));
                    StopTest(EPass);
                    found = ETrue;
                    break;
                    }
                }
            else if (iUM->iNegativeTest)
                {
                INFO_PRINTF1(_L("GetPluginPropertiesL(): Extension interface(s)/AutoRotate during decode options is not supported by the codec, expected"));
                StopTest(EPass);
                return;
                }
            }
       }
        
    if (!found && iUM->iNegativeTest)
        {
        // Other codecs do not support the extensions
        INFO_PRINTF1(_L("GetPluginPropertiesL(): Extension interface(s) is not supported by the codec, expected"));
        StopTest(EPass);
        return;
        }

    if (!found)
        {
        ERR_PRINTF1(_L("KICLExtensionUidValue/KUidJPGAutoRotateSupportValue not found in properties array or Extension interface/AutoRotateDecodeOption is not supported by the codec, unexpected"));
        StopTest(EFail);
        return;
        }

    // Log codec name and version
    CImplementationInformationType* implementationInfoType = 
        CImageDecoder::GetImplementationInformationL(iUM->iDecoderImplementationUidValue);

    INFO_PRINTF2(_L("Display name: %S"), &implementationInfoType->DisplayName());
    INFO_PRINTF2(_L("Version     : %d"), implementationInfoType->Version());

    delete implementationInfoType;
    implementationInfoType = NULL;
    }


// Confirm that a codec, which supports extension interface during decoding, 
// correctly reports support (or no support) of the feature by loading the codec
RStepImageDecodeExtInterfaceLoadCodec* RStepImageDecodeExtInterfaceLoadCodec::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepImageDecodeExtInterfaceLoadCodec(aStepName, aTestCaseDescription));
    }

RStepImageDecodeExtInterfaceLoadCodec::RStepImageDecodeExtInterfaceLoadCodec(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageDecoderExtensionInterface(aStepName, aTestCaseDescription)
    {
    }

void RStepImageDecodeExtInterfaceLoadCodec::DoTestL()
    {
    if ((iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0040-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0045-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-SCAL-I-0080-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-SCAL-I-0085-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0040-HP")) == 0) || 
        (iTestStepName.Compare(_L("MM-ICL-DEC-STRM-I-0045-HP")) == 0))
        {
        iUM->iNegativeTest = ETrue;
        }

	//if true, read the decoderuid if false pick the default uid        
	if ((GetHexFromConfig(iSectionName, _L("decoderuid"), iUM->iDecUid)))
		{
		if(iUM->iDecUid != 0)
			{
			iUM->iDecoderUid.iUid = iUM->iDecUid;	
			}
		}
							
    INFO_PRINTF2(_L("Input file: %S"), &iUM->iInputImage);
    CImplementationInformationType* implementationInfoType = 
        CImageDecoder::GetImplementationInformationL(iUM->iDecoderImplementationUidValue);

    TUid imageType = ImageType();
	TInt err = KErrNone;
    // Check whether the codec can be loaded
    if(iUM->iDecoderUid.iUid != 0)
    	{
    	TRAP(err, iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions, imageType, KNullUid, iUM->iDecoderUid));	
    	}
    else
    	{
    	TRAP(err, iUM->iImageDecoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, iUM->iDeOptions, KNullUid, KNullUid, implementationInfoType->ImplementationUid()));
    	}
    if ((iUM->iNegativeTest) && (err == KErrNotFound))
        {
        INFO_PRINTF1(_L("FileNewL(): Loading the codec unsuccessful, expected"));
        StopTest(EPass);
        }
    else if (err != KErrNone)
        {
        if (err == KErrNotFound && (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0120-HP")) == 0))
        	{
        	ERR_PRINTF2(_L("FileNewL(): Loading the codec unsuccessful, failed with expected error = %d"), err);
        	ERR_PRINTF2(_L("Autorotate and mirror horizontal during decode with JPEG codec which doesn't support autorotate, failed with expected error = %d"), err);
        	delete implementationInfoType;
			implementationInfoType = NULL;
        	StopTest(EPass);        	
        	return;
        	}        
        ERR_PRINTF2(_L("FileNewL(): Loading the codec unsuccessful, error = %d"), err);
        StopTest(err, EFail);
        }
    else
        {
        // Log codec name and version
        INFO_PRINTF1(_L("FileNewL(): Loading the codec successful!"));
        INFO_PRINTF2(_L("Display name: %S"), &implementationInfoType->DisplayName());
        INFO_PRINTF2(_L("Version     : %d"), implementationInfoType->Version());
        StopTest(EPass);
        }
        
	// Code added to improve the coverage for CImageDecoder
	if (iUM->iImageDecoder)
		{
		TFrameInfo frameInfo1 = iUM->iImageDecoder->FrameInfo(0);
		if (iTestStepName.Compare(_L("MM-ICL-DEC-ARTO-I-0150-HP")) == 0)
			{
			CImageDecoder* decoder = NULL;
			TRAP(err, decoder = CImageDecoder::FileNewL(iUM->iFs, iUM->iInputImage, CImageDecoder::EOptionNone, KNullUid, KNullUid, implementationInfoType->ImplementationUid()));
			if (err != KErrNone)
			    {
			    INFO_PRINTF2(_L("DecoderCreation for Frameinfo failed %d: "), err);
			    StopTest(EFail);
			    return;
			    }
		    TFrameInfo frameInfo2 = decoder->FrameInfo(0);
            
            if (frameInfo1.iOverallSizeInPixels.iHeight == frameInfo2.iOverallSizeInPixels.iWidth && 
            frameInfo1.iOverallSizeInPixels.iWidth ==frameInfo2.iOverallSizeInPixels.iHeight)
                {
                INFO_PRINTF1(_L("Frameinfo returned taking into account the AutoRotate Orientation tag"));
                INFO_PRINTF3(_L("Frameinfo Without AutoRotate Orientation tag Height : %d  Width : %d"), frameInfo1.iOverallSizeInPixels.iHeight, frameInfo1.iOverallSizeInPixels.iWidth);
                INFO_PRINTF3(_L("Frameinfo With AutoRotate Orientation tag Height : %d  Width : %d"), frameInfo2.iOverallSizeInPixels.iHeight, frameInfo2.iOverallSizeInPixels.iWidth);
                delete decoder; decoder = NULL;
                delete implementationInfoType; implementationInfoType = NULL;  
                StopTest(EPass);
                return;
                }
            else
                {
                INFO_PRINTF1(_L("Frameinfo failed to returned taking into account the AutoRotate Orientation tag"));
                StopTest(EFail);
                return;
                }
			}
		TInt fcount = iUM->iImageDecoder->FrameCount();
		INFO_PRINTF2(_L("Numaber of FrameCount: %d "), fcount);
				
		while(!iUM->iImageDecoder->IsImageHeaderProcessingComplete())
			{
			iUM->iImageDecoder->ContinueProcessingHeaderL();			
			}
		INFO_PRINTF1(_L("Image Header Processing Complete "));	
		
		// FrameData
		const CFrameImageData& framedata = iUM->iImageDecoder->FrameData(fcount-1);
		TInt commentCount = iUM->iImageDecoder->NumberOfFrameComments(fcount-1);
		INFO_PRINTF2(_L("No.of frame comments: %d"), commentCount);

		HBufC* hbuf;
		if(commentCount > 0)
		    {
		    hbuf = iUM->iImageDecoder->FrameCommentL(fcount-1,commentCount-1);
                    INFO_PRINTF2(_L("FrameComment: %S "), hbuf);
                    delete hbuf;				
		    hbuf=NULL;
		    }
		else
		    {
		    INFO_PRINTF1(_L("FrameComment less than Framenumber "));
		    }	
		
		
		
		if (iTestStepName.Compare(_L("MM-ICL-DEC-CROP-I-0046-HP")) == 0)
			{
			TUid impdecUid = iUM->iImageDecoder->ImplementationUid();
			if (impdecUid != implementationInfoType->ImplementationUid())
				{
				INFO_PRINTF1(_L("ImplementationUid(): returned wrong decoder Uid"));
				delete implementationInfoType;
				implementationInfoType = NULL;
			    StopTest(err, EFail);
			    return;
				}
			else
				{
				INFO_PRINTF2(_L("ImplementationUid(): returned correct decoder Uid : %d "), impdecUid.iUid);
				}
				
			TInt icount = iUM->iImageDecoder->NumberOfImageComments();
			HBufC* hbuf1 = iUM->iImageDecoder->ImageCommentL(icount); 
			
			delete hbuf1;				
			hbuf1=NULL;
			}
		
		// GetImageTypesL 
		RImageTypeDescriptionArray theimagetypearry;        
		TRAP(err, iUM->iImageDecoder->GetImageTypesL(theimagetypearry));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("GetImageTypesL() left, error = %d"), err);
			theimagetypearry.ResetAndDestroy();   
			theimagetypearry.Close();
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}

		if (theimagetypearry.Count() > 0)
			{
			INFO_PRINTF1(_L("GetImageTypesL(): Codec may support the imagetypes during decoding, check further"));
			}
		else
			{
			ERR_PRINTF1(_L("GetImageTypesL() returned 0 entries"));
			theimagetypearry.ResetAndDestroy();   
			theimagetypearry.Close();
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}

		theimagetypearry.ResetAndDestroy();   
		theimagetypearry.Close();
		
		//    GetPluginPropertiesL coverage
		TRAP(err, iUM->iImageDecoder->GetPluginPropertiesL(iUM->iDecoderImplementationUidValue, iUM->iPropertiesArray));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("GetPluginPropertiesL() left, error = %d"), err);
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}

		if (iUM->iPropertiesArray.Count() > 0)
			{
			INFO_PRINTF1(_L("GetPluginPropertiesL(): Codec may support the extension interface during decoding, check further"));
			}
		else
			{
			ERR_PRINTF1(_L("GetPluginPropertiesL() returned 0 entries"));
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}


		//  GetImageSubTypesL coverage 
		RImageTypeDescriptionArray theimagesubtypearray;        
		TUid theimageType(KImageTypeJPGUid);	
		TRAP(err, iUM->iImageDecoder->GetImageSubTypesL(theimageType, theimagesubtypearray));
		if (err != KErrNotFound)
			{
			ERR_PRINTF2(_L("GetImageSubTypesL() left with unexpected error, error = %d"), err);
			theimagesubtypearray.ResetAndDestroy();
			theimagesubtypearray.Close();
			StopTest(err, EFail);
			return;
			}

		INFO_PRINTF2(_L("GetImageSubTypesL() returned with expected error KErrNotFound: %d"), err);
		theimagesubtypearray.ResetAndDestroy();
		theimagesubtypearray.Close();

		//GetInterfaceImplementationsL
		RUidDataArray theinterfacearray1;
		RUidDataArray theimageType1;
		theimageType1.Append(KImageTypeJPGUid);
		TRAP(err, CImageDecoder::GetInterfaceImplementationsL(theimageType1, theinterfacearray1));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("GetInterfaceImplementationsL() left, error = %d"), err);
			theinterfacearray1.Reset();
			theinterfacearray1.Close();
			theimageType1.Reset();
			theimageType1.Close();			
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}

		if (theinterfacearray1.Count() > 0)
			{
			INFO_PRINTF1(_L("GetInterfaceImplementationsL(): Codec may support the imagetypes during decoding, check further"));
			}
		else
			{
			ERR_PRINTF1(_L("GetInterfaceImplementationsL() returned 0 entries"));
			theinterfacearray1.Reset();
			theinterfacearray1.Close();
			theimageType1.Reset();
			theimageType1.Close();
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}
		
		theimageType1.Reset();
		theimageType1.Close();
		theinterfacearray1.Reset();
		theinterfacearray1.Close();
		
		//GetInterfaceImplementationsL					
		RUidDataArray theinterfacearray;
		const TInt length = 2;
		TUid theimageType2(KImageTypeJPGUid);    
		TRAP(err, CImageDecoder::GetInterfaceImplementationsL(&theimageType2, length, theinterfacearray));
		if (err != KErrNone)
			{
			ERR_PRINTF2(_L("GetInterfaceImplementationsL() left, error = %d"), err);
			theinterfacearray.Reset();
			theinterfacearray.Close();
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}

		if (theinterfacearray.Count() > 0)
			{
			INFO_PRINTF1(_L("GetInterfaceImplementationsL(): Codec may support the imagetypes during decoding, check further"));
			}
		else
			{
			ERR_PRINTF1(_L("GetInterfaceImplementationsL() returned 0 entries"));
			theinterfacearray.Reset();
			theinterfacearray.Close();
			delete implementationInfoType;
			implementationInfoType = NULL;
			StopTest(err, EFail);
			return;
			}	
					
		theinterfacearray.Reset();
		theinterfacearray.Close();					
		}		
    delete implementationInfoType;
    implementationInfoType = NULL;    
    }
