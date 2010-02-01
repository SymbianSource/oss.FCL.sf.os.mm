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

#ifndef EXIFDECODER_H
#define EXIFDECODER_H

#include "ifdgeneral.h"
#include "exifreaderwriter.h"

// CExifDecoder - provides specialisation of CExifReaderWriter for use within the 
// Jpeg Exif Decoder
class CExifDecoder : public CExifReaderWriter
	{	
public:
	static CExifDecoder* NewL(const TUint aApp1Size, const TUint8* aData);

	~CExifDecoder();
	
	// Setters (not supported - this is a decoder)
	TInt SetParam8(TUint aTag, TUint aIfd, HBufC8* aParam);
	TInt SetParam16(TUint aTag, TUint aIfd, HBufC16* aParam);
	TInt SetIntegerParam(TUint aTag, TUint aIfd, TInt aParam);
	TInt SetShortParam(TUint aTag, TUint aIfd, TUint16 aParam);
	TInt SetRationalParam(TUint aTag, TUint aIfd, TInt aNumerator, TInt aDenominator);	
	TInt SetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam);
	TInt SetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam);
	TInt SetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam);
	
private: // functions
	CExifDecoder();
	void ConstructL(const TUint8* aData, TUint aApp1Size);
	virtual void SetIfd1L();
	
private: // member variables
	};
	
#endif // EXIFDECODER_H
