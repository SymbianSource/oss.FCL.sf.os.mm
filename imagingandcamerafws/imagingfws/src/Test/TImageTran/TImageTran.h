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
// This file contains declaration for image transform
// Test Design: 
// Test Spec: 
// 
//

#ifndef __IMAGETRANS_H__
#define __IMAGETRANS_H__

#include "e32base.h"
#include "e32std.h"
#include "e32cons.h"
#include "e32cmn.h"
#include "bacline.h"
#include "f32file.h"
#include <imagetransform.h>
#include <imageconversion.h>
#include <icl/squeezetransformextension.h>
#include <icl/orientationtransformextension.h>
#include <icl/overlaytransformextension.h>

#ifndef EXIT_FAILURE		        /* define exit() codes if not provided */
#define EXIT_FAILURE  0
#endif
#ifndef EXIT_SUCCESS		        /* define exit() codes if not provided */
#define EXIT_SUCCESS  1
#endif
typedef enum 
    {
	ITRAN_NONE,             		/* no transformation */
	ITRAN_FLIP,
	ITRAN_ROTATE,
	ITRAN_TRANSPOSE,				/* 90-degree rotation + horzontal flip */
	ITRAN_TRANSVERSE,				/* 270-degree rotation + horzontal flip */
	ITRAN_OVERLAY,
	ITRAN_SQU,                      /* squeezing with proposed size */
	ITRAN_AUTOSQU,					/* autosqueezing with proposed size and resize action */
	ITRAN_HELP						/* help for the imagetran tool */
    } ITRAN_CODES;

typedef struct
    {
    ITRAN_CODES iTransform;
    TFileName   iOutputfile;
    TFileName   iInputfile;
    } ITRAN_CMD_INFO;
    
_LIT8(KToolName, "imagetran");
const TInt KChunkDataOffset = 0;        //the data offset from where the image frame starts
const TUid KImageFramePluginUid = {0x101F7C60};

static ITRAN_CMD_INFO          iTransOpt; // Transformation option

class CActiveListener : public CActive
	{
public:
	CActiveListener() : CActive(CActive::EPriorityIdle) { CActiveScheduler::Add(this); }
	void InitialiseActiveListener() { SetActive(); }
	// From CActive
	virtual void RunL() { CActiveScheduler::Stop(); }
	virtual void DoCancel() {};
	};
	
class CActiveTransformListener : public CActiveListener
	{
public:
	void InitialiseActiveListener(CImageTransform* aTransform)
		{
		iTransform = aTransform;
		CActiveListener::InitialiseActiveListener();
		}
	// From CActive
	virtual void DoCancel()
		{
		if (iTransform)
			iTransform->CancelTransform();
		}
protected:
	CImageTransform* iTransform;
	};
	
