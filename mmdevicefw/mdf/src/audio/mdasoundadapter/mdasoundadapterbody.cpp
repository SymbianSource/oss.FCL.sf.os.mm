// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mdasoundadapterconsts.h"
#include "mdasoundadapterbody.h"
#include <e32debug.h>

#include "mmf/utils/rateconvert.h" // if we need to resample

#ifdef SYMBIAN_SOUNDADAPTER_BYTESPLAYED
	#include <hal.h>
#endif

_LIT(KPddFileName,"SOUNDSC.PDD");
_LIT(KLddFileName,"ESOUNDSC.LDD");


const TInt KBytesPerSample = 2;
const TInt KMinBufferSize = 2;

/**
This function raises a panic
EDeviceNotOpened is raised when any of the RMdaDevSound APIs are called before opening the device. 
*/
GLDEF_C void Panic(TSoundAdapterPanicCodes aPanicCode)
	{
	User::Panic(KSoundAdapterPanicCategory, aPanicCode);
	}
	
RMdaDevSound::CBody* RMdaDevSound::CBody::NewL()
	{
	CBody* self = new(ELeave) CBody();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

RMdaDevSound::CBody::~CBody()
	{
	for(TInt i = 0; i < KNumPlayers; i++)
		{
		delete iPlayers[i];
		iPlayers[i] = NULL;
		}
	iBufferRemaining.Close();
	delete iPlayData.iConverter;
	delete iRecordData.iConverter;
	iChunk.Close();
	iPlaySoundDevice.Close();
	iRecordSoundDevice.Close();
	}
	
RMdaDevSound::CBody::CBody()
	:iState(ENotReady), iBufferIndex(-1), iBufferOffset(-1), iSecondPhaseData(0,0)
	{
	
	}

TVersion RMdaDevSound::CBody::VersionRequired() const
	{
	if(iPlaySoundDevice.Handle())
		{
		return iPlaySoundDevice.VersionRequired();
		}
	else
		{
		return TVersion();
		}
	}

TInt RMdaDevSound::CBody::IsMdaSound()
	{
	return ETrue;
	}
	
void RMdaDevSound::CBody::ConstructL()
	{
	// Try to load the audio physical driver
    TInt err = User::LoadPhysicalDevice(KPddFileName);
	if ((err!=KErrNone) && (err!=KErrAlreadyExists))
		{
		User::Leave(err);
		}
    // Try to load the audio logical driver
	err = User::LoadLogicalDevice(KLddFileName);
    if ((err!=KErrNone) && (err!=KErrAlreadyExists))
    	{
    	User::Leave(err);
    	}
	for(TInt i=0; i<KNumPlayers; i++)
		{
		iPlayers[i] = new(ELeave) CPlayer(CActive::EPriorityUserInput, *this, i);
		}
#ifdef SYMBIAN_SOUNDADAPTER_BYTESPLAYED
	User::LeaveIfError(HAL::Get(HALData::EFastCounterFrequency,iFCFrequency));
#endif
	}

TInt RMdaDevSound::CBody::Open(TInt /*aUnit*/)
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("RMdaDevSound::CBody::Open "));
    #endif	
	TInt err = KErrNone;
	//Default behavior of this method is to open both the play and record audio devices.
	if(!iPlaySoundDevice.Handle() && !iRecordSoundDevice.Handle())
        {
		err = iPlaySoundDevice.Open(KSoundScTxUnit0);
    	if(err == KErrNone)
    		{
    		err = iRecordSoundDevice.Open(KSoundScRxUnit0);
    		}
		}
	if(err != KErrNone)
		{
		Close();
		}
	else
	    {
	    iState = EOpened;
	    }
	return err;
	}
		
TInt RMdaDevSound::CBody::PlayVolume()
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	return iPlaySoundDevice.Volume();	
	}
	
void RMdaDevSound::CBody::SetPlayVolume(TInt aVolume)
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	if(aVolume >=0 && aVolume<=KSoundMaxVolume)
		{
		iPlaySoundDevice.SetVolume(KLinerToDbConstantLookup[aVolume].iDBValue);
		}
	}
void RMdaDevSound::CBody::SetVolume(TInt aLogarithmicVolume)
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	if(aLogarithmicVolume >= 0 && aLogarithmicVolume <= KSoundMaxVolume)
		{
		iPlaySoundDevice.SetVolume(aLogarithmicVolume);
		}
	}
	
void RMdaDevSound::CBody::CancelPlayData()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("RMdaDevSound::CBody::CancelPlayData:"));
    #endif	
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	iPlaySoundDevice.CancelPlayData();
	iPauseDeviceDriverOnNewData = EFalse;
	SoundDeviceError(KErrNone);//cancel the players
	}
	
TInt RMdaDevSound::CBody::RecordLevel()
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
	return iRecordSoundDevice.Volume();
	}
	
void RMdaDevSound::CBody::SetRecordLevel(TInt aLevel)
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
	iRecordSoundDevice.SetVolume(aLevel);	
	}
	
void RMdaDevSound::CBody::CancelRecordData()
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("RMdaDevSound::CBody::CancelRecordData:"));
    #endif
	iRecordSoundDevice.CancelRecordData();
	SoundDeviceError(KErrNone);
	}
	
void RMdaDevSound::CBody::FlushRecordBuffer()
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("RMdaDevSound::CBody::FlushRecordBuffer"));
    #endif
	
	if(iState == ERecording)
	    {
		iRecordSoundDevice.Pause();//this is equivalent call in the new sound driver
    	}
	}
	
