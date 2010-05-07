/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>

#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/log.h>
#include <openmax/il/common/omxilfsm.h>
#include <openmax/il/common/omxilcallbackmanager.h>
#include <openmax/il/common/omxilportmanager.h>
#include <openmax/il/common/omxilconfigmanager.h>
#include <openmax/il/common/omxilspecversion.h>
#else
#include "log.h"
#include "omxilfsm.h"
#include "omxilcallbackmanager.h"
#include "omxilportmanager.h"
#include "omxilconfigmanager.h"
#include "omxilspecversion.h"
#endif

#include "omxilpcmprocessor.h"

#include "omxilpcmprocessor.hrh"
#include <openmax/il/loader/omxilsymbiancomponentif.h>

#include "omxilpcmprocessorport.h"
#include "omxilpcmprocessorprocessingfunction.h"



const TUint8 COmxILPcmProcessor::iComponentVersionMajor;
const TUint8 COmxILPcmProcessor::iComponentVersionMinor;
const TUint8 COmxILPcmProcessor::iComponentVersionRevision;
const TUint8 COmxILPcmProcessor::iComponentVersionStep;

TInt COmxILPcmProcessor::CreateComponent(OMX_HANDLETYPE aComponent, const TDesC8& aComponentName, const TDesC8& aComponentRole)
	{
    DEBUG_PRINTF(_L8("COmxILPcmProcessor::CreateComponent"));

    COmxILPcmProcessor* self = new COmxILPcmProcessor();
	if (!self)
		{
		return KErrNoMemory;
		}

	TRAPD(err, self->ConstructL(aComponent, aComponentName, aComponentRole));
	if (err != KErrNone)
		{
		delete self;
		}

	return err;
	}

void COmxILPcmProcessor::ConstructL(OMX_HANDLETYPE aComponent, const TDesC8& aComponentName, const TDesC8& aComponentRole)
	{
    DEBUG_PRINTF(_L8("COmxILPcmProcessor::ConstructL"));

	// STEP 1: Initialize the data received from the IL Core
	ipHandle	= static_cast<OMX_COMPONENTTYPE*>(aComponent);
	ipAppData	= 0;
	ipCallbacks = 0;

	// STEP 2: Create the call backs manager...
	ipCallbackManager = COmxILCallbackManager::NewL(ipHandle, ipAppData, ipCallbacks);

	// STEP 3: Create the decoder-specific Processing Function...
	ipProcessingFunction = COmxILPcmProcessorProcessingFunction::NewL(*ipCallbackManager);

	// STEP 4: Create Port manager...
	ipPortManager = COmxILPortManager::NewL(
		*ipProcessingFunction,	// The component's processing function
		*ipCallbackManager,     // The call back manager object
		iOmxILVersion,	        // OMX Version
		2,						// The number of audio ports in this component
		0,						// The starting audio port index
		0,						// The number of image ports in this component
		0,						// The starting image port index
		0,						// The number of video ports in this component
		0,						// The starting video port index
		0,						// The number of other ports in this component
		0						// The starting other port index
		);

	// ... create the decoder component ports...
	ConstructPortsL();

	// ..  and add them to the port manager...
	// TODO: remove workaround when unnecessary - BUG IN FRAMEWORK
	// Workaround: in some cases, AddPort returns KErrGeneral in OOM cases. In order to get OOM tests for this component to run
	// 		successfully, leave with OOM if KErrGeneral is returned.
	TInt err = ipPortManager->AddPort(ipb0Port, OMX_DirInput);
	if (!err)
		{
		err = ipPortManager->AddPort(ipb1Port, OMX_DirOutput);
		}
	
	if (err==KErrGeneral) 
		{
		err= KErrNoMemory;
		}
	User::LeaveIfError(err);

	// STEP 5: Create the non-port related configuration manager...
	RPointerArray<TDesC8> componentRoles;
	CleanupClosePushL(componentRoles);
	User::LeaveIfError(componentRoles.Append(&aComponentRole));

	ipConfigManager = COmxILConfigManager::NewL(
		*ipPortManager,
		aComponentName,
		TOmxILVersion(iComponentVersionMajor,
					  iComponentVersionMinor,
					  iComponentVersionRevision,
					  iComponentVersionStep),
		componentRoles);

	CleanupStack::Pop();	// componentRoles
	componentRoles.Close(); // Must not destroy pointers

	// STEP 6: Create the FSM object...
	ipFsm = COmxILFsm::NewL(*this,
							*ipProcessingFunction,
							*ipPortManager,
							*ipConfigManager,
							*ipCallbackManager);

	// And finally, let's get everything started
	InitComponentL();
	}

