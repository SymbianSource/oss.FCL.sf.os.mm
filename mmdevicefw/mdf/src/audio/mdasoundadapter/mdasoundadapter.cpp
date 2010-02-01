// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mdasoundadapter.h"
#include "mdasoundadapterbody.h"
#include <e32debug.h>

EXPORT_C RMdaDevSound::RMdaDevSound()
	:iBody(NULL)
	{
	}
		
EXPORT_C TInt RMdaDevSound::Open(TInt aUnit)
	{
	TInt err = KErrNone;
	if(iBody == NULL)
		{
		TRAP(err, iBody = RMdaDevSound::CBody::NewL());
		}
	if(err == KErrNone)
		{
		err = iBody->Open(aUnit);
		}
	return err;
	}
		
EXPORT_C TVersion RMdaDevSound::VersionRequired() const
	{
	if(iBody)
		{
		return iBody->VersionRequired();
		}
	return TVersion();
	}

EXPORT_C TInt RMdaDevSound::IsMdaSound()
	{
	return iBody->IsMdaSound();
	}

EXPORT_C TInt RMdaDevSound::PlayVolume()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->PlayVolume();
	}
	
EXPORT_C void RMdaDevSound::SetPlayVolume(TInt aVolume)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->SetPlayVolume(aVolume);
	}
	
EXPORT_C void RMdaDevSound::SetVolume(TInt aLogarithmicVolume)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->SetVolume(aLogarithmicVolume);	
	}
	
EXPORT_C void RMdaDevSound::CancelPlayData()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->CancelPlayData();
	}
	
EXPORT_C TInt RMdaDevSound::RecordLevel()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->RecordLevel();
	}
	
EXPORT_C void RMdaDevSound::SetRecordLevel(TInt aLevel)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->SetRecordLevel(aLevel);
	}
	
EXPORT_C void RMdaDevSound::CancelRecordData()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->CancelRecordData();
	}
	
EXPORT_C void RMdaDevSound::FlushRecordBuffer()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->FlushRecordBuffer();
	}
	
EXPORT_C TInt RMdaDevSound::BytesPlayed()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->BytesPlayed();
	}

EXPORT_C void RMdaDevSound::ResetBytesPlayed()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->ResetBytesPlayed();
	}
	
EXPORT_C void RMdaDevSound::PausePlayBuffer()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->PausePlayBuffer();
	}
	
EXPORT_C void RMdaDevSound::ResumePlaying()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->ResumePlaying();
	}

EXPORT_C void RMdaDevSound::ResumePlaying(TRequestStatus&)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->ResumePlaying();
	}

EXPORT_C void RMdaDevSound::PauseRecordBuffer()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->PauseRecordBuffer();
	}

EXPORT_C void RMdaDevSound::ResumeRecording()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->ResumeRecording();
	}

EXPORT_C TInt RMdaDevSound::GetTimePlayed(TTimeIntervalMicroSeconds& aTimePlayed)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->GetTimePlayed(aTimePlayed);
	}


	
EXPORT_C void RMdaDevSound::PlayFormatsSupported(TSoundFormatsSupportedBuf& aFormatsSupported)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->PlayFormatsSupported(aFormatsSupported);
	}
	
EXPORT_C void RMdaDevSound::GetPlayFormat(TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->GetPlayFormat(aFormat);
	}
	
EXPORT_C TInt RMdaDevSound::SetPlayFormat(const TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->SetPlayFormat(aFormat);
	}

EXPORT_C void RMdaDevSound::RecordFormatsSupported(TSoundFormatsSupportedBuf& aFormatsSupported)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->RecordFormatsSupported(aFormatsSupported);
	}

EXPORT_C void RMdaDevSound::GetRecordFormat(TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->GetRecordFormat(aFormat);
	}

EXPORT_C TInt RMdaDevSound::SetRecordFormat(const TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	return iBody->SetRecordFormat(aFormat);
	}
	
EXPORT_C void RMdaDevSound::Close()
	{
	if(iBody)
		{
		iBody->Close();
		delete iBody;
		iBody = NULL;
		}
	}
		
EXPORT_C TInt RMdaDevSound::Handle()
	{
	if(iBody)
		{
		return iBody->Handle();
		}
	return 0;
	}

EXPORT_C void RMdaDevSound::PlayData(TRequestStatus& aStatus, const TDesC8& aData)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->PlayData(aStatus, aData);
	}

EXPORT_C void RMdaDevSound::RecordData(TRequestStatus& aStatus, TDes8& aData)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->RecordData(aStatus, aData);
	}

EXPORT_C void RMdaDevSound::NotifyRecordError(TRequestStatus& aStatus)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->NotifyRecordError(aStatus);
	}

EXPORT_C void RMdaDevSound::NotifyPlayError(TRequestStatus& aStatus)
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->NotifyPlayError(aStatus);
	}

EXPORT_C void RMdaDevSound::CancelNotifyPlayError()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->CancelNotifyPlayError();
	}

EXPORT_C void RMdaDevSound::CancelNotifyRecordError()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->CancelNotifyRecordError();
	}

EXPORT_C void RMdaDevSound::FlushPlayBuffer()
	{
	__ASSERT_DEBUG(iBody != NULL, Panic(EDeviceNotOpened));
	iBody->FlushPlayBuffer();
	}