TInt RMdaDevSound::CBody::BytesPlayed()
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	TInt currentBytesPlayed = 0;
#ifdef SYMBIAN_SOUNDADAPTER_BYTESPLAYED
	if(iTimerActive)
		{
		TUint32 endTime = User::FastCounter();
		TUint timePlayed = 0;
		if(endTime<iStartTime)
			{
			timePlayed = (KMaxTUint32-iStartTime)+endTime;
			}
		else
			{
			timePlayed = endTime-iStartTime;
			}	
        TUint64 bytesPlayed = iPlayData.iSampleRate*KBytesPerSample;    //A TUint64 is used because during the multiplying segment of the calculation we regularly overflow what a TUint32 can handle
        bytesPlayed = (bytesPlayed * timePlayed)/iFCFrequency;  //The division brings it back into TUint32 territory, however.  We cannot do this before the multiplication without risking significant loss of accuracy

		currentBytesPlayed = iBytesPlayed+I64LOW(bytesPlayed);
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("EstimatedBytesPlayed[%d]  Driver's bytesPlayed[%d]", currentBytesPlayed, iBytesPlayed);
        #endif
		}
	else
		{
		currentBytesPlayed = iPlaySoundDevice.BytesTransferred();
		}		
	
#else
	currentBytesPlayed = iPlaySoundDevice.BytesTransferred();
#endif
	if (iPlayData.iConverter)
		{
		// need to scale bytes played to fit with requested rate and channels, not actual
		if (iPlayData.iActualChannels != iPlayData.iRequestedChannels)
			{
			if (iPlayData.iActualChannels == 2)
				{
				// requested was mono, we have stereo
				currentBytesPlayed /= 2;
				}
			else 
				{
				// requested was stereo, we have mono
				currentBytesPlayed *= 2;
				}
			}
		if (iPlayData.iSampleRate != iPlayData.iActualRate)
			{
			currentBytesPlayed = TInt(currentBytesPlayed*
					TReal(iPlayData.iSampleRate)/TReal(iPlayData.iActualRate)); // don't round
			}
		}
	return currentBytesPlayed;
	}

void RMdaDevSound::CBody::ResetBytesPlayed()
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	return iPlaySoundDevice.ResetBytesTransferred();
	}
	
void RMdaDevSound::CBody::PausePlayBuffer()
	{
	if (iState == EPlaying)
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG   
            RDebug::Print(_L("RMdaDevSound::CBody::PausePlayBuffer() offset = %d length = %d"), iBufferOffset, iBufferLength);
        #endif
		__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
		// If iPlayerStatus is NULL, we're not playing currently any data, and the device driver won't pause properly. In this case,
		// we set a reminder to ourselves to pause the driver once we have data later
		if (iPlayerStatus == NULL)
			{
			iPauseDeviceDriverOnNewData = ETrue;
			}
		else
			{
			TInt res = iPlaySoundDevice.Pause();
			#ifdef SYMBIAN_SOUNDADAPTER_DEBUG   
				RDebug::Printf("iPlaySoundDevice.Pause res = %d", res);
			#endif
			(void)res;
			}
		iState = EPaused;
		iTimerActive = EFalse;
		}		
	}
	
void RMdaDevSound::CBody::ResumePlaying()
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	iPauseDeviceDriverOnNewData = EFalse;
	#ifdef SYMBIAN_SOUNDADAPTER_DEBUG   
		RDebug::Printf("RMdaDevSound::CBody::ResumePlaying");
	#endif	
	if (iFlushCalledDuringPause)
		{
		// if we resume having called flush, we can't just keep going as the driver does not work
		// that way. Instead we cancel so that buffer play completes and a new buffer will be passed
		iFlushCalledDuringPause = EFalse;
		PlayCancelled();
		}
	else
		{
		iState = EPlaying;
		iPlaySoundDevice.Resume();
		}
	}

void RMdaDevSound::CBody::PauseRecordBuffer()
	{
	if(iState == ERecording)
	    {	
		__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
            RDebug::Printf("RMdaDevSound::CBody::PauseRecordBuffer");
        #endif
		iRecordSoundDevice.Pause();
	    }
	}

void RMdaDevSound::CBody::ResumeRecording()
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	iRecordSoundDevice.Resume();
	}

TInt RMdaDevSound::CBody::GetTimePlayed(TTimeIntervalMicroSeconds& aTimePlayed)
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	TTimeIntervalMicroSecondsBuf aTimePlayedBuf;
	TInt err;
	err = iPlaySoundDevice.TimePlayed(aTimePlayedBuf);
	if (err == KErrNone)
	  {
	    aTimePlayed = aTimePlayedBuf();
	  }

	return err;
	}

	
void RMdaDevSound::CBody::FormatsSupported(TSoundFormatsSupportedBuf& aFormatsSupported, RSoundSc& aSoundDevice)
	{
	TSoundFormatsSupportedV02Buf supportedFormat;
	aSoundDevice.Caps(supportedFormat);
	TUint32 rates = supportedFormat().iRates;
	
	for(TInt i = KNumSampleRates-1; i > 0 ;i--)//min to max
		{
		if(rates & KRateEnumLookup[i].iRateConstant)
			{
			aFormatsSupported().iMinRate = KRateEnumLookup[i].iRate;
			break;
			}
		}
	for(TInt i = 0; i < KNumSampleRates; i++)//max to min
		{
		if(rates & KRateEnumLookup[i].iRateConstant)
			{
			aFormatsSupported().iMaxRate = KRateEnumLookup[i].iRate;
			break;
			}
		}
	TUint32 enc = supportedFormat().iEncodings;
	
	if (enc & KSoundEncoding16BitPCM)
		{
		aFormatsSupported().iEncodings = EMdaSoundEncoding16BitPCM;// Always defaults to this
		}
	if (enc & KSoundEncoding8BitPCM)
		{
		aFormatsSupported().iEncodings |= EMdaSoundEncoding8BitPCM;
		}
	TUint32 channels = supportedFormat().iChannels;
	
	if (channels & KSoundStereoChannel)
		{
		aFormatsSupported().iChannels = 2;
		}
	else
		{
		aFormatsSupported().iChannels = 1;
		}
	aFormatsSupported().iMinBufferSize = supportedFormat().iRequestMinSize;
	aFormatsSupported().iMaxBufferSize = KMaxBufferSize;
	aFormatsSupported().iMinVolume = 0;
	aFormatsSupported().iMaxVolume = KSoundMaxVolume;
	}
	
