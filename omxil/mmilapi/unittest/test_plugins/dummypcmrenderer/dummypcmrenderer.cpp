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

#include "dummypcmrenderer.h"
#include "iltestuids.hrh"
#include "tilstructtypes.h"
#include <ecom/implementationproxy.h>
#include <mmf/server/mmfdatabuffer.h>


#define KDummyPCM16FormatUid 0x10111126 //dummy

const TInt KBufferSize = 16384;			// we need 16k to hold a pcm packet

const TInt KDefaultSampleRate = 8000;
const TInt KDefaultNumberChannels = 1;

const TInt KInputPortIndex = 0;
	

// ------------------------------------------------------------------------------------------
// CDummyPcmRenderer::CInputPort Implementation
	
CDummyPcmRenderer::CInputPort::CInputPort(CDummyPcmRenderer& aParent) 
	: CActive(EPriorityNormal),
	  iParent(aParent),
	  iSampleRate(KDefaultSampleRate),
	  iChannels(KDefaultNumberChannels),
	  iBufferSize(KBufferSize)
	{
	CActiveScheduler::Add(this);	
	}
		
CDummyPcmRenderer::CInputPort::~CInputPort()
	{
	Cancel();
	iBuffers.Close();
	}

CDummyPcmRenderer::CInputPort* CDummyPcmRenderer::CInputPort::NewL(CDummyPcmRenderer& aParent)
	{
	CInputPort* self = new (ELeave) CInputPort(aParent);
	return self;
	}

TInt CDummyPcmRenderer::CInputPort::FillThisBuffer(CMMFBuffer& /*aInputBuffer*/)
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::CInputPort::EmptyThisBuffer(const CMMFBuffer& aInputBuffer)
	{
	TInt err = iBuffers.Append(&aInputBuffer);
	if (err == KErrNone)
		{
		TILComponentState state;
		if (iParent.GetState(state) != KErrNone)
			{
			return EComponentInvalid;		
			}
		if (state == EComponentExecuting && !IsActive())
			{
			SetActive();
			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			}
		}
	return err;
	}
	
TInt CDummyPcmRenderer::CInputPort::TunnelRequest(MILComponentPortIf* aPort)
	{
	TILComponentState state;
	if (iParent.GetState(state) != KErrNone)
		{
		return EComponentInvalid;		
		}
	if (( state != EComponentLoaded) && (!iStopped))
		{
		return EComponentInvalid;
		}
	
	if (iPortConnectedTo)
		{
		// the port is already connected, return an error
		return EPortAlreadyTunnelled;
		}
	iPortConnectedTo = const_cast<MILComponentPortIf*>(aPort);
	return KErrNone;
	}
	
TInt CDummyPcmRenderer::CInputPort::DisconnectTunnel(MILComponentPortIf* /*aPort*/)
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::CInputPort::PortIndex() const
	{
	return KInputPortIndex;
	}	
	
TPortDirection CDummyPcmRenderer::CInputPort::PortDirection() const
	{
	return EDirInput;
	}

	
CMMFBuffer* CDummyPcmRenderer::CInputPort::CreateBufferL(TInt /*aBufferSize*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
TInt CDummyPcmRenderer::CInputPort::UseBuffer(CMMFBuffer& /*aBuffer*/)
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::CInputPort::FreeBuffer(CMMFBuffer* /*aBuffer*/)
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::CInputPort::FlushPort()
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::CInputPort::EnablePort()
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::CInputPort::DisablePort()
	{
	return KErrNotSupported;
	}

MILComponentIf* CDummyPcmRenderer::CInputPort::PortComponent() const
	{
	return &iParent;
	}

void CDummyPcmRenderer::CInputPort::SetVolume(TUint aVolume)
	{
	iVolume = aVolume;
	}

TUint CDummyPcmRenderer::CInputPort::Volume()
	{
	return iVolume;
	}

TUint CDummyPcmRenderer::CInputPort::BytesPlayed()
	{
	return iBytesPlayed;
	}

void CDummyPcmRenderer::CInputPort::SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration)
	{
	iRampDuration = aRampDuration;
	}

TTimeIntervalMicroSeconds& CDummyPcmRenderer::CInputPort::VolumeRamp()
	{
	return iRampDuration;
	}

TInt CDummyPcmRenderer::CInputPort::SampleRate()
	{
	return iSampleRate;
	}
	
TInt CDummyPcmRenderer::CInputPort::Channels()
	{
	return iChannels;
	}	
	
TUint32 CDummyPcmRenderer::CInputPort::BufferSize() const
	{
	return iBufferSize;
	}

void CDummyPcmRenderer::CInputPort::Execute()
	{
	if (!IsActive() && iBuffers.Count()>0)
		{
		SetActive();
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		}
	}

