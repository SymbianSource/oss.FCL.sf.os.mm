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

/*
*    Starting point of the application
*/

TInt E32Main() 
    {
    __UHEAP_MARK;
    // Setup TRAP harness
    CTrapCleanup* cleanup = CTrapCleanup::New();
    CITArgsandFuncts *iITArgsandFunct = NULL;
    TRAPD(error, iITArgsandFunct = CITArgsandFuncts::NewL());
    if(error != KErrNone)
        {
        return error;
        }

    TInt r = KErrNoMemory;
    if ( cleanup ) 
        {
        TRAP(r, iITArgsandFunct->ImageTranMainL());
        __ASSERT_ALWAYS( !r, User::Invariant() );
        delete cleanup;
        }
    
    delete  iITArgsandFunct; 
    // check we didn't leak any memory
    __UHEAP_MARKEND;
    return r;
    }

void CITArgsandFuncts::ConstructL()
    {
    
    }
        
CITArgsandFuncts* CITArgsandFuncts::NewL()
	{
	CITArgsandFuncts* self = new (ELeave) CITArgsandFuncts();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
    
/*
 * Start the image transformation by parsing the commandline arguments
 */
void CITArgsandFuncts::ImageTranMainL() 
    {
    iConsole = Console::NewL( _L( "iConsole"), TSize( KConsFullScreen,KConsFullScreen ) );
    CleanupStack::PushL( iConsole );
    iCmdArgs  = CCommandLineArguments::NewLC();
    
    CActiveScheduler* iScheduler =new(ELeave) CActiveScheduler;
	CleanupStack::PushL(iScheduler);
	CActiveScheduler::Install(iScheduler);
    
    TInt err;
    TRAP( err, ImageTransformL() ); /*Retrive the command line args, parse args  and transform*/
    if ( err ) 
        {
        iConsole->Printf( _L("TRAP: error code(%d)\n"), err );
        }

    iConsole->Printf( _L( "[press any key]\n" ) );
    iConsole->Getch();
    CleanupStack::PopAndDestroy();
    CActiveScheduler::Stop();
    }
    
/*
*  Retrive the command line args, parse args and perform transform
*  operation based on the option.
*/
void CITArgsandFuncts::ImageTransformL() 
    {
    iConsole->Printf( _L( "   Image Transformation Tool\n" ) );
    TInt err = KErrNone;
 
    iToolName.Copy(KToolName);    

    if ( iCmdArgs->Count() >= 0 ) 
        {
        iConsole->Printf( _L( "Arguments available\n" ) );
        
        for ( int i=0; i < iCmdArgs->Count(); i++ ) 
            {
            iArgumentPtr.Set(iCmdArgs->Arg(i));
            iConsole->Printf(_L("%S "), &iArgumentPtr);
            iArgBuf.Append( iCmdArgs->Arg(i) );
            iArgBuf.Append( _L( " " ) );
            }
         iConsole->Printf(_L("\n"));
        //parse switches to get the filenames and the comamnd options
        ParseSwitches(iCmdArgs->Count());

        //Fecth the image size for autosqueeze purpose
        if(iTransOpt.iTransform == ITRAN_AUTOSQU)        
            {
            Fetch_ImageSize();
            }
        
        //prepare for transformation
        //RFbsSession::Connect();            
        User::LeaveIfError( iFs.Connect() );
        CleanupClosePushL(iFs);

        iImageTransform = CImageTransform::NewL(iFs);
        TInt fSize = 0;
        // get the maxinputfileSize
		RFile file;
		err = file.Open(iFs, iInputfile, EFileRead);
		CleanupClosePushL(file);
		
		if(err != KErrNone)
		    {
		    if(err == KErrNotFound)
		        {
                iConsole->Printf(_L("Inputfile doesn't exist in the specified path\n"));
				ExitImageTranTool();
		        }
            else if(err == KErrBadName)    		        
                {
                iConsole->Printf(_L("Inputfile name doesn't confirm to required syntax\n"));
    		    ExitImageTranTool();
                }
            else if(err == KErrPathNotFound)    		        
                {
                iConsole->Printf(_L("Specified Inputfile path or folder not found\n"));
    		    ExitImageTranTool();
                }
            else if(err == KErrInUse)    		        
                {
                iConsole->Printf(_L("Specified Inputfile being opened for another task\n"));
    		    ExitImageTranTool();
                }
			else
				{
			    iConsole->Printf(_L("Opening the inputfile has failed with error : %d\n"), err);
	    		ExitImageTranTool();
				}                                
		    }

		// get the filesize
		file.Size(fSize);
	    CleanupStack::PopAndDestroy(&file);		
		if(iProposedSize >= fSize)
		    {
		    iConsole->Printf(_L(" Proposed size for squeeze should be less than the input file size\n"));
            ExitImageTranTool();
		    }
        //perform transformations based on command
        TRAP(err, Peform_TransformationL());
        if(err != KErrNone)
	        {
	        iConsole->Printf(_L(" Transformation failed %d\n"), err);
            ImageTranUsage();
	        }
        CleanupStack::PopAndDestroy(&iFs);
        }
    }
    
/* Parse optional switches.
 * Returns iCmdArgs[] index of first file-name argument (== aArgCount if none).
 * Any file names with indexes <= aLastFileArg are ignored;
 * they have presumably been processed in a previous iteration.
 * (Pass 0 for last_file_arg_seen on the first or only iteration.)
 */
void CITArgsandFuncts::ParseSwitches ( TInt aArgCount)
    {
      int argNum = 1;
      
      ReadNextArg(argNum);
      /* Scan command line options, adjust parameters */
        if (iArgumentPtr.Left(1).Compare(_L("-")) == 0) 
            {
            if (KeywordMatch(iArgumentPtr.Mid(1), _L("squeeze"), 2)) //Squeeze
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseSqueeze parameters */
                argNum = ParseSqueezeParams(argNum);
                }
            else if (KeywordMatch(iArgumentPtr.Mid(1),_L("autosqueeze"), 4)) //Autosqueeze
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseAutoSqueeze parameters */
                argNum = ParseAutoSqueezeParams(argNum);
                }  
            else if (KeywordMatch(iArgumentPtr.Mid(1),_L("flip"),  1)) //Flip
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseFlip parameters */
                argNum = ParseFlipParams(argNum);
                } 
            else if (KeywordMatch(iArgumentPtr.Mid(1),_L("rotate"), 2)) //Rotate
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseRotate parameters */
                argNum = ParseRotateParams(argNum);
                }
			else if (KeywordMatch(iArgumentPtr.Mid(1),_L("transpose"), 2)) //Transpose
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseTranspose parameters */
                argNum = ParseTransposeParams(argNum);
                }
			else if (KeywordMatch(iArgumentPtr.Mid(1),_L("transverse"), 2)) //Transverse
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseTransverse parameters */
                argNum = ParseTransverseParams(argNum);
                }
			else if (KeywordMatch(iArgumentPtr.Mid(1),_L("overlay"), 2)) //Overlay
                {
                /* Read Next argument */
                ReadNextArg(++argNum);
                /* ParseTransverse parameters */
                argNum = ParseOverlayParams(argNum);
                }
			else if (KeywordMatch(iArgumentPtr.Mid(1),_L("help"), 2)) //Overlay
                {
                Select_Transform(ITRAN_HELP);
                ImageTranUsage();
                }                                                                  
            else 
                {
                /* unknown switch */
                iConsole->Printf( _L( "Unknown [switches] in the command\n") );
                ImageTranUsage();
                }
            }

    if(aArgCount <= 1)
        {
        iConsole->Printf( _L( "[switches] is missng from the command\n"));
        ImageTranUsage();
        }      
    }
    
