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

#include "tphotoeditor.h"

//
// Private constructor, does nothing.
//
CTPhotoEditorAppView::CTPhotoEditorAppView(const TSize& aScreenSize)
 :	iMainPreviewIndex(0),
 	iZoomFactor(KDefaultZoom),
 	iScreenSize(aScreenSize)
	{
	}
	
//	
// Destructor
//
CTPhotoEditorAppView::~CTPhotoEditorAppView()
	{
	iPreviews.ResetAndDestroy();
	delete iText;
	}
	

void CTPhotoEditorAppView::ConstructL(const TRect& aRect)
	{
	iText = NULL;	
	CreateWindowL();
	SetRect(aRect);
	ActivateL();
	}

//
// Clears a part of the screen.
//
void CTPhotoEditorAppView::ClearBitmap(const TPoint& aPos, const TSize& aSize)
	{
	TRect rect(aPos, aSize);		
		
	CWindowGc& gc = SystemGc();
	this->Window().Invalidate(rect);
	this->Window().BeginRedraw(rect);
	gc.Activate(this->Window());
	gc.Clear(rect);
	this->Window().EndRedraw();
	gc.Deactivate();
	}

//
// Draws the bitmap on the screen.
//
void CTPhotoEditorAppView::DrawBitmap(CFbsBitmap* aBitmap, const TPoint* aPos)
	{
	ASSERT(aBitmap && aPos);
	TRect rect(*aPos, aBitmap->SizeInPixels());
	
	CWindowGc& gc = SystemGc();
	this->Window().Invalidate(rect);
	this->Window().BeginRedraw(rect);
	gc.Activate(this->Window());
	gc.BitBlt(*aPos, aBitmap);
	this->Window().EndRedraw();
	gc.Deactivate();
	
	}

void CTPhotoEditorAppView::DrawBitmap(TPreviewData* aData)
	{
	ASSERT(aData);
	TRect rect(aData->iLocation, aData->iBitmap->SizeInPixels());
	
	CWindowGc& gc = SystemGc();
	this->Window().Invalidate(rect);
	this->Window().BeginRedraw(rect);
	gc.Activate(this->Window());
	gc.BitBlt(aData->iLocation, aData->iBitmap);
	this->Window().EndRedraw();
	gc.Deactivate();
	}

//
// Draws rectangle over bitmap on the screen to highlight selected image in Gallery View
//
void CTPhotoEditorAppView::DrawRectangle(const TPoint& aPos, const TSize& aSize, const TBool aClear) const
	{
	TRect rect(aPos, aSize);
	
	CWindowGc& gc = SystemGc();
	this->Window().Invalidate(rect);
	this->Window().BeginRedraw(rect);
	gc.Activate(this->Window());
	
	gc.SetBrushStyle(CGraphicsContext::ENullBrush);
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	gc.SetPenColor(aClear ? KRgbWhite : KRgbRed);
	gc.DrawRect(rect);
	gc.MoveTo(aPos);
	gc.DrawLineBy(TPoint(aSize.iWidth - 1, aSize.iHeight - 1));
	gc.MoveTo(TPoint(aPos.iX + aSize.iWidth - 1, aPos.iY));
	gc.DrawLineTo(TPoint(aPos.iX, aPos.iY + aSize.iHeight - 1));
	this->Window().EndRedraw();
	gc.Deactivate();
	}

//
// Draws the text on the screen
//
void CTPhotoEditorAppView::Draw(const TRect& /*aRect*/) const
	{
	if (!iText)
		{
		return;
		}
	
	CWindowGc& gc = SystemGc();
	TRect drawRect = Rect();
	gc.Clear();
	}

void CTPhotoEditorAppView::AppendPreviewL(ImageProcessor::TPreview* aPreview, CFbsBitmap* aBitmap, const TPoint& aLocation, TInt aPreviewId)
	{
	TPreviewData* data = new(ELeave) TPreviewData();
	data->iPreview = aPreview;
	data->iBitmap = aBitmap;
	data->iLocation = aLocation;
	data->iPreviewId = aPreviewId;
	iPreviews.AppendL(data);
	}

//
// The first expected asynchronous callback when generating a preview.
// This is called once the preview has been initialised.
//
void CTPhotoEditorAppView::PreviewInitializedL(TInt aPreviewId)
	{
	TPreviewData* data = iPreviews[aPreviewId];
	data->iPreview = iImageProcessor->PreviewL(aPreviewId);
	
	data->iPreview->SetOutputL(*(data->iBitmap));
	data->iPreview->SetZoomL(iZoomFactor);
	TRAP_IGNORE(data->iPreview->SetPanL(data->iPanX, data->iPanY));
	data->iPreview->RenderL();
	}

//
// The second expected asynchronous callback when generating a preview.
// This is called once the preview has been rendered to the memory bitmap.
//
void CTPhotoEditorAppView::PreviewRenderedL(TInt aPreviewId)
	{
	BlitPreviewToScreenL(aPreviewId);
	}

