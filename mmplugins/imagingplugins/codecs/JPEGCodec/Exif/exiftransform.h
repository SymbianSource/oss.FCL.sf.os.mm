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

#ifndef EXIFTRANSFORM_H
#define EXIFTRANSFORM_H

#include "exifreaderwriter.h" // For MExifMetadata declaration

// CExifTransform - provides specialisation of CExifReaderWriter for use within the 
// Jpeg Exif transform plugin

class CExifTransform : public CExifReaderWriter
	{	
public:
	static CExifTransform* NewL(const TUint8* aData = NULL, const TUint aApp1Size = 0);
	~CExifTransform();
	
	// Functions not derived from MExifMetadata.
	TInt CreateExifChunk(HBufC8*& aExifChunk);
	
	void CheckUpdateMandatoryTagsL();
	TBool CheckImageSizeTags();
	void UpdateImageSizeTagsL(const TSize &aSize);
private: // functions
	CExifTransform();
	void ConstructL(const TUint8* aData, const TUint aApp1Size);
	virtual void SetIfd1L();
	void CreateExifHeaderL(HBufC8*& aBuffer);
	};

#endif // EXIFTRANSFORM_H