void RMdaDevSound::CBody::GetFormat(TCurrentSoundFormatBuf& aFormat, 
									RSoundSc& /*aSoundDevice*/,
									const TFormatData &aFormatData)
	{
	// always return the requested, or the initial, not current device setting
	aFormat().iChannels = aFormatData.iRequestedChannels; // never clear if this is bitmap or value, but effectively the same
	aFormat().iRate = aFormatData.iSampleRate;
	}
	
TInt RMdaDevSound::CBody::SetFormat(const TCurrentSoundFormatBuf& aFormat, 
									RSoundSc& aSoundDevice,
									TFormatData &aFormatData)
	{
	TInt err = KErrNotFound;
	TCurrentSoundFormatV02Buf formatBuf;
	
	delete aFormatData.iConverter; 
	aFormatData.iConverter = NULL; // setting this to NULL indicates we are not using converter. No other flag
	
	TInt wantedRate = aFormat().iRate;
	for(TInt index = 0; index < KNumSampleRates; index++ )
		{
		if(wantedRate == KRateEnumLookup[index].iRate)
			{
			formatBuf().iRate = KRateEnumLookup[index].iRateEnum;
			aFormatData.iSampleRate = wantedRate;
			err = KErrNone;
			break;
			}
		}
	
	if(err == KErrNone)
		{
		formatBuf().iChannels = aFormat().iChannels;
		formatBuf().iEncoding = ESoundEncoding16BitPCM;
		formatBuf().iDataFormat = ESoundDataFormatInterleaved;
		err = aSoundDevice.SetAudioFormat(formatBuf);
        #if defined(SYMBIAN_SOUNDADAPTER_FORCECDRATES) || defined (SYMBIAN_SOUNDADAPTER_FORCESTEREO)
            err = KErrNotSupported; // force Negotiate - for debugging
        #endif
		if (err==KErrNotSupported)
			{
			// don't support directly. Perhaps can rate convert?
			err = NegotiateFormat(aFormat, aSoundDevice, aFormatData);
			}
		}
	return err;	
	}
	
TInt RMdaDevSound::CBody::NegotiateFormat(const TCurrentSoundFormatBuf& aFormat, 
										  RSoundSc& aSoundDevice, 
										  TFormatData &aFormatData)
	{
	ASSERT(!aFormatData.iConverter); // we don't clear on fail - so assuming NULL to start with
	
	TInt err = KErrNotFound;
	TCurrentSoundFormatV02Buf formatBuf;

	// find out first what the driver supports
	TSoundFormatsSupportedV02Buf supportedFormat;
	aSoundDevice.Caps(supportedFormat);
	TUint32 supportedRates = supportedFormat().iRates;
    #ifdef SYMBIAN_SOUNDADAPTER_FORCECDRATES
        supportedRates &= KSoundRate11025Hz| KSoundRate22050Hz | KSoundRate44100Hz; // only use CD rates - for debugging
    #endif
	
	// For PlayCase:
	// 		first try to find the first rate below or equal to the requested that is supported
	// 		initially go down and be fussy, but if we pass the requested rate find the first that
	// 		is supported
	// For RecordCase:
	//		We want the next rate above consistently - we go down from this to the requested rate.
	//		If there is one, we don't support - we _never_ upsample.
	// note that the table is given in descending order, so we start with the highest
	TInt wantedRate = aFormat().iRate;
	TInt takeTheFirst = EFalse;
	TInt nextUpValidIndex = -1;
	for(TInt index = 0; index < KNumSampleRates; index++ )
		{
		TBool lookingAtRequestedRate = wantedRate == KRateEnumLookup[index].iRate;
		TSoundRate wantedEnum = KRateEnumLookup[index].iRateEnum;
		TUint32 equivBitmap = KRateEnumLookup[index].iRateConstant;
		TBool isSupported = (equivBitmap & supportedRates) != EFalse;
		if (lookingAtRequestedRate || takeTheFirst)
			{
			if (isSupported)
				{
				// this rate is supported
				formatBuf().iRate = wantedEnum;
				aFormatData.iActualRate = KRateEnumLookup[index].iRate;
				err = KErrNone;
				break;				
				}
			}
		else if (!takeTheFirst)
			{
			// while we are still looking for the rate, want to cache any supported index
			// at end of loop, this will be the first rate above ours that is supported
			// use for fallback if required
			if (isSupported)
				{
				nextUpValidIndex = index;
				}
			}
		if (lookingAtRequestedRate)
			{
			// if we get this far we've gone passed the wanted rate. For play we enable
			// "takeTheFirst". For record we just abort.
			if (&aSoundDevice==&iPlaySoundDevice)
				{
				takeTheFirst = ETrue;
				}
			else
				{
				break;
				}
			}
		}
		
	if (err)
		{
		// if there is one above the requested rate, use that
		if (nextUpValidIndex>=0)
			{
			TSoundRate wantedEnum = KRateEnumLookup[nextUpValidIndex].iRateEnum;
			formatBuf().iRate = wantedEnum;
			aFormatData.iActualRate = KRateEnumLookup[nextUpValidIndex].iRate;
			err = KErrNone;		
			}
		}
		
	if (err)
		{
		// should have something!
		return err;
		}
		
	aFormatData.iSampleRate = wantedRate; // iSampleRate is our requested/apparent rate, not the device rate.
	
	TUint32 channelsSupported = supportedFormat().iChannels;
    #ifdef SYMBIAN_SOUNDADAPTER_FORCESTEREO
        channelsSupported &= KSoundStereoChannel; // don't use mono - for debugging
    #endif
	if(KSoundAdapterForceStereo==1)
	    {
	    channelsSupported &= KSoundStereoChannel;
#ifdef SYMBIAN_SOUNDADAPTER_DEBUG
	    RDebug::Print(_L("Added stereo support."));
#endif
	    }
	if (aFormat().iChannels == 1)
		{
		aFormatData.iRequestedChannels = 1;
		// want mono
		if (channelsSupported & KSoundMonoChannel)
			{
			// mono is supported, as usual
			aFormatData.iActualChannels = 1;
			}
		else if (channelsSupported & KSoundStereoChannel)
			{
			aFormatData.iActualChannels = 2;
			}
		else
			{
			return KErrNotSupported; // should not get this far for real
			}
		}
	else if (aFormat().iChannels == 2)
		{
		aFormatData.iRequestedChannels = 2;
		// want stereo
		if (channelsSupported & KSoundStereoChannel)
			{
			// stereo is supported, as usual
			aFormatData.iActualChannels = 2;
			}
		else if (channelsSupported & KSoundMonoChannel)
			{
			aFormatData.iActualChannels = 1;
			}
		else
			{
			return KErrNotSupported; // should not get this far for real
			}
		}
	else
		{
		return KErrNotSupported; // unknown number of channels requested!
		}
	
	formatBuf().iChannels = aFormatData.iActualChannels;
	
	formatBuf().iEncoding = ESoundEncoding16BitPCM;
	formatBuf().iDataFormat = ESoundDataFormatInterleaved;
	err = aSoundDevice.SetAudioFormat(formatBuf);
	
	if (!err)
		{
		ASSERT(!aFormatData.iConverter); // pre-condition at top of function anyway
		if (&aSoundDevice==&iPlaySoundDevice)
			{
            #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
                RDebug::Print(_L("RMdaDevSound::CBody::NegotiateFormat: Convert:CreateL from %d/%d to %d/%d"), 
                            aFormatData.iSampleRate, aFormatData.iRequestedChannels, 
                            aFormatData.iActualRate, aFormatData.iActualChannels);
            #endif																	       
			// when playing we convert from requested to actual
			TRAP(err, aFormatData.iConverter = CChannelAndSampleRateConverter::CreateL(aFormatData.iSampleRate, 
																		   aFormatData.iRequestedChannels, 
																	       aFormatData.iActualRate, 
																	       aFormatData.iActualChannels));
			}
		else
			{
			// when recording we convert from actual to requested
			TInt outputRateToUse = aFormatData.iSampleRate;
            #ifdef SYMBIAN_SKIP_RESAMPLE_ON_RECORD
                // with this macro just channel convert at most
                outputRateToUse = aFormatData.iActualRate;
            #endif
            #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
                RDebug::Print(_L("RMdaDevSound::CBody::NegotiateFormat: Convert:CreateL from %d/%d to %d/%d"), 
                            aFormatData.iActualRate, aFormatData.iActualChannels,
                            aFormatData.iSampleRate, aFormatData.iRequestedChannels); 
            #endif																	       
			TRAP(err, aFormatData.iConverter = CChannelAndSampleRateConverter::CreateL(aFormatData.iActualRate, 
																	       aFormatData.iActualChannels,
																	       outputRateToUse, 
																		   aFormatData.iRequestedChannels));
			}
		}
	
	return err;
	}

	
