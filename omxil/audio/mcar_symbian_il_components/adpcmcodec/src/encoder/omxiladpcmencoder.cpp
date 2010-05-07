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

#include <e32base.h>

#include "log.h"
#include "omxilfsm.h"
#include "omxilcallbackmanager.h"
#include "omxilportmanager.h"
#include "omxilconfigmanager.h"
#include "omxilspecversion.h"

#include "omxiladpcmencoder.h"

#include "omxiladpcmencoder.hrh"
#include <openmax/il/loader/omxilsymbiancomponentif.h>

#include "omxiladpcmcodecport.h"
#include "OmxILPcmPortAdPcmVariant.h"
#include "omxiladpcmcodecprocessingfunction.h"
#include "omxiladpcmcodecprocessingfunctioncorepcmtoadpcm.h"


_LIT8(KSymbianOmxILAdPcmEncoderNameLit, KSymbianOmxILAdPcmEncoderName);
_LIT8(KSymbianOmxILAdPcmEncoderRoleLit, KSymbianOmxILAdPcmEncoderRole);


const TUint8 COmxILAdPcmEncoder::iComponentVersionMajor;
const TUint8 COmxILAdPcmEncoder::iComponentVersionMinor;
const TUint8 COmxILAdPcmEncoder::iComponentVersionRevision;
const TUint8 COmxILAdPcmEncoder::iComponentVersionStep;


OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidSymbianOmxILAdPcmEncoder);


// Component Entry Point
OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE aComponent)
	{
	TInt err = COmxILAdPcmEncoder::CreateComponent(aComponent);

	// This method should be called as a result of a OMX_GetHandle call. Let's
	// return something that is consistent with the return codes allowed for
	// that API call.

	switch(err)
		{
	case KErrNone:
		return OMX_ErrorNone;
	case KErrNoMemory:
		return OMX_ErrorInsufficientResources;
	case KErrArgument:
		return OMX_ErrorBadParameter;
		};

	return OMX_ErrorUndefined;
	}

TInt COmxILAdPcmEncoder::CreateComponent(OMX_HANDLETYPE aComponent)
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmEncoder::CreateComponent"));

    COmxILAdPcmEncoder* self = new COmxILAdPcmEncoder();
	if (!self)
		{
		return KErrNoMemory;
		}

	TRAPD(err, self->ConstructL(aComponent));
	if (err != KErrNone)
		{
		delete self;
		}

	return err;
	}

void COmxILAdPcmEncoder::ConstructL(OMX_HANDLETYPE aComponent)
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmEncoder::ConstructL"));

	// STEP 1: Initialize the data received from the IL Core
	ipHandle	= static_cast<OMX_COMPONENTTYPE*>(aComponent);
	ipAppData	= 0;
	ipCallbacks = 0;

	// STEP 2: Create the call backs manager...
	ipCallbackManager = COmxILCallbackManager::NewL(ipHandle, ipAppData, ipCallbacks);

	// STEP 3: Create the decoder-specific Processing Function...
	ConstructPortsL();//this has to be called before iCore is constructed 
	iCore = COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm::NewL( *ipb0Port, *ipb1Port);
	ipProcessingFunction = COmxILAdPcmCodecProcessingFunction::NewL(iCore, *ipCallbackManager);


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

	

	// ..  and add them to the port manager...
	User::LeaveIfError( AddPorts() );

	// STEP 5: Create the non-port related configuration manager...
	RPointerArray<TDesC8> componentRoles;
	CleanupClosePushL(componentRoles);
	User::LeaveIfError(componentRoles.Append(&KSymbianOmxILAdPcmEncoderRoleLit()));

	ipConfigManager = COmxILConfigManager::NewL(
		*ipPortManager,
		KSymbianOmxILAdPcmEncoderNameLit,
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

TInt COmxILAdPcmEncoder::AddPorts()
	{
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
	return err;
	}

COmxILAdPcmEncoder::COmxILAdPcmEncoder()
	: iOmxILVersion(TOmxILSpecVersion())
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmEncoder::COmxILAdPcmEncoder"));
	}

COmxILAdPcmEncoder::~COmxILAdPcmEncoder()
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmEncoder::~COmxILAdPcmEncoder"));

	delete ipProcessingFunction;
	delete ipPortManager;
	delete ipb0Port;
	delete ipb1Port;
	delete ipConfigManager;
	delete ipFsm;
	delete ipCallbackManager;
	delete iCore;
	}


void COmxILAdPcmEncoder::ConstructPortsL()
	{
	//STEP 1. Input pcm port
	ConstructPcmPortL( ipb0Port, OMX_DirInput, 0 );
	//STEP 2. Output adpcm port
	ConstructAdPcmPortL( ipb1Port, OMX_DirOutput, 1 );
	}

