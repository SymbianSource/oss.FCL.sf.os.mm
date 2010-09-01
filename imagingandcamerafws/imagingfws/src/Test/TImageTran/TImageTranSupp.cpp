// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TImageTran.h"
#include <iclexifimageframe.h>

    
/*
 * Case-insensitive matching of possibly-abbreviated keyword switches.
 * keyword is the constant keyword (must be lower case already),
 * minchars is length of minimum legal abbreviation.
 */
TBool CITArgsandFuncts::KeywordMatch (TPtrC aKeyMatch, const TDesC16& aKeyWord, TInt aMinCharsMatch)
    {

    if(aKeyMatch.MatchF(aKeyWord) != KErrNotFound)
        {
            return ETrue;
        }

    if((aKeyWord.Length() < aMinCharsMatch))
        {
         return EFalse;   //no matching keyword found  
        }
    return EFalse;        
    }
    
/* Read next argument from commandline */    
void CITArgsandFuncts::ReadNextArg(TInt aArgNum)
    {
    if (aArgNum >= iCmdArgs->Count())	/* advance to next argument */
        {
        iEndOfCmd = ETrue;
        //ExitImageTranTool();  
        }
    else
        {
        iArgumentPtr.Set(iCmdArgs->Arg(aArgNum));    
        }        
    }
         
/*
*   Get the input image size to check that MaxImagesize and MinImagesize are 
*   within the input image size range
*
*/
void CITArgsandFuncts::Fetch_ImageSize()    
    {
    CJPEGImageFrameDecoder *decoder = NULL;
    TRAPD(error, decoder= static_cast<CJPEGImageFrameDecoder*> (CImageDecoder::FileNewL(iFs, iInputfile, CImageDecoder::EOptionNone, KNullUid, KNullUid, KImageFramePluginUid)));
    if(error != KErrNone)
        {
        iConsole->Printf(_L("JpegCodec does not support this image format.InputImageSize caluculation for autosqueeze transformation failed with %d\n"), error);
        ExitImageTranTool();
        }

    const TFrameInfo* frameInfo = NULL;
    frameInfo = &decoder->FrameInfo();
    iInputImageSize = frameInfo->iOverallSizeInPixels;
    
    if(iSqueezeAutoResizeParams.iMaxImageSize.iWidth > iInputImageSize.iWidth )
        {
        iConsole->Printf(_L("MaxImage width is greater than the input image width\n"));
        ExitImageTranTool();
        }  
        
    if(iSqueezeAutoResizeParams.iMaxImageSize.iHeight > iInputImageSize.iHeight )
        {
        iConsole->Printf(_L("MaxImage height is greater than the input image height\n"));
        ExitImageTranTool();
        }  
        
    if(iSqueezeAutoResizeParams.iMinImageSize.iWidth > iInputImageSize.iWidth && iSqueezeAutoResizeParams.iMinImageSize.iWidth > iSqueezeAutoResizeParams.iMaxImageSize.iWidth)
        {
        iConsole->Printf(_L("MinImage width is greater than the input image width/Maximage height\n"));
        ExitImageTranTool();
        }
    if(iSqueezeAutoResizeParams.iMinImageSize.iHeight > iInputImageSize.iHeight && iSqueezeAutoResizeParams.iMinImageSize.iHeight > iSqueezeAutoResizeParams.iMaxImageSize.iHeight)
        {
        iConsole->Printf(_L("MinImage height is greater than the input image height/Maximage height\n"));
        ExitImageTranTool();
        }
    delete decoder;
    }
    
/* 
* Validate whether its a valid numeric number or not 
* validation is done based on the ascii value of the character
* for digits 0 to 9 it starts from 48 to 57
*/ 
TUint CITArgsandFuncts::ValidateImageSize()
    {
    TUint tmpVal;
    TLex lLex(iArgumentPtr);
    TLex lLextmp(iArgumentPtr);
    TChar charArg = lLex.Get();
    do
        {
        if(charArg > 47 && charArg < 58 )
            {
            charArg = lLex.Get();
            }
        else
            {
            if(charArg < 47 || charArg > 58)
                {
                iConsole->Printf(_L("SizeinBytes or ImageSize is incorrect alphabetic or alphanumeric or special characters\n"));
                ExitImageTranTool();
                }           
            }
        }while(!lLex.Eos());

    lLextmp.Val(tmpVal);    
    return tmpVal;        
    }
           
/* 
 * Set source and destination file, squeeze option 
 */
