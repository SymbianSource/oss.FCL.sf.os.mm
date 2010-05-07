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
// vorbisdecilhwdevice.cpp
// 
//

#include "omxilvorbdechwdevice.h"

#include <ecom/implementationproxy.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <mmf/server/mmfhwdevicesetup.h>
#include <a3f/audioformatuids.h>

#include "tilstructtypes.h"
#include "ilcomponentif.h"
#include "iltestuids.hrh"



_LIT8(KNULLName,"");
_LIT8(KVorbisDecoderILComponentName,"OMX.SYMBIAN.AUDIO.DECODER.VORBIS");
			
const TInt KZerothPort = 0;
const TInt KInputPortIndex = 0;
const TInt KMaxNumberPort = 1;

// _________________________________________________________________________________
// Map the interface implementation KUidAudioDecILHwDevice UID to the implementation

const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidOmxILVorbDecHwDevice , COmxILVorbDecHwDevice::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}
// _________________________________________________________________________________


/*
The destructor.  Releases the components and frees any owned buffers.
@see CMMFHwDevice::~CMMFHwDevice()
*/
COmxILVorbDecHwDevice::~COmxILVorbDecHwDevice()
	{
	Stop();

	if (iCodecIf)
		{
		iCodecIf->ReleaseComponent();
		iCodecIf = NULL;
		}
	if (iRendererIf)
		{
		iRendererIf->ReleaseComponent();
		iRendererIf = NULL;
		}

	iCodecInPortIf = NULL;
	iCodecOutPortIf = NULL;
	iRendererInPortIf = NULL;
		
	delete iInBuffer;
	iInBuffer = NULL;
	delete iOutBuffer;
	iOutBuffer = NULL;
	
	delete iActiveWait;

	OMX_Deinit();
	}

