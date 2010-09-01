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

/** 	@file
	@internalTechnology */
#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include <icl/imageplugin.h>
#include <icl/imagedisplay.hrh>
#include <icl/mngimagedisplay.hrh>
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#include <icl/imagecodecdef.h>
#endif
#include <101F7C51_extra.rsg>

#include "MngPanic.h"
#include "MngChunks.h"
#include "MngReadStream.h"


#include "MngIclDecoder.h"

// Mng decoder.
CMngIclDecoder* CMngIclDecoder::NewL()
	{
	return new(ELeave) CMngIclDecoder;
	}

CMngIclDecoder::CMngIclDecoder()
	{
	}

CMngIclDecoder::~CMngIclDecoder()
	{
	if (iMngIclReadStream)
		{
		iMngIclReadStream->Release();
		}
	iMngIclReadStream = NULL;

	}

void CMngIclDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	__ASSERT_ALWAYS((aFrameNumber >= 0) , MngPanic(EPanicInvalidFrameNumber));
	aImageType		= TUid::Uid(KMngMimeTypeUidValue);
	aImageSubType	= KNullUid;
	}

void CMngIclDecoder::ScanDataL()
	{
	ASSERT(ImageReadCodec() == NULL);
	//Subframes with No Loops
	if((DecoderOptions() & CImageDecoder::EOptionMngSubframesNoLoops) == CImageDecoder::EOptionMngSubframesNoLoops )
		{
		iMngSubframesNoLoops = ETrue;
		}
	else
		{
		iMngSubframesNoLoops = EFalse;
		}
	//
	ReadFormatL();
	iMngIclReadStream	= CMngIclReadStream::NewL(*this);
	CMngReadCodec* imageReadCodec = CMngReadCodec::NewL(*iMngIclReadStream, *this, iMngSubframesNoLoops);//Subframes with No Loops
	SetImageReadCodec(imageReadCodec);
	iMngIclReadStream->InvalidateBuffer();
	ReadFrameHeadersL();
	iState = EStateReadData;
	}

void CMngIclDecoder::ReadFormatL()
	{
	// MNG Player will do signature verification
	// Set start position.
	SetStartPosition(0);
	SetDataLength(KMaxTInt);
	}

CFrameInfoStrings* CMngIclDecoder::FrameInfoStringsL(RFs& aFs, TInt aFrameNumber)
	{
	const TUid KMngCodecDllUid = {KUidImageDisplayMngPluginDll};

	RResourceFile resourceFile;
	OpenExtraResourceFileLC(aFs, KMngCodecDllUid, resourceFile);

	HBufC8* resourceInfo = resourceFile.AllocReadLC(THEDECODERINFO);
	TResourceReader resourceReader;
	resourceReader.SetBuffer(resourceInfo);

	TBuf<KCodecResourceStringMax> info;
	TBuf<KCodecResourceStringMax> templte;

	const TFrameInfo& frameInfo = FrameInfo(aFrameNumber);
	CFrameInfoStrings* frameInfoStrings = CFrameInfoStrings::NewLC();

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetDecoderL(info);

	info = resourceReader.ReadTPtrC();
	frameInfoStrings->SetFormatL(info);

	TInt width	= frameInfo.iOverallSizeInPixels.iWidth;
	TInt height = frameInfo.iOverallSizeInPixels.iHeight;
	TInt depth	= frameInfo.iBitsPerPixel;

	templte = resourceReader.ReadTPtrC();
	info.Format(templte, width, height);
	frameInfoStrings->SetDimensionsL(info);

	CDesCArrayFlat* resourceArray = resourceReader.ReadDesCArrayL();
	CleanupStack::PushL(resourceArray);
	TUint formatIndex = (frameInfo.iFlags & TFrameInfo::EColor) ? 1 : 0;
	formatIndex += (frameInfo.iFlags & TFrameInfo::ETransparencyPossible) ? 2 : 0;
	templte = (*resourceArray)[formatIndex];
	CleanupStack::PopAndDestroy(resourceArray);
	info.Format(templte, depth);
	frameInfoStrings->SetDepthL(info);

// additional image details// 
	templte = resourceReader.ReadTPtrC();
	info.Format(templte, static_cast<CMngReadCodec*>(ImageReadCodec())->ImageFeatures());
	frameInfoStrings->SetDetailsL(info);

	CleanupStack::Pop(frameInfoStrings);
	CleanupStack::PopAndDestroy(2, &resourceFile); 
	return frameInfoStrings;
	}

