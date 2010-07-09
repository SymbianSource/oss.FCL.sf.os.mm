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

#include <imageprocessor/imageprocessoroutputinfointf.h>
#include <imageprocessor/imageprocessoroutputinfo.h>
#include "imageprocessorimpl.h"
#include <imageprocessor/imageprocessor.h>

namespace ImageProcessor
	{

TOutputInfo::TOutputInfo(Plugin::MOutputInfo& aOutputInfoImpl, CImageProcessorImpl& aImageProcessorImpl)
:iImageProcessorImpl(aImageProcessorImpl), 
	iOutputInfoImpl(aOutputInfoImpl),
	iReserved(0)
	{
	}

/**
Sets the output JPEG image quality

@param	aQuality
	    The new JPEG image quality to set to the output. Range (0, 1], where 1 means best JPEG quality (100%).
*/
EXPORT_C void TOutputInfo::SetJpegQualityL(TReal32& aQuality)
	{
	__ASSERT_ALWAYS((0.0f<aQuality)&&(1.0f>=aQuality),User::Leave(KErrArgument));
	__ASSERT_ALWAYS(iImageProcessorImpl.IsOutputSet(), User::Leave(KErrNotReady));
	iOutputInfoImpl.SetJpegQualityL(aQuality);
	}

/**
Sets the output JPEG image sampling rate

@param	aSampling
	    The new JPEG image sampling rate to set to the output.
*/
EXPORT_C void TOutputInfo::SetSamplingL(TUid& aSampling)
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsOutputSet(), User::Leave(KErrNotReady));
	iOutputInfoImpl.SetSamplingL(aSampling);
	}

	}//namespace ImageProcessor

//EOF
