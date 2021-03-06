// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <mmf/plugin/mmfbthwdeviceimplementationuids.hrh>
#include "BtSoundDeviceBody.h"
#include <mmf/server/mmfbtswcodecwrappercustominterfacesuids.hrh> // KUidBtRefDevSoundTaskConfig
#include "MmfBtFileDependencyUtil.h"

/*
 *
 *	Default Constructor.
 *
 *	No default implementation. CMMFDevSound implements 2-phase construction.
 *
 */
CMMFDevSoundClientImp::CMMFDevSoundClientImp()
	{
	}

/*
 *
 *	Destructor.
 *
 *	Deletes all objects and releases all resource owned by this
 *	instance.
 *
 */
CMMFDevSoundClientImp::~CMMFDevSoundClientImp()
	{
	CancelCalbackHandlers();
	
	delete iBTBFHandler; 
	delete iPEHandler;
	delete iREHandler;
	delete iTFHandler;
	delete iBTBEHandler;
	delete iSETCHandler;
	delete iICHandler;

	if( iDevSoundProxy != NULL)
		{
		iDevSoundProxy->Close();
		delete iDevSoundProxy;
		}
	}

/*
 *
 *	Constructs, and returns a pointer to, a new CMMFDevSound object.
 *
 *	Leaves on failure.
 *
 */
