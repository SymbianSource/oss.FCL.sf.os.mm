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

#include <e32std.h>
#include <imageprocessor/imageprocessorprogressinfointf.h>
#include <imageprocessor/imageprocessorprogressinfo.h>
#include "imageprocessorimpl.h"
#include <imageprocessor/imageprocessor.h>
#include "imageprocessormain.h"

namespace ImageProcessor
	{

TProgressInfo::TProgressInfo(Plugin::MProgressInfo& aProgressInfoImpl, CImageProcessorImpl& aImageProcessorImpl)
:iImageProcessorImpl(aImageProcessorImpl), 
	iProgressInfoImpl(aProgressInfoImpl),
	iReserved(0)
	{
	}

/**
Gets the current iteration count.

@return	the current iteration value.
*/
EXPORT_C TInt TProgressInfo::CurrentIterationL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iProgressInfoImpl.CurrentIterationL();
	}

/**
Gets the total number of iterations expected to complete the task.

@return	the total number of expected iterations.
*/
EXPORT_C TInt TProgressInfo::NumberOfIterationsL() const 
	{
	__ASSERT_ALWAYS(iImageProcessorImpl.IsInputSet(), User::Leave(KErrNotReady));
	return iProgressInfoImpl.NumberOfIterationsL();
	}

/**
Gets access to custom extension.
*/
EXPORT_C TAny* TProgressInfo::Extension(TUid aExtension) const 
	{
	__ASSERT_ALWAYS(((iImageProcessorImpl.State() != CImgProcessor::EUninitialized) && (iImageProcessorImpl.State() != CImgProcessor::EInitializing)), Panic(EPanicProgressInfoNotAvailable));
	return iProgressInfoImpl.Extension(aExtension); 
	}
   
	}//namespace ImageProcessor

//EOF
