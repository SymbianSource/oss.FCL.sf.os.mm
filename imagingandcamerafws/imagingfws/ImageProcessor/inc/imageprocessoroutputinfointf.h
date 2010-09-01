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
// imageprocessorioutputinfointf.h
// 
//

/**
 @file
 @publishedPartner 
 @released 
*/

#ifndef __IMAGE_PROCESSOR_OUTPUT_INFO_INF_H__
#define __IMAGE_PROCESSOR_OUTPUT_INFO_INF_H__

#include <e32base.h>

namespace ImageProcessor
	{
namespace Plugin
	{

/**
This class provides functions to write image output details.

Plugin API to write image output details.
The TOutputInfo implementation loads a plugin based on this interface class. 
Once this has been constructed, calls to method functions of TOutputInfo are passed
verbatim to this interface. For further description of required functionality, 
see TOutputInfo.

@see TOutputInfo
@publishedPartner
@released
*/
class MOutputInfo
	{
public:
	virtual void SetJpegQualityL(TReal32& aQuality) = 0;
	virtual void SetSamplingL(TUid& aSampling) = 0;
	virtual TAny* Extension(TUid aExtension) const = 0;
	
private:
	TInt iReserved; // future proof
	};

	}
	}

#endif //__IMAGE_PROCESSOR_OUTPUT_INFO_INF_H__