/*
* Perform transformations squeeze,autosqueeze,
* rotation/flip,overlay,
*/
void CITArgsandFuncts::Peform_TransformationL()
    {
	CActiveTransformListener* listener= new (ELeave) CActiveTransformListener;
	CleanupStack::PushL(listener);		
    TInt error = KErrNone;
   
    if(iTransOpt.iTransform == ITRAN_SQU)
        {
        SetupTransformation(iTransOpt.iTransform);
        iTransExtn->SetDestSizeInBytes((TInt)iProposedSize);
        listener->InitialiseActiveListener(iImageTransform);
        // launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
        }
    else if(iTransOpt.iTransform == ITRAN_AUTOSQU)
        {
        SetupTransformation(iTransOpt.iTransform);
        /* Set proposed size to destination size */
        iSqueezeAutoResizeParams.iMaxDestSizeInBytes = (TInt)iProposedSize;
        iTransExtn->SetAdvancedSqueezeModeL(&iSqueezeAutoResizeParams);
        listener->InitialiseActiveListener(iImageTransform);
    	// launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
        }
	else if(iTransOpt.iTransform == ITRAN_ROTATE)
		{
		SetupTransformation(iTransOpt.iTransform);
		iTransOrientation->SetOrientationL(iOrientation);
		listener->InitialiseActiveListener(iImageTransform);
    	// launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
		}
	else if(iTransOpt.iTransform == ITRAN_FLIP)
		{
		SetupTransformation(iTransOpt.iTransform);
		iTransOrientation->SetOrientationL(iOrientation);
		listener->InitialiseActiveListener(iImageTransform);
    	// launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
		}
	else if(iTransOpt.iTransform == ITRAN_TRANSPOSE)
		{
		SetupTransformation(iTransOpt.iTransform);
		iTransOrientation->SetOrientationL(iOrientation);
		listener->InitialiseActiveListener(iImageTransform);
    	// launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
		}
	else if(iTransOpt.iTransform == ITRAN_TRANSVERSE)
		{
		SetupTransformation(iTransOpt.iTransform);
		iTransOrientation->SetOrientationL(iOrientation);
		listener->InitialiseActiveListener(iImageTransform);
    	// launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
		}
	else if(iTransOpt.iTransform == ITRAN_OVERLAY)
		{
		SetupOverlayTrans();
		listener->InitialiseActiveListener(iImageTransform);
    	// launch the transform
    	iImageTransform->Transform(listener->iStatus);
    	CActiveScheduler::Start();
		}										
	//check status of transformation
	error = listener->iStatus.Int();
	if(error != KErrNone)
	    {
	    if(error == KErrArgument)
		    {
		    iConsole->Printf(_L("Bad transofrmation arguments in the specified command \n"));
		    }
		else if(error == KErrNoMemory)
			{
			iConsole->Printf(_L("Transofrmation failed due to not enough memory for processing\n"));		
			}
		else if(error == KErrNotSupported)
			{
			iConsole->Printf(_L("Transofrmation on the specifed inputfile format/arguments is not supported\n"));				
			}
		else
			{
			iConsole->Printf(_L("Transformation failed with error : %d\n"), error);				
			}
        ExitImageTranTool();
	    }
    else
        {
	    iConsole->Printf(_L("Transformation is successful\n"));
        iConsole->Printf( _L( "[press any key]\n" ) );
        iConsole->Getch();		    
	    User::Exit(EXIT_SUCCESS);            
        }		    
	CleanupStack::PopAndDestroy(listener);
    }
    
