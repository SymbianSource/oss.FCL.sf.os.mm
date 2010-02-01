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
// Parsing functions for imagetran tool
// 
//

#include "TImageTran.h"

/* 
* Parsing squeeze transformation parameters 
*/     
TInt CITArgsandFuncts::ParseSqueezeParams(TInt aArgNum)
    {
    Select_Transform(ITRAN_SQU);
    /* To get the proposed size or inputfile or outputfile for squeeze */
    do
        {
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("p"), 1)) //Proposedsizeinbytes
                {
                aArgNum = ParseProposedSize(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the squeeze command\n") );
                ImageTranUsage();
                }                                                       
            }
        else
            {
            /* Missing parameters for squeeze command */ 
            iConsole->Printf( _L( "Missing squeeze parameters in command\n") );
            ExitImageTranTool();
            }
        
        ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }
        } while(aArgNum < iCmdArgs->Count());
     /* Validate required parameter for squeeze */   
     if(iProposedSize <= 0)
        {
        /* Missing parameters for squeeze command */ 
        iConsole->Printf( _L( "Missing squeeze parameters in command\n") );
        ExitImageTranTool();
        }
    return aArgNum;        
    }

/* 
* Parsing autosqueeze transformation parameters 
*/ 
TInt CITArgsandFuncts::ParseAutoSqueezeParams(TInt aArgNum)
    {
    Select_Transform(ITRAN_AUTOSQU);
    // To get the proposed size or inputfile or outputfile or maximagesize 
    // or minimagesize for auto squeeze 
    do
        {
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("p"), 1)) //Proposedsizeinbytes
                {
                aArgNum = ParseProposedSize(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum); 
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("max"), 1)) //MaxImagesize
                {
                aArgNum = ParseMaxImageSize(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("min"), 1)) //MinImagesize
                {
                aArgNum = ParseMinImageSize(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("mode"), 1)) //SamplingMode
                {
                aArgNum = ParseColorMode(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("q"), 1)) //Qualityfactor
                {
                aArgNum = ParseQualityFactor(aArgNum);
                }        
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("sqopt"), 1)) //Autosqueezeoption
                {
                aArgNum = ParseSqueezeOptions(aArgNum);
                }                          
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the autosqueeze command\n") );
                ImageTranUsage();
                }                                                       
            }
        else
            {
            /* Missing parameters for squeeze command */ 
            iConsole->Printf( _L( "Missing autosqueeze parameters in command\n") );
            ExitImageTranTool();
            }
        ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }                                    
        } while(aArgNum < iCmdArgs->Count());
    /* Validate whether all the autosqueeze parameters are entered */                    
    if(iProposedSize <= 0 || iSqueezeAutoResizeParams.iMaxImageSize.iWidth <= 0 ||
       iSqueezeAutoResizeParams.iMaxImageSize.iHeight <= 0 ||
       iSqueezeAutoResizeParams.iMinImageSize.iWidth <= 0 || 
       iSqueezeAutoResizeParams.iMinImageSize.iHeight <= 0)
        {
        /* Missing parameters for squeeze command */ 
        iConsole->Printf( _L( "Missing autosqueeze parameters in command\n") );
        ExitImageTranTool();
        }
    return aArgNum;
    }
    
/* 
* Parsing flip transformation parameters 
*/    
TInt CITArgsandFuncts::ParseFlipParams(TInt aArgNum)  
    {
    Select_Transform(ITRAN_FLIP);
    /* To flip horzontal or vertical for flip command */
    do
        {
        if(iEndOfCmd)
            {
            iConsole->Printf(_L("Missing flipoption in flip command\n"));
            ImageTranUsage();
            }
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("flipopt"), 1)) //Flipoption
                {
                aArgNum = ParseFlipOpt(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the flip command\n") );
                ImageTranUsage();
                }                                                       
            }
        else
            {
            /* Missing parameters for flip command */ 
            iConsole->Printf( _L( "Missing flip parameters in command\n") );
            ExitImageTranTool();
            }
		ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }             
        } while(aArgNum < iCmdArgs->Count()); 
    return aArgNum;                      
    }

/* 
* Parsing Rotate transformation parameters 
*/    
TInt CITArgsandFuncts::ParseRotateParams(TInt aArgNum)
    {
    Select_Transform(ITRAN_ROTATE);
    /* To rotate 0,90,180,270 degrees for rotate command */
    do
        {
        if(iEndOfCmd)
            {
            iConsole->Printf(_L("Missing rotation angle in rotate command\n"));
            ImageTranUsage();
            }
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("ang"), 1)) //Rotationangle
                {
                aArgNum = ParseRotationAngle(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the rotation command\n") );
                ImageTranUsage();
                }                                                       
            }
        else
            {
            /* Missing parameters for squeeze command */ 
            iConsole->Printf( _L( "Missing rotation parameters in command\n") );
            ExitImageTranTool();
            }
		ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }		            
        } while(aArgNum < iCmdArgs->Count());
    return aArgNum;        
    }

