/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


/**
@file
@internalComponent
*/

#include "omxilincontextcallbackmanager.h"
#include "omxilportmanager.h"
#include "omxilspecversion.h"
#include "omxilfsm.h"
#include <openmax/il/loader/omxilcomponentif.h>
#include <openmax/il/loader/omxilsymbiancomponentif.h>

#include "comxilclockcomponent.h"
#include "comxilclockoutputport.h"
#include "comxilclockprocessingfunction.h"
#include "comxilclockconfigmanager.h"
#include "clockpanics.h"
#include "omxilclock.hrh"

_LIT8(KSymbianOmxILClockNameDes, KCompNameSymbianOmxILClock);
_LIT8(KSymbianOmxILClockRoleDes, KRoleSymbianOmxILClock);

OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidSymbianOmxILClock);

OMX_ERRORTYPE SymbianErrorToOmx(TInt aError);

// Component Entry Point
OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE aComponent)
	{
	TInt err = COmxILClockComponent::CreateComponent(aComponent);
	return SymbianErrorToOmx(err);
	}

TInt COmxILClockComponent::CreateComponent(OMX_HANDLETYPE hComponent)
	{
	COmxILClockComponent* self = new COmxILClockComponent();

	if (!self)
		{
		return KErrNoMemory;
		}

	TRAPD(error, self->ConstructL(hComponent));
	if(error != KErrNone)
		{
		delete self;
		}
	return error;
	}

COmxILClockComponent::COmxILClockComponent()
	{
	// nothing to do
	}

void COmxILClockComponent::ConstructL(OMX_HANDLETYPE hComponent)
	{
	ipHandle = static_cast<OMX_COMPONENTTYPE*>(hComponent);

	// use synchronous callback manager since BufferDoneNotifications must be serviced at precise times 
	ipCallbackManager = COmxILInContextCallbackManager::NewL(ipHandle, ipAppData, ipCallbacks);

	ipProcessingFunction = COmxILClockProcessingFunction::NewL(*ipCallbackManager, *this);

	ipPortManager = COmxILPortManager::NewL(
		*ipProcessingFunction,	// The component's processing function
		*ipCallbackManager,     // The call back manager object
		TOmxILSpecVersion(),	        // OMX Version
		0,						// The number of audio ports in this component
		0,						// The starting audio port index
		0,						// The number of image ports in this component
		0,						// The starting image port index
		0,						// The number of video ports in this component
		0,						// The starting video port index
		KNumPorts,				// The number of other ports in this component
		0,						// The starting other port index
		OMX_FALSE				// Process the time port buffer as usual in port manager
		);


	RPointerArray<TDesC8> roleList;
	CleanupClosePushL(roleList);
	roleList.AppendL(&KSymbianOmxILClockRoleDes);
	COmxILClockConfigManager* configManager = COmxILClockConfigManager::NewL(*ipPortManager, KSymbianOmxILClockNameDes, 
							TOmxILSpecVersion(), roleList, *(static_cast<COmxILClockProcessingFunction*>(ipProcessingFunction)));
	ipConfigManager = configManager;
	CleanupStack::PopAndDestroy(&roleList);
	
	for(TInt portIndex = 0; portIndex < KNumPorts; portIndex++)
		{
		AddPortL();
		}

	ipFsm = COmxILFsm::NewL(*this, *ipProcessingFunction, *ipPortManager, *ipConfigManager, *ipCallbackManager);

	InitComponentL();
	}

COmxILClockComponent::~COmxILClockComponent()
	{
	delete ipConfigManager;
	delete ipProcessingFunction;
	delete ipPortManager;
	delete ipFsm;
	delete ipCallbackManager;
	iPorts.ResetAndDestroy();
	}
	
void COmxILClockComponent::AddPortL()
	{
	TOmxILSpecVersion omxIlVersion;
	TOmxILCommonPortData portData(
			omxIlVersion, 
			iPorts.Count(),										// port index
			OMX_DirOutput,
			4,													// minimum number of buffers
			sizeof(OMX_TIME_MEDIATIMETYPE),						// minimum buffer size, in bytes
			OMX_PortDomainOther,
			OMX_FALSE,											// do not need contigious buffers
			1,													// byte alignment
			// Clock being buffer supplier allows it to send notifications to clients without
			// waiting for clients to pass their buffers after a state transition
			OMX_BufferSupplyOutput,
			COmxILPort::KBufferMarkPropagationPortNotNeeded		
			);

	RArray<OMX_OTHER_FORMATTYPE> supportedOtherFormats;

	CleanupClosePushL(supportedOtherFormats);
	supportedOtherFormats.AppendL(OMX_OTHER_FormatTime);

	COmxILClockOutputPort* port = COmxILClockOutputPort::NewL(portData, supportedOtherFormats,
															*(static_cast<COmxILClockProcessingFunction*>(ipProcessingFunction)));

	CleanupStack::PushL(port);
	iPorts.AppendL(port);
	CleanupStack::Pop(port);
	
	CleanupStack::PopAndDestroy(&supportedOtherFormats);
	User::LeaveIfError(ipPortManager->AddPort(port, OMX_DirOutput));
	}

/**
 * Returns true iff the specified port is currently enabled.
 */
TBool COmxILClockComponent::PortEnabled(TInt aPortIndex) const
	{
	return iPorts[aPortIndex]->IsEnabled();
	}

/**
 * Returns the number of buffers as configured in the port definition.
 */
TInt COmxILClockComponent::PortBufferCount(TInt aPortIndex) const
	{
	return iPorts[aPortIndex]->BufferCount();
	}

OMX_ERRORTYPE SymbianErrorToOmx(TInt aError)
	{
	switch(aError)
		{
	case KErrNone:
		return OMX_ErrorNone;
	case KErrNoMemory:
		return OMX_ErrorInsufficientResources;
	case KErrArgument:
		return OMX_ErrorUnsupportedSetting;
	case KErrNotSupported:
		return OMX_ErrorUnsupportedIndex;
	case KErrNotReady:
		return OMX_ErrorIncorrectStateOperation;
	default:
#ifdef _DEBUG
		// Panic in a debug build. It will make people think about how the error should be handled.
		Panic(EUndefinedErrorCode);
#endif
		return OMX_ErrorUndefined;
		}
	}