void RMdaDevSound::CBody::PlayFormatsSupported(TSoundFormatsSupportedBuf& aFormatsSupported)
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	FormatsSupported(aFormatsSupported, iPlaySoundDevice);
	}
	
void RMdaDevSound::CBody::GetPlayFormat(TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	GetFormat(aFormat, iPlaySoundDevice, iPlayData);
	}
	
TInt RMdaDevSound::CBody::SetPlayFormat(const TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	return SetFormat(aFormat, iPlaySoundDevice, iPlayData);
	}

void RMdaDevSound::CBody::RecordFormatsSupported(TSoundFormatsSupportedBuf& aFormatsSupported)
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
	FormatsSupported(aFormatsSupported, iRecordSoundDevice);
	}

void RMdaDevSound::CBody::GetRecordFormat(TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
	GetFormat(aFormat, iRecordSoundDevice, iRecordData);	
	}

TInt RMdaDevSound::CBody::SetRecordFormat(const TCurrentSoundFormatBuf& aFormat)
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
	return SetFormat(aFormat, iRecordSoundDevice, iRecordData);
	}
	
void RMdaDevSound::CBody::Close()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
        RDebug::Printf("void RMdaDevSound::CBody::Close() started");
    #endif
	iBufferIndex = -1;
	iBufferOffset = -1;
	iBufferLength = 0;
	iCurrentPlayer = 0;
	iTimerActive = EFalse;
	iChunk.Close();
	iPlaySoundDevice.Close();
	iRecordSoundDevice.Close();
	iState = ENotReady;
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
        RDebug::Printf("void RMdaDevSound::CBody::Close() ended");
    #endif
	}
		
TInt RMdaDevSound::CBody::Handle()
	{//This method is actually used to check whether the device is opened. Below logic should work
	if(iPlaySoundDevice.Handle())
		{
		return iPlaySoundDevice.Handle();
		}
	return 0;
	}