/**
Creates a new COmxILVorbDecHwDevice object.  
The COmxILVorbDecHwDevice state is set to EILHwDeviceLoaderLoaded.
@see TILHwDevState
*/
COmxILVorbDecHwDevice* COmxILVorbDecHwDevice::NewL()
	{
	COmxILVorbDecHwDevice* self = new (ELeave) COmxILVorbDecHwDevice;
	CleanupStack::PushL (self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
Default constructor.
*/	
COmxILVorbDecHwDevice::COmxILVorbDecHwDevice()
	{	
	}

/*
Second phase constructor
*/	
void COmxILVorbDecHwDevice::ConstructL()
	{		
	iActiveWait = new (ELeave) CActiveSchedulerWait;
	iState = EILHwDeviceCreated;
  	OMX_Init();

	iCodecIf = CILComponentIf::CreateImplementationL(TUid::Uid(KUidOmxILDummyAudioDecMILIFPlugin));
	iCodecIf->CreateComponentL(KVorbisDecoderILComponentName, *this);
	User::LeaveIfError(iCodecIf->SetComponentIfRole(KUidFormatOGG)); 
	
	iRendererIf = CILComponentIf::CreateImplementationL(TUid::Uid(KUidDummyPcmRendererMILIFPlugin));
	iRendererIf->CreateComponentL(KNULLName, *this); //
	
	}	

/*
@see CMMFHwDevice::Start()
@see TDeviceFunc
*/
TInt COmxILVorbDecHwDevice::Start(TDeviceFunc aFuncCmd, TDeviceFlow /*aFlowCmd*/)
	{		
	if (!((aFuncCmd == EDevDecode)|(aFuncCmd == EDevNullFunc)))
		{
		return KErrArgument;	
		}
			
	TInt err = KErrNone;
	switch(aFuncCmd)
		{
		case EDevDecode:
			{
			err = StartDecode();
			break;
			}
		default:
			{
			err = KErrNotSupported;	
			}		
		}
	
	return err;
	}
	

/*
Starts the execution of the decode operation, and sets the state
of the COmxILVorbDecHwDevice to EILHwDeviceExecuting.
@return  An error code indicating if the function call was successful.
KErrNone on success, otherwise another of the system-wide error codes.
*/	
TInt COmxILVorbDecHwDevice::StartExecuting()
	{
	TInt err = KErrNone;
	iCodecOutPortIf->FillThisBuffer(*iOutBuffer);		
	err = iHwDeviceObserver->FillThisHwBuffer(*iInBuffer);
	if(err != KErrNone)
		{
		return err;
		}
		
	iState = EILHwDeviceExecuting;
		
	iCodecIf->Execute();
	iRendererIf->Execute();
	
	return err;
	}
	
/*
Initialises the decode operation, and set the state of the COmxILVorbDecHwDevice
to EILHwDeviceInitializing.
@return  An error code indicating if the function call was successful.
KErrNone on success, otherwise another of the system-wide error codes.
*/	
TInt COmxILVorbDecHwDevice::InitializeDecode()
	{
	ASSERT(iCodecOutPortIf && iRendererInPortIf);	
	
	//GetConfig from codec input port;
	TILPortDefinition paramStruct;
	TPortDefinitionType* config = NULL;
	TInt err =iCodecIf->GetConfig(paramStruct, iCodecInPortIf);
	if (err != KErrNone)
		{
		return err;
		}
 	config = TILPortDefinition::GetStructure(paramStruct);		
	TRAP(err, iInBuffer = CMMFDescriptorBuffer::NewL(config->nBufferSize));
	if(err != KErrNone)
		{
		return err;
		}	
	iCodecInPortIf->UseBuffer(*iInBuffer);	

	//GetConfig from codec output port;
	err =iCodecIf->GetConfig(paramStruct, iCodecOutPortIf);
	if (err != KErrNone)
		{
		return err;
		}
	config = TILPortDefinition::GetStructure(paramStruct);
	TRAP(err, iOutBuffer = CMMFDescriptorBuffer::NewL(config->nBufferSize));
	if(err != KErrNone)
		{
		return err;
		}	
 	iCodecOutPortIf->UseBuffer(*iOutBuffer);	

	iState = EILHwDeviceInitializing;

	iCodecIf->Initialize();
	iRendererIf->Initialize();

	iActiveWait->Start();
	return KErrNone;
	}

/*
Starts the decode operation, providing that the intial state of the
COmxILVorbDecHwDevice is EILHwDeviceLoaded.
@return  An error code indicating if the function call was successful.
KErrNone on success, otherwise another of the system-wide error codes.
*/
TInt COmxILVorbDecHwDevice::StartDecode()
	{	
	TInt err = KErrNone;
	if (iState == EILHwDeviceLoaded)
		{
		err = InitializeDecode();
		}
	if (err != KErrNone)
		{
		return err;
		}
	if (iState == EILHwDeviceIdle)
		{
		// Ensure that the LastBuffer flags are reset.
		iInBuffer->SetLastBuffer(EFalse);
		iOutBuffer->SetLastBuffer(EFalse);
		}
	return StartExecuting();	
	}

/*
@see CMMFHwDevice::Stop()
*/
TInt COmxILVorbDecHwDevice::Stop()
	{	
	if(iState == EILHwDeviceExecuting || iState == EILHwDevicePaused)
		{			
		iStopping = ETrue; 
		
		// reset the flags
		iCodecCallbackComplete = EFalse;
		iRendererCallbackComplete = EFalse;
		
		if(iRendererIf)
			{
			iRendererIf->Stop();	
			}
		if(iCodecIf)
			{
			iCodecIf->Stop();	
			}
							
		iState = EILHwDeviceIdle;
		}
	return KErrNone;		
	}

/*
@see CMMFHwDevice::Pause()
*/
TInt COmxILVorbDecHwDevice::Pause()
	{
	TInt err = KErrNone;
	if(iState != EILHwDevicePaused)
		{
		if(iRendererIf)
			{	
			err = iRendererIf->Pause();	
			}
		iState = EILHwDevicePaused;
		}	
	return err;	
	}

/*
@see CMMFHwDevice::Init()
*/
TInt COmxILVorbDecHwDevice::Init(THwDeviceInitParams& aDevInfo)
	{		
	RPointerArray<MILComponentPortIf> ifPorts;

	if(!aDevInfo.iHwDeviceObserver)
		{
		return KErrArgument;
		}
	iHwDeviceObserver = aDevInfo.iHwDeviceObserver;
	
	if(!iCodecIf)
		{
		return KErrNotSupported;
		}

	// Get decoder input port. For this particular scenario expects one single port.
	TInt err = iCodecIf->GetComponentInputPorts(ifPorts);		
	if (err == KErrNone && ifPorts.Count() == KMaxNumberPort)
		{
		iCodecInPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (!ifPorts.Count() != KMaxNumberPort)
			{
			err = KErrNotFound;
			}
		ifPorts.Close();	
		return err;				
		}	
		
	// Get decoder ouput port. For this particular scenario expects one single port.
	err = iCodecIf->GetComponentOutputPorts(ifPorts);		
	if (err == KErrNone && ifPorts.Count() == KMaxNumberPort)
		{
		iCodecOutPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (!ifPorts.Count() != KMaxNumberPort)
			{
			err = KErrNotFound;
			}
		ifPorts.Close();	
		return err;				
		}	

	if(!iRendererIf)
		{
		return KErrNotSupported;
		}
		
	// Get renderer inport port. For this particular scenario expects one single port.
	err = iRendererIf->GetComponentInputPorts(ifPorts);		
	if (err == KErrNone && ifPorts.Count() == KMaxNumberPort)
		{
		iRendererInPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (!ifPorts.Count() != KMaxNumberPort)
			{
			err = KErrNotFound;
			}
		ifPorts.Close();	
		return err;				
		}	

	iState = EILHwDeviceLoaded;
	return KErrNone;		
	}	

/*
@see CMMFHwDevice::CustomInterface()
*/
TAny* COmxILVorbDecHwDevice::CustomInterface(TUid aInterfaceId)
	{
	if (aInterfaceId.iUid == KMmfPlaySettingsCustomInterface)
		{
		return iRendererIf->CustomInterface(aInterfaceId);
		}
	else		
		{
		return NULL;
		}
	}

/*
@see CMMFHwDevice::ThisHwBufferFilled()
*/
TInt COmxILVorbDecHwDevice::ThisHwBufferFilled(CMMFBuffer& aFillBufferPtr)
	{
	aFillBufferPtr.SetStatus(EFull);	
	iCodecInPortIf->EmptyThisBuffer(aFillBufferPtr);		
	return KErrNone;
	}

/*
@see CMMFHwDevice::ThisHwBufferEmptied()
*/
TInt COmxILVorbDecHwDevice::ThisHwBufferEmptied(CMMFBuffer& /*aEmptyBufferPtr*/)
	{
	// Not used while decoding
	return KErrNotSupported;
	}

/*
@see CMMFHwDevice::SetConfig()
*/
TInt COmxILVorbDecHwDevice::SetConfig(TTaskConfig& aConfig)
	{
	TInt err = KErrNone;

	// Configure the Codec component
	TILTaskConfig configStruct(aConfig);
	err = iCodecIf->SetConfig(configStruct, iCodecInPortIf);
	if(err != KErrNone)
		{
		return err;
		}
	err = iCodecIf->SetConfig(configStruct, iCodecOutPortIf);
	if(err != KErrNone)
		{
		return err;
		}
	err = iRendererIf->SetConfig(configStruct, iCodecInPortIf);
	if(err != KErrNone)
		{
		return err;
		}
		
	return KErrNone;
	}

/*
@see CMMFHwDevice::StopAndDeleteCodec()
*/
TInt COmxILVorbDecHwDevice::StopAndDeleteCodec()
	{	
	TInt stopError = Stop();
	TInt deleteError = DeleteCodec();

	if (stopError != KErrNone)
		{
		return stopError;
		}		
	else
		{
		return deleteError;
		}		
	}

/*
Unloads all components, and deletes any locally owned buffers.
State is set to EILHwDeviceLoaderLoaded.

@see CMMFHwDevice::DeleteCodec()
*/
TInt COmxILVorbDecHwDevice::DeleteCodec()
	{
	Stop();	
	if (iCodecIf)
		{
		iCodecIf->ReleaseComponent();
		iCodecIf = NULL;
		}
	if (iRendererIf)
		{
		iRendererIf->ReleaseComponent();
		iRendererIf = NULL;
		}
	
	iCodecInPortIf = NULL;
	iCodecOutPortIf = NULL;
	iRendererInPortIf = NULL;
		
	delete iInBuffer;
	iInBuffer = NULL;
	delete iOutBuffer;
	iOutBuffer = NULL;
	
	iState = EILHwDeviceCreated;
	
	return KErrNone;
	}

/*
Called to indicate that the Hardware Device has been stopped, leading
to callbacks to the observer. 
@see MMMFHwDeviceObserver::Stopped()
@see MMMFHwDeviceObserver::Error()
*/
void COmxILVorbDecHwDevice::StopHwDevice(TInt error)
	{
	iHwDeviceObserver->Stopped();
	iHwDeviceObserver->Error(error);		
	}

/*
Called to process messages from the IL components
*/
TInt COmxILVorbDecHwDevice::MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent) 
	{
	TInt err = KErrNone;
	switch (aEvent.iEvent)
		{
		case EEventCmdComplete:
			{
			if (aEvent.iData1 == ECommandStateSet)
				{
				if (aEvent.iData2 == EComponentIdle)
					{
					if (iState == EILHwDeviceInitializing)
						{
						InitializeComplete(aComponent, KErrNone);
						}
					else
						{
						ExecuteComplete(aComponent, KErrNone);
						}
					}
				else if(aEvent.iData2 == EComponentExecuting)
					{
					ExecuteComplete(aComponent, KErrNone);
					}
				else
					{
					StopHwDevice(KErrNotSupported);	
					}			
				break;
				}
			else if (aEvent.iData1 == ECommandFlush)
				{
				//Command Flush completed
				err = KErrNotSupported; // check what a3f should do here if anything
				break;
				}
			}
		case EEventBufferFlag:
			{
			if (aEvent.iData1 == KInputPortIndex)
				{
				if (iState == EILHwDeviceExecuting)
					{
					StopHwDevice(aEvent.iData2);		
					iState = EILHwDeviceIdle;
					}
				}
			else
				{
				err = KErrNotSupported;					
				}
			break;
			}
		default:
			{
			err = KErrNotSupported;
			StopHwDevice(err);	
			}
		}
	return err;
	}

/*
Called to indicate that the buffer has been delivered.
*/
TInt COmxILVorbDecHwDevice::BufferDelivered(const MILComponentPortIf* aPort, const CMMFBuffer* aBuffer) 
	{
	TInt err = KErrNone;

	if(aPort == iCodecInPortIf)
		{				
		if (!aBuffer->LastBuffer())
			{	
			// if the device is idle (having been Stopped) then
			// FillThisHwBuffer will cause a kern-exec,
			// as the DevSoundSession no longer has a buffer
			if(iState == EILHwDeviceExecuting) 
				{
				err = iHwDeviceObserver->FillThisHwBuffer(const_cast<CMMFBuffer&>(*aBuffer));
				if(err != KErrNone)
					{
					StopHwDevice(err);
					}						
				}
			}
		}
	else if (aPort == iRendererInPortIf)
		{
		if (!aBuffer->LastBuffer())
			{				
			err = iCodecOutPortIf->FillThisBuffer(*iOutBuffer);
			}
		}
	else if(aPort == iCodecOutPortIf)
		{
		err = iRendererInPortIf->EmptyThisBuffer(*aBuffer);
		}
	else
		{
		err = KErrNotSupported;	
		}
	return err;
	}
	
void COmxILVorbDecHwDevice::InitializeComplete(const MILComponentIf* aComponent, TInt aErrorCode)
	{
	if(aErrorCode != KErrNone)
		{
		// stop waiting on the active scheduler if we were doing so
		if (iState == EILHwDeviceInitializing)
			{
			// change state back to loaded
			iState = EILHwDeviceLoaded;
			iActiveWait->AsyncStop();
			}
		return;
		}
			
	if(aComponent == iCodecIf)
		{
		iCodecCallbackComplete = ETrue;
		}
	else if(aComponent == iRendererIf)
		{
		iRendererCallbackComplete = ETrue;
		}	

	if(iCodecCallbackComplete && iRendererCallbackComplete)
		{
		// reset the flags
		iCodecCallbackComplete = EFalse;
		iRendererCallbackComplete = EFalse;
		
		// Both components initialised OK
		if (iState == EILHwDeviceInitializing)
			{
			iActiveWait->AsyncStop();
			}
		iState = EILHwDeviceIdle;
		}
	}
	
void COmxILVorbDecHwDevice::ExecuteComplete(const MILComponentIf* aComponent, TInt aErrorCode)
	{

	if(aComponent == iCodecIf)
		{
		iCodecCallbackComplete = ETrue;
		}
	else if(aComponent == iRendererIf)
		{
		iRendererCallbackComplete = ETrue;
		}	

	if (iCodecCallbackComplete && iRendererCallbackComplete)
		{
		if (iStopping)
			{
			StopHwDevice(aErrorCode);		
			iState = EILHwDeviceIdle;
			iStopping = EFalse;
			}
		else if((aErrorCode != KErrNone ) && (iState == EILHwDeviceExecuting))
			{
			// stop the hardware device if we are still executing
			StopHwDevice(aErrorCode);		
				
			iCodecCallbackComplete = EFalse;
			iRendererCallbackComplete = EFalse;					
			}
		}
	}
