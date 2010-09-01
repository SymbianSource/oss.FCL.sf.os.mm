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
// This file contains test step class implementation for confirming which extension interface(s) are supported.
// Test Design: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Design/ICL_IntTestDesign_ImageDecoder.doc
// Test Spec: //EPOC/DV3/team/2005/Multimedia/WorkingDocumentation/Test_Specs/ICL_IntTestSpec_ImageEncoder.xls
// 
//


// Test user includes
#include "tsi_icl_encode_extinterface.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif


// Read common input from the configuration file
TInt RStepBaseImageEncoderExtInterface::ReadCommonInputFromConfigFile()
	{
    // last 14 chars of the test case number is the name of the section
    iSectionName = iTestStepName.Right(KSectionNameLen + 7);

    TPtrC tmpFilename;
	// iOptionsIndex corresponds to extension interface(s)
	if ((!GetStringFromConfig(iSectionName, _L("outputimage"), tmpFilename)) || 
	    (!GetStringFromConfig(_L("directory"), _L("outputpath"), iOutputPath)) ||
		(!GetIntFromConfig(iSectionName, _L("optionsindex"), iOptionsIndex)) || 
		(!GetIntFromConfig(iSectionName, _L("implementationuidindex"), iImplementationUidIndex)))
		{
		return KErrNotFound;
		}
	iOutputImage.Append(tmpFilename);
	return KErrNone;
	}

// Get the encoder Implementation uid which supports multiple options
TUid RStepBaseImageEncoderExtInterface::EncoderImplementationUidValue()
	{
	TUid implementationUid;
	implementationUid.iUid = 0;
	switch(iImplementationUidIndex)
		{
        case 1:
            implementationUid.iUid = KBMPEncoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: BMP Encoder"));
            break;
            		
		case 2:
            implementationUid.iUid = KJPGEncoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: JPG Encoder"));
            break;

        case 3:
            implementationUid.iUid = KGIFEncoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: GIF Encoder"));
            break;

        case 4:
            implementationUid.iUid = KMBMEncoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: MBM Encoder"));
            break;

        case 5:
	        implementationUid.iUid = KPNGEncoderImplementationUidValue;
            INFO_PRINTF1(_L("Codec: PNG Encoder"));
	        break;
	        
		case 6:
            implementationUid.iUid = KJPGEncoderImplementation2UidValue;
            INFO_PRINTF1(_L("Codec: JPG Encoder 2"));
			break;
			
		default:
		    // in this case, implUid.iUid = 0;
		    ERR_PRINTF2(_L("Invalid implementation Uid index, %d"), iImplementationUidIndex);
		    break;
		}

	return implementationUid;
	}

void RStepBaseImageEncoderExtInterface::DoKickoffTestL()
    {
    INFO_PRINTF2(_L("%S"), &iTestCaseDescription);    
    
    // read input from the configuration file
    User::LeaveIfError(ReadCommonInputFromConfigFile());

	// Create output directory
	User::LeaveIfError(CreateOutputDirectory());

	TFileName tmpname;
	tmpname.Append(iOutputPath);
	tmpname.Append(iOutputImage);
	iOutputImage.Copy(tmpname);
	
	// Getting implementation uid
	iImageEncoderImplementationUidValue = EncoderImplementationUidValue();
	if (iImageEncoderImplementationUidValue.iUid == 0)
		{
		ERR_PRINTF2(_L("Implementation uid %d is not supported"), iImageEncoderImplementationUidValue.iUid);
		StopTest(EFail);
		return;
		}

	// Set iOptions
	User::LeaveIfError(GetEncodeOptions());

    switch (iOptionsIndex)
        {
        // case 1- 3 are not included as they specify how the encoding should happen i.e client thread,seperatethread etc
        // following options specify the extension interfaces
        case 4: INFO_PRINTF1(_L("Extensions: Streamed encoding")); break;
        case 5: INFO_PRINTF1(_L("Extensions: Rotation during encoding")); break;
        case 6: INFO_PRINTF1(_L("Extensions: Mirroring during encoding along horizontal axis")); break;
        case 7: INFO_PRINTF1(_L("Extensions: Mirroring during encoding along vertical axis")); break;
        case 8: INFO_PRINTF1(_L("Extensions: Setthumbnail duing encoding")); break;
        case 9: INFO_PRINTF1(_L("Extensions: Reserved1")); break;
        case 10: INFO_PRINTF1(_L("Extensions: Reserved2")); break;
        case 11: INFO_PRINTF1(_L("Extensions: Reserved3")); break;
		// Multiple CImageEncoder::TOptions
		case 30: INFO_PRINTF1(_L("Extensions: Mirror horizontal and vertical during encoding")); break;
		case 31: INFO_PRINTF1(_L("Extensions: Mirror horizontal and vertical then setthumbnail during encoding")); break;
		case 32: INFO_PRINTF1(_L("Extensions: Rotation and set thumbnail during encoding")); break;
		case 33: INFO_PRINTF1(_L("Extensions: Rotation and Mirror horizontal and vertical during encoding")); break;
        case 34: INFO_PRINTF1(_L("Extensions: Stream encoding and rotation during encoding")); break;
        case 35: INFO_PRINTF1(_L("Extensions: Stream encoding then setthumbnail during encoding")); break;
        case 36: INFO_PRINTF1(_L("Extensions: Stream encoding and Mirror horizontal during encoding")); break;
        case 37: INFO_PRINTF1(_L("Extensions: Mirror vertical and set Reserved1 option during encoding")); break;

        default: ERR_PRINTF2(_L("Invalid value for CImageEncoder::TOptions, %d"), iOptionsIndex);
        	    StopTest(EFail);
        	    return;
        }

    DoTest();        	
    }