void RMdaDevSound::CBody::PlayData(TRequestStatus& aStatus, const TDesC8& aData)
	{
	#ifdef SYMBIAN_SOUNDADAPTER_DEBUG
        RDebug::Printf("RMdaDevSound::CBody::PlayData(0x%x,%d) State=%d Current=%d. Handle=%d.",&aStatus, 
                aData.Length(), iState, iCurrentPlayer, iChunk.Handle());
        RDebug::Printf("KPlayMaxSharedChunkBuffersMask=0x%x KNumPlayersMask=0x%x", 
                KPlayMaxSharedChunkBuffersMask, KNumPlayersMask);
	#endif
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	aStatus = KRequestPending;
	iPlayerStatus = &aStatus;//store the status of datapath player
	//No support for simultaneous play and record in RMdaDevSound
	if(iState == ERecording)
		{
		SoundDeviceError(KErrInUse);
		return;
		}
	const TDes8* data = static_cast<const TDes8*>(&aData);
	
	if(!iChunk.Handle())
		{
		//This is where we setup to play. 
		//Configure the shared chunk for two buffers with iBufferSize each
		iBufferConfig.iNumBuffers = KPlayMaxSharedChunkBuffers;
		iDeviceBufferLength = data->MaxLength();
		iBufferConfig.iFlags = 0;//data will be continuous
		// If required, use rate converter etc
		if (iPlayData.iConverter)
			{
			iDeviceBufferLength = iPlayData.iConverter->MaxConvertBufferSize(iDeviceBufferLength, ETrue);
			}
		iBufferConfig.iBufferSizeInBytes = iDeviceBufferLength;
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("number of buffers: [%d]",iBufferConfig.iNumBuffers);
            RDebug::Printf("BufferSize in Bytes [%d]",iBufferConfig.iBufferSizeInBytes);
        #endif
		TPckg<TPlaySharedChunkBufConfig> bufferConfigBuf(iBufferConfig);
		TInt error = iPlaySoundDevice.SetBufferChunkCreate(bufferConfigBuf,iChunk);
		if(error == KErrNone)
			{
			iPlaySoundDevice.GetBufferConfig(bufferConfigBuf);
			TSoundFormatsSupportedV02Buf modnumber;
			iPlaySoundDevice.Caps(modnumber);
			TInt minBufferSize = KMinBufferSize;
            #ifdef SYMBIAN_FORCE_32BIT_LENGTHS
                minBufferSize = Max(minBufferSize, 4); // force to 32-bit buffer align
            #endif
			iRequestMinSize = Max(modnumber().iRequestMinSize, minBufferSize); 
			error = iBufferRemaining.Create(iRequestMinSize);
			// work out mask so that x&iRequestMinMask is equiv to x/iRequestMinSize*iRequestMinSize
			iRequestMinMask = ~(iRequestMinSize-1); // assume iRequestMinSize is power of 2
			}
		if (error)
			{
			SoundDeviceError(error);
			return;
			}
		}
	
	iBufferIndex = (iBufferIndex+1) & KPlayMaxSharedChunkBuffersMask;
	
	TPtr8 dataPtr(iChunk.Base()+ iBufferConfig.iBufferOffsetList[iBufferIndex], 0, iDeviceBufferLength);

	__ASSERT_DEBUG(!(iBufferRemaining.Length()>0 && iPlayData.iConverter), 
		Panic(EPanicPartialBufferConverterNotSupported)); // can't deal with conversion with restrictions on buffer size
	
	if (iBufferRemaining.Length() != 0)
		{
		// This checks if any data was left over from last times rounding and adds it to the dataPtr
		dataPtr.Copy(iBufferRemaining);
		iBufferRemaining.SetLength(0);
		}
		
	if (iPlayData.iConverter)
		{
		iPlayData.iConverter->Convert(aData, dataPtr);
		ASSERT(iSecondPhaseData.Length()==0); // assume this below, so check 
		ASSERT(iBufferRemaining.Length()==0);
		}
	else
		{
		TInt dataLength = aData.Length();

		TInt lengthAlreadyInDeviceBuffer = dataPtr.Length();
		TInt desiredDeviceBufferLength = (lengthAlreadyInDeviceBuffer + dataLength) & iRequestMinMask;
		if (desiredDeviceBufferLength > dataPtr.MaxLength())
			{
			// the buffer would be two long to do in one go, so do as two phases
			desiredDeviceBufferLength = (lengthAlreadyInDeviceBuffer + (dataLength/2)) & iRequestMinMask;
			}

		TInt lengthToCopy = desiredDeviceBufferLength - lengthAlreadyInDeviceBuffer;
		lengthToCopy = Max(lengthToCopy, 0); // ensure lengthToCopy is not negative
		
		TInt remainingToBeCopied = dataLength - lengthToCopy;
		TInt secondPhaseLength = remainingToBeCopied & iRequestMinMask;
		TInt remainingForNextRun = remainingToBeCopied - secondPhaseLength;
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG			
            RDebug::Printf("dataLength: [%d]",dataLength);
            RDebug::Printf("lengthAlreadyInDeviceBuffer: [%d]",lengthAlreadyInDeviceBuffer);
            RDebug::Printf("desiredDeviceBufferLength: [%d]",desiredDeviceBufferLength);
            RDebug::Printf("lengthToCopy: [%d]",lengthToCopy);
            RDebug::Printf("remainingToBeCopied: [%d]",remainingToBeCopied);
            RDebug::Printf("secondPhaseLength: [%d]",secondPhaseLength);
            RDebug::Printf("remainingForNextRun: [%d]",remainingForNextRun);
        #endif
		dataPtr.Append(aData.Left(lengthToCopy));
		iSecondPhaseData.Set(aData.Mid(lengthToCopy, secondPhaseLength));
		iBufferRemaining.Copy(aData.Mid(lengthToCopy + secondPhaseLength, remainingForNextRun));
		iHaveSecondPhaseData = secondPhaseLength>0;
		}
			
	if(iState == EOpened || iState == EPlayBuffersFlushed)
		{
		if(dataPtr.Length() > 0 && iSecondPhaseData.Length()==0)
			{
			// Note: if we have identified second phase, don't call BufferEmptied() here as we can't cope with a new PlayData() call
			//Make sure that next player do not overtake the current player, especially when recovering from underflow
			TInt otherPlayer = (iCurrentPlayer+1) & KNumPlayersMask;
			iPlayers[otherPlayer]->Deque();
			CActiveScheduler::Add(iPlayers[otherPlayer]);
			//Beginning we need to give two play requests for an uninterrupted playback	with the new driver
			BufferEmptied();
			}
		iState = EPlaying;
		}
    #ifdef _DEBUG
        TInt cachePlayer = iCurrentPlayer; // TODO: remove
    #endif
	iPlayers[iCurrentPlayer]->PlayData(iBufferConfig.iBufferOffsetList[iBufferIndex], dataPtr.Length());
	ASSERT(iCurrentPlayer==cachePlayer); // check have not changed since previous calc
	iCurrentPlayer = (iCurrentPlayer+1) & KNumPlayersMask;
	#ifdef SYMBIAN_SOUNDADAPTER_DEBUG
        RDebug::Printf("RMdaDevSound::CBody::PlayData() Exit. Current=%d, Handle=%d.", 
                iCurrentPlayer, iChunk.Handle());
	#endif
	}