void CDummyPcmRenderer::CInputPort::Pause()
	{
	if (iParent.SoundDevice().Handle())
		{
		iParent.SoundDevice().PausePlayBuffer();
		}
	}
	
void CDummyPcmRenderer::CInputPort::Stop()
	{
	Cancel();	
	}

void CDummyPcmRenderer::CInputPort::RunL()
	{

	if (iCurrentBuffer != NULL)
		{
		// If we've been signalled with a buffer, callback that we've completed the writing of the
		// buffer
		if (iParent.Observer())
			{		
			iParent.Observer()->BufferDelivered(this, iCurrentBuffer);
			if (iCurrentBuffer->LastBuffer())
				{
				TILEvent thisEvent;
				thisEvent.iEvent = EEventBufferFlag;
				thisEvent.iData1 = PortIndex();				
				thisEvent.iData2 = static_cast<TUint32>(KErrUnderflow); 
				iParent.Observer()->MsgFromILComponent(&iParent, thisEvent); 
				iParent.SoundDevice().Close();
				}
			}
		iCurrentBuffer = NULL;
		}
		
	// only process the next buffer if there is no error
	// error callbacks were handled in the previous block
	if (iStatus == KErrNone)
		{
		if (iBuffers.Count()>0)
			{
			iCurrentBuffer = iBuffers[0];
			iBuffers.Remove(0);
			
			if (CMMFBuffer::IsSupportedDataBuffer(iCurrentBuffer->Type()))
				{
 				TDes8& aBufferDes = (static_cast<CMMFDataBuffer*>(iCurrentBuffer))->Data();
 				iStatus = KRequestPending;
				iParent.SoundDevice().PlayData(iStatus, aBufferDes);
				SetActive();
				}
			}
		}
	else
		{
		// In the real implementation here should be sending an error event back to the client
		}
	}

void CDummyPcmRenderer::CInputPort::DoCancel()
	{
	if (iParent.SoundDevice().Handle())
		{
		iParent.SoundDevice().CancelPlayData();
		iParent.SoundDevice().FlushPlayBuffer();
		}
	}

// ------------------------------------------------------------------------------------------
// CDummyPcmRenderer Implementation
	
CDummyPcmRenderer::CDummyPcmRenderer()
	{
	}
	
CDummyPcmRenderer::~CDummyPcmRenderer()
	{
	delete iInputPort;
	iSoundDevice.Close();
	}
	
CDummyPcmRenderer* CDummyPcmRenderer::NewL()
	{
	CDummyPcmRenderer* self = new (ELeave) CDummyPcmRenderer;
	return self;
	}
	
void CDummyPcmRenderer::CreateComponentL(const TDesC8& /*aComponentName*/, MILIfObserver& aComponentIfObserver)
	{
	iObserver = &aComponentIfObserver;
	iInputPort = CInputPort::NewL(*this);
	iState = EComponentLoaded;
	}

void CDummyPcmRenderer::CreateComponentL(const TUid& /*aUid*/, MILIfObserver& /*aComponentIfObserver*/)	
	{
	User::LeaveIfError(KErrNotSupported);
	};

void CDummyPcmRenderer::ReleaseComponent()
	{
	delete this;	
	}

TInt CDummyPcmRenderer::GetComponentInputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const
	{
	return aComponentPorts.Append(iInputPort);
	}