/* 
* Parsing transpose transformation parameters 
*/    
TInt CITArgsandFuncts::ParseTransposeParams(TInt aArgNum)
    {
    Select_Transform(ITRAN_TRANSPOSE);
    
    /* To flip horzontal or vertical for flip command */
    do
        {
        
        if(iEndOfCmd)
            {
            iConsole->Printf(_L("Missing inputfile and outputfile in transverse command\n"));
            ImageTranUsage();
            }
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the transpose command\n") );
                ImageTranUsage();
                }
			iOrientation = COrientationTransformExtension::ETransposeOverMainDiagonal;                
            }
        else
            {
            /* Missing parameters for squeeze command */ 
            iConsole->Printf( _L( "Missing transpose parameters in command\n") );
            ExitImageTranTool();
            }
        ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }                         
        } while(aArgNum < iCmdArgs->Count());
    return aArgNum;        
    }

/* 
* Parsing transpose transformation parameters 
*/    
TInt CITArgsandFuncts::ParseTransverseParams(TInt aArgNum)
    {
    Select_Transform(ITRAN_TRANSVERSE);
    
    /* To flip horzontal or vertical for flip command */
    do
        {
        if(iEndOfCmd)
            {
            iConsole->Printf(_L("Missing inputfile and outputfile in transverse command\n"));
            ImageTranUsage();
            }
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the transverse command\n") );
                ImageTranUsage();
                }
			iOrientation = COrientationTransformExtension::ETransposeOverMinorDiagonal;                
            }
        else
            {
            /* Missing parameters for squeeze command */ 
            iConsole->Printf( _L( "Missing transverse parameters in command\n") );
            ExitImageTranTool();
            }
        ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }		                                    
        } while(aArgNum < iCmdArgs->Count());
    return aArgNum;        
    }
/* 
* Parsing proposedsizeinbytes for squeeze & autosqueeze transformation
*/    
TInt CITArgsandFuncts::ParseOverlayParams(TInt aArgNum)
	{
    Select_Transform(ITRAN_OVERLAY);
    
    /* To set overlay/replace image for overlay command */
    do
        {
        if(iEndOfCmd)
            {
            iConsole->Printf(_L("Missing rotation angle in rotate command\n"));
            ImageTranUsage();
            }
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0)
            {
            if(KeywordMatch(iArgumentPtr.Mid(1), _L("pos"), 1)) //Rotationangle
                {
                aArgNum = ParseOverlayPosition(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("i"), 1)) //Inputfile
                {
                aArgNum = ParseInputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("o"), 1)) //Outputfile
                {
                aArgNum = ParseOutputFile(aArgNum);
                }
            else if(KeywordMatch(iArgumentPtr.Mid(1), _L("ovl"), 1)) //Outputfile
                {
                aArgNum = ParseOverLayFile(aArgNum);
                }    
            else
                {
                /* unknown switch */ 
                iConsole->Printf( _L( "Unknown parameter in the rotation command\n") );
                ImageTranUsage();
                }                                                       
            }
        else
            {
            /* Missing parameters for squeeze command */ 
            iConsole->Printf( _L( "Missing rotation parameters in command\n") );
            ExitImageTranTool();                                   
            }
		ReadNextArg(++aArgNum);
        if(aArgNum == iCmdArgs->Count())
            {
            break;
            }		            
        } while(aArgNum < iCmdArgs->Count());
    return aArgNum;        
    }
        
/* 
* Parsing proposedsizeinbytes for squeeze & autosqueeze transformation
*/    
TInt CITArgsandFuncts::ParseProposedSize(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing proposedsizeinbytes in squeeze/autosqueeze command\n"));
        ExitImageTranTool();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Validate whether its a valid numeric number or not */
        iProposedSize = ValidateImageSize();
        }
    else
        {
        iConsole->Printf(_L("Missing proposedsizeinbytes  in squeeze/autosqueeze command\n"));
        ExitImageTranTool();
        }
    return aArgNum;        
    }

/* 
* Parsing maximagesize for autosqueeze transformation 
*/    
TInt CITArgsandFuncts::ParseMaxImageSize(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing maximagesize in autosqueeze command\n"));
        ImageTranUsage();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /*validate whether its a valid MaxImagesize or not*/                
        iSqueezeAutoResizeParams.iMaxImageSize.iWidth = ValidateImageSize();
        
        ReadNextArg(++aArgNum);
        if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing maximageheight in autosqueeze command\n"));
        ImageTranUsage();
        }
        if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
            {
            /*validate whether its a valid MaxImagesize or not*/                
            iSqueezeAutoResizeParams.iMaxImageSize.iHeight = ValidateImageSize();
            }
        else
            {
            iConsole->Printf(_L("Missing maximageheight in autosqueeze command\n"));
            ExitImageTranTool();
            }
        }
    else
        {
        iConsole->Printf(_L("Missing maximagewidth in autosqueeze command\n"));
        ExitImageTranTool();
        }
    return aArgNum;       
    }
    