void RMdaDevSound::CBody::RecordData(TRequestStatus& aStatus, TDes8& aData)
	{
	__ASSERT_DEBUG(iRecordSoundDevice.Handle(), Panic(EDeviceNotOpened));
	aStatus = KRequestPending;
	iRecorderStatus = &aStatus;
	//No support for simultaneous play and record in RMdaDevSound
	if(iState == EPlaying)
		{
		SoundDeviceError(KErrInUse);
		return;
		}

	iData = &aData;
	
	if(!iChunk.Handle())
		{
		//Configure the shared chunk for two buffers with iBufferSize each
		iRecordBufferConfig.iNumBuffers = KRecordMaxSharedChunkBuffers;
		iDeviceBufferLength = iData->MaxLength(); // initial size - resize if needs be
		if (iRecordData.iConverter)
			{
			// if number of channels used differs from request, resize buffer
			// assume we have nice rounded values for buffer.
			if (iRecordData.iActualChannels>iRecordData.iRequestedChannels)
				{
				iDeviceBufferLength *= 2; // will record at stereo and convert to mono 
				}
			else if (iRecordData.iActualChannels<iRecordData.iRequestedChannels)
				{
				iDeviceBufferLength /= 2; // will record at mono and convert to stereo 
				}
			}
		iRecordBufferConfig.iBufferSizeInBytes = iDeviceBufferLength;
		iRecordBufferConfig.iFlags = 0;
		TPckg<TRecordSharedChunkBufConfig> bufferConfigBuf(iRecordBufferConfig);
		TInt error = iRecordSoundDevice.SetBufferChunkCreate(bufferConfigBuf,iChunk);
		if(error == KErrNone)
			{
			iRecordSoundDevice.GetBufferConfig(bufferConfigBuf);
			}
		else
			{
			SoundDeviceError(error);
			return;
			}
		iState = ERecording;
		}		
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
        RDebug::Printf("RMdaDevSound::CBody::RecordData,iBufferOffset[%d]",iBufferOffset);
    #endif
	iPlayers[iCurrentPlayer]->RecordData(iBufferLength);
	}
	
void RMdaDevSound::CBody::SoundDeviceError(TInt aError, TInt /*aPlayerIndex*/)
// This is called by CPlayer objects when there is an error in RunL
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
	RDebug::Print(_L("RMdaDevSound::CBody::SoundDeviceError: aError[%d]"), aError);
    #endif	

	//When we get an underflow from one of the players and the other player is active, we are 
	//bound to get an underflow from the other player too. So we ignore the first and process
	//the second
	TInt otherPlayerIndex = (iCurrentPlayer+1) & KNumPlayersMask;
	if (iPlayers[otherPlayerIndex]->IsActive() && aError==KErrUnderflow)
		{
		return;
		}
	SoundDeviceError(aError);
	}
/**
   Note for maintainers: Please note that this method is called from
   CancelPlayData and CancelRecordData with KErrNone as a parameter in order to
   cancel the players and reset the internal state.
 */
void RMdaDevSound::CBody::SoundDeviceError(TInt aError)
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
	RDebug::Print(_L("RMdaDevSound::CBody::SoundDeviceError: Error[%d] CurrentPlayer[%d]"), aError, iCurrentPlayer);
    #endif

	for (TInt i=0; i<KNumPlayers; i++)
		{
		if(KErrNone == aError)
			{
			// If we are here, SoundDeviceError(KErrNone) has been called from
			// CancelPlayData or CancelRecordData to maje sure the players are
			// cancel and their state reset
			iPlayers[i]->Stop();
			}
		else
			{
			if (!iPlayers[i]->IsActive())
				{
				iPlayers[i]->ResetPlayer();
				}
			}
		}

	iBufferRemaining.SetLength(0);
	if(iPlayErrorStatus && aError!=KErrNone)//error receiver is only for errors
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("RMdaDevSound::CBody::SoundDeviceError Completing iPlayerErrorStatus");
        #endif
		User::RequestComplete(iPlayErrorStatus, aError);
		iPlayErrorStatus = NULL;
		}
	if(iPlayerStatus)
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("RMdaDevSound::CBody::SoundDeviceError Completing iPlayerStatus");
        #endif
		User::RequestComplete(iPlayerStatus, KErrNone); // effectively buffer emptied
		iPlayerStatus = NULL;
		}
  	if(iRecordErrorStatus && aError!=KErrNone)
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("RMdaDevSound::CBody::SoundDeviceError Completing iRecordErrorStatus");
        #endif
		User::RequestComplete(iRecordErrorStatus, aError);
		iRecordErrorStatus = NULL;
		}
  	else if(iRecorderStatus)
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("RMdaDevSound::CBody::SoundDeviceError Completing iRecorderStatus");
        #endif
		User::RequestComplete(iRecorderStatus, aError);
		iRecorderStatus = NULL;
		}
  	iBufferIndex = -1;
	iCurrentPlayer = 0;
	iBufferOffset = -1;
	iBufferLength = 0;
	iTimerActive = EFalse;
	iState = EOpened;
	}

void RMdaDevSound::CBody::NotifyRecordError(TRequestStatus& aStatus)
	{
	aStatus = KRequestPending;
	iRecordErrorStatus = &aStatus;
	}

