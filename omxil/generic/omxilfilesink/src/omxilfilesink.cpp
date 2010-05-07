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
 * @file
 * @internalTechnology
 */

#include "omxilport.h"

#include "omxilfsm.h"
#include "omxilportmanager.h"
#include "omxilcallbackmanager.h"
#include "omxilspecversion.h"
#include <openmax/il/loader/omxilsymbiancomponentif.h>

#include "omxilfilesink.h"
#include "omxilfilesinkprocessingfunction.h"
#include "omxilfilesinkconfigmanager.h"
#include "omxilfilesink.hrh"


#ifdef OMXIL_AUDIO_FILESINK
#include "omxilaudiofilesinkopb0port.h"
_LIT8(KNokiaOMXFileSinkComponentName, "OMX.NOKIA.AUDIO.FILESINK");
_LIT8(KNokiaOMXFileSinkRole, "audio_writer.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILAudioFileSink);

#elif defined(OMXIL_VIDEO_FILESINK)
#include "omxilvideofilesinkopb0port.h"
_LIT8(KNokiaOMXFileSinkComponentName, "OMX.NOKIA.VIDEO.FILESINK");
_LIT8(KNokiaOMXFileSinkRole, "video_writer.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILVideoFileSink);

#elif defined(OMXIL_IMAGE_FILESINK)
#include "omxilimagefilesinkopb0port.h"
_LIT8(KNokiaOMXFileSinkComponentName, "OMX.NOKIA.IMAGE.FILESINK");
_LIT8(KNokiaOMXFileSinkRole, "image_writer.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILImageFileSink);

#elif defined(OMXIL_OTHER_FILESINK)
#include "omxilotherfilesinkopb0port.h"
_LIT8(KNokiaOMXFileSinkComponentName, "OMX.NOKIA.OTHER.FILESINK");
_LIT8(KNokiaOMXFileSinkRole, "other_writer.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidSymbianOmxILOtherFileSink);

#endif

const TUint8 KComponentVersionMajor = 1;
const TUint8 KComponentVersionMinor = 1;
const TUint8 KComponentVersionRevision = 0;
const TUint8 KComponentVersionStep = 0;

static const TInt KMinBuffers = 1;
static const TInt KMinBufferSize = 15360;


// Component Entry Point
OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE aComponent)
	{
	TRAPD(err, COmxILFileSink::CreateComponentL(aComponent));
	if (err == KErrNone)
		{
		return OMX_ErrorNone;
		}
	else
		{
		return err == KErrNoMemory ? OMX_ErrorInsufficientResources : OMX_ErrorUndefined;
		}
	}

void COmxILFileSink::CreateComponentL(OMX_HANDLETYPE aComponent)
	{
	COmxILFileSink* self = new (ELeave) COmxILFileSink();
	CleanupStack::PushL(self);
	self->ConstructL(aComponent);
	CleanupStack::Pop(self);
	}

COmxILFileSink::COmxILFileSink()
	{
	// nothing to do
	}

COmxILFileSink::~COmxILFileSink()
	{
	delete ipCallbackManager;
	delete ipProcessingFunction;
	delete ipPortManager;
	delete iOPB0Port;
	delete ipConfigManager;
	delete ipFsm;
	}

void COmxILFileSink::ConstructL(OMX_HANDLETYPE aComponent)
	{
	// STEP 1: Initialize the data received from the IL Core
	ipHandle	= static_cast<OMX_COMPONENTTYPE*>(aComponent);
	ipAppData	= 0;
	ipCallbacks = 0;

	// STEP 2: Create the call backs manager...
	ipCallbackManager = COmxILCallbackManager::NewL(ipHandle, ipAppData, ipCallbacks);

	// STEP 3: Create the file sink-specific Processing Function...
	ipProcessingFunction = COmxILFileSinkProcessingFunction::NewL(*ipCallbackManager);

	// STEP 4: Create Port manager...

#ifdef OMXIL_AUDIO_FILESINK
    ipPortManager = COmxILPortManager::NewL(
        *ipProcessingFunction,  // The component's processing function
        *ipCallbackManager,     // The call back manager object
        TOmxILSpecVersion(),    // OMX Version
        1,                      // The number of audio ports in this component
        0,                      // The starting audio port index
        0,                      // The number of image ports in this component
        0,                      // The starting image port index
        0,                      // The number of video ports in this component
        0,                      // The starting video port index
        0,                      // The number of other ports in this component
        0                       // The starting other port index
        );

#elif defined(OMXIL_VIDEO_FILESINK)
    ipPortManager = COmxILPortManager::NewL(
        *ipProcessingFunction,  // The component's processing function
        *ipCallbackManager,     // The call back manager object
        TOmxILSpecVersion(),    // OMX Version
        0,                      // The number of audio ports in this component
        0,                      // The starting audio port index
        0,                      // The number of image ports in this component
        0,                      // The starting image port index
        1,                      // The number of video ports in this component
        0,                      // The starting video port index
        0,                      // The number of other ports in this component
        0                       // The starting other port index
        );

#elif defined(OMXIL_IMAGE_FILESINK)
    ipPortManager = COmxILPortManager::NewL(
        *ipProcessingFunction,  // The component's processing function
        *ipCallbackManager,     // The call back manager object
        TOmxILSpecVersion(),    // OMX Version
        0,                      // The number of audio ports in this component
        0,                      // The starting audio port index
        1,                      // The number of image ports in this component
        0,                      // The starting image port index
        0,                      // The number of video ports in this component
        0,                      // The starting video port index
        0,                      // The number of other ports in this component
        0                       // The starting other port index
        );


#elif defined(OMXIL_OTHER_FILESINK)
    ipPortManager = COmxILPortManager::NewL(
        *ipProcessingFunction,  // The component's processing function
        *ipCallbackManager,     // The call back manager object
        TOmxILSpecVersion(),    // OMX Version
        0,                      // The number of audio ports in this component
        0,                      // The starting audio port index
        0,                      // The number of image ports in this component
        0,                      // The starting image port index
        0,                      // The number of video ports in this component
        0,                      // The starting video port index
        1,                      // The number of other ports in this component
        0                       // The starting other port index
        );
#endif	
	
	// STEP 5: Create the File Sink component port...
	iOPB0Port = ConstructOPB0PortL();
	
	// STEP 6: Add to the port manager...
	User::LeaveIfError(ipPortManager->AddPort(iOPB0Port, OMX_DirInput));
	
	// STEP 7: Create the non-port related configuration manager...
	RPointerArray<TDesC8> componentRoles;
	CleanupClosePushL(componentRoles);
	
	componentRoles.AppendL(&KNokiaOMXFileSinkRole);
	ipConfigManager = COmxILFileSinkConfigManager::NewL(
		*ipPortManager,
		KNokiaOMXFileSinkComponentName,
		TOmxILVersion(KComponentVersionMajor,
					  KComponentVersionMinor,
					  KComponentVersionRevision,
					  KComponentVersionStep),
		componentRoles,
		*ipProcessingFunction);

	CleanupStack::PopAndDestroy(&componentRoles);

	// STEP 8: Create the FSM object...
	ipFsm = COmxILFsm::NewL(*this, *ipProcessingFunction, *ipPortManager, *ipConfigManager, *ipCallbackManager);

	// STEP 9: Finally, let's get everything started
	InitComponentL();
	}

COmxILPort* COmxILFileSink::ConstructOPB0PortL() const
	{
	OMX_U32 thisPortIndex = 0;
	//const TUint32 KBufferAlignment = 4;
#ifdef OMXIL_AUDIO_FILESINK
	RArray<OMX_AUDIO_CODINGTYPE> supportedAudioFormats;
	CleanupClosePushL(supportedAudioFormats);
	supportedAudioFormats.AppendL(OMX_AUDIO_CodingUnused);
	COmxILAudioFileSinkOPB0Port* opb0Port = COmxILAudioFileSinkOPB0Port::NewL(
			TOmxILCommonPortData (
			TOmxILSpecVersion(),	// OMX specification version information
			thisPortIndex, 			// Port number the structure applies to
			OMX_DirInput, 			// Direction of this port
			KMinBuffers,			// The minimum number of buffers this port requires
			KMinBufferSize,			// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainAudio,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,		                // Buffer aligment requirements
			OMX_BufferSupplyOutput,	// supplier preference when tunneling between two ports
			COmxILPort::KBufferMarkPropagationPortNotNeeded),
			supportedAudioFormats,
			*ipProcessingFunction);
	CleanupStack::PopAndDestroy(&supportedAudioFormats);
	return opb0Port;
	
#elif defined(OMXIL_VIDEO_FILESINK)
    RArray<OMX_VIDEO_CODINGTYPE> supportedVideoFormats;
    CleanupClosePushL(supportedVideoFormats);
    RArray<OMX_COLOR_FORMATTYPE> supportedColourFormats;
    CleanupClosePushL(supportedColourFormats);
    COmxILVideoFileSinkOPB0Port* opb0Port = COmxILVideoFileSinkOPB0Port::NewL(
            TOmxILCommonPortData (
            TOmxILSpecVersion(),    // OMX specification version information
            thisPortIndex,          // Port number the structure applies to
            OMX_DirInput,           // Direction of this port
            KMinBuffers,            // The minimum number of buffers this port requires
            KMinBufferSize,         // Minimum size, in bytes, for buffers to be used for this port
            OMX_PortDomainVideo,    // Domain of the port
            OMX_FALSE,              // Buffers contiguous requirement (true or false)
            0,                      // Buffer aligment requirements
            OMX_BufferSupplyOutput, // supplier preference when tunneling between two ports
            COmxILPort::KBufferMarkPropagationPortNotNeeded),
            supportedVideoFormats,
            supportedColourFormats,
            *ipProcessingFunction);
    CleanupStack::PopAndDestroy(2);
    return opb0Port;
    
#elif defined(OMXIL_IMAGE_FILESINK)
    RArray<OMX_IMAGE_CODINGTYPE> supportedImageFormats;
    CleanupClosePushL(supportedImageFormats);
    RArray<OMX_COLOR_FORMATTYPE> supportedColourFormats;
    CleanupClosePushL(supportedColourFormats);
    COmxILImageFileSinkOPB0Port* opb0Port = COmxILImageFileSinkOPB0Port::NewL(
            TOmxILCommonPortData (
            TOmxILSpecVersion(),    // OMX specification version information
            thisPortIndex,          // Port number the structure applies to
            OMX_DirInput,           // Direction of this port
            KMinBuffers,            // The minimum number of buffers this port requires
            KMinBufferSize,         // Minimum size, in bytes, for buffers to be used for this port
            OMX_PortDomainImage,    // Domain of the port
            OMX_FALSE,              // Buffers contiguous requirement (true or false)
            0,                      // Buffer aligment requirements
            OMX_BufferSupplyOutput, // supplier preference when tunneling between two ports
            COmxILPort::KBufferMarkPropagationPortNotNeeded),
            supportedImageFormats,
            supportedColourFormats,
            *ipProcessingFunction);
    CleanupStack::PopAndDestroy(2);
    return opb0Port;

#elif defined(OMXIL_OTHER_FILESINK)
	RArray<OMX_OTHER_FORMATTYPE> supportedOtherFormats;
	CleanupClosePushL(supportedOtherFormats);
	supportedOtherFormats.AppendL(OMX_OTHER_FormatBinary);
	COmxILOtherFileSinkOPB0Port* opb0Port = COmxILOtherFileSinkOPB0Port::NewL(
			TOmxILCommonPortData (
			TOmxILSpecVersion(),	// OMX specification version information
			thisPortIndex, 			// Port number the structure applies to
			OMX_DirInput, 			// Direction of this port
			KMinBuffers,			// The minimum number of buffers this port requires
			KMinBufferSize,			// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainOther,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,		                // Buffer aligment requirements
			OMX_BufferSupplyOutput,	// supplier preference when tunneling between two ports
			COmxILPort::KBufferMarkPropagationPortNotNeeded),
			supportedOtherFormats,
			*ipProcessingFunction);
	CleanupStack::PopAndDestroy(&supportedOtherFormats);
	return opb0Port;
	
#endif
	}