/* 
* Parsing minimagesize for autosqueeze transformation 
*/    
TInt CITArgsandFuncts::ParseMinImageSize(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing minimagesize in autosqueeze command\n"));
        ImageTranUsage();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Validate whether its a valid MinImagesize or not */                
        iSqueezeAutoResizeParams.iMinImageSize.iWidth = ValidateImageSize();

        ReadNextArg(++aArgNum);
        if(iEndOfCmd)
            {
            iConsole->Printf(_L("Missing minimageheight in autosqueeze command\n"));
            ImageTranUsage();
            }
        if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
            {
            /* Validate whether its a valid MinImagesize or not */                
            iSqueezeAutoResizeParams.iMinImageSize.iHeight = ValidateImageSize();
            }
        else
            {
            iConsole->Printf(_L("Missing minimageheight in autosqueeze command\n"));
            ExitImageTranTool();
            }
        }
    else
        {
        iConsole->Printf(_L("Missing minimagewidth in autosqueeze command\n"));
        ExitImageTranTool();
        }
    return aArgNum;
    }    
    
/* 
* Parsing squeeze options for autosqueeze transformation
* Squeeze options are 
*    PreserveSourceEncodingQuality
*    PrioritizeLargeImageSize
*    ResizePrioritizeHighEncodingQuality
*    MiddleCourse
*/
TInt CITArgsandFuncts::ParseSqueezeOptions(TInt aArgNum)
    {
    /* to get autosqueeze option. */
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing autosqueezeoption in autosqueeze command\n"));
        ImageTranUsage();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Validate whether its a valid numeric or not */                
        iSqueezeAutoResizeParams.iResizeAction = ValidateImageSize();
        if(iSqueezeAutoResizeParams.iResizeAction < 0 || iSqueezeAutoResizeParams.iResizeAction > 3)
	        {
	        iConsole->Printf(_L("UnKnow autosqueezeoption in autosqueeze command\n"));
	        ImageTranUsage();
	        }    
        }
    else
        {
        iConsole->Printf(_L("Missing autosqueezeoption in autosqueeze command\n"));
        ExitImageTranTool();
        }
    return aArgNum;        
    }
    
/* 
* Parsing qualityfactor  for autosqueeze transformation 
* Must be in the range (0.0) to (1.0)
*/    
TInt CITArgsandFuncts::ParseQualityFactor(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing qualityfactor in autosqueeze command\n"));
        ExitImageTranTool();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        TLex lLex(iArgumentPtr);
        lLex.Val(iSqueezeAutoResizeParams.iMinEncodingQuality);
        if(iSqueezeAutoResizeParams.iMinEncodingQuality < 0.0 || iSqueezeAutoResizeParams.iMinEncodingQuality > 1.0)
            {
            iConsole->Printf(_L("Qulaityfactor for autosqueeze is not within the range 0.0 to 1.0\n"));
            ExitImageTranTool();
            }
        }
    else
        {
        iConsole->Printf(_L("Missing qualityfactor in autosqueeze command\n"));
        ExitImageTranTool();
        }
    return aArgNum;        
    }
    
/* 
* Parsing colormode(sampling) for autosqueeze tarnsformation 
*/    
TInt CITArgsandFuncts::ParseColorMode(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing samplingmode in autosqueeze command\n"));
        ImageTranUsage();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Validate whether its a valid numeric or not */                
        iColorMode = ValidateImageSize();
        if(iColorMode == 0)
            {
            iSqueezeAutoResizeParams.iSamplingUid = KUidSamplingColor420;
            }
        else if(iColorMode == 1)
            {
            iSqueezeAutoResizeParams.iSamplingUid = KUidSamplingColor422;
            }
        else if(iColorMode == 2)
            {
            iSqueezeAutoResizeParams.iSamplingUid = KUidSamplingColor444;
            }
        else if(iColorMode == 3)
            {
            iSqueezeAutoResizeParams.iSamplingUid = KUidSamplingColor411;
            }
        else
            {
            iConsole->Printf(_L("Unknow samplingmode in autosqueeze command\n"));
            ImageTranUsage();
            }                                    
        }
    else
        {
        iConsole->Printf(_L("Missing samplingmode in autosqueeze command\n"));
        ExitImageTranTool();
        }
    return aArgNum;        
    }
    