void RMdaDevSound::CBody::NotifyPlayError(TRequestStatus& aStatus)
	{
	aStatus = KRequestPending;
	iPlayErrorStatus = &aStatus;
	}

void RMdaDevSound::CBody::CancelNotifyPlayError()
	{
	if(iPlayErrorStatus)
		{
		User::RequestComplete(iPlayErrorStatus, KErrCancel);
		}
	}

void RMdaDevSound::CBody::CancelNotifyRecordError()
	{
	if(iRecordErrorStatus)
		{
		User::RequestComplete(iRecordErrorStatus, KErrCancel);
		}
	}

void RMdaDevSound::CBody::FlushPlayBuffer()
	{
	__ASSERT_DEBUG(iPlaySoundDevice.Handle(), Panic(EDeviceNotOpened));
	//There is no equivalent of FlushPlaybuffer in the new sound driver. So use CancelPlayData
	//to simulate the original behavior
	if ((iState == EPlaying) || (iState == EPaused))
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Print(_L("RMdaDevSound::CBody::FlushPlayBuffers in Playing or Paused state"));
        #endif

		if (iState == EPaused)
			{
			iFlushCalledDuringPause = ETrue;
			}


		iPlaySoundDevice.CancelPlayData();
		iBufferRemaining.SetLength(0);
		iState = EPlayBuffersFlushed;			
		}

	}




RSoundSc& RMdaDevSound::CBody::PlaySoundDevice()
	{
	return iPlaySoundDevice;
	}

RSoundSc& RMdaDevSound::CBody::RecordSoundDevice()
	{
	return iRecordSoundDevice;
	}
	
RMdaDevSound::CBody::TState RMdaDevSound::CBody::State()
	{
	return iState;
	}

void RMdaDevSound::CBody::BufferEmptied()
	{
	if(iPlayerStatus)
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Print(_L("***Buffer Emptied***"));
        #endif
		User::RequestComplete(iPlayerStatus, KErrNone);
		iPlayerStatus = NULL;
		}
	}

void RMdaDevSound::CBody::BufferFilled(TInt aBufferOffset)
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("RMdaDevSound::CBody::BufferFilled:"));
    #endif	

	ASSERT(aBufferOffset>=0 || aBufferOffset==KErrCancel);
	ASSERT(iData); // request should not get this without

	if(aBufferOffset==KErrCancel)
		{
		//we can get KErrCancel when we call pause and there is no more data left with the driver
		//we send the empty buffer to the HwDevice, where this should trigger the shutdown mechanism
		iData->SetLength(0);
		User::RequestComplete(iRecorderStatus, KErrNone);
		iRecorderStatus = NULL;
		return;
		}
		
	iBufferOffset = aBufferOffset;
	//when last buffer is flushed, new driver sometimes gives buffer size of odd number. One of our codecs
	//expects that the buffer size should always be even. Base suggested that we fix in multimedia
	//as it is quite complicated to fix in overthere.
	iBufferLength = iBufferLength & 0xfffffffe;
	TPtr8 dataPtr(iChunk.Base()+ iBufferOffset, iBufferLength, iData->MaxLength());
	if (iRecordData.iConverter)
		{
		iRecordData.iConverter->Convert(dataPtr, *iData);
		}
	else
		{
		iData->Copy(dataPtr);
		}
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
        RDebug::Print(_L("RMdaDevSound::CBody::BufferFilled: BufferOffset[%d] BufferLen[%d]"), iBufferOffset, iBufferLength);
    #endif
	if(iBufferOffset >= 0)
		{
		iRecordSoundDevice.ReleaseBuffer(iBufferOffset);
		}
	if(iRecorderStatus)
		{
		User::RequestComplete(iRecorderStatus, KErrNone);
		iRecorderStatus = NULL;
		}
	}
							
void RMdaDevSound::CBody::PlayCancelled()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("RMdaDevSound::CBody::PlayCancelled:"));
    #endif	

	for (TInt index=0; index<KNumPlayers; index++)
		{
		iPlayers[index]->Cancel();
		}
	iBufferIndex = -1;
	iCurrentPlayer = 0;
	iBufferOffset = -1;
	iBufferLength = 0;
	iTimerActive = EFalse;
	if(iPlayerStatus)
		{
		User::RequestComplete(iPlayerStatus, KErrNone);
		iPlayerStatus = NULL;
		}
	}
	
void RMdaDevSound::CBody::UpdateTimeAndBytesPlayed()
	{
	iBytesPlayed = iPlaySoundDevice.BytesTransferred();
	iStartTime = User::FastCounter();
	iTimerActive=ETrue;
	}
	
TBool RMdaDevSound::CBody::TimerActive()
	{
	return iTimerActive;
	}

TBool RMdaDevSound::CBody::FlushCalledDuringPause()
	{
	return iFlushCalledDuringPause;
	}
	
RMdaDevSound::CBody::CPlayer::CPlayer(TInt aPriority, RMdaDevSound::CBody& aParent, TInt aIndex):
	CActive(aPriority), iParent(aParent), iIndex(aIndex), iBufferOffset(-1), iBufferLength(0)
	{
	CActiveScheduler::Add(this);
	}

RMdaDevSound::CBody::CPlayer::~CPlayer()
	{
	Cancel();
	}

