
// CImageCodecBody.cpp
//   Support for the Codec Body classes

// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ImageConversion.h"
#include "icl/ImageCodec.h"
#include "ImageCodecBody.h"
#include "icl/imageprocessor.h"

//
// CImageProcessorReadCodecBody

CImageProcessorReadCodecBody::~CImageProcessorReadCodecBody()
	{
	delete iImageProc;
	}

CImageProcessorReadCodecBody* CImageProcessorReadCodecBody::NewL()
	{
	CImageProcessorReadCodecBody* result = new (ELeave) CImageProcessorReadCodecBody;
	return result;
	}

void CImageProcessorReadCodecBody::SetImageProcessor(CImageProcessor* aImageProc)
	{
	delete iImageProc; iImageProc = NULL;
	iImageProc = aImageProc;
	}

//
// CImageMaskProcessorReadCodecBody

CImageMaskProcessorReadCodecBody::~CImageMaskProcessorReadCodecBody()
	{
	delete iMaskProc;
	}

CImageMaskProcessorReadCodecBody* CImageMaskProcessorReadCodecBody::NewL()
	{
	CImageMaskProcessorReadCodecBody* result = new (ELeave) CImageMaskProcessorReadCodecBody;
	return result;
	}

void CImageMaskProcessorReadCodecBody::SetMaskProcessor(CImageProcessor* aMaskProc)
	{
	delete iMaskProc; iMaskProc = NULL;
	iMaskProc = aMaskProc;
	}

