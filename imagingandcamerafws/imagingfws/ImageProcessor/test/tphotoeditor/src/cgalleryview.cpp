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
// cgalleryview.h
// 
//

#include "tphotoeditor.h"

CTPhotoEditorGalleryView::CTPhotoEditorGalleryView(const TSize& aScreenSize)
 :	CTPhotoEditorAppView(aScreenSize)
	{
	}

CTPhotoEditorGalleryView::~CTPhotoEditorGalleryView()
	{
	iCanvasPoints.Close();
	delete iSavedImage;
	}


CTPhotoEditorGalleryView* CTPhotoEditorGalleryView::NewL(const TRect& aRect, const TSize& aScreenSize)
	{
	CTPhotoEditorGalleryView* self = new(ELeave) CTPhotoEditorGalleryView(aScreenSize);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}

void CTPhotoEditorGalleryView::ConstructL(const TRect& aRect)
	{
	CTPhotoEditorAppView::ConstructL(aRect);
	
#ifdef __WINS__
	iMaxNumberOfPreviews = 7;
	iMainPreviewIndex = 3;
	
	iMainPreviewSize = TSize(324, 180);
	iCanvasPoints.AppendL(TPoint(25, 170));
	iCanvasPoints.AppendL(TPoint(25, 105));
	iCanvasPoints.AppendL(TPoint(60, 25));
	iCanvasPoints.AppendL(TPoint(150, 10));
	iCanvasPoints.AppendL(TPoint(505, 25));
	iCanvasPoints.AppendL(TPoint(540, 105));
	iCanvasPoints.AppendL(TPoint(540, 170));
	
#else
	iMaxNumberOfPreviews = 1;
	iMainPreviewIndex = 0;
	
	iMainPreviewSize = TSize(320, 240); 
	iCanvasPoints.AppendL(TPoint(0, 0));
#endif
	
	iNumberOfPreviews = iMaxNumberOfPreviews;	
	ASSERT(iCanvasPoints.Count() == iMaxNumberOfPreviews);
	
	for (TInt i = 0; i < iNumberOfPreviews; i++)
		{
		CFbsBitmap* bitmap = new(ELeave) CFbsBitmap();
		bitmap->Create((i == iMainPreviewIndex ? iMainPreviewSize : KThumbnailSize), KDefaultDisplayMode);
		AppendPreviewL(NULL, bitmap, iCanvasPoints[i],i);
		}
	iSavedImage = HBufC::NewL(KMaxFileName);	
	}

//
//Called when a preview has been drawn to the screen.
//Can be called in both sync and async modes.
//
void CTPhotoEditorGalleryView::PreviewBlittedL(TInt /*aPreviewId*/)
	{
	if (iAsync)
		{
		// See if there are any more files to be previewed.
		DrawNextPreviewL();
		}
	}


void CTPhotoEditorGalleryView::LoadGalleryL(RPointerArray<HBufC>& aFiles, ImageProcessor::CImgProcessor& aProcessor)
	{
	iImageProcessor = &aProcessor;
	iFiles = &aFiles;
	
	if (aFiles.Count() <= 0)
		{
		ClearScreen();
		return;
		}
	
	CheckProcessingMode();
	
	iNavigationOffset = 0;
	iBitmapToUse = -1;
	iNumberOfPreviews = Min(aFiles.Count(), iMaxNumberOfPreviews);
	iMode = ERenderModeGallery;
	ConfigurePreviewsL();

	DrawNextPreviewL();
	}

//
// This function kicks off the preview asynchronous sequence.
// If synchronous processing each preview will be dealt with
// in sequence, for asynchronous processing 'iNumberOfPreviews'
// requests will be generated for displaying later once they're ready.
//
void CTPhotoEditorGalleryView::DrawNextPreviewL()
	{
	ASSERT(iFiles);
	ASSERT(iImageProcessor);
	ASSERT(iPreviews.Count() > 0);
	iBitmapToUse++;

	if (iBitmapToUse >= iNumberOfPreviews)
		{
		iMode = ERenderModeNone;
		
		// Restore the input to the main preview.
		// All operations should be applied to this.
		HBufC* mainFileName = iPreviews[iMainPreviewIndex]->iFileName;
		if (mainFileName)
			{
			iImageProcessor->ResetL();
			iImageProcessor->SetOptionsL(ImageProcessor::CImgProcessor::EOptionSyncProcessing);		
			TPreviewData* data = iPreviews[iMainPreviewIndex];
			data->iPreview = iImageProcessor->PreviewL(iMainPreviewIndex);
			TRAPD(err,iImageProcessor->SetInputL(*mainFileName));
			// Check to see if it is a DRM file.
			if (err != KErrNone)
				{
				TFileName drmfile(*(data->iFileName));
				TMMFileSource drmFile1(drmfile, KRightsUniqueId001, ContentAccess::EView);					
				iImageProcessor->SetInputL(drmFile1);					
				}
			iImageProcessor->SetBackgroundColorL(KRgbWhite);
			}
		else
			{
			RDebug::Printf("TODO: Reached indecision point");
			}

		return;
		}
	
	TPreviewData *data = iPreviews[iBitmapToUse];			
	
	// Avoid rendering main image next time because imageprocessor loose all the effects with 
	// call ReSetImageProcessorL, continue with next preview.

	ASSERT(data);
	
	if (!data->iFileName)
		{
		// The file isn't an image so display a big X.
		ASSERT(data->iBitmap);
		DrawRectangle(data->iLocation, data->iBitmap->SizeInPixels(), EFalse);
		DrawNextPreviewL();	// TODO: Is recursion a problem?
		}
	else
		{
		iImageProcessor->ResetL();
		iImageProcessor->SetOptionsL(ImageProcessor::CImgProcessor::EOptionSyncProcessing);	
		CheckProcessingMode();
		TPreviewData* data = iPreviews[iBitmapToUse];
		data->iPreview = iImageProcessor->PreviewL(iBitmapToUse);
		TRAPD(err, iImageProcessor->SetInputL(*(data->iFileName)));
		// Check to see if it is a DRM file.
		if (err != KErrNone)
			{
			TFileName drmfile(*(data->iFileName));
			TMMFileSource drmFile1(drmfile, KRightsUniqueId001, ContentAccess::EView);					
			TRAPD(err, iImageProcessor->SetInputL(drmFile1));
			if (err != KErrNone)
				{
				// Not an image so display a big X.
				data->iFileName = NULL;
				ASSERT(data->iBitmap);
				DrawRectangle(data->iLocation, data->iBitmap->SizeInPixels(), EFalse);
				DrawNextPreviewL();
				return;
				}
			}
			
		iImageProcessor->SetBackgroundColorL(KRgbWhite);
		// It's an image so render it.
		if (iAsync)
			{
			data->AsyncRenderL();
			}
		else
			{
			data->SyncRenderL();
			BlitPreviewToScreenL(iBitmapToUse);
			DrawNextPreviewL();	
			}
		}
	}

