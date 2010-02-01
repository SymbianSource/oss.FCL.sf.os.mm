// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent 
*/

#ifndef MNGICLDECODER_H

#define MNGICLDECODER_H

#include "InputStream.h"
#include "MngReadCodec.h"

class CMngIclReadStream;

/**
	Restricted implementation of the ICL decoder for MGN-LC format, 
	doesn't support FrameCount() and decoding from a frame other than
	the 1st one.
*/
class CMngIclDecoder :	public CImageDecoderPlugin, 
						protected MDataFeed,
						protected MMngCodecObserver
	{
public:
 	static CMngIclDecoder* NewL();
	virtual ~CMngIclDecoder();

	virtual void ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const;
	CFrameInfoStrings* FrameInfoStringsL(RFs& aFs, TInt aFrameNumber);

protected:
// from the CImageDecoderPlugin //
	virtual void InitConvertL();
	virtual void DoConvert();
	virtual void ScanDataL();
	virtual void Cleanup();

// from the MDataFeed // 
	virtual TInt LockBuffer(TInt aPosition, TPtrC8& aBuffer, TInt aBytesToLock);
// from the MMngCodecObserver  // 
	virtual void OnCodecEvent(TCodecEvent aEventCode, TInt aErrorCode);
private:
	CMngIclDecoder();

	// From CImageDecoderPlugin
	// Called by ScanDataL.
	void ReadFormatL();
private:
	TBool iMngSubframesNoLoops;   //Subframes with No Loops
	CMngIclReadStream*	iMngIclReadStream;

	enum 
		{
		EStateReadData		=0,	//0
		EStateDataParsed,		//1
		EStatePlayAnimation,	//2
		EStateFrameReady,		//3
		EStateImageCompleted	//4
		}				iState;
	TBool				iCodecEventPending;
	TInt				iLatestCodecError;
	};


#endif // ndef MNGICLDECODER_H