/* 
 * To detect multiple transform options,
 * and set the transforma option slected 
 */
void CITArgsandFuncts::Select_Transform (ITRAN_CODES aTransform)
    {
    if (iTransOpt.iTransform == ITRAN_NONE || iTransOpt.iTransform == aTransform) 
        {
        iTransOpt.iTransform = aTransform;
        } 
    else
        {
        iConsole->Printf(_L("%s: can only do one image transformation at a time\n"), &iToolName);
        ImageTranUsage();
        }
    }
    
/* 
* Usage of the Imagetran Command line tool */

void CITArgsandFuncts::ImageTranUsage ()
    {
    TKeyCode charKey;
    
    iConsole->Printf(_L("usage: %S [switches] [parameters] \n"), &iToolName);
    iConsole->Printf(_L("Switches for modifying the image:\n"));
	iConsole->Printf(_L("       -squeeze     : squeeze the image <Press 1 for parameters>\n"));
	iConsole->Printf(_L("       -autosqueeze : autosqueeze the image <Press 2 for parameters>\n"));
	iConsole->Printf(_L("       -flip        : flip the image <Press 3 for parameters>\n"));
	iConsole->Printf(_L("       -rotate      : rotate the image <Press 4 for parameters>\n"));
	iConsole->Printf(_L("       -transpose   : transpose the image <Press 5 for parameters>\n"));
	iConsole->Printf(_L("       -transverse  : transverse the image <Press 6 for parameters>\n"));
	iConsole->Printf(_L("       -overlay     : overlay/replace the image <Press 7 for parameters>\n"));
	iConsole->Printf(_L(" <Press 1 to 7 for [switch] parameters>\n"));
	charKey = iConsole->Getch();	    	
	do
		{
	    if(charKey == 49)
		    {
			iConsole->Printf(_L("Parameters  for squeeze are.....:\n"));
			iConsole->Printf(_L("       -p [proposedsizeinbytes]  <proposedszie for autosqueeze>\n"));		    	
		    }
	    else if(charKey == 50)
		    {
			iConsole->Printf(_L("Parameters  for autosqueeze are.....:\n"));
			iConsole->Printf(_L("       -p [proposedsizeinbytes]       <proposedszie for autosqueeze>\n"));
			iConsole->Printf(_L("       -max [maximagesize]            <maximagesize for autosqueeze>\n"));
			iConsole->Printf(_L("       -min [minimagesize]            <minimagesize for autosqueeze>\n"));
			iConsole->Printf(_L("       -sqopt [autosqueezeoptions]    <autosqueeze options as mentioned below>\n"));
			iConsole->Printf(_L("              		0  					<PreserveSourceEncodingQuality>\n"));
			iConsole->Printf(_L("              		1					<PrioritizeLargeImageize>\n"));
			iConsole->Printf(_L("              		2					<ResizePrioritizeHighEncodingQuality>\n"));
			iConsole->Printf(_L("              		4					<MiddleCourse>\n")); 
			iConsole->Printf(_L("       -mode [colourmodeoptions]             <colourmodeoptions for autosqueeze are as below>\n"));
			iConsole->Printf(_L("                   0                         <420>\n"));
			iConsole->Printf(_L("                   1                         <422>\n"));
			iConsole->Printf(_L("                   2                         <444>\n"));
			iConsole->Printf(_L("                   3                         <411>\n"));
			iConsole->Printf(_L("        -q [qualityfactor]             <quality factor for autosqueeze>\n"));
		    }
		else if(charKey == 51)
		    {
			iConsole->Printf(_L("Parameters  for flip are.....:\n"));
			iConsole->Printf(_L("       -flipopt [horizontal|vertical] <flip image left-right or top-bottom>\n"));
		    }
		else if(charKey == 52)
		    {
			iConsole->Printf(_L("Parameters  for rotate are.....:\n"));
	    	iConsole->Printf(_L("       -ang [0|90|180|270]            <rotate image (degrees clockwise)>\n"));
		    }
		else if(charKey == 53)
		    {
			iConsole->Printf(_L("Parameters  for transpose are.....:\n"));
		    }
		else if(charKey == 54)
		    {
			iConsole->Printf(_L("Parameters  for transverse are.....:\n"));
		    }	    	    	    
		else if(charKey == 55)
		    {
			iConsole->Printf(_L("Parameters  for overlay are.....:\n"));
			iConsole->Printf(_L("       -flipopt [horizontal|vertical] <flip image left-right or top-bottom>\n"));
		    iConsole->Printf(_L("       -ovl [overlayfile]             <overlay image of type jpg,png,mbm>\n"));
	    	iConsole->Printf(_L("       -pos [X,Y]            		   <X and Y co-ordinates of an overlay image>\n"));
		    }
	 	iConsole->Printf(_L("Common parameters for all the switches and the only parameters for transpose and transverse switch\n"));
	    iConsole->Printf(_L("       -i [inputfile]                 <inputfilename for transformation>\n"));
	    iConsole->Printf(_L("       -o [outputfile]                <outputfilename for transformation>\n"));
	    iConsole->Printf(_L(" <Press 1 to 7 for [switch] parameters>\n"));
		charKey = iConsole->Getch();
		
		}while(charKey >= 49 && charKey <= 55);
	if(charKey < 49 || charKey > 55)
		{
		iConsole->Printf(_L(" Invalid keypress no switch command available for keypress \n"));
		iConsole->Getch();
		}
	User::Exit(EXIT_SUCCESS);
    }
 

/* Exit from the imagetran tool  */    
void CITArgsandFuncts::ExitImageTranTool()
	{
    iConsole->Printf(_L( "[press any key]\n" ) );
    iConsole->Getch();    
    User::Exit(EXIT_FAILURE);		
	}
	
/*
*   Destructor
*/    
CITArgsandFuncts::~CITArgsandFuncts()
    {
    delete iImageTransform;
    delete iCmdArgs;
    delete iConsole;
    delete iScheduler;
    delete iTransExtn;
    }