void CTPhotoEditorGalleryView::ConfigurePreviewsL(TBool aNavigateForward)
	{
	TInt minis = iNumberOfPreviews - 1;	// The number of small previews.
	TInt lhs = minis / 2;				// The number of previews to the left of the main preview.
	TInt rhs = minis - lhs;				// The number of previews to the right of the main preview.
	
	switch (iMode)
		{
		case ERenderModeGallery:
			{
			// We need to populate the previews.
			TInt previewIndex = 0;
			for (TInt i = -lhs; i <= rhs; i++)
				{
				HBufC *filename = GetFileAt(iNavigationOffset + i);
				if (!iSavedImage->Compare(*filename))
					{
					rhs++;
					continue;						
					}
				iPreviews[previewIndex++]->iFileName = filename;
				}
			// Main preview should be saved image.	
			if (iImageSaved)
				{
				for (TInt i = 0; i < lhs; i++)
					{
					iPreviews[i]->iFileName = iPreviews[i+1]->iFileName;
					}
				iPreviews[iMainPreviewIndex]->iFileName = iSavedImage;					
				}
			}
			break;
			
		case ERenderModeNavigate:
			{
			TInt limit = iNumberOfPreviews - 1;
			
			if (aNavigateForward)
				{
				// We need to shift the array to the left.
				for (TInt i = 0; i < limit; i++)
					{
					iPreviews[i]->iFileName = iPreviews[i + 1]->iFileName;
					}
				
				iPreviews[limit]->iFileName = GetFileAt(iNavigationOffset + rhs);
				}
			else
				{
				// We need to shift the array to the right.
				for (TInt i = limit; i > 0; i--)
					{
					iPreviews[i]->iFileName = iPreviews[i - 1]->iFileName;
					}
				
				iPreviews[0]->iFileName = GetFileAt(iNavigationOffset - lhs);
				}
			}
			break;
			
		default:
			ASSERT(EFalse);
		}
	}


void CTPhotoEditorGalleryView::NavigateL(TBool aForward)
	{
	if (iMode != ERenderModeNone)
		{
		// A navigation is already underway.
		User::Leave(KErrNotReady);
		}
	
	if (aForward)
		{
		iNavigationOffset++;
		if (iNavigationOffset >= iFiles->Count())
			{
			iNavigationOffset = 0;
			}
		}
	else
		{
		iNavigationOffset--;
		if (iNavigationOffset < 0)
			{
			iNavigationOffset = iFiles->Count() - 1;
			}
		}
	
	CheckProcessingMode();
	ClearScreen();
	
	ZoomL(EZoomReset, EFalse);
	PanL(EPanReset, EFalse);
	
	iMode = ERenderModeNavigate;
	iBitmapToUse = -1;
	iZoomFactor = 1.0f;
	ConfigurePreviewsL(aForward);
	DrawNextPreviewL();
	}

HBufC* CTPhotoEditorGalleryView::GetFileAt(TInt aIndex)
	{
	TInt count = iFiles->Count();
	
	// TODO: There must be a better way of doing this.
	while (aIndex < 0)
		{
		aIndex += count;
		}
	
	while (aIndex >= count)
		{
		aIndex -= count;
		}

	return (*iFiles)[aIndex];
	}

void CTPhotoEditorGalleryView::SetSavedFilesConfigurePreviewL(RPointerArray<HBufC>& aFiles, const TDesC& aSavedImage)
	{
	iFiles = NULL;
	iFiles = &aFiles;
	iBitmapToUse = -1;
	*iSavedImage = aSavedImage;
	iImageSaved = ETrue;
	iMode = ERenderModeGallery;
	ClearScreen();
	ConfigurePreviewsL();
	DrawNextPreviewL();
	iImageSaved = EFalse;
	}