CMMFDevSoundClientImp* CMMFDevSoundClientImp::NewL()
	{
	FileDependencyUtil::CheckFileDependencyL();

	CMMFDevSoundClientImp* self = new (ELeave) CMMFDevSoundClientImp();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
 *
 *	3rd phase constructor - assumes that iParent has already been set up properly
 *                          (During ConstructL() it has yet to be
 *
 */
void CMMFDevSoundClientImp::ConstructL()
	{
	// all these data properties should be NULL, but add ASSERTs to verity

	ASSERT(iDevSoundProxy==NULL);
	iDevSoundProxy = new (ELeave) RMMFDevSoundProxy();

	TInt err = iDevSoundProxy->Open();
	if(err)
		{
		delete iDevSoundProxy;
		iDevSoundProxy = NULL;
		}

	User::LeaveIfError(err);
	}

/*
 *  @prototype
 */
void CMMFDevSoundClientImp::CancelCalbackHandlers()
	{	
	if(iBTBFHandler)
		iBTBFHandler->CancelReceiveEvents();
	if(iPEHandler)
		iPEHandler->CancelReceiveEvents();
	if(iREHandler)
		iREHandler->CancelReceiveEvents();
	if(iTFHandler)
		iTFHandler->CancelReceiveEvents();
	if(iBTBEHandler)
		iBTBEHandler->CancelReceiveEvents();
	if(iSETCHandler)
		iSETCHandler->CancelReceiveEvents();
	if(iICHandler)
		iICHandler->CancelReceiveEvents();
	}

/*
 *
 *	Initializes CMMFDevSound object to play and record PCM16 raw audio data
 *	with sampling rate of 8 KHz.
 *
 *	On completion of Initialization, calls InitializeComplete() on
 *	aDevSoundObserver.
 *
 *	Leaves on failure.
 *
 *	@param	"MDevSoundObserver& aDevSoundObserver"
 *			A reference to DevSound Observer instance.
 *
 *	@param	"TMMFState aMode"
 *			Mode for which this object will be used.
 *
 */
void CMMFDevSoundClientImp::InitializeL(MDevSoundObserver& aDevSoundObserver, TMMFState aMode)

	{
	TInt initError = KErrNone;
	iDevSoundObserver = &aDevSoundObserver;

	// Need to set up an init complete event and message handler
	// prior to calling InitializeL()
	if (iICHandler)
		{
		iICHandler->CancelReceiveEvents();
		}
	else
		{
		iICHandler = CMMFInitializeCompleteHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iICHandler->ReceiveEvents();

	initError = iDevSoundProxy->InitializeL(aMode);
	
	if (initError)
		{		
		iDevSoundObserver->InitializeComplete(initError);
		User::Leave(initError);
		}

    if(iBTBFHandler)
		{
		iBTBFHandler->CancelReceiveEvents();
		}
	else
		{
		iBTBFHandler = CMMFBufferToBeFilledHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iBTBFHandler->ReceiveEvents();


    if(iPEHandler)
		{
		iPEHandler->CancelReceiveEvents();
		}
	else
		{
		iPEHandler = CMMFPlayErrorHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iPEHandler->ReceiveEvents();


    if(iREHandler)
		{
		iREHandler->CancelReceiveEvents();
		}
	else
		{
		iREHandler = CMMFRecordErrorHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iREHandler->ReceiveEvents();


    if(iTFHandler)
		{
		iTFHandler->CancelReceiveEvents();
		}
	else
		{
		iTFHandler = CMMFToneFinishedHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iTFHandler->ReceiveEvents();


    if(iBTBEHandler)
		{
		iBTBEHandler->CancelReceiveEvents();
		}
	else
		{
		iBTBEHandler = CMMFBufferToBeEmptiedHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iBTBEHandler->ReceiveEvents();


    if(iSETCHandler)
		{
		iSETCHandler->CancelReceiveEvents();
		}
	else
		{
		iSETCHandler = CMMFSendEventToClientHandler::NewL(iDevSoundProxy, *iDevSoundObserver);
		}
	iSETCHandler->ReceiveEvents();
	}

/*
 *
 *	Configure CMMFDevSound object for the settings in aConfig.
 *
 *	Use this to set sampling rate, Encoding and Mono/Stereo.
 *
 *	@param	"TMMFCapabilities& aConfig"
 *			Attribute values to which CMMFDevSound object will be configured to.
 *
 *  As part of defect 20796, the iRecordFormat has been set under the iPlayFormat, 
 *  before it was not set at all.
 *
 */
void CMMFDevSoundClientImp::SetConfigL(const TMMFCapabilities& aConfig)
	{
	iDevSoundProxy->SetConfigL(aConfig);
	}

/*
 *
 *	Changes the current playback volume to a specified value.
 *
 *	The volume can be changed before or during playback and is effective
 *	immediately.
 *
 *	@param	"TInt aVolume"
 *			The volume setting. This can be any value from zero to the value
 *			returned by a call to CMdaAudioPlayerUtility::MaxVolume(). If the
 *			volume is not within this range, the volume is automatically set to
 *			minimum or maximum value based on the value that is being passed.
 *			Setting a zero value mutes the sound. Setting the maximum value
 *			results in the loudest possible sound.
 *
 */
void CMMFDevSoundClientImp::SetVolume(TInt aVolume)
	{
	iDevSoundProxy->SetVolume(aVolume);
	}

/*
 *
 *	Changes the current recording gain to a specified value.
 *
 *	The gain can be changed before or during recording and is effective
 *	immediately.
 *
 *	@param	"TInt aGain"
 *			The volume setting. This can be any value from zero to the value
 *			returned by a call to CMdaAudioPlayerUtility::MaxVolume(). If the
 *			volume is not within this range, the gain is automatically set to
 *			minimum or maximum value based on the value that is being passed.
 *			Setting a zero value mutes the sound. Setting the maximum value
 *			results in the loudest possible sound.
 *
 */
void CMMFDevSoundClientImp::SetGain(TInt aGain)
	{
	iDevSoundProxy->SetGain(aGain);
	}

/*
 *
 *	Sets the speaker balance for playing.
 *
 *	The speaker balance can be changed before or during playback and is
 *	effective immediately.
 *
 *	@param	"TInt& aLeftPercentage"
 *			On return contains left speaker volume perecentage. This can be any
 *			value from zero to 100. Setting a zero value mutes the sound on left
 *			speaker.
 *
 *	@param	"TInt& aRightPercentage"
 *			On return contains right speaker volume perecentage. This can be any
 *			value from zero to 100. Setting a zero value mutes the sound on
 *			right speaker.
 *
 */
void CMMFDevSoundClientImp::SetPlayBalanceL(TInt aLeftPercentage, TInt aRightPercentage)
	{
	iDevSoundProxy->SetPlayBalanceL(aLeftPercentage, aRightPercentage);
	}

/*
 *
 *	Sets the microphone gain balance for recording.
 *
 *	The microphone gain balance can be changed before or during recording and
 *	is effective immediately.
 *
 *	@param	"TInt aLeftPercentage"
 *			Left microphone gain precentage. This can be any value from zero to
 *			100. Setting a zero value mutes the gain on left microphone.
 *
 *	@param	"TInt aRightPercentage"
 *			Right microphone gain precentage. This can be any value from zero to
 *			100. Setting a zero value mutes the gain on right microphone.
 *
 */
void CMMFDevSoundClientImp::SetRecordBalanceL(TInt aLeftPercentage, TInt aRightPercentage)
	{
	iDevSoundProxy->SetRecordBalanceL(aLeftPercentage, aRightPercentage);
	}

/*
 *
 *	Initializes audio device and start play process. This method queries and
 *	acquires the audio policy before initializing audio device. If there was an
 *	error during policy initialization, PlayError() method will be called on
 *	the observer with error code KErrAccessDenied, otherwise BufferToBeFilled()
 *	method will be called with a buffer reference. After reading data into the
 *	buffer reference passed, the client should call PlayData() to play data.
 *
 *	The amount of data that can be played is specified in
 *	CMMFBuffer::RequestSize(). Any data that is read into buffer beyond this
 *	size will be ignored.
 *
 *	Leaves on failure.
 *
 */
void CMMFDevSoundClientImp::PlayInitL()
	{
	if (!iDevSoundObserver)
		User::Leave(KErrNotReady);
	iDevSoundProxy->PlayInitL();
	}

/*
 *
 *	Initializes audio device and start record process. This method queries and
 *	acquires the audio policy before initializing audio device. If there was an
 *	error during policy initialization, RecordError() method will be called on
 *	the observer with error code KErrAccessDenied, otherwise BufferToBeEmptied()
 *	method will be called with a buffer reference. This buffer contains recorded
 *	or encoded data. After processing data in the buffer reference passed, the
 *	client should call RecordData() to continue recording process.
 *
 *	The amount of data that is available is specified in
 *	CMMFBuffer::RequestSize().
 *
 *	Leaves on failure.
 *
 */
void CMMFDevSoundClientImp::RecordInitL()
	{

	if (!iDevSoundObserver)
		User::Leave(KErrNotReady);
	iDevSoundProxy->RecordInitL();
	}

/*
 *
 *	Plays data in the buffer at the current volume. The client should fill
 *	the buffer with audio data before calling this method. The Observer gets
 *	reference to buffer along with callback BufferToBeFilled(). When playing of
 *	the audio sample is complete, successfully or otherwise, the method
 *	PlayError() on observer is called.
 *
 */
void CMMFDevSoundClientImp::PlayData()
	{
	ASSERT(iDevSoundObserver);
	iDevSoundProxy->PlayData();
	}

/*
 *
 *	Stops the ongoing operation (Play, Record, TonePlay, Convert)
 *
 */
void CMMFDevSoundClientImp::Stop()
	{
	iDevSoundProxy->Stop();
	}

/*
 *
 *	Temporarily Stops the ongoing operation (Play, Record, TonePlay, Convert)
 *
 */
void CMMFDevSoundClientImp::Pause()
	{
	iDevSoundProxy->Pause();
	}

/*
 *
 *	Returns the sample recorded so far.
 *
 *	@return "TInt"
 *			Returns the samples recorded.
 *
 */
TInt CMMFDevSoundClientImp::SamplesRecorded()
	{
	return iDevSoundProxy->SamplesRecorded();
	}

/*
 *
 *	Returns the sample played so far.
 *
 *	@return "TInt"
 *			Returns the samples recorded.
 *
 */
TInt CMMFDevSoundClientImp::SamplesPlayed()
	{
	return iDevSoundProxy->SamplesPlayed();
	}


/*
 *
 *	Initializes audio device and start playing tone. Tone is played with
 *	frequency and for duration specified.
 *
 *	Leaves on failure.
 *
 *	@param	"TInt aFrequency"
 *			Frequency at with the tone will be played.
 *
 *	@param	"TTimeIntervalMicroSeconds& aDuration"
 *			The period over which the tone will be played. A zero value causes
 *			the no tone to be played (Verify this with test app).
 *
 */
void CMMFDevSoundClientImp::PlayToneL(TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration)
	{
	iDevSoundProxy->PlayToneL(aFrequency, aDuration);
	}

/*
 *	Initializes audio device and start playing a dual tone. 
 *  The tone consists of two sine waves of different frequencies summed together
 *  Dual Tone is played with specified frequencies and for specified duration.
 *
 *	@param	"aFrequencyOne"
 *			First frequency of dual tone
 *
 *	@param	"aFrequencyTwo"
 *			Second frequency of dual tone
 *
 *	@param	"aDuration"
 *			The period over which the tone will be played. A zero value causes
 *			the no tone to be played (Verify this with test app).
 */
void CMMFDevSoundClientImp::PlayDualToneL(TInt aFrequencyOne, TInt aFrequencyTwo, const TTimeIntervalMicroSeconds& aDuration)
	{
	iDevSoundProxy->PlayDualToneL(aFrequencyOne, aFrequencyTwo, aDuration);
	}

/*
 *
 *	Initializes audio device and start playing DTMF string aDTMFString.
 *
 *	Leaves on failure.
 *
 *	@param	"TDesC& aDTMFString"
 *			DTMF sequence in a descriptor.
 *
 */
void CMMFDevSoundClientImp::PlayDTMFStringL(const TDesC& aDTMFString)
	{
	if (!iDevSoundObserver)
		User::Leave(KErrNotReady);

	iDevSoundProxy->PlayDTMFStringL(aDTMFString);
	}

/*
 *
 *	Initializes audio device and start playing tone sequence.
 *
 *	Leaves on failure.
 *
 *	@param	"TDesC8& aData"
 *			Tone sequence in a descriptor.
 *
 */
void CMMFDevSoundClientImp::PlayToneSequenceL(const TDesC8& aData)
	{
	if (!iDevSoundObserver)
		User::Leave(KErrNotReady);

	iDevSoundProxy->PlayToneSequenceL(aData);
	}

/*
 *
 *	Initializes audio device and start playing the specified pre-defined tone
 *	sequence.
 *
 *	Leaves on failure.
 *
 *	@param	"TInt aSequenceNumber"
 *			The index identifying the specific pre-defined tone sequence. Index
 *			values are relative to zero.
 *			This can be any value from zero to the value returned by a call to
 *			CMdaAudioPlayerUtility::FixedSequenceCount() - 1.
 *			The function raises a panic if sequence number is not within this
 *			range.
 *
 */
void CMMFDevSoundClientImp::PlayFixedSequenceL(TInt aSequenceNumber)
	{
	if (!iDevSoundObserver)
		User::Leave(KErrNotReady);

	iDevSoundProxy->PlayFixedSequenceL(aSequenceNumber);
	}

/*
 *
 *	Defines the duration of tone on, tone off and tone pause to be used during the
 *	DTMF tone playback operation.
 *
 *	Supported only during tone playing.
 *
 *	@param	"TTimeIntervalMicroSeconds32& aToneOnLength"
 *			The period over which the tone will be played. If this is set to
 *			zero, then the tone is not played.
 *
 *	@param	"TTimeIntervalMicroSeconds32& aToneOffLength"
 *			The period over which the no tone will be played.
 *
 *	@param	"TTimeIntervalMicroSeconds32& aPauseLength"
 *			The period over which the tone playing will be paused.
 *
 */
void CMMFDevSoundClientImp::SetDTMFLengths(TTimeIntervalMicroSeconds32& aToneOnLength,
								TTimeIntervalMicroSeconds32& aToneOffLength,
								TTimeIntervalMicroSeconds32& aPauseLength) 
	{
	iDevSoundProxy->SetDTMFLengths(aToneOnLength, aToneOffLength, aPauseLength);
	}

/*
 *
 *	Defines the period over which the volume level is to rise smoothly from
 *	nothing to the normal volume level.
 *
 *	@param	"TTimeIntervalMicroSeconds& aRampDuration"
 *			The period over which the volume is to rise. A zero value causes 
 *			the tone sample to be played at the normal level for the full
 *			duration of the playback. A value, which is longer than the duration
 *			of the tone sample, that the sample never reaches its normal
 *			volume level.
 *
 *
 */
void CMMFDevSoundClientImp::SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration)
	{
	iDevSoundProxy->SetVolumeRamp(aRampDuration);
	}


/**
 *	@see sounddevice.h
 */
void CMMFDevSoundClientImp::GetSupportedInputDataTypesL(RArray<TFourCC>& aSupportedDataTypes, const TMMFPrioritySettings& aPrioritySettings) const
	{
	iDevSoundProxy->GetSupportedInputDataTypesL(aSupportedDataTypes, aPrioritySettings);
	}

/**
 *	@see sounddevice.h
 */
void CMMFDevSoundClientImp::GetSupportedOutputDataTypesL(RArray<TFourCC>& aSupportedDataTypes, const TMMFPrioritySettings& aPrioritySettings) const
	{
	iDevSoundProxy->GetSupportedOutputDataTypesL(aSupportedDataTypes, aPrioritySettings);
	}

/**
 *	@see sounddevice.h
 */
TInt CMMFDevSoundClientImp::SetClientThreadInfo(TThreadId aTid)
	{
	return iDevSoundProxy->SetClientThreadInfo(aTid);
	}


TInt CMMFDevSoundClientImp::RegisterAsClient(TUid aEventType, const TDesC8& aNotificationRegistrationData)
	{
	return iDevSoundProxy->RegisterAsClient(aEventType,aNotificationRegistrationData);
	}
	
TInt CMMFDevSoundClientImp::CancelRegisterAsClient(TUid aEventType)
	{
	return iDevSoundProxy->CancelRegisterAsClient(aEventType);
	}
	
TInt CMMFDevSoundClientImp::GetResourceNotificationData(TUid aEventType,TDes8& aNotificationData)
	{
	return iDevSoundProxy->GetResourceNotificationData(aEventType,aNotificationData);
	}
	
TInt CMMFDevSoundClientImp::WillResumePlay()
	{
	return iDevSoundProxy->WillResumePlay();
	}