void COmxILAdPcmEncoder::ConstructPcmPortL( COmxILPcmPortAdPcmVariant*& aPort, OMX_DIRTYPE aDirection, TInt aPortIndex )
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmEncoder::ConstructPcmPortL"));
    
	OMX_AUDIO_PARAM_PCMMODETYPE		profile =
	{
		sizeof profile,				/**< Size of this structure, in Bytes */ 
		iOmxILVersion,				/**< OMX specification version information */ 
		aPortIndex,					/**< port that this structure applies to */ 
		1,							/**< Number of channels (e.g. 2 for stereo) */
		OMX_NumericalDataSigned,	/**< indicates PCM data as signed or unsigned */ 
		OMX_EndianLittle,			/**< indicates PCM data as little or big endian */ 
		OMX_TRUE,					/**< True for normal interleaved data; false for 
			                                           non-interleaved data (e.g. block data) */ 
		16,							/**< Bit per sample */ 
		8000,						/**< Sampling rate of the source data.  Use 0 for 
			                                           variable or unknown sampling rate. */
		OMX_AUDIO_PCMModeLinear,	/**< PCM mode enumeration */ 
		
		{ OMX_AUDIO_ChannelLF, OMX_AUDIO_ChannelRF }		// all other entries will be 0 due to aggregate initialisation rules (C++98, 8.5.1 clause 7)
	};
	
	const OMX_BUFFERSUPPLIERTYPE bufferSupplydirection = ( aDirection == OMX_DirInput ) ? OMX_BufferSupplyInput : OMX_BufferSupplyOutput;

	TOmxILCommonPortData portdata(
			iOmxILVersion,     		 // OMX specification version information
			aPortIndex,				// Port number the structure applies to
			aDirection,				// Direction of this port
			1,						// The minimum number of buffers this port requires
			4096,					// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainAudio,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,						// Buffer aligment requirements
			bufferSupplydirection,	// supplier preference when tunneling between two ports
			aPortIndex == 1 ? 1 : 0 // This is the index of the compoent's port that will
									// propagate buffer marks received in this port
			);

    RArray<OMX_AUDIO_CODINGTYPE> supportedOutputFormats;
    CleanupClosePushL(supportedOutputFormats);
    supportedOutputFormats.AppendL(OMX_AUDIO_CodingPCM);
    
    aPort = COmxILPcmPortAdPcmVariant::NewL( portdata,supportedOutputFormats,profile );
	CleanupStack::PopAndDestroy(&supportedOutputFormats); 
	}


void COmxILAdPcmEncoder::ConstructAdPcmPortL( COmxILAdPcmCodecPort*& aPort, OMX_DIRTYPE aDirection, TInt aPortIndex )
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmEncoder::ConstructAdPcmPortL"));

	// profile and port data for input port at index 0
	OMX_AUDIO_PARAM_ADPCMTYPE profile = { 
	    sizeof profile, 		//OMX_U32 nSize;              /**< size of the structure in bytes */ 
		iOmxILVersion,			//OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */ 
		aPortIndex,				//OMX_U32 nPortIndex;         /**< port that this structure applies to */ 
		1,						//OMX_U32 nChannels;          /**< Number of channels in the data stream (not 
								//                                 necessarily the same as the number of channels 
								//                                 to be rendered. */ 
		4,//always 4 for adpcm  //OMX_U32 nBitsPerSample;     /**< Number of bits in each sample */ 
		8000,					//OMX_U32 nSampleRate;        /**< Sampling rate of the source data.  Use 0 for 
								//                                variable or unknown sampling rate. */ 
	};
	
	const OMX_BUFFERSUPPLIERTYPE bufferSupplydirection = ( aDirection == OMX_DirInput ) ? OMX_BufferSupplyInput : OMX_BufferSupplyOutput;
	
	TOmxILCommonPortData portdata(
			iOmxILVersion,     		 // OMX specification version information
			aPortIndex,						// Port number the structure applies to
			aDirection,				// Direction of this port
			1,						// The minimum number of buffers this port requires
			22272,					// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainAudio,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,						// Buffer aligment requirements
			bufferSupplydirection,	// supplier preference when tunneling between two ports
			aPortIndex == 1 ? 0 : 1	// This is the index of the compoent's port that will
									// propagate buffer marks received in this port
			);
	
	RArray<OMX_AUDIO_CODINGTYPE> supportedInputFormats;
	CleanupClosePushL(supportedInputFormats);
	supportedInputFormats.AppendL(OMX_AUDIO_CodingADPCM);
	
	aPort = COmxILAdPcmCodecPort::NewL(portdata, supportedInputFormats, profile);
	CleanupStack::PopAndDestroy(&supportedInputFormats);
	}
