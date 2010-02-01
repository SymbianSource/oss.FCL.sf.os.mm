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
// Implementation of Unit test step classes for MILIF testing.
// 
//

#include "tsu_milif_omxiliftests.h"
#include "iltestuids.hrh"
#include "ilcomponentif.h"
#include "tilstructtypes.h"
#include "tomxilstruct.h"
#include <openmax/il/khronos/v1_x/OMX_Core.h>

_LIT8(KDummyDecoderILComponentName,"OMX.SYMBIAN.AUDIO.DECODER.DUMMY");
_LIT8(KExtensionPortDefinitionName,"OMX.CompanyXYZ.index.param.PortDefinition");
_LIT8(KFakeRole,"audio_decoder.vrb");

const TUid KUidFormatAAC = {0x1028347C};

const TInt KMaxComponentRoleLength = 128;

const TInt KZerothPort = 0;


//
// RTestStepMILIFBase: Base class for all RTestStepMILIF* tests
//

/*
 *
 * RTestStepMILIFBase - Test step constructor
 *
 */
RTestStepMILIFBase::RTestStepMILIFBase(const TDesC& aTestName)
:RAsyncTestStep()
	{
	iTestStepName = aTestName; 
	}

/*
 *
 * KickoffTestL - Starts the test
 *
 */
