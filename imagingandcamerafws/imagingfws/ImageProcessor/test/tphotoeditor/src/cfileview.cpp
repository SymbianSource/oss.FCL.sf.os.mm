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
// cfileview.h
// 
//

#include "tphotoeditor.h"



CTPhotoEditorFileView::CTPhotoEditorFileView(const TSize& aScreenSize)
 :	CTPhotoEditorAppView(aScreenSize)
	{
	}

CTPhotoEditorFileView::~CTPhotoEditorFileView()
	{
	delete iFileName;
	}

CTPhotoEditorFileView* CTPhotoEditorFileView::NewL(const TRect& aRect, const TSize& aScreenSize)
	{
	CTPhotoEditorFileView* self = new(ELeave) CTPhotoEditorFileView(aScreenSize);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}

void CTPhotoEditorFileView::ConstructL(const TRect& aRect)
	{
	CTPhotoEditorAppView::ConstructL(aRect);
	
	TPoint pos(0, 0);
	
	iNumberOfPreviews = 1;
	CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
	bitmap->Create(ScreenSize(), KDefaultDisplayMode);
	AppendPreviewL(NULL, bitmap, pos, 0);
	}

//
// This does the image rendering once the ImageProcessor has been set up
// by one of the LoadFileL() functions.
//
void CTPhotoEditorFileView::DoLoadFileL(const TDesC& aFileName)
	{
	ASSERT(iImageProcessor);
	
	if (iFileName)
		{
		delete iFileName;
		iFileName = NULL;
		}
	
	iFileName = aFileName.AllocL();
	
	TPreviewData* data = iPreviews[0];
	data->iPreview = iImageProcessor->PreviewL(0);
	data->iFileName = iFileName;
	
	CheckProcessingMode();
	ZoomL(EZoomReset, EFalse);
	PanL(EPanReset, EFalse);
	ClearScreen();
	
	if (iAsync)
		{
		data->AsyncRenderL();
		}
	else
		{
		data->SyncRenderL();
		BlitPreviewToScreenL(0);
		}
	}

//
// Loads a DRM'ed file.
//
void CTPhotoEditorFileView::LoadFileL(TMMFileSource& aDrmFile, ImageProcessor::CImgProcessor* aImageProcessor)
	{
	ReSetImageProcessor(aImageProcessor);
	iImageProcessor->SetInputL(aDrmFile);
	iImageProcessor->SetBackgroundColorL(KRgbWhite);
	DoLoadFileL(aDrmFile.Name());
	}

//
// Loads a normal file.
//
void CTPhotoEditorFileView::LoadFileL(const TDesC& aFile, ImageProcessor::CImgProcessor* aImageProcessor)
	{
	ReSetImageProcessor(aImageProcessor);
	iImageProcessor->SetInputL(aFile);
	iImageProcessor->SetBackgroundColorL(KRgbWhite);
	DoLoadFileL(aFile);
	}

void CTPhotoEditorFileView::ReSetImageProcessor(ImageProcessor::CImgProcessor* aImageProcessor)
	{
	ASSERT(aImageProcessor);
	iImageProcessor = aImageProcessor;
	iImageProcessor->ResetL();
	iImageProcessor->SetOptionsL(ImageProcessor::CImgProcessor::EOptionSyncProcessing);
	}

