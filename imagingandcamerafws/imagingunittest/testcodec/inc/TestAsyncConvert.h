// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TESTASYNCCONVERT_H__
#define __TESTASYNCCONVERT_H__

#include "imageconversion.h"
#include "TestAsyncCodec.h"
#include <icl/imageplugin.h>

// forward refs
class CTestAsyncReadCodec;
class CTestAsyncWriteCodec;

// MAsyncSchedulerObserver
// mixin class to handle callbacks from an CAsyncScheduler object
class MAsyncSchedulerObserver
	{
public:
	virtual void MasoProcessL(TInt error) = 0;
	};

class CAsyncScheduler : public CActive
	{
public:
	CAsyncScheduler(MAsyncSchedulerObserver& aSchedulerObserver);
	void Start();

	// From CActive
	virtual void RunL();
	virtual void DoCancel() {};

private:
	MAsyncSchedulerObserver& iSchedulerObserver;
	};


class CTestAsyncDecoder : public CImageDecoderPlugin, 
	public MAsyncSchedulerObserver
	{
public:
	static CTestAsyncDecoder* NewL();
	virtual ~CTestAsyncDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

	// From CImageDecoderPlugin
	virtual void DoConvert();
	virtual TInt NumberOfImageComments() const;
	virtual HBufC* ImageCommentL(TInt aCommentNumber) const;
	virtual TInt NumberOfFrameComments(TInt aFrameNumber) const;
	virtual HBufC* FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const;
	void ScanDataL();
	virtual void Cleanup();

	// from MAsyncSchedulerObserver
	virtual void MasoProcessL(TInt aError);

	// callback from CTestAsyncReadCodec to indicate completion
	// of CTestAsyncReadCodec::ProcessFrameL()
	void ProcessFrameComplete(TInt aError);

private:
	CTestAsyncDecoder();
	void ConstructL();


private:
	enum TDecodeState
		{
		EDecodeInit,
		EDecodeInProgress,
		EDecodeFinished
		};
	TDecodeState iDecodeState;
	
	CTestAsyncReadCodec* iImageReadCodec;	// not owned
	TInt iDecodeIterations;

	TInt iMaxValue;
	TBool iReadHeaders;

	CAsyncScheduler* iScheduler;

	TInt iError;
	};


class CTestAsyncEncoder : public CImageEncoderPlugin, public MAsyncSchedulerObserver
	{
public:
	static CTestAsyncEncoder* NewL();
	virtual ~CTestAsyncEncoder();

	// From CImageEncoderPlugin
	virtual void DoConvert();
	void PrepareEncoderL(const CFrameImageData* aFrameImageData);
	virtual void Cleanup();
	void UpdateHeaderL();

	// from MAsyncSchedulerObserver
	virtual void MasoProcessL(TInt aError);

	// callback from CTestAsyncWriteCodec to indicate completion
	// of CTestAsyncWriteCodec::ProcessFrameL()
	void ProcessFrameComplete(TInt aError);

private:
	CTestAsyncEncoder();
	void ConstructL();


private:
	enum TEncodeState
		{
		EEncodeInit,
		EEncodeInProgress,
		EEncodeFinished
		};
	TEncodeState iEncodeState;
	
	CTestAsyncWriteCodec* iImageWriteCodec;	// not owned
	TInt iEncodeIterations;

	TInt iMaxValue;
	TBool iReadHeaders;

	CAsyncScheduler* iScheduler;

	TInt iError;
	};


#endif