COmxILPcmProcessor::COmxILPcmProcessor()
	: iOmxILVersion(TOmxILSpecVersion())
	{
    DEBUG_PRINTF(_L8("COmxILPcmProcessor::COmxILPcmProcessor"));
	}

COmxILPcmProcessor::~COmxILPcmProcessor()
	{
    DEBUG_PRINTF(_L8("COmxILPcmProcessor::~COmxILPcmProcessor"));

	delete ipProcessingFunction;
	delete ipPortManager;
	delete ipb0Port;
	delete ipb1Port;
	delete ipConfigManager;
	delete ipFsm;
	delete ipCallbackManager;
	}


void COmxILPcmProcessor::ConstructPortsL()
	{
    DEBUG_PRINTF(_L8("COmxILPcmProcessor::ConstructPortsL"));


	// profile and port data for input port at index 0
	OMX_AUDIO_PARAM_PCMMODETYPE		profile =
	{
		sizeof profile,				/**< Size of this structure, in Bytes */ 
		iOmxILVersion,				/**< OMX specification version information */ 
		0,		/* !!! changed for port 1 */
									/**< port that this structure applies to */ 
		2,							/**< Number of channels (e.g. 2 for stereo) */
		OMX_NumericalDataSigned,	/**< indicates PCM data as signed or unsigned */ 
		OMX_EndianLittle,			/**< indicates PCM data as little or big endian */ 
		OMX_TRUE,					/**< True for normal interleaved data; false for 
			                                           non-interleaved data (e.g. block data) */ 
		16,							/**< Bit per sample */ 
		48000,						/**< Sampling rate of the source data.  Use 0 for 
			                                           variable or unknown sampling rate. */
		OMX_AUDIO_PCMModeLinear,	/**< PCM mode enumeration */ 
		
		{ OMX_AUDIO_ChannelLF, OMX_AUDIO_ChannelRF }		// all other entries will be 0 due to aggregate initialisation rules (C++98, 8.5.1 clause 7)
	};

	TOmxILCommonPortData port0data(
			iOmxILVersion,     		 // OMX specification version information
			0,						// Port number the structure applies to
			OMX_DirInput,			// Direction of this port
			1,						// The minimum number of buffers this port requires
			2048,					// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainAudio,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,						// Buffer aligment requirements
			OMX_BufferSupplyUnspecified,	// supplier preference when tunneling between two ports
			1						// This is the index of the compoent's port that will
									// propagate buffer marks received in this port
			);


    // create the supported format list for port 0
	RArray<OMX_AUDIO_CODINGTYPE> supportedAudioFormats0;
	CleanupClosePushL(supportedAudioFormats0);
	supportedAudioFormats0.AppendL(OMX_AUDIO_CodingPCM);
	
	// create port 0
	ipb0Port = COmxILPcmProcessorPort::NewL(
		port0data,
		supportedAudioFormats0,
		profile
		);
	
	CleanupStack::PopAndDestroy(&supportedAudioFormats0);
	
	// update the profile and port data for 
	TOmxILCommonPortData port1data(
			iOmxILVersion,  	   	 // OMX specification version information
			1,						// Port number the structure applies to
			OMX_DirOutput,			// Direction of this port
			1,						// The minimum number of buffers this port requires
			2048,					// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainAudio,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,						// Buffer aligment requirements
			OMX_BufferSupplyUnspecified,	// supplier preference when tunneling between two ports
			// This component is not a source component so there's no need
			// propagate buffer marks received on output ports
			COmxILPort:: KBufferMarkPropagationPortNotNeeded
			);
	
    // create the supported format list for port 1
	RArray<OMX_AUDIO_CODINGTYPE> supportedAudioFormats1;
	CleanupClosePushL(supportedAudioFormats1);
	supportedAudioFormats1.AppendL(OMX_AUDIO_CodingPCM);

	// adjust profile and create port 1
	profile.nPortIndex = 1;
	ipb1Port = COmxILPcmProcessorPort::NewL(
		port1data,
		supportedAudioFormats1,
		profile
		);
	
	CleanupStack::PopAndDestroy(&supportedAudioFormats1);
	}