TInt CDummyPcmRenderer::GetComponentOutputPorts(RPointerArray<MILComponentPortIf>& /*aComponentPorts*/) const
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::SetConfig(const TILStruct& aConfig, const MILComponentPortIf* /*aComponentPort*/)
	{
	if (aConfig.Uid() == TUid::Uid(KUidMMFTTaskConfig))
		{
		const TTaskConfig* config = TILTaskConfig::GetStructure(aConfig);	
		ASSERT(config);
		iInputPort->iSampleRate = config->iRate;
		iInputPort->iChannels = (config->iStereoMode & ETaskMono)? 1 : 2;
		iInputPort->iInterleaved = (config->iStereoMode & ETaskInterleaved)?ETrue : EFalse;
		return KErrNone;
		}
	
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::GetConfig(TILStruct& /*aConfig*/, const MILComponentPortIf* /*aComponentPort*/) const
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::Initialize()
	{
	ASSERT(iObserver);
	
	TILEvent thisEvent;
	thisEvent.iEvent = EEventCmdComplete;
	thisEvent.iData1 = ECommandStateSet;				
	thisEvent.iData2 = EComponentIdle;
	iObserver->MsgFromILComponent(this, thisEvent); 

	iState = EComponentIdle;
	return KErrNone;	
	}
	
TInt CDummyPcmRenderer::UnInitialize()
	{
	ASSERT(iObserver);
	
	TILEvent thisEvent;
	thisEvent.iEvent = EEventCmdComplete;
	thisEvent.iData1 = ECommandStateSet;				
	thisEvent.iData2 = EComponentLoaded;
	iObserver->MsgFromILComponent(this, thisEvent); 

	iState = EComponentLoaded;
	return KErrNone;	
	}

TInt CDummyPcmRenderer::Execute()
	{
	ASSERT(iObserver);
	TInt err = KErrNone;
	if(!iSoundDevice.Handle())
		{
		err = iSoundDevice.Open();
		}
	
	RMdaDevSound::TCurrentSoundFormatBuf buf;
	if (err == KErrNone)
		{
		if(iState == EComponentPaused)
			{
			iSoundDevice.ResumePlaying();
			}
		else
			{
			// Set play format (for input port)
			iSoundDevice.GetPlayFormat(buf);
			buf().iRate = iInputPort->SampleRate();
			buf().iChannels = iInputPort->Channels();
			buf().iBufferSize = KBufferSize;
			buf().iEncoding = RMdaDevSound::EMdaSoundEncoding16BitPCM;
			err = iSoundDevice.SetPlayFormat(buf);
			}
		}
	iState = EComponentExecuting;	

	TILEvent thisEvent;
	thisEvent.iEvent = EEventCmdComplete;
	thisEvent.iData1 = ECommandStateSet;				
	thisEvent.iData2 = EComponentExecuting;
	iObserver->MsgFromILComponent(this, thisEvent); 

	return err;		
	}

TInt CDummyPcmRenderer::Pause()
	{
	iState = EComponentPaused;
	iInputPort->Pause();
	return KErrNone;
	}

TInt CDummyPcmRenderer::Stop()
	{
	if(iState == EComponentExecuting || iState == EComponentPaused)
		{
		// Cancel and flush the device driver		
		iInputPort->Stop();

		TILEvent thisEvent;
		thisEvent.iEvent = EEventCmdComplete;
		thisEvent.iData1 = ECommandStateSet;				
		thisEvent.iData2 = EComponentIdle;
		iObserver->MsgFromILComponent(this, thisEvent); 

		iState = EComponentIdle;
		
		// Close the sound device
		iSoundDevice.Close();
			
		return KErrNone;			
		}	
	else
		{
		// invalid state
		return EComponentInvalid;			
		}	
	}

TInt CDummyPcmRenderer::GetState(TILComponentState& aState) const
	{
	aState = iState;
	return KErrNone;	
	}
			
TInt CDummyPcmRenderer::SendCommand(const TILCommand& /*aCommand*/)
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::FlushAllPorts()
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::ComponentRoleEnum(TPtr8& /*aComponentRole*/, TUint32 /*aIndex*/) const
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::GetComponentVersion(TILComponentVersion& /*aVersion*/) const
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::SetComponentIfRole(const TDesC8& /*aComponentRole*/)
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::GetComponentIfRole(TDes8& /*aComponentRole*/) const
	{
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::SetComponentIfRole(const TUid& /*aFormat*/)
	{
	// This interface is specific writen to support only this PCM renderer role
	return KErrNotSupported;
	}

TInt CDummyPcmRenderer::GetComponentIfRole(TUid& aFormat) const
	{
	aFormat = TUid::Uid(KDummyPCM16FormatUid);
	return KErrNone;	
	}
	
TAny* CDummyPcmRenderer::CustomInterface(TUid aUid)
	{
	if (aUid.iUid == KMmfPlaySettingsCustomInterface)
		{
		return static_cast<MPlayCustomInterface*>(iInputPort);
		}
	return NULL;	
	}	

TInt CDummyPcmRenderer::SetExtConfig(const TDesC8& /*aParameterName*/, const TILStruct& /*aConfig*/, const MILComponentPortIf* /*aComponentPort*/)
	{
	return KErrNotSupported;		
	}

TInt CDummyPcmRenderer::GetExtConfig(const TDesC8& /*aParameterName*/, TILStruct& /*aConfig*/, const MILComponentPortIf* /*aComponentPort*/) const
	{
	return KErrNotSupported;		
	}


MILIfObserver* CDummyPcmRenderer::Observer() const
	{
	return iObserver;	
	}
		
RMdaDevSound& CDummyPcmRenderer::SoundDevice()
	{
	return iSoundDevice;
	}
	

// ------------------------------------------------------------------------------------------	
// ECOM Implementation table entry	
	
const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidDummyPcmRendererMILIFPlugin,	CDummyPcmRenderer::NewL),
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
