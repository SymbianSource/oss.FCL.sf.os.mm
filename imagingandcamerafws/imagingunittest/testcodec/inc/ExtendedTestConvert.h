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

#ifndef __EXTENDED_TEST_CONVERT_H__
#define __EXTENDED_TEST_CONVERT_H__

#include "ExtendedPlugin.h"

// Decoder.
class CExtendedTestDecoderPlugin : public CExtendedDecoderPlugin
	{
public:
	static CExtendedTestDecoderPlugin* NewL();
	virtual ~CExtendedTestDecoderPlugin();

protected:
	//From CImageDecoderPlugin
	virtual void ScanDataL();
	virtual CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber = 0);
	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;

private:
	CExtendedTestDecoderPlugin();
	};

// Encoder.
class CExtendedTestEncoderPlugin : public CExtendedEncoderPlugin
	{
public:
	static CExtendedTestEncoderPlugin* NewL();
	virtual ~CExtendedTestEncoderPlugin();

protected:
	//From CImageEncoderPlugin
	virtual void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	virtual void UpdateHeaderL();

private:
	CExtendedTestEncoderPlugin();
	};


#endif // __EXTENDED_TEST_CONVERT_H__