/* 
* Parsing flip options for flip transformation 
* flip options are
*       horizontal
*       vertical
*/    
TInt CITArgsandFuncts::ParseFlipOpt(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {              
        if(KeywordMatch(iArgumentPtr, _L("horizontal"), 1)) //Fliphorizontal
            {
            iOrientation = COrientationTransformExtension::EMirrorHorizontalAxis;
            }
        else if(KeywordMatch(iArgumentPtr, _L("vertical"), 1)) //Flipvertical
            {
            iOrientation = COrientationTransformExtension::EMirrorVerticalAxis;
            }
        else
            {
            /* unknown flip option */ 
            iConsole->Printf( _L( "Unknown flip option in the command\n") );
            ImageTranUsage();                                
            }
        }
    else
        {
        iConsole->Printf(_L("Missing flipoption in flip command\n"));
        ExitImageTranTool();
        }
    return aArgNum;        
    }

/* 
* Parsing rotation angle for rotate transformation 
* rotation angles are
*       0,90,180,270 degrees
*/ 
TInt CITArgsandFuncts::ParseRotationAngle(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing rotation angle in rotation command\n"));
        ExitImageTranTool();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {             
        if (KeywordMatch(iArgumentPtr,_L("0"), 2))
            {
            iOrientation = COrientationTransformExtension::ERotationNone;
            }
        else if (KeywordMatch(iArgumentPtr,_L("90"), 2))
            {
            iOrientation = COrientationTransformExtension::ERotation90DegreesClockwise;
            }
        else if (KeywordMatch(iArgumentPtr,_L("180"), 3))
            {
            iOrientation = COrientationTransformExtension::ERotation180DegreesClockwise;
            }
        else if (KeywordMatch(iArgumentPtr,_L("270"), 3))
            {
            iOrientation = COrientationTransformExtension::ERotation270DegreesClockwise;
            }
        else
            {
            /* unknown flip option */ 
            iConsole->Printf( _L( "Unknown rotation angle in the rotation command\n") );
            ImageTranUsage();                                
            }
        }
    else 
        {
        iConsole->Printf(_L("Missing rotationangle in rotate command\n"));
        ExitImageTranTool();
        }
    return aArgNum;                                           
    }

/* 
* Parsing overlay position for overlay transformation 
*/       
TInt CITArgsandFuncts::ParseOverlayPosition(TInt aArgNum)
	{
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing overlay position in overlay command command\n"));
        ImageTranUsage();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /*validate whether its a valid X point or not*/                
        iPoint.iX = ValidateImageSize();
        
        ReadNextArg(++aArgNum);
        if(iEndOfCmd)
		    {
		    iConsole->Printf(_L("Missing X point in overlay command\n"));
		    ExitImageTranTool();
		    }
        if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
            {
            /*validate whether its a valid MaxImagesize or not*/                
            iPoint.iY = ValidateImageSize();
            }
        else
            {
            iConsole->Printf(_L("Missing Y point in overlay command\n"));
            ExitImageTranTool();
            }
        }
    else 
        {
        iConsole->Printf(_L("Missing overlay position in overlay command\n"));
        ExitImageTranTool();
        }
    return aArgNum;                                           
    }
/* 
* Parsing inputfile name for transformation 
*/       
TInt CITArgsandFuncts::ParseInputFile(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);
    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing inputfile in command\n"));
        ExitImageTranTool();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Set input file */
        iInputfile.Set(iArgumentPtr);
        }
    else
        {
        iConsole->Printf(_L("Missing inputfile in command\n"));
        ExitImageTranTool();
        }
    return aArgNum;
    }
    
/* 
* Parsing outputfile name for transformation 
*/
TInt CITArgsandFuncts::ParseOutputFile(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);

    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing outputfile in command\n"));
        ExitImageTranTool();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Set output file */
        iOutputfile.Set(iArgumentPtr);
        }
    else
        {
        iConsole->Printf(_L("Missing outputfile in command\n"));
        ExitImageTranTool();
        }
    return aArgNum;
    }

/* 
* Parsing outputfile name for transformation 
*/
TInt CITArgsandFuncts::ParseOverLayFile(TInt aArgNum)
    {
    ReadNextArg(++aArgNum);

    if(iEndOfCmd)
        {
        iConsole->Printf(_L("Missing overlayfile in command\n"));
        ExitImageTranTool();
        }
    if (iArgumentPtr.Left(1).Compare(_L("-")) != 0)
        {
        /* Set output file */
        iOverlayfile.Set(iArgumentPtr);
        }
    else
        {
        iConsole->Printf(_L("Missing overlayfile in command\n"));
        ExitImageTranTool();
        }
    return aArgNum;
    }    
