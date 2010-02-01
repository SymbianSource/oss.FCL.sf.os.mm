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

#ifndef __IMAGE_PROCESSOR_PROGRESS_INFO_INF_H__
#define __IMAGE_PROCESSOR_PRGORESS_INFO_INF_H__

#include <e32base.h>

namespace ImageProcessor
	{
namespace Plugin
	{

/**
This class provides functions to read progress information.

The TProgressInfo gets access to a plugin implementation based on this interface class. 
Once this has been constructed, calls to method functions of TProgressInfo are passed
verbatim to this interface. For further description of required functionality, 
see TProgressInfo.

@see TProgressInfo
@publishedPartner
@released
*/
class MProgressInfo
	{
public:
	virtual TInt CurrentIterationL() const = 0;
	virtual TInt NumberOfIterationsL() const = 0;
	virtual TAny* Extension(TUid aExtension) const = 0;
	
private:
	TInt iReserved; // future proof
	};

	}
	}

#endif //__IMAGE_PROCESSOR_PROGRESS_INFO_INF_H__