class CITArgsandFuncts :public CBase
    {
public:
    /* Constructor for parsing the command line arguments */
    CITArgsandFuncts(){};
    
    /* Create the CITArgsandFuncts object */
    static CITArgsandFuncts* NewL();
    void ConstructL();
    
    /* Destrcutor */
    ~CITArgsandFuncts();
    
    /* Start the image transformation by parsing the commandline arguments */
    void ImageTranMainL();

    /*Receive Arguments and perform the operation based on the option */
    void ImageTransformL();
    
    /* Perform transformations squeeze,autosqueeze,rotation/flip,overlay, */
    void Peform_TransformationL();
    
    /* Usage of the Imagetran Command line tool */
    void ImageTranUsage ();
    
    /* exit imagetran tool */
    void ExitImageTranTool();
    
    /* Parse switches */        
    void ParseSwitches (TInt aArgCount);
    
    /* Parsing squeeze transformation parameters */
    TInt ParseSqueezeParams(TInt aArgNum);
    
    /* Parsing autosqueeze transformation parameters */
    TInt ParseAutoSqueezeParams(TInt aArgNum);
    
    /* Parsing flip transformation parameters */ 
    TInt ParseFlipParams(TInt aArgNum);
    
    /* Parsing Rotate transformation parameters */
    TInt ParseRotateParams(TInt aArgNum);
    
    /* Parsing Transpose transformation parameters */
    TInt ParseTransposeParams(TInt aArgNum);
    
    /* Parsing Transverse transformation parameters */
    TInt ParseTransverseParams(TInt aArgNum);
    
    /*  Parsing Overlay transformation parameters */
    TInt ParseOverlayParams(TInt aArgNum);
    
    /* Parsing proposedsizeinbytes for squeeze & autosqueeze transformation */
    TInt ParseProposedSize(TInt aArgNum);
    
    /* Parsing maximagesize for autosqueeze transformation */
    TInt ParseMaxImageSize(TInt aArgNum);
    
    /* Parsing minimagesize for autosqueeze transformation */
    TInt ParseMinImageSize(TInt aArgNum);
    
    /* Parsing squeeze options for autosqueeze transformation */
    TInt ParseSqueezeOptions(TInt aArgNum);
    
    /* Parsing qualityfactor  for autosqueeze transformation */
    TInt ParseQualityFactor(TInt aArgNum);
    
    /* Parsing colormode(sampling) for autosqueeze tarnsformation */
    TInt ParseColorMode(TInt aArgNum);
        
    /* Parsing flip options for flip transformation */
    TInt ParseFlipOpt(TInt aArgNum);
    
    /* Parsing rotation angle for rotate transformation */
    TInt ParseRotationAngle(TInt aArgNum);
    
    /* Parsing overlay position for overlay transformation */
    TInt ParseOverlayPosition(TInt aArgNum);
    
    /* Parsing inputfile name for transformation */
    TInt ParseInputFile(TInt aArgNum);
    
    /* Parsing outputfile name for transformation */
    TInt ParseOutputFile(TInt aArgNum);
    
    /* Parsing overlayfile name for overlay transformation */
    TInt ParseOverLayFile(TInt aArgNum);
    
    /*Case-insensitive matching of possibly-abbreviated keyword switches*/
    TBool KeywordMatch (TPtrC aKeyMatch, const TDesC16& aKeyword, TInt aMinchars);
        
    /* Read next argument from commandline */    
    void ReadNextArg(TInt aArgNum);

    /* Get the input image size to valiadte MaxImagesize and MinImagesize against input image size */
    void Fetch_ImageSize();
        
    /* Validate  imagesize to check it not a alphanumeric or alphabetic */
    TUint ValidateImageSize();
    
    /* To detect multiple transform options and set the transforma option slected  */
    void Select_Transform (ITRAN_CODES aTransform);
        
    /* Set source and destination file, squeeze option transformation, 
    	orientation uid for rotate,flip,transpose or transverse option */
    void SetupTransformation (ITRAN_CODES aTransCode);
    
    /* Set extension, overlayfile and overlay position for overlay comamnd */
    void SetupOverlayTrans();
  
 
private:
    CImageTransform*            iImageTransform;
    CCommandLineArguments*      iCmdArgs;                   // To hold teh command line args
    CConsoleBase*               iConsole;                   // To print the messages on console
    TPtrC                       iOutputfile, iInputfile;    // To hold the i/p and o/p files
    TPtrC						iOverlayfile;				// To hold the overlay file
    TFileName                   iToolName;                  // To hold the toolname for console messages purposes
    TBuf8<596>                  iArgBuf;                    // To hold all the comamnd line args in buffer
    TPtrC                       iArgumentPtr;               // To extract the command line args
    TUint                       iProposedSize;              // Proposed size for squeezing
    RFs                         iFs;                        
    TSize                       iInputImageSize;            // Inputfile image size for validating max and min image size
    CSqueezeTransformExtension* iTransExtn;                 // Transofrmation extension such as squeeze, rotate, flip or overlay
    CActiveScheduler*           iScheduler;
    TInt                        iColorMode;                 // If 0-420,1-422,2-444,3-411 colour samlping autosqueeze. This is an optional parameter.
    TBool                       iEndOfCmd;                  // If true-> end of cmd args reached, if false-> still cmd args are there
    TUid                        iSamplingUid;               // Sampling uid based on clourmode for autosqueeze
    COrientationTransformExtension* iTransOrientation;			// Transformation orientation for image like rotate,flip,transpose,transverse
    COverlayTransformExtension*  iOverlayExtn;				//  OverLay extension for overlay transformation
    COrientationTransformExtension::TOrientation iOrientation; // Transformation orientation for image like rotate,flip,transpose,transverse
    TAdvancedSqueezeParams  	iSqueezeAutoResizeParams;	// Provides advanced parameters for autosqueezing
    TPoint 						iPoint;
    };
#endif // __IMAGETRANS_H__
