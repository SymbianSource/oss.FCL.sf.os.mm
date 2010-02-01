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

/**
 @file
 @publishedPartner 
 @released 
*/

#ifndef __IMAGE_PROCESSOR_INPUT_INFO_INF_H__
#define __IMAGE_PROCESSOR_INPUT_INFO_INF_H__

#include <e32base.h>
#include <gdi.h>

namespace ImageProcessor
	{
namespace Plugin
	{

/**
This class provides functions to read image input details.

Plugin API to read image input details.
The TInputInfo implementation loads a plugin based on this interface class. 
Once this has been constructed, calls to method functions of TInputInfo are passed
verbatim to this interface. For further description of required functionality, 
see TInputInfo.

@see TInputInfo
@publishedPartner
@released
*/
class MInputInfo
	{
public:
	virtual TSize SizeL() const = 0;
	virtual void FormatL(TUid& aFormat, TUid& aSubFormat) const = 0;
	virtual TDisplayMode DisplayModeL() const = 0;
	virtual TUid ImageFrameFormatL() const = 0;
	virtual TUid SamplingL() const = 0; 
	virtual TUint32 BitsPerPixelL() const = 0; 
	virtual TBool IsProgressiveL() const = 0; 
	virtual TBool IsInterlacedL() const = 0;
	virtual TBool HasAlphaL() const = 0;
	virtual TAny* Extension(TUid aExtension) const = 0;
	
private:
	TInt iReserved; // future proof
	};

	}
	}

#endif //__IMAGE_PROCESSOR_INPUT_INFO_INF_H__