void CITArgsandFuncts::SetupTransformation (ITRAN_CODES aTransCode)
    {
    TInt error = KErrNone;
    		
	TRAP(error, iImageTransform->SetSourceFilenameL(iInputfile));
	if(error != KErrNone)
	    {
	    iConsole->Printf(_L("Input file is having a problem failed with error : %d\n"), error);
        ExitImageTranTool();
	    }
	
	TRAP(error, iImageTransform->SetDestFilenameL(iOutputfile));
	if(error != KErrNone)
	    {
	    iConsole->Printf(_L("Output file is having a problem failed with error: %d\n"), error);
        ExitImageTranTool();
	    }
	if(aTransCode == ITRAN_SQU || aTransCode == ITRAN_AUTOSQU)
		{
		TRAP(error, iImageTransform->SetTransformationsL(CImageTransform::ESqueeze));
		}
	else if (aTransCode == ITRAN_ROTATE || aTransCode == ITRAN_FLIP ||
			aTransCode == ITRAN_TRANSPOSE || aTransCode == ITRAN_TRANSVERSE)
		{
		TRAP(error, iImageTransform->SetTransformationsL(CImageTransform::EOrientation));
		}
	if(error != KErrNone)
		{
		iConsole->Printf(_L("Not a valid option : %d\n"), error);
		ExitImageTranTool();
		}
		
	TRAP(error, iImageTransform->SetupL());
	if(error != KErrNone)
        {
        iConsole->Printf(_L("Not a valid input or outputfile : %d\n"), error);
        ExitImageTranTool();
        }

	if(aTransCode == ITRAN_SQU || aTransCode == ITRAN_AUTOSQU)
		{
		TUid lSqUid = {KUidSqueezeTransformExtension};
		iTransExtn = static_cast<CSqueezeTransformExtension*>(iImageTransform->Extension(lSqUid, error));
		if(!iTransExtn)
			{
			iConsole->Printf(_L("Extension Returned unexpected error: %d"), error);
	        ExitImageTranTool();
			}
		}
	else if(aTransCode == ITRAN_ROTATE || aTransCode == ITRAN_FLIP ||
			aTransCode == ITRAN_TRANSPOSE || aTransCode == ITRAN_TRANSVERSE)
		{    
		TUid lTransOrUid = {KUidOrientationTransformExtension};
		iTransOrientation = static_cast<COrientationTransformExtension*>(iImageTransform->Extension(lTransOrUid, error));
		if(!iTransOrientation)
			{
			iConsole->Printf(_L("Extension Returned unexpected error: %d"), error);
	        ExitImageTranTool();
			}
		}
    }
    
/* Set extension, overlayfile and overlay position for overlay comamnd */    
void CITArgsandFuncts::SetupOverlayTrans()
	{
	TInt error = KErrNone;
	TUid lTransOlUid = {KUidOverlayTransformExtension};
		
	TRAP(error, iImageTransform->SetSourceFilenameL(iInputfile));
	if(error != KErrNone)
	    {
	    iConsole->Printf(_L("Input file is having a problem failed with error : %d\n"), error);
        ExitImageTranTool();
	    }
	
	TRAP(error, iImageTransform->SetDestFilenameL(iOutputfile));
	if(error != KErrNone)
	    {
	    iConsole->Printf(_L("Output file is having a problem failed with error: %d\n"), error);
        ExitImageTranTool();
	    }
	    	
	TRAP(error, iImageTransform->SetTransformationsL(CImageTransform::EOverlay));
	if(error != KErrNone)
		{
		iConsole->Printf(_L("Not a valid option : %d\n"), error);
		ExitImageTranTool();
		}

	TRAP(error, iImageTransform->SetupL());
	if(error != KErrNone)
        {
        iConsole->Printf(_L("Not a valid input or outputfile : %d\n"), error);
        ExitImageTranTool();
        }		

	iOverlayExtn = static_cast<COverlayTransformExtension*>(iImageTransform->Extension(lTransOlUid, error));
	if(!iOverlayExtn)
		{
		iConsole->Printf(_L("Extension Returned unexpected error: %d"), error);
        ExitImageTranTool();
		}
		
	iOverlayExtn->SetPosition(iPoint);
	
    TParse fileExtn;
    fileExtn.Set(iOverlayfile,NULL,NULL);

	if(error != KErrNotFound)
        {
        if(KeywordMatch(fileExtn.Ext(),_L(".mbm"),3))
	        {
	        CFbsBitmap	bitmap;
	        error = bitmap.Load(iOverlayfile);
			if (error != KErrNone)
				{
				iConsole->Printf(_L("Cannot load overlayfile into a bitmap, failed with error: %d\n"), error);
			    ExitImageTranTool();
				}	        
			TRAP(error, iOverlayExtn->SetOverlayImageL(bitmap));			        	
	        }
		else if(KeywordMatch(fileExtn.Ext(),_L(".jpg"),3))
			{
			TUid uUid = KImageTypeJPGUid;
			TRAP(error, iOverlayExtn->SetOverlayFileL(iOverlayfile, uUid));
			}
		else if(KeywordMatch(fileExtn.Ext(),_L(".png"),3))
			{
			TUid uUid = KImageTypePNGUid;
			TRAP(error, iOverlayExtn->SetOverlayFileL(iOverlayfile, uUid));
			}					
        }
	if(error != KErrNone)
		{
		iConsole->Printf(_L("Not a valid option : %d\n"), error);
		ExitImageTranTool();
		}		

	}
