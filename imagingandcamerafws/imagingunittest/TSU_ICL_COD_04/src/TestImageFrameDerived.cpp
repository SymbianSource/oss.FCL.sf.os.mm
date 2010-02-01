// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestImageFrameDerived.h"

/** CImageFrame type definition for CImageFrameDerived */ 
static const TUid KUidImageFrameExtraRChunkType	= {0x10273756};


CImageFrameDerived::CImageFrameDerived(CFbsBitmap& aBitmap) : 
	  								iImageFrameType(KUidImageFrameExtraRChunkType),
									iFrameBitmap(aBitmap)
	{

	}

CImageFrameDerived::~CImageFrameDerived()
	{
	}

CImageFrameDerived* CImageFrameDerived::NewL(const RChunk* aChunk, TInt aMaxSize, TInt aChunkOffset, CFbsBitmap& aBitmap)
	{
	CImageFrameDerived* self = new(ELeave) CImageFrameDerived(aBitmap);
	CleanupStack::PushL(self);
	self->ConstructL(aChunk, aMaxSize, aChunkOffset);
	CleanupStack::Pop();
	return self;
	}
	
TUid CImageFrameDerived::ImageFrameType()
	{
	return iImageFrameType;
	}
	
CFbsBitmap& CImageFrameDerived::FrameBitmap()
	{
	return iFrameBitmap;
	}
