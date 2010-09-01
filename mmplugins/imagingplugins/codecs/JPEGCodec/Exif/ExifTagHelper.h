// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __EXIFTAGHELPER_H__
#define __EXIFTAGHELPER_H__

#include <e32base.h>
#include "ExifGeneralConsts.h"

//ExifTag class defines the Tags found in the Exif specification.

class TExifTagHelper
	{
public:
	static TBool IsCorrectParentIfd(TUint aTag, TUint aIfd); // Return ETrue if the given tag is in the given Ifd.
	static HBufC8* CreateUnicodePrefixedBufferL(const TDesC16& aUnicodeBuffer); // Returns an HBufC8* containing aUnicodeBuffer prefixed with UNICODE/0. Caller owns memory.
	};


class TExifIfdHelper
	{
public:
	// The upper bound of the TIfd enum.
	enum { EMaxIfd = EGpsSub };
	// Returns ETrue if aInternalIfdNum is in the TIfd enum
	static TBool IsValidInternalIfd(const TInt aInternalIfdNum); 
	// Tests if aExternalIfdNum contains the IFD0 sub-ifd "aSubIfd" in its upper 16 bits.
	static TBool IfdContainsIfd0SubIfd(const TInt aExternalIfdNum, const TInt aSubIfd);
	};
#endif //__EXIFTAGHELPER_H__
