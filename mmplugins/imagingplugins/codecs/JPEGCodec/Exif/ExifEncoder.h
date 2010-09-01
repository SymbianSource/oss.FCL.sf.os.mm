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

#ifndef EXIFENCODER_H
#define EXIFENCODER_H

#include <e32base.h>
#include "ifdgeneral.h"
#include <iclexif.h>
#include "ExifGeneralConsts.h"
#include "exifreaderwriter.h"

// CExifEncoder - provides specialisation of CExifReaderWriter for use within the 
// Jpeg Exif Encoder
class CExifEncoder : public CExifReaderWriter
	{
public:

	static CExifEncoder* NewL(TBool aEncodeThumbnail=ETrue);
	~CExifEncoder();
	
	// Functions derived from CExifReaderWriter.
	// (Getters not used - this is an encoder).
	TInt GetParam8(TUint aTag, TUint aIfd, HBufC8*& aParam) const;
	TInt GetParam16(TUint aTag, TUint aIfd, HBufC16*& aParam) const;	
	TInt GetIntegerParam(TUint aTag, TUint aIfd, TInt& aParam) const;
	TInt GetRationalParam(TUint aTag, TUint aIfd, TInt& aNumerator, TInt& aDenominator) const;	
	TInt GetShortParam(TUint aTag, TUint aIfd, TUint16& aParam) const;
	TInt GetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam) const;
	TInt GetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam) const;
	TInt GetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam) const;

	
	//Functions not derived from MExifMetaData.	
	// Creates the Exif Header, and returns pointer to the block of data.
	TPtrC8 CreateExifHeaderL();
	void SetEnableThumbnailL(TBool aEnable);

	// Returns an offset to the JPEGInterchangeFormatLength tag value/offset field.
	TInt ThumbnailLengthOffset();
	TInt ExifLengthOffset();

private: // Functions
	
	CExifEncoder(TBool aEncodeThumbnail);
	void ConstructL();
	virtual void SetIfd1L();
		
private: // Member variables.

	HBufC8* iExifData;
	};

#endif // EXIFENCODER_H
