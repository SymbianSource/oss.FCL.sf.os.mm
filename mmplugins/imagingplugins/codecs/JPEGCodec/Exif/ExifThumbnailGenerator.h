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

#ifndef EXIFTHUMBNAILGENERATOR_H
#define EXIFTHUMBNAILGENERATOR_H


#include <e32base.h>
#include <f32file.h>
#include <icl/imagecodec.h>
#include <imageconversion.h>
#include "JpegTypes.h"

class CFbsBitmap;
class CBitmapScaler;
class CImageWriteCodec;
class MExifSource;

//class CFrameImageData;
class CScaledJpegGenerator : public CActive
	{
public:
	enum TImageToGenerate
		{
		EMainImage,
		EThumbnail
		};


public:
	static CScaledJpegGenerator* NewL(TRequestStatus* aNotifier, const MExifSource* aSource, 
		TBool aMaintainAspectRatio = ETrue, CImageDecoder::TOptions aDecoderOptions = CImageDecoder::EOptionNone);
	~CScaledJpegGenerator();
	
	void StartL(TSize aSize, TImageToGenerate aImageType);
	HBufC8* GetJpegDataL();
	HBufC8* JpegDataBufferL(const TUint aIndex);
	void GetScaledImageSize(TSize &aSize);
private:
	CScaledJpegGenerator(TRequestStatus* aNotifier, TBool aMaintainAspectRatio, 
		CImageDecoder::TOptions aDecoderOptions);
	void ConstructL(const MExifSource* aSource);
	
	void RunL();
	void DoCancel();

	void GenerationCompleteL();	
	TInt RunError(TInt aError);

	void DoScaleBitmapL();
	void DoConvertL();

	void CreateJpegCodecL();
	void CleanTempData();
	void CleanResultData();
	void CleanData();
	
private:
	const MExifSource* iSource;
	TRequestStatus* iNotifier;
	TSize iSize;
	TImageToGenerate iImageType;
	
	CFbsBitmap* iBitmap;
	CImageDecoder* iImageDecoder;
	CBitmapScaler* iBitmapScaler;
	CImageWriteCodec* iJpegCodec;
	TFrameState iFrameState;

	RPointerArray<HBufC8> iJPegImage;
	HBufC8* iJpegChunck;
	TBufPtr8 iJpegChunckPtr;
	CImageDecoder::TOptions iDecoderOptions;
	enum TThumbnailGeneratorState
		{
		EEmpty,
		EDecode,
		EScale,
		EConvert,
		EFinished
		};
	TThumbnailGeneratorState iState;

	RPointerArray<HBufC8> iComment;
	TQTable* iLumaTable;
	TQTable* iChromaTable;

	TBool iMaintainAspectRatio;
	};



#endif 	// EXIFTHUMBNAILGENERATOR_H
