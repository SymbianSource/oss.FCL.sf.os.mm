// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

TPreviewData::TPreviewData()
 :	iPreview(NULL),
 	iBitmap(NULL),
 	iZoomFactor(KDefaultZoom),
 	iPanX(KDefaultPanX),
 	iPanY(KDefaultPanY),
 	iFileName(NULL),
 	iPreviewId(0)
	{
	}

TPreviewData::~TPreviewData()
	{
	ASSERT(iBitmap);
	delete iBitmap;
	iBitmap = NULL;
	}

TBool TPreviewData::IsInitialized()
	{
	ASSERT(iPreview);
	return (iPreview->State() == ImageProcessor::TPreview::EInitialized);
	}

//
// Kicks off an asynchronous preview render.
//
void TPreviewData::AsyncRenderL()
	{
	ASSERT(iPreview && iBitmap);
				
	if (!IsInitialized())
		{
		iPreview->InitializeL();
		return;
		}
	
	iPreview->SetOutputL(*iBitmap);
	iPreview->SetZoomL(iZoomFactor);
	if (iZoomFactor > KDefaultZoom)
		{
		iPreview->SetPanL(iPanX, iPanY);
		}
	
	iPreview->RenderL();
	}
			
//
// Kicks of a synchronous preview render.
//
void TPreviewData::SyncRenderL()
	{
	// Assuming ImageProcessor->SetInputL() has been called.
	ASSERT(iPreview && iBitmap);
			
	if (!IsInitialized())
		{
		iPreview->InitializeL();
		}
	
	iPreview->SetOutputL(*iBitmap);
	iPreview->SetZoomL(iZoomFactor);
	
	if (iZoomFactor > KDefaultZoom)
		{
		iPreview->SetPanL(iPanX, iPanY);
		}
	
	iPreview->RenderL();
	}
