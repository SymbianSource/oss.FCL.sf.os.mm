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

#ifndef __PPMCONVERT_H__
#define __PPMCONVERT_H__

#include "icl/imageplugin.h"
#include "PPM1Codec.h"
#include "icl/imageconversionextension.h"
#include "pluginextensionmanager.h"

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
	virtual void GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr);
	virtual void SetClippingRectL(const TRect* aClipRect);
	virtual TInt GetDestinationSize(TSize& aSize, TInt aFrameNumber);

private:
	CPpmDecoder();

	void InternalizeHeaderL(TLex8& aLex);
	void SkipCommentAndWhiteSpaceL(TLex8& aLex);
	void DoSkipCommentL(TLex8& aLex);

	TBool Compressed() const;
	void SetCompressed(TBool aCompressed);
	TBool InComment() const;
	void SetInComment(TBool aInComment);
	TInt DataShift() const;

	// From CImageDecoderPlugin
	void ScanDataL();
	// Called by ScanDataL.
	// Opens file, scans header and validates format.
	void ReadFormatL();

private:
	TInt iDataShift;
	TInt iMaxValue;
	TUint iFlags;
	enum TFlags
		{
		EFlagCompressed=0x01,
		EFlagInComment=0x02
		};
	CPluginExtensionManager* iExtensionManager; // owned
	};

// CPpmEncoder
class CPpmEncoder : public CImageEncoderPlugin
	{
public:
	static CPpmEncoder* NewL();
	virtual ~CPpmEncoder();
protected:
	// from CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	void UpdateHeaderL();
private:
	CPpmEncoder();
	};

#include "Ppm1Convert.inl"

#endif // __PPMCONVERT_H__

