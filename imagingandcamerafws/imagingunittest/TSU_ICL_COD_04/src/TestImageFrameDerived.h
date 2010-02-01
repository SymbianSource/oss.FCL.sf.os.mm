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

#ifndef TESTIMAGEFRAMEDERIVED_H
#define TESTIMAGEFRAMEDERIVED_H

#include <e32std.h>
#include <e32base.h> 
#include <imageframe.h>
#include <fbs.h>

class CImageFrameDerived : public CImageFrame 
    {
public:  
    
	static CImageFrameDerived* NewL(const RChunk* aChunk, TInt aMaxBufferSize, TInt aChunkOffset, CFbsBitmap& aBitmap);
	~CImageFrameDerived();

	CFbsBitmap& FrameBitmap();
	
	TUid ImageFrameType();

protected:	
	CImageFrameDerived(CFbsBitmap& aBitmap);

protected:	
	TUid iImageFrameType;
	CFbsBitmap& iFrameBitmap;        	 
   };


#endif //TESTIMAGEFRAMEDERIVED_H