// Confirm that a codec, which supports
// mirror/rotation/setthumbnail/streamed encoding
// extension interface during encoding
RStepEncodeExtInterfaceSupportUsingPluginProp* RStepEncodeExtInterfaceSupportUsingPluginProp::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepEncodeExtInterfaceSupportUsingPluginProp(aStepName, aTestCaseDescription));
    }

RStepEncodeExtInterfaceSupportUsingPluginProp::RStepEncodeExtInterfaceSupportUsingPluginProp(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageEncoderExtInterface(aStepName, aTestCaseDescription)
    {
    }

void RStepEncodeExtInterfaceSupportUsingPluginProp::DoTest()
    {
    iPropertiesArray.Reset();
    
	// Extracting plugin proeprties
	TRAPD(err, CImageEncoder::GetPluginPropertiesL(iImageEncoderImplementationUidValue, iPropertiesArray));
	if (err != KErrNone)
	    {
	    ERR_PRINTF2(_L("GetPluginPropertiesL() left, error = %d"), err);
        StopTest(err, EFail);
        return;
	    }

    if (iPropertiesArray.Count() > 0)
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
	for (TInt i = 0; i < iPropertiesArray.Count()-1; i++)
		{
		if (iPropertiesArray[i] == KICLExtensionUidValue)
			{
			if ((iPropertiesArray[i+1].iUid & iOptions) == iOptions)
				{
				INFO_PRINTF1(_L("Extension interface is supported by the Codec"));
				found = ETrue;
				StopTest(EPass);
				return;
				}
			else
				{
				if ((iTestStepName.Compare(_L("MM-ICL-ENC-RTMR-I-0085-HP")) == 0) ||
				    (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0020-HP")) == 0) ||
				    (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0030-HP")) == 0))
					{
					ERR_PRINTF1(_L("Extension interface is not supported by the Codec"));
					StopTest(EPass);
					return;
					}
				}
			}
		}

	if (!found)
		{
		ERR_PRINTF1(_L("Extension interface is not supported by the Codec"));
		StopTest(EFail);
		}
    }


// Confirm that a codec, which supports 
// streamed encoding/mirror/rotation/setthumbnail/streamedencoding
// extension interface during encoding
RStepEncodeExtInterfaceSupportByLoadingCodec* RStepEncodeExtInterfaceSupportByLoadingCodec::NewL(const TDesC& aStepName, const TDesC& aTestCaseDescription)
    {
    return (new (ELeave) RStepEncodeExtInterfaceSupportByLoadingCodec(aStepName, aTestCaseDescription));
    }

RStepEncodeExtInterfaceSupportByLoadingCodec::RStepEncodeExtInterfaceSupportByLoadingCodec(const TDesC& aStepName, const TDesC& aTestCaseDescription) :
    RStepBaseImageEncoderExtInterface(aStepName, aTestCaseDescription)
    {
    }

void RStepEncodeExtInterfaceSupportByLoadingCodec::DoTest()
    {
	iImageTypeUid = ImageType();

	// Check the support for this extension interface in the implementation uid
	TRAPD(err, iImageEncoder = CImageEncoder::FileNewL(iFs, iOutputImage, iOptions, iImageTypeUid, KNullUid, iImageEncoderImplementationUidValue));
	if (err != KErrNone)
		{
		if ((iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0050-HP")) == 0) ||
		   (iTestStepName.Compare(_L("MM-ICL-ENC-STRM-I-0060-HP")) == 0))
		    {
		    ERR_PRINTF1(_L("Extension interface is not supported by the Codec"));
		    StopTest(err, EPass);
		    return;
		    }
		ERR_PRINTF1(_L("Extension interface is not supported by the Codec"));
		StopTest(err, EFail);
		return;
		}
    INFO_PRINTF1(_L("Extension interface is supported by the Codec"));
	StopTest(EPass);
    }