void RMdaDevSound::CBody::CPlayer::RunL()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
        RDebug::Print(_L("****RMdaDevSound::CBody::CPlayer(%d)::RunL: Error[%d] ParentState[%d] Outstanding[%d], pending[%d]"), 
                            iIndex, iStatus.Int(), iParent.State(), iParent.iHaveSecondPhaseData, iRequestPending);
    #endif

	//this is required to avoid receiving the request completions in the order diff. from the 
	//issued order
	Deque();
	CActiveScheduler::Add(this);
	
	TInt error = iStatus.Int();
	
	// It's fine to schedule buffers to the driver in the paused state (i.e. iRequestPending == EFalse)
	if(!iRequestPending && (iParent.State() == EPlaying || iParent.State() == EPaused) && error == KErrNone)
		{
		//this is from playdata
		#ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
		RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::RunL: Playing BufferOffset[%d] BufferLength[%d]"), iIndex, iBufferOffset, iBufferLength);
		#endif
		//Make sure the length is even. We may get odd for the last partial buffer.
		iBufferLength = iBufferLength & 0xfffffffe;

		PlaySoundDevice();
		//Need this for the first time only
		if(!iParent.TimerActive())
			{
			iParent.UpdateTimeAndBytesPlayed();
			}
		iRequestPending = ETrue;
		}
	// TODO: The case below shouldn't be valid under EPaused state, i.e. the driver shouldn't complete playback if it was paused. However due to a current problem in the driver we have to handle this case
	else if (iRequestPending && (iParent.State() == EPlaying || iParent.State() == EPaused) && error == KErrNone) //this is from driver
		{		
		#ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
		RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::RunL: Buffer emptied successfully"), iIndex);
		#endif
		if (iParent.iHaveSecondPhaseData)
			{
			TPtr8 dataPtr(iParent.iChunk.Base()+ iParent.iBufferConfig.iBufferOffsetList[iParent.iBufferIndex], 0, iParent.iDeviceBufferLength);
			dataPtr.Copy(iParent.iSecondPhaseData);
							
			PlaySoundDevice();
			iParent.iCurrentPlayer = (iParent.iCurrentPlayer+1) & KNumPlayersMask;
			iParent.UpdateTimeAndBytesPlayed();
			iParent.iHaveSecondPhaseData = EFalse;			
			}
		else
			{
			iRequestPending = EFalse;
			iParent.UpdateTimeAndBytesPlayed();
			iParent.BufferEmptied();
			}
		}
	else if(iParent.State() == EPlayBuffersFlushed && error == KErrCancel)
		{
		iRequestPending = EFalse;
		if (!iParent.FlushCalledDuringPause())
			{
			iParent.PlayCancelled();
			}
		}
	else if(iParent.State() == ERecording && (error >= 0 || error == KErrCancel))
		{//we can get KErrCancel when we call pause and there is no more data left with the driver
		iParent.BufferFilled(error);
		}
	else 
		{
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
            RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::RunL: Error[%d] Outstanding[%d], pending[%d]"), 
						  iIndex, error, iParent.iHaveSecondPhaseData,iRequestPending);
        #endif
		iParent.SoundDeviceError(error, iIndex);
		}
	}

TInt RMdaDevSound::CBody::CPlayer::RunError(TInt aError)
	{
	iParent.SoundDeviceError(aError, iIndex);
	return KErrNone;
	}
	
void RMdaDevSound::CBody::CPlayer::DoCancel()
	{
	//nothing to do
#ifdef SYMBIAN_SOUNDADAPTER_DEBUG
	RDebug::Printf("RMdaDevSound::CBody::CPlayer(%d)::DoCancel", iIndex);
#endif
	}

void RMdaDevSound::CBody::CPlayer::ResetPlayer()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
	RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::ResetPlayer: IsActive[%d] pending[%d] iBufferOffset[%d] iBufferLength[%d]"), iIndex, IsActive(), iRequestPending, iBufferOffset, iBufferLength);
    #endif

	iRequestPending = EFalse;
	iBufferOffset = -1;
	iBufferLength = 0;
	}

void RMdaDevSound::CBody::CPlayer::Stop()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
	RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::Stop: IsActive[%d] pending[%d] iBufferOffset[%d] iBufferLength[%d]"), iIndex, IsActive(), iRequestPending, iBufferOffset, iBufferLength);
    #endif

	ResetPlayer();
	Cancel();
	}
	
void RMdaDevSound::CBody::CPlayer::PlayData(TInt aBufferOffset, TInt aLength)
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
	RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::PlayData : IsActive[%d]"), iIndex,    IsActive());
    #endif	

	ASSERT(!IsActive()); // TODO: remove or replace redundant test
	iBufferOffset = aBufferOffset;
	iBufferLength = aLength;

	iStatus = KRequestPending;
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	}
	
void RMdaDevSound::CBody::CPlayer::RecordData(TInt& aBufferLength)
	{
	if (!IsActive())
	    {
	    iStatus = KRequestPending;
        #ifdef SYMBIAN_SOUNDADAPTER_DEBUG
            RDebug::Printf("Recording request: BufferLength[%d]", aBufferLength);
        #endif
	    iParent.RecordSoundDevice().RecordData(iStatus, aBufferLength);
	    SetActive();
	    }
	}

void RMdaDevSound::CBody::CPlayer::PlaySoundDevice()
	{
    #ifdef SYMBIAN_SOUNDADAPTER_DEBUG	
	RDebug::Print(_L("RMdaDevSound::CBody::CPlayer(%d)::PlaySoundDevice : IsActive[%d]"), iIndex, IsActive());
    #endif	

#ifdef SYMBIAN_FORCE_32BIT_LENGTHS
	if (iBufferLength%4 != 0)
		{
		// simulate the limitation of some hardware, where -6 is generated if the
		// buffer length is not divisible by 4.
		TRequestStatus*status = &iStatus;
		User::RequestComplete(status, KErrArgument);
		}
	else
#endif
		{
		iParent.PlaySoundDevice().PlayData(iStatus, iBufferOffset, iBufferLength, EFalse);
		// Pause was called when there was no data available. Now that we have data available, we should pause the driver
		if (iParent.iPauseDeviceDriverOnNewData)
			{
			#ifdef SYMBIAN_SOUNDADAPTER_DEBUG
				RDebug::Printf("Pausing the driver after receiving data to play");
			#endif			
			iParent.PlaySoundDevice().Pause();
			iParent.iPauseDeviceDriverOnNewData = EFalse;
			}
		}
	SetActive();

	}
