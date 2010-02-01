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

#ifndef EXIFIMAGEDISPLAY_H
#define EXIFIMAGEDISPLAY_H

/** 	@file
	@internalComponent */

#include "GenericIclWrapper.h"
#include <icl/exifimagedisplayext.h>


class MExifMetadataReader;

class CExifImageDisplayPlugin : public CGenericImageDisplayPlugin,
								protected MExifImageDisplayExtension
	{
public:
 	static CExifImageDisplayPlugin* NewL();
	~CExifImageDisplayPlugin();

protected:
// from the MExifImageDisplayExtension
	virtual TInt GetExifMetadataReader(MExifMetadataReader*& aPtr);
	virtual const TUid Uid() const;
	virtual void Release();
	virtual TInt SetScaleQuality(TInt aQualityLevel);
	virtual TInt SetMaximumReductionFactor(TInt aMaxReductionFactor);	

// CIclWrapperImageDisplayPlugin
	virtual TBool ThumbnailExists() const;
	virtual TUint EffectiveRotation() const;
	virtual void  AfterOpenL();
	virtual TUid  RequiredImageClass() const;
	virtual TUint ImageStatus() const;

// from the CImageDisplayPlugin
	virtual TInt ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr);

private:
	CExifImageDisplayPlugin();

private:
	MExifMetadataReader* iExifReader;
	};	


#endif // EXIFIMAGEDISPLAY_H

