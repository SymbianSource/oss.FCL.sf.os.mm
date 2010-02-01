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

#ifndef __PNGCONVERT_H__
#define __PNGCONVERT_H__

#include <icl/imageplugin.h>
#include "PNGCodec.h"

/** @file
	@internalComponent */

class MPngDecoder 
	{
public:
	virtual void GoToProcessDataState()=0;
	};

// Decoder.
class CPngReadCodec;
class CPngDecoder : public CImageDecoderPlugin, public MPngDecoder
	{
public:
	enum TDecodeState
		{
		EStateStart,
		EStateNormalProcess,
		EStateDataProcess
		};
public:
	CPngDecoder();

 	static CPngDecoder* NewL();
	virtual ~CPngDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

	void GoToProcessDataState();

private:
	// From CImageDecoderPlugin
	void ReadFormatL();
	void ScanDataL();
	void DoConvert();
	void InitConvertL();
	void NotifyComplete();

	TDecodeState iState;
	};

// CPngEncoder
class CPngEncoder : public CImageEncoderPlugin
	{
private:
	enum TEncodeState
		{
		EStateNormalProcess,
		EStateDataProcess
		};
public:
	static CPngEncoder* NewL();
	virtual ~CPngEncoder();
	void GoToProcessDataState();
protected:
	// from CImageEncoderPlugin
	void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	void DoConvert();
	void UpdateHeaderL();
	void InitConvertL();
	void NotifyComplete();
private:
	CPngEncoder();
private:
	TEncodeState iState;
	};
#endif // __PNGCONVERT_H__