//
// Blits a preview image to the screen.
//
void CTPhotoEditorAppView::BlitPreviewToScreenL(TInt aPreviewId)
	{
	TPreviewData* data = iPreviews[aPreviewId];
	ASSERT(data);
	
	DrawBitmap(data);
	PreviewBlittedL(aPreviewId);
	}


void CTPhotoEditorAppView::CheckProcessingMode()
	{
	iAsync = ETrue;
	TUint64 options = iImageProcessor->Options();
	
	if ((options & ImageProcessor::CImgProcessor::EOptionSyncProcessing))
		{
		iAsync = EFalse;
		}
	}

void CTPhotoEditorAppView::RefreshMainPreviewL(TBool aClearFirst)
	{
	RedrawPreviewL(iPreviews[iMainPreviewIndex], aClearFirst);
	}


void CTPhotoEditorAppView::RedrawPreviewL(TPreviewData* aPreviewData, TBool aClearFirst)
	{
	ASSERT(iImageProcessor);
	ASSERT(aPreviewData);
	
	CheckProcessingMode();
	
	HBufC* filename = aPreviewData->iFileName;
	if (!filename)
		{
		// Draw a red box
		ASSERT(aPreviewData->iBitmap);
		DrawRectangle(aPreviewData->iLocation, aPreviewData->iBitmap->SizeInPixels(), EFalse);
		User::Leave(KErrNotReady);
		return;
		}
	
	// Some effect such as Rotation require the previous preview
	// image to be cleared.
	if (aClearFirst)
		{
		ASSERT(aPreviewData->iBitmap);
		ClearBitmap(aPreviewData->iLocation, aPreviewData->iBitmap->SizeInPixels());
		}
	
	if (iAsync)
		{
		aPreviewData->AsyncRenderL();
		}
	else
		{
		aPreviewData->SyncRenderL();
		BlitPreviewToScreenL(aPreviewData->iPreviewId);
		}
	}

RPointerArray<TPreviewData> CTPhotoEditorAppView::GetPreviewsL()
	{
	return iPreviews;
	}

void CTPhotoEditorAppView::ZoomL(TZoomMode aDirection, TBool aRedraw)
	{
	// Zooming is only applied to the main preview.
	iData = iPreviews[iMainPreviewIndex];
	ASSERT(iData);
	
	TReal32 zoomMinimumLimit, zoomMaximumLimit;
	
	if (iData->iPreview)
		{
		iData->iPreview->GetZoomLevelSettings(zoomMinimumLimit, zoomMaximumLimit);			
		}
				
	switch (aDirection)
		{
		case EZoomIn:
			iData->iZoomFactor = Min(zoomMaximumLimit, iData->iZoomFactor + KZoomStep);
			break;
			
		case EZoomOut:
			iData->iZoomFactor = Max(zoomMinimumLimit, iData->iZoomFactor - KZoomStep);
			break;
			
		case EZoomReset:
			iData->iZoomFactor = KDefaultZoom;
			break;
			
		default:
			ASSERT(EFalse);
		}
	
	if (aRedraw)
		{
		RedrawPreviewL(iData);
		}
	}
	
void CTPhotoEditorAppView::ReFreshScreenL(TBool aClearFirst)
	{
	// clear the screen and redraw the image.
	RedrawPreviewL(iData,aClearFirst);	
	}
	
void CTPhotoEditorAppView::PanL(TPanMode aPanMode, TBool aRedraw)
	{
	TPreviewData* data = iPreviews[iMainPreviewIndex];
	ASSERT(data);
	
	TReal32 panMinimumLimit, panMaximumLimit;

	if (data->iPreview != NULL)
		{
		data->iPreview->GetPanLevelSettings(panMinimumLimit, panMaximumLimit);	
		}
		
	switch (aPanMode)
		{
		case EPanReset:
			data->iPanX = KDefaultPanX;
			data->iPanY = KDefaultPanY;
			break;
			
		case EPanUp:
			data->iPanY = Max(panMinimumLimit, data->iPanY - KPanStep);
			break;
			
		case EPanDown:
			data->iPanY = Min(panMaximumLimit, data->iPanY + KPanStep);
			break;
			
		case EPanLeft:
			data->iPanX = Max(panMinimumLimit, data->iPanX - KPanStep);
			break;
		
		case EPanRight:
			data->iPanX = Min(panMaximumLimit, data->iPanX + KPanStep);
			break;
			
		default:
			ASSERT(EFalse);
		}
	
	if (aRedraw)
		{
		RedrawPreviewL(data);
		}
	}

TSize& CTPhotoEditorAppView::ScreenSize()
	{
	return iScreenSize;
	}


void CTPhotoEditorAppView::ClearScreen()
	{
	ClearBitmap(TPoint(0, 0), iScreenSize);
	}

