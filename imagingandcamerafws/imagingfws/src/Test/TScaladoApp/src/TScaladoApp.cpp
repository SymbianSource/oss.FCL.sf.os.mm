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
// Name		: TScaladoApp.cpp
// Author	  : 
// Exe source file
// Include Files  
// 
//

#include "TScaladoApp.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <f32file.h>
#include <ecom/ecom.h>
#include <capsspmoutility/capsspmoutility.h>
#include <imageprocessor/imageprocessor.h>
#include <imageprocessor/imageprocessorobserver.h>
#include <capsimageprocessor/capsimageprocessorextension.h>


//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

using namespace ImageProcessor;
	

//  Global Variables

LOCAL_D CConsoleBase* gConsole; // write all messages to this


class CImageProcessorObserver : public CBase, public MImgProcessorObserver
	{
public:

	void ImageProcessorInitializingComplete(CImgProcessor& /*aImageProcessor*/, TInt aError)
		{
		//RDebug::Printf("[ImageProcessorInitializingComplete(aError=%d)]", aError);
		
		iError = aError;
		CActiveScheduler::Stop();
		}
	
	
	void ImageProcessorEvent(CImgProcessor& /*aImageProcessor*/, TInt /*aEventId*/, TUid /*aUid*/, TInt /*aId*/)
		{}

	
	void ImageProcessingComplete(CImgProcessor& /*aImageProcessor*/, TInt aError)
		{
		//RDebug::Printf("[ImageProcessingComplete(aError=%d)]", aError);
		
		iError = aError;
		CActiveScheduler::Stop();
		}

	
	void ImageProcessorPreviewInitializingComplete(CImgProcessor& /*aImageProcessor*/,
			TInt /*aPreviewId*/,
			TInt /*aError*/) {}
	
	void ImageProcessorPreviewRenderingComplete(CImgProcessor& /*aImageProcessor*/,
			TInt /*aPreviewId*/,
			TInt /*aError*/) {}
	
	TInt Error() { return iError; }
	
	
private:
	TCapsImageProcessorExtension* iCaps;	// not owned
	TInt iError;
	};

//
//
//
LOCAL_C void ScanDirectoriesL(const TDesC& aRootDir, TBool aAsync = ETrue)
	{
	RFs fs;

	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	TBuf<256> path;
	_LIT(KFormatString, "%S*.jpg");
	path.AppendFormat(KFormatString, &aRootDir);
	
	CDirScan* dirScan = CDirScan::NewLC(fs);
	dirScan->SetScanDataL(path, KEntryAttNormal, 0);
	
	CDir* dir = NULL;
	dirScan->NextL(dir);
	if (dir)
		{
		CleanupStack::PushL(dir);
		
		if (dir->Count())
			{
			gConsole->Printf(_L("Number of JPG files: %d\n"), dir->Count());
			
			CImageProcessorObserver* observer = new(ELeave) CImageProcessorObserver();
			CleanupStack::PushL(observer);
			
			CImgProcessor* processor = CImgProcessor::NewL(fs, *observer);
			CleanupStack::PushL(processor);

		
			// CImgProcessor::InitializeL falls over if called several times.
			if (aAsync)
				{
				processor->InitializeL();
				CActiveScheduler::Start();
				User::LeaveIfError(observer->Error());
				}
			else
				{
				processor->InitializeL(CImgProcessor::EOptionSyncProcessing);
				}
			
			// Load the SpeedView thing.
			TCapsImageProcessorExtension* caps;
			caps = static_cast<TCapsImageProcessorExtension*>(processor->Extension(KCapsImageProcessorExtensionUid));
			if (!caps)
				{
				User::Leave(KErrNotFound);
				}
			
			// Find JPEG files to process.
			for (TInt i = 0; i < dir->Count(); i++)
				{
				TInt err = KErrNone;
				
				const TEntry& entry = (*dir)[i];
				
				TBuf<256> filename;
				
				_LIT(KFilenameFormat, "%S%S");
				filename.AppendFormat(KFilenameFormat, &aRootDir, &entry.iName);
				
				//  Set up the processing.
				TRAP(err,
					{
					processor->SetInputL(filename);
					processor->SetInputL(filename);
					processor->SetOutputL(_L("c:\\blah.jpg"));
					caps->SetAddSpmoToOutputL(ETrue, TSize(100, 100));
					processor->ProcessL();
					});
				
				if (err == KErrNone)
					{
					if (aAsync)
						{
						CActiveScheduler::Start();
						err = observer->Error();
						}
					}
				
				if (err == KErrNone)
					{
					gConsole->Printf(_L("Processed: %S\n"), &filename);
					}
				else
					{
					gConsole->Printf(_L("Skipped  : %S (%d)\n"), &filename, err);
					}
				
				}
			
			CleanupStack::PopAndDestroy(processor);
			CleanupStack::PopAndDestroy(observer);
			}
		
		CleanupStack::PopAndDestroy(dir);
		}
	
	
	CleanupStack::PopAndDestroy(dirScan);
	CleanupStack::PopAndDestroy(&fs);
	}


//
// 
//
LOCAL_C void MainL()
	{
	gConsole->Write(_L("Searching for image files\n"));

	_LIT(KCDrive, "c:\\");
	
	ScanDirectoriesL(KCDrive, ETrue);
	ScanDirectoriesL(KCDrive, EFalse);
	}


//
// In order to use Active Objects an active scheduler must be installed on the thread.
//
LOCAL_C void DoStartL()
	{
	CActiveScheduler* scheduler = new(ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	MainL();

	CleanupStack::PopAndDestroy(scheduler);
	}


//
//  Global Functions
//
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;

	TInt err = KErrNoMemory;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if (cleanup)
		{
		gConsole = NULL;
		
		TRAP(err,
			{
			gConsole = Console::NewL(KTextConsoleTitle, TSize(KConsFullScreen, KConsFullScreen));
			DoStartL();
			});

		if (gConsole)
			{
			if (err != KErrNone)
				{
				gConsole->Printf(KTextFailed, err);
				}
			
			gConsole->Printf(KTextPressAnyKey);
			gConsole->Getch();
			}
		
	
		delete gConsole;
		delete cleanup;
		}

	REComSession::FinalClose();
	
	__UHEAP_MARKEND;	// Panic if any memory leaks on exit.
	return err;
	}