void CMngIclDecoder::InitConvertL()
	{
	if (iLatestCodecError != KErrNone)
		{
		User::Leave(KErrNotReady);
		}
	if (iState==EStateImageCompleted)
		{
		User::Leave(KErrEof);
		}
	CImageDecoderPlugin::InitConvertL();
	}

void CMngIclDecoder::Cleanup()
	{
	CMngReadCodec* codec=static_cast<CMngReadCodec*>(ImageReadCodec());
	iState = EStateReadData;
	iCodecEventPending = EFalse;
	if (codec != NULL)
		{
		codec->StopDecode();
		}
	if (iMngIclReadStream)
		{
		iMngIclReadStream->Reset();
		}
	
	CImageDecoderPlugin::Cleanup();

	}

void CMngIclDecoder::DoConvert()
	{
	TBool issuedPending = EFalse;
	switch (iState)
		{
		case EStateReadData:
			{
			CMngReadCodec* readCodec = static_cast<CMngReadCodec*>(ImageReadCodec());
			TRAPD(error, 
					iMngIclReadStream->RestartL();
					readCodec->DecodeL();
				);
			if(error!=KErrNone)
				{
				if(error == KErrUnderflow)
					{
					error = KErrEof;
					}
				RequestComplete(error);
				}
			else
				{
				SetSelfPending();
				issuedPending = ETrue;
				}
			}
			break;

		case EStateDataParsed:
			ASSERT(iCodecEventPending);
			iCodecEventPending = EFalse;
			if (iLatestCodecError != KErrNone)
				{
				RequestComplete(iLatestCodecError);
				break;
				}
			else
				{
				SetSelfPending();
				issuedPending = ETrue;
				}
			iState = EStatePlayAnimation; 
			// and go further
		case EStatePlayAnimation:
			{
			CMngReadCodec* readCodec = static_cast<CMngReadCodec*>(ImageReadCodec());
			TRAPD(error, 
					readCodec->DecodeL();
				);
			if (error!=KErrNone)
				{
				RequestComplete(error);
				}
			else
				{
				if(!issuedPending && !iCodecEventPending)
					{
					SetSelfPending();
					}
				}
			}
			break;
	
		case EStateFrameReady:
		case EStateImageCompleted:
			ASSERT(iCodecEventPending);
			iCodecEventPending = EFalse;
			RequestComplete(iLatestCodecError);
			if (iState != EStateImageCompleted)
				{
				iState = EStatePlayAnimation;
				}
			break;

		default:
			ASSERT(EFalse); // we shan't be here
		}
	}
TInt CMngIclDecoder::LockBuffer(TInt aPosition, TPtrC8& aBuffer, TInt aBytesToLock)
	{
	aBuffer.Set(KNullDesC8);
	TRAPD(err, 
			ReadDataL(aPosition, aBuffer, aBytesToLock);
			);
	return err;
	}

void CMngIclDecoder::OnCodecEvent(TCodecEvent aEventCode, TInt aErrorCode)
	{
	ASSERT( iCodecEventPending == EFalse);
	iLatestCodecError	= aErrorCode;
	if (aErrorCode != KErrNone)
		{
		RequestComplete(aErrorCode);
		return;
		}

	switch (aEventCode)
		{
		case MMngCodecObserver::KDataParseCompleted:
			ASSERT(iState == EStateReadData);
			iState = EStateDataParsed;
			break;
		case MMngCodecObserver::KFrameReady:
			ASSERT(iState == EStatePlayAnimation || iState == EStateFrameReady);
			iState = EStateFrameReady;
			break;

		case MMngCodecObserver::KDecodeCompleted:
			ASSERT(iState == EStatePlayAnimation);
			iState = EStateImageCompleted;
			break;
		default:
			ASSERT(EFalse); // we shan't reach it
		}
	iCodecEventPending	= ETrue;
	SelfComplete(iLatestCodecError);
	}


