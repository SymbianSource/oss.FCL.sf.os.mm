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

/**
 @file
 @internalComponent
*/

#ifndef OMXILVORBDECHWDEVICE_H
#define OMXILVORBDECHWDEVICE_H

#include <mmf/server/mmfhwdevice.h>
#include <mmf/server/mmfdatabuffer.h>
#include <mmf/common/mmfutilities.h>

#include "milcomponentportif.h"
#include "milifobserver.h"

class CMMFBuffer;

/**
Audio Decoder IL Hardware Device class that uses IL Components in order to decode audio data.
@see CMMFHwDevice
@see MMdfHwDeviceSetup
@see MILIfObserver
*/
class COmxILVorbDecHwDevice : public CMMFHwDevice,
				    		  public MILIfObserver
	{
public:	
	/*
	The current state of the IL Hardware Device.
	*/
	enum TILHwDevState
		{
		/*
		The HwDevice has just been created.
		*/
		EILHwDeviceCreated,
		/*
		The components have been loaded.
		*/
		EILHwDeviceLoaded,
		/*
		The components are currently being initialised.
		*/
		EILHwDeviceInitializing,
		/*
		The components are currently in the idle state.
		*/
		EILHwDeviceIdle,
		/*
		The components are currently in the executing state.
		*/
		EILHwDeviceExecuting,
		/*
		The components are currently in the paused state.
		*/
		EILHwDevicePaused		
		};	
	
public:
	static COmxILVorbDecHwDevice* NewL();
	
	// from CMMFHwDevice
	TInt Start(TDeviceFunc aFuncCmd, TDeviceFlow aFlowCmd);
	TInt Stop();
	TInt Pause();
	TInt Init(THwDeviceInitParams& aDevInfo);
	TAny* CustomInterface(TUid aInterfaceId);
	TInt ThisHwBufferFilled(CMMFBuffer& aFillBufferPtr);
	TInt ThisHwBufferEmptied(CMMFBuffer& aEmptyBufferPtr);
	TInt SetConfig(TTaskConfig& aConfig);
	TInt StopAndDeleteCodec();
	TInt DeleteCodec();	
	~COmxILVorbDecHwDevice();

	// from MILIfObserver
	TInt MsgFromILComponent(const MILComponentIf* aComponent,const TILEvent& aEvent);
	TInt BufferDelivered(const MILComponentPortIf* aPort, const CMMFBuffer* aBuffer);


private:
	COmxILVorbDecHwDevice();
	void ConstructL();
	TInt CreateBuffers();
	TInt StartDecode();
	TInt InitializeDecode();
	TInt StartExecuting();
	void StopHwDevice(TInt error);

	void InitializeComplete(const MILComponentIf* aComponent, TInt aErrorCode);
	void ExecuteComplete(const MILComponentIf* aComponent, TInt aErrorCode);

private:
	MILComponentIf* iCodecIf;
	MILComponentIf* iRendererIf;
	MILComponentPortIf* iCodecInPortIf;
	MILComponentPortIf* iCodecOutPortIf;
	MILComponentPortIf* iRendererInPortIf;
	CMMFBuffer* iInBuffer;
	CMMFBuffer* iOutBuffer;
	TBool iCodecCallbackComplete;
	TBool iRendererCallbackComplete;

	TILHwDevState iState;
	TBool iStopping;
	
	CActiveSchedulerWait* iActiveWait;
	};
	
#endif // OMXILVORBDECHWDEVICE_H