void RTestStepMILIFBase::KickoffTestL()
	{
	__MM_HEAP_MARK;
	
	DoKickoffTestL();
	DoTestBody();
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFBase::DoKickoffTestL()
	{
  	OMX_Init();

	if ((iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0001"))||
		(iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0002"))||
		(iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0003"))||
		(iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0004"))||
		(iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0005"))||
		(iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0006")))
		{
		INFO_PRINTF1(_L("1. Create an instance of the KUidOmxILAudioDecGenericIf test ECOM plugin interface."));
		TRAPD(error, iDummyComponent = CILComponentIf::CreateImplementationL(TUid::Uid(KUidOmxILDummyAudioDecMILIFPlugin)));
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Could not instantiate the generic audio decoder OMX IL test ECOM plugin interface: error = %d"), error);
			StopTest(error, EFail);
			return;
			}	

		INFO_PRINTF1(_L("2. Use the interface to instanciate a dummy omx component."));
		TRAP(error, iDummyComponent->CreateComponentL(KDummyDecoderILComponentName, *this));
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Could not create omx component: error = %d"), error);
			StopTest(error, EFail);
			return;
			}
				
		INFO_PRINTF1(_L("3. Set the role of the interface."));
		error = iDummyComponent->SetComponentIfRole(KUidFormatAAC); 
		if(error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Could not set the role of the interface: error = %d"), error);
			StopTest(error, EFail);
			}	
		}
	else
		{
		StopTest(KErrGeneral, EFail);	
		}
	}

/*
 *
 * DoTestBodyL
 *
 */
void RTestStepMILIFBase::DoTestBody()
	{
	// Nothing to do in the base class
	}
	
/*
 *
 * CloseTest
 *
 */
void RTestStepMILIFBase::CloseTest()
	{
	if (iDummyComponent)
		{
		iDummyComponent->ReleaseComponent();
		iDummyComponent = NULL;
		}

	iComponentInPortIf = NULL;
	iComponentOutPortIf = NULL;
	
	OMX_Deinit();
	
	__MM_HEAP_MARKEND;
	}

/*
 *
 * MsgFromILComponent - Callback from component for component events
 *
 */
TInt RTestStepMILIFBase::MsgFromILComponent(const MILComponentIf* /*aComponent*/, const TILEvent& /*aEvent*/)
	{
	// Nothing to do in the base class	
	return KErrNotSupported;
	}

/*
 *
 * BufferDelivered	- Callback from component for buffer delivered events
 *
 */
TInt RTestStepMILIFBase::BufferDelivered(const MILComponentPortIf* /*aPort*/, const CMMFBuffer* /*aBuffer*/)
	{
	// Nothing to do in the base class	
	return KErrNotSupported;
	}


//
// RTestStepMILIFPorts: To test access to IL component ports through MILIF.
//

/*
 *
 * RTestStepMILIFPorts - Test step constructor
 *
 */
RTestStepMILIFPorts::RTestStepMILIFPorts(const TDesC& aTestName)
	:RTestStepMILIFBase(aTestName)
	{
	}
	
/*
 *
 * NewL
 *
 */
RTestStepMILIFPorts* RTestStepMILIFPorts::NewL(const TDesC& aTestName)
	{
	return (new (ELeave) RTestStepMILIFPorts(aTestName));
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFPorts::DoKickoffTestL()
	{
	INFO_PRINTF1(_L("RTestStepMILIFPorts: To test access to IL component ports through MILIF interface."));
	
	RTestStepMILIFBase::DoKickoffTestL();
	}
	
/*
 *
 * DoTestBodyL
 *
 */
void RTestStepMILIFPorts::DoTestBody()
	{
	RPointerArray<MILComponentPortIf> ifPorts;

	INFO_PRINTF1(_L("4. Get Component Input ports through MILIF interface."));
	TInt error = iDummyComponent->GetComponentInputPorts(ifPorts);		
	if (error == KErrNone && ifPorts.Count() == 1)
		{
		iComponentInPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (ifPorts.Count() != 1)
			{
			error = KErrNotFound;
			}
		ifPorts.Close();	
		ERR_PRINTF2(_L("	ERROR! - Fail to grab input ports: error = %d"), error);
		StopTest(error, EFail);
		return;
		}	


	INFO_PRINTF1(_L("5. Get Component Output ports through MILIF interface."));
	error = iDummyComponent->GetComponentOutputPorts(ifPorts);		
	if (error == KErrNone && ifPorts.Count() > 0)
		{
		iComponentOutPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (ifPorts.Count() != 1)
			{
			error = KErrNotFound;
			}
		ifPorts.Close();	
		ERR_PRINTF2(_L("	ERROR! - Fail to grab output ports: error = %d"), error);
		StopTest(error, EFail);
		return;
		}	
	StopTest(KErrNone);
	}


//
// RTestStepMILIFConfig: To test configuration of IL component through MILIF.
//

/*
 *
 * RTestStepMILIFConfig - Test step constructor
 *
 */
RTestStepMILIFConfig::RTestStepMILIFConfig(const TDesC& aTestName)
:RTestStepMILIFBase(aTestName)
	{
	}

/*
 *
 * NewL
 *
 */
RTestStepMILIFConfig* RTestStepMILIFConfig::NewL(const TDesC& aTestName)
	{
	RTestStepMILIFConfig* self = new (ELeave) RTestStepMILIFConfig(aTestName);
	return self;
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFConfig::DoKickoffTestL()
	{
	INFO_PRINTF1(_L("RTestStepMILIFConfig: To test configuration of the IL component through MILIF interface."));
	
	RTestStepMILIFBase::DoKickoffTestL();
	}

/*
 *
 * DoTestBodyL - 
 *
 */
void RTestStepMILIFConfig::DoTestBody()
	{
	TInt error = KErrNone;

	INFO_PRINTF1(_L("4. Get Component Input ports through MILIF interface."));
	RPointerArray<MILComponentPortIf> ifPorts;
	error = iDummyComponent->GetComponentInputPorts(ifPorts);		
	if (error == KErrNone && ifPorts.Count() == 1)
		{
		iComponentInPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (ifPorts.Count() != 1)
			{
			error = KErrNotFound;
			}
		ifPorts.Close();	
		ERR_PRINTF2(_L("	ERROR! - Fail to grab input ports: error = %d"), error);
		StopTest(error, EFail);
		return;
		}
	
	INFO_PRINTF1(_L("5a. Set a (partial) config port parameter through the interface."));
	INFO_PRINTF1(_L("		Using TPortDefinitionType and TILPortDefinition."));
	TPortDefinitionType partialConfig_s;
	partialConfig_s.nBufferCountActual = 1;
	TILPortDefinition paramStruct_s(partialConfig_s);
	error = iDummyComponent->SetConfig(paramStruct_s, iComponentInPortIf);
	if (error != KErrNone)
		{
		ERR_PRINTF2(_L("	ERROR! - Fail to set TILPortDefinition: error = %d"), error);
		StopTest(error, EFail);
		return;
		}

	INFO_PRINTF1(_L("5b. Now Get the TPortDefinitionType config parameter through the interface."));
	TILPortDefinition paramStruct_g;
	TPortDefinitionType* partialConfig_g = NULL;
	error = iDummyComponent->GetConfig(paramStruct_g, iComponentInPortIf);
	if (error != KErrNone)
		{
		ERR_PRINTF2(_L("	ERROR! - Fail to get TILPortDefinition: error = %d"), error);
		StopTest(error, EFail);
		return;
		}
 	partialConfig_g = TILPortDefinition::GetStructure(paramStruct_g);		
	if(partialConfig_g->nBufferCountActual != 1)
		{
		ERR_PRINTF1(_L("	ERROR! - Unexpected value. The get value does not match the value set."));
		StopTest(KErrCorrupt, EFail);
		return;
		}
		
	if ((iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0002")))
		{
		INFO_PRINTF1(_L("6a. Configure component through the interface."));
		INFO_PRINTF1(_L("		Using TOMXILStructParam and TILPortDefinition."));
		OMX_PARAM_PORTDEFINITIONTYPE omxconfig_s;
		omxconfig_s.nBufferCountActual = 3;
		TOMXILStructParam omxParamStruct_s(&omxconfig_s, OMX_IndexParamPortDefinition);
		error = iDummyComponent->SetConfig(omxParamStruct_s, iComponentInPortIf);
		if (error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Fail to set TOMXILStructParam: error = %d"), error);
			StopTest(error, EFail);
			return;
			}

		INFO_PRINTF1(_L("6b. Now Get the TOMXILStructParam config parameter through the interface."));
		OMX_PARAM_PORTDEFINITIONTYPE omxconfig_g;
		TOMXILStructParam omxParamStruct_g(&omxconfig_g, OMX_IndexParamPortDefinition);
		error = iDummyComponent->GetConfig(omxParamStruct_g, iComponentInPortIf);
		if (error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Fail to get TOMXILStructParam: error = %d"), error);
			StopTest(error, EFail);
			return;
			}
		if(omxconfig_g.nBufferCountActual != 3)
			{
			ERR_PRINTF1(_L("	ERROR! - Unexpected value. The get value does not match the value set."));
			StopTest(KErrCorrupt, EFail);
			return;
			}
		}
		
	StopTest(KErrNone);
	}


//
// RTestStepMILIFStates: To test driving the IL component state machine through MILIF.
//

/*
 *
 * RTestStepMILIFStates - Test step constructor
 *
 */
RTestStepMILIFStates::RTestStepMILIFStates(const TDesC& aTestName)
:RTestStepMILIFBase(aTestName)
	{
	}

/*
 *
 * NewL
 *
 */
RTestStepMILIFStates* RTestStepMILIFStates::NewL(const TDesC& aTestName)
	{
	RTestStepMILIFStates* self = new (ELeave) RTestStepMILIFStates(aTestName);
	return self;
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFStates::DoKickoffTestL()
	{
	INFO_PRINTF1(_L("RTestStepMILIFStates: To test driving the IL component state machine through MILIF interface."));
	
	RTestStepMILIFBase::DoKickoffTestL();
	}
	
/*
 *
 * DoTestBodyL - 
 *
 */
void RTestStepMILIFStates::DoTestBody()
	{
	Fsm(EEventInit, KErrNone); 
	}
	
/*
 *
 * Fsm - Executes this test events in sequence
 *
 *
 */
void RTestStepMILIFStates::Fsm(TTestEvents aEvent, TInt aError)
	{
	TInt error = KErrNone;
	
	switch (iTestState)
		{
		case ETestStateStart:
			{
			if (aEvent == EEventInit)
				{
				INFO_PRINTF1(_L("4. Trigger transition from Load to Idle in component by calling Initialize()"));
				error = iDummyComponent->Initialize();
				if (error != KErrNone)
					{
					ERR_PRINTF2(_L("	ERROR! - MILComponentIf::Initialize() error = %d"), error);
					StopTest(error, EFail);
					break;
					}
				INFO_PRINTF1(_L("	-> Test State: EStatePlayInitializing"));
				iTestState = ETestStateInitializing;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateStart did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}
			
		case ETestStateInitializing:
			{
			if (aEvent == EEventInitComplete)
				{
				INFO_PRINTF1(_L("5a. Check the component was left in the correct state after EEventInitComplete event"));
				TILComponentState aState;
				error = iDummyComponent->GetState(aState);
				if ((error != KErrNone) || (aState != EComponentIdle))
					{
					ERR_PRINTF3(_L("	ERROR! - MILComponentIf::GetState Incorrect state = %d, error = %d."), error, aState);
					if (aState != EComponentIdle)
						{
						error = KErrGeneral;	
						}
					StopTest(error, EFail);
					break;
					}

				INFO_PRINTF1(_L("5b. Trigger transition from Idle to Execute in component by calling Execute()"));
				error = iDummyComponent->Execute();
				if (error != KErrNone)
					{
					ERR_PRINTF2(_L("	ERROR! - MILComponentIf::Execute() error = %d"), error);
					StopTest(error, EFail);
					break;
					}
				INFO_PRINTF1(_L("	-> Test State: EStatePlayInitialized"));
				iTestState = ETestStateInitialized;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateInitializing did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		case ETestStateInitialized:
			{
			if (aEvent == EEventExecuteComplete)
				{
				INFO_PRINTF1(_L("6a. Check the component was left in the correct state after EEventExecuteComplete event"));
				TILComponentState aState;
				error = iDummyComponent->GetState(aState);
				if ((error != KErrNone) || (aState != EComponentExecuting))
					{
					ERR_PRINTF3(_L("	ERROR! - MILComponentIf::GetState Incorrect state = %d, error = %d."), error, aState);
					if (aState != EComponentExecuting)
						{
						error = KErrGeneral;	
						}
					StopTest(error, EFail);
					break;
					}

				INFO_PRINTF1(_L("6b. Trigger transition from Executing to Pause in component by calling Pause()"));
				error = iDummyComponent->Pause();
				if (error != KErrNone)
					{
					ERR_PRINTF2(_L("	ERROR! - MILComponentIf::Pause() error = %d"), error);
					StopTest(error, EFail);
					break;
					}
				INFO_PRINTF1(_L("	-> Test State: EStatePlayPausing"));
				iTestState = ETestStatePausing;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateInitialized did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		case ETestStatePausing:
			{
			if (aEvent == EEventPauseComplete)
				{
				INFO_PRINTF1(_L("7a. Check the component was left in the correct state after EEventPauseComplete event"));
				TILComponentState aState;
				error = iDummyComponent->GetState(aState);
				if ((error != KErrNone) || (aState != EComponentPaused))
					{
					ERR_PRINTF3(_L("	ERROR! - MILComponentIf::GetState error = %d, state = %d"), error, aState);
					if (aState != EComponentPaused)
						{
						error = KErrGeneral;	
						}
					StopTest(error, EFail);
					break;
					}

				INFO_PRINTF1(_L("7b. Trigger transition from Pause to Idle in component by calling Stop()"));
				error = iDummyComponent->Stop();
				if (error != KErrNone)
					{
					ERR_PRINTF2(_L("	ERROR! - MILComponentIf::Stop() error = %d"), error);
					StopTest(error, EFail);
					break;
					}
				INFO_PRINTF1(_L("	-> Test State: ETestStateStopping"));
				iTestState = ETestStateStopping;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStatePausing did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}
			
		case ETestStateStopping:
			{
			if (aEvent == EEventStopComplete)
				{
				INFO_PRINTF1(_L("8. Check the component was left in the correct state after EEventStopComplete event"));
				TILComponentState aState;
				error = iDummyComponent->GetState(aState);
				if ((error != KErrNone) || (aState != EComponentIdle))
					{
					ERR_PRINTF3(_L("	ERROR! - MILComponentIf::GetState error = %d, state = %d"), error, aState);
					if (aState != EComponentIdle)
						{
						error = KErrGeneral;	
						}
					StopTest(error, EFail);
					break;
					}
				StopTest(KErrNone);
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateStopping did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		default:
			{
			ERR_PRINTF2(_L("	ERROR! - Invalid state received: %d"), iTestState);
			StopTest(KErrGeneral, EFail);
			}
		}
	
	}

/*
 *
 * MsgFromILComponent - 
 *
 */
TInt RTestStepMILIFStates::MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent) 
	{
	TInt error = KErrNone;
	
	if (aComponent != iDummyComponent)
		{
		ERR_PRINTF1(_L("	ERROR! - Invalid callback received"));
		StopTest(KErrGeneral, EFail);
		return KErrGeneral;
		}
	
	switch (aEvent.iEvent)
		{
		case EEventCmdComplete:
			{
			if (aEvent.iData1 == ECommandStateSet)
				{
				if (aEvent.iData2 == EComponentIdle)
					{
					INFO_PRINTF1(_L(" MsgFromILComponent:EEventCmdComplete -> EComponentIdle received"));
					if (iTestState == ETestStateInitializing)
						{
						Fsm(EEventInitComplete, KErrNone); 
						}
					else if (iTestState == ETestStateStopping)
						{
						Fsm(EEventStopComplete, KErrNone); 
						}
					else
						{
						ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
						error = KErrGeneral;
						}
					}
				else if(aEvent.iData2 == EComponentExecuting)
					{
					INFO_PRINTF1(_L(" MsgFromILComponent:EEventCmdComplete -> EComponentExecuting received"));
					if (iTestState == ETestStateInitialized)
						{
						Fsm(EEventExecuteComplete, KErrNone); 
						}
					else
						{
						ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
						error = KErrGeneral;
						}
					}
				else if(aEvent.iData2 == EComponentPaused)
					{
					INFO_PRINTF1(_L(" MsgFromILComponent:EEventCmdComplete -> EComponentPaused received"));
					if (iTestState == ETestStatePausing)
						{
						Fsm(EEventPauseComplete, KErrNone); 
						}
					else
						{
						ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
						error = KErrGeneral;
						}
					}
				else
					{
					ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
					error = KErrGeneral;
					}			
				break;
				}
			}
		default:
			{
			ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
			error = KErrNotSupported;
			}
		}

		if (error != KErrNone)
			{
			StopTest(error, EFail);
			}
		return error;
	}

		
//
// RTestStepMILIFCommands: To test sending commands to the IL component through MILIF.
//

/*
 *
 * RTestStepMILIFCommands - Test step constructor
 *
 */
RTestStepMILIFCommands::RTestStepMILIFCommands(const TDesC& aTestName)
:RTestStepMILIFBase(aTestName)
	{
	}

/*
 *
 * NewL
 *
 */
RTestStepMILIFCommands* RTestStepMILIFCommands::NewL(const TDesC& aTestName)
	{
	RTestStepMILIFCommands* self = new (ELeave) RTestStepMILIFCommands(aTestName);
	return self;
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFCommands::DoKickoffTestL()
	{
	INFO_PRINTF1(_L("RTestStepMILIFCommands: To test sending commands to the IL component through MILIF interface."));
	
	RTestStepMILIFBase::DoKickoffTestL();
	}

/*
 *
 * DoTestBodyL - 
 *
 */
void RTestStepMILIFCommands::DoTestBody()
	{
	Fsm(EEventInit, KErrNone); 
	}
	
/*
 *
 * Fsm	- Executes this test events in sequence
 *
 */
void RTestStepMILIFCommands::Fsm(TTestEvents aEvent, TInt aError)
	{
	TInt error = KErrNone;
	
	switch (iTestState)
		{
		case ETestStateStart:
			{
			if (aEvent == EEventInit)
				{
				INFO_PRINTF1(_L("4. Trigger FlushAllPorts"));
				error = iDummyComponent->FlushAllPorts();
				if (error != KErrNone)
					{
					ERR_PRINTF2(_L("	ERROR! - MILComponentIf::FlushAllPorts error = %d"), error);
					StopTest(error, EFail);
					break;
					}
				INFO_PRINTF1(_L("	-> Test State: ETestStateFlushing"));
				iTestState = ETestStateFlushing;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateFlushing did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		case ETestStateFlushing:
			{
			if (aEvent == EEventFlushComplete)
				{
				INFO_PRINTF1(_L("	Flush Command partially completed."));
				INFO_PRINTF1(_L("	-> Test State: ETestStateFirstPortFlushed"));
				iTestState = ETestStateFirstPortFlushed;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateFlushing did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		case ETestStateFirstPortFlushed:
			{
			if (aEvent == EEventFlushComplete)
				{
				INFO_PRINTF1(_L("	Flush Command completed.")); 
				INFO_PRINTF1(_L("5. Trigger SendCommand() with ECommandPortDisable"));
				TILCommand aCommand;
				aCommand.iCmd = ECommandPortDisable;
				aCommand.iData1 = 0;
				aCommand.iExtraData = NULL;
				error = iDummyComponent->SendCommand(aCommand);

				INFO_PRINTF1(_L("	-> Test State: ETestStateDisablingPort"));
				iTestState = ETestStateDisablingPort;
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateFirstPortFlushed did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		case ETestStateDisablingPort:
			{
			if (aEvent == EEventPortDisableComplete)
				{
				INFO_PRINTF1(_L("	-> Test State: Port Disabled. Fisnished Test"));
				StopTest(KErrNone);
				}
			else
				{
				ERR_PRINTF2(_L("	ERROR! - ETestStateInitialized did not received expected event. Received event: %d"), aEvent);
				StopTest(aError, EFail);
				}
			break;
			}

		default:
			{
			ERR_PRINTF2(_L("	ERROR! - Invalid state received: %d"), iTestState);
			StopTest(KErrGeneral, EFail);
			}
		}
	}

/*
 *
 * MsgFromILComponent - 
 *
 */
TInt RTestStepMILIFCommands::MsgFromILComponent(const MILComponentIf* aComponent, const TILEvent& aEvent) 
	{
	TInt error = KErrNone;

	if (aComponent != iDummyComponent)
		{
		ERR_PRINTF1(_L("	ERROR! - Invalid callback received"));
		StopTest(KErrGeneral, EFail);
		return KErrGeneral;
		}

	switch (aEvent.iEvent)
		{
		case EEventCmdComplete:
			{
			if (aEvent.iData1 == ECommandFlush)
				{
				INFO_PRINTF1(_L(" MsgFromILComponent:EEventCmdComplete -> ECommandFlush received"));
				Fsm(EEventFlushComplete, KErrNone); 
				}
			else if (aEvent.iData1 == ECommandPortDisable)
				{
				INFO_PRINTF1(_L(" MsgFromILComponent:EEventCmdComplete -> ECommandPortDisable received"));
				Fsm(EEventPortDisableComplete, KErrNone); 
				}
			else
				{
				ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
				error = KErrGeneral;
				}			
			break;
			}
		default:
			{
			ERR_PRINTF1(_L("	ERROR! - Unexpected callback received"));
			error = KErrNotSupported;
			}
		}

		if (error != KErrNone)
			{
			StopTest(error, EFail);
			}
		return error;
	}


//
// RTestStepMILIFExtensions: To test configuration of the IL component with extended params through MILIF.
//

/*
 *
 * RTestStepMILIFExtensions - Test step constructor
 *
 */
RTestStepMILIFExtensions::RTestStepMILIFExtensions(const TDesC& aTestName)
:RTestStepMILIFBase(aTestName)
	{
	}

/*
 *
 * NewL
 *
 */
RTestStepMILIFExtensions* RTestStepMILIFExtensions::NewL(const TDesC& aTestName)
	{
	RTestStepMILIFExtensions* self = new (ELeave) RTestStepMILIFExtensions(aTestName);
	return self;
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFExtensions::DoKickoffTestL()
	{
	INFO_PRINTF1(_L("RTestStepMILIFExtensions: To test configuration of the IL component with extended params through MILIF interface."));
	
	RTestStepMILIFBase::DoKickoffTestL();
	}
	
/*
 *
 * DoTestBodyL - 
 *
 */
void RTestStepMILIFExtensions::DoTestBody()
	{

	TInt error = KErrNone;

	INFO_PRINTF1(_L("4. Get Component Input ports through MILIF interface."));
	RPointerArray<MILComponentPortIf> ifPorts;
	error = iDummyComponent->GetComponentInputPorts(ifPorts);		
	if (error == KErrNone && ifPorts.Count() == 1)
		{
		iComponentInPortIf = ifPorts[KZerothPort];
		ifPorts.Close();	
		}
	else
		{
		if (ifPorts.Count() != 1)
			{
			error = KErrNotFound;
			}
		ifPorts.Close();	
		StopTest(error, EFail);
		return;
		}
	
	INFO_PRINTF1(_L("5a. Set a (partial) config port parameter through the interface's extension mechanism."));
	TPortDefinitionType partialConfig_s;
	partialConfig_s.nBufferCountActual = 1;
	TILPortDefinition paramStruct_s(partialConfig_s);
	error = iDummyComponent->SetExtConfig(KExtensionPortDefinitionName, paramStruct_s, iComponentInPortIf);
	if ((iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0005")))
		{
		if (error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Fail to set extension: error = %d"), error);
			StopTest(error, EFail);
			return;
			}
		}
	else
		{
		StopTest(KErrGeneral, EFail);
		}

	INFO_PRINTF1(_L("5b. Get the parameter set in 5a through the interface's extension mechanism."));
	TILPortDefinition paramStruct_g;
	TPortDefinitionType* partialConfig_g = NULL;
	error = iDummyComponent->GetExtConfig(KExtensionPortDefinitionName, paramStruct_g, iComponentInPortIf);
	if ((iTestStepName == _L("MM-MILIF-DUMMYOMXILIF-U-0005")))
		{
		if (error != KErrNone)
			{
			ERR_PRINTF2(_L("	ERROR! - Fail to get TILPortDefinition: error = %d"), error);
			StopTest(error, EFail);
			return;
			}
	 	partialConfig_g = TILPortDefinition::GetStructure(paramStruct_g);		
		if(partialConfig_g->nBufferCountActual != 1)
			{
			ERR_PRINTF1(_L("	ERROR! - Unexpected value. The get value does not match the value set."));
			StopTest(KErrCorrupt, EFail);
			return;
			}
		}
	else
		{
		StopTest(KErrGeneral, EFail);
		return;
		}
		
	StopTest(KErrNone);
	}


//
// RTestStepMILIFComponentRole: To test access to the IL component role through MILIF.
//

/*
 *
 * RTestStepMILIFComponentRole - Test step constructor
 *
 */
RTestStepMILIFComponentRole::RTestStepMILIFComponentRole(const TDesC& aTestName)
:RTestStepMILIFBase(aTestName)
	{
	}

/*
 *
 * NewL
 *
 */
RTestStepMILIFComponentRole* RTestStepMILIFComponentRole::NewL(const TDesC& aTestName)
	{
	RTestStepMILIFComponentRole* self = new (ELeave) RTestStepMILIFComponentRole(aTestName);
	return self;
	}

/*
 *
 * DoKickoffTestL
 *
 */
void RTestStepMILIFComponentRole::DoKickoffTestL()
	{
	INFO_PRINTF1(_L("RTestStepMILIFComponentRole: To test access to the IL component role through MILIF interface."));
	
	RTestStepMILIFBase::DoKickoffTestL();
	}
	
/*
 *
 * DoTestBodyL - 
 *
 */
void RTestStepMILIFComponentRole::DoTestBody()
	{
	TInt error = KErrNone;
	TInt index = 0;

	HBufC8* buf = HBufC8::New(KMaxComponentRoleLength);
	if (buf == NULL)
		{
		StopTest(KErrNoMemory, EFail);
		}
	else
		{
		TPtr8 componentRole = buf->Des();

		INFO_PRINTF1(_L("4. Get component role through MILIF interface."));
		error = iDummyComponent->ComponentRoleEnum(componentRole, index);

		INFO_PRINTF1(_L("5. Check the component's role returned is the expected role."));
		if (error != KErrNone) 
			{
			ERR_PRINTF2(_L("	ERROR! - MILComponentIf::ComponentRoleEnum failed. Error returned = %d"), error);
			StopTest(error, EFail);
			delete buf;
			return;
			}
		else if	(buf->Compare(KFakeRole) != 0 )
			{
			ERR_PRINTF1(_L("	ERROR! - MILComponentIf::ComponentRoleEnum failed. Unexpected role."));
			StopTest(error, EFail);
			delete buf;
			return;
			}
		StopTest(KErrNone);
		
		delete buf;
		}
	}

