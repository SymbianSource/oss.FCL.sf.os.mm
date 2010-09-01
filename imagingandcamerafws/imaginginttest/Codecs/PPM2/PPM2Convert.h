// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PPM2CONVERT_H__
#define __PPM2CONVERT_H__

#include <icl/imageplugin.h>
#include "PPM2Codec.h"

// Decoder.
class CPpmReadCodec;
class CPpmDecoder : public CImageDecoderPlugin
	{
friend class CPpmReadCodec;

public:
	static CPpmDecoder* NewL();
	virtual ~CPpmDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

private:
	CPpmDecoder();

	void InternalizeHeaderL();

	TBool Compressed() const;
	void SetCompressed(TBool aCompressed);
	TBool BytesValid() const;
	void SetBytesValid(TBool aBytesValid);
	TInt DataShift() const;

	void SkipCommentAndWhiteSpaceL();

	TUint PeekByteL();
	void IncByte();
	TUint GetByteL();
	TInt ReadIntL();
	void ResetPosition(TInt aPosition);

	// From CImageDecoderPlugin
	void ScanDataL();
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();
	void DoConvert();

private:
	TInt iDataShift;
	TInt iMaxValue;
	TUint iFlags;
	TPtrC8 iSourceBuffer;
	TInt iBase;
	enum TFlags
		{
		EFlagCompressed=0x01,
		EFlagBytesValid=0x02
		};
	};

// CPpmEncoder
class CPpmEncoder : public CImageEncoderPlugin
	{
friend class CPpm2WriteCodec;	
	
public:
	static CPpmEncoder* NewL();
	virtual ~CPpmEncoder();
	
protected:
	void AppendDataL(const TDesC8& aData);
	// from CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	void UpdateHeaderL();
	void DoConvert();
	
private:
	CPpmEncoder();
	void DoConvertL();

	};

#include "Ppm2Convert.inl"

#endif // __PPM2CONVERT_H__

