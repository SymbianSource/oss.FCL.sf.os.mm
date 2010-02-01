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

#ifndef __WMFCONVERT_H__
#define __WMFCONVERT_H__

#include "icl/ImagePlugin.h"
#include "WMFCodec.h"

#include <w32std.h>

// Decoder.
class CWmfReadCodec;
class CWmfDecoder : public CImageDecoderPlugin
	{
public:
	enum TWmfSubType
		{
		EWmfUnknownSubType, EWmfStdSubType, EWmfApmSubType, EWmfClpSubType
		};
	
public:
 	static CWmfDecoder* NewL();
 	static CWmfDecoder* NewStdL();
 	static CWmfDecoder* NewApmL();
 	static CWmfDecoder* NewClpL();
	virtual ~CWmfDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);
	void EnableMaskGeneration();

protected:
	void Cleanup();

private:
	CWmfDecoder(TWmfSubType aWmfSubType);
	void ReadFormatL();
	void ScanDataL();

	void ProcessWmfApmHeaderL(const TUint16* aData, TRect& aFrameCoords, TSize& aOverallSize, TSize& aFrameSize);
	void ProcessWmfClpHeaderL(const TUint16* aData, TRect& aFrameCoords, TSize& aOverallSize, TSize& aFrameSize);
	void CheckDeviceL();
	TSize FindSetWindowExtL();

private:
	CFbsScreenDevice* iDevice;
	TInt iWordsExpected;
	TBool iMaskGenerationEnabled;
	TWmfSubType iWmfSubType;
	TBool iRFbsSessionIsOurs;
	};

#endif // __WMFCONVERT_H__
