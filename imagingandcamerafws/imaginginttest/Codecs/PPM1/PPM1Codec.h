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

#ifndef __PPMCODEC_H__
#define __PPMCODEC_H__

#include <bitdev.h>
#include <icl/imagecodec.h>
#include <icl/imageprocessor.h>

#include "PPM1Convert.h"

class CPpmDecoder;
class CPluginExtensionManager;

// PPM Read codec.
class CPpmReadCodec : public CImageProcessorReadCodecExtension
	{
public:
	static CPpmReadCodec* NewL(CPpmDecoder& aDecoder);
	// From CImageReadCodec
	virtual void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	virtual void InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& aFrameData);
	virtual TFrameState ProcessFrameHeaderL(TBufPtr8& aData);
	virtual ~CPpmReadCodec();
	virtual void GetNewDataPosition(TInt &aPosition, TInt &aLength);
	
	void SetExtensionManager(CPluginExtensionManager* aExtensionManager);

protected:
	CPpmReadCodec(CPpmDecoder& aDecoder);

	void DoProcessCompressed();
	void DoProcessUncompressedL(TLex8& aLex);
	// from CImageReadCodec
	virtual TFrameState ProcessFrameL(TBufPtr8& aSrc);
	virtual void Complete();
	
	TInt ValidateDestinationSize(const TSize& aDestSize) const;
	TInt ScalingCoefficientL(const TSize& aDestinationSize, const TSize& aFrameSize) const;
	void SetOperationData(const TRect& aDestinationBitmap);
	
protected:
	CPpmDecoder *const iDecoder;
	TSize iFrameSize;
	const TUint8* iDataPtr; // Not owned
	const TUint8* iDataPtrLimit; // Not owned
	TInt iPixelsExpected;
	TInt iPixelsProcessed;
	TFrameInfo* iFrameInfo; // Not owned
	CPluginExtensionManager* iExtensionManager; // Not owned
	
	TInt iNewBytePosition;
	TBool iSeekRequested;
	TPoint iCurrentPosition;
	TPoint iStartPosition;
	TPoint iEndPosition;
	TInt iXAxisIncrement;
	TInt iYAxisIncrement;
	TBool iXAxisFirst;
	};

// Write codec
class CPpm1WriteCodec : public CImageWriteCodec
	{
public:
	static CPpm1WriteCodec* NewL();
	virtual ~CPpm1WriteCodec();
	// from CImageWriteCodec
	virtual void InitFrameL(TBufPtr8& aDst, const CFbsBitmap& aSource);
	virtual TFrameState ProcessFrameL(TBufPtr8& aDst);
private:
	CPpm1WriteCodec();
	void ConstructL ();

	void DoProcessL(const CFbsBitmap& aSource);
	void SwitchRGB(TDes8 &aBuf);
private:
	TUint8* iDestStartPtr;
	TUint8* iDestPtr;
	TUint8* iDestPtrLimit;
	TPoint iPos;
	TRect iSourceRect;
	};


#endif // __PPMCODEC_H__

