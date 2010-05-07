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

#include "omxilfilesource.h"
#include "omxilotherfilesourceopb0port.h"
#include "omxilfilesourceprocessingfunction.h"
#include "omxilfilesourceconfigmanager.h"
#include "omxilfilesource.hrh"

#ifdef OMXIL_AUDIO_FILESOURCE
#include "omxilaudiofilesourceopb0port.h"
_LIT8(KNokiaOMXFileSourceComponentName, "OMX.NOKIA.AUDIO.FILESOURCE");
_LIT8(KNokiaOMXFileSourceRole, "audio_reader.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILAudioFileSource);

#elif defined(OMXIL_VIDEO_FILESOURCE)
#include "omxilvideofilesourceopb0port.h"
_LIT8(KNokiaOMXFileSourceComponentName, "OMX.NOKIA.VIDEO.FILESOURCE");
_LIT8(KNokiaOMXFileSourceRole, "video_reader.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILVideoFileSource);

#elif defined(OMXIL_IMAGE_FILESOURCE)
#include "omxilimagefilesourceopb0port.h"
_LIT8(KNokiaOMXFileSourceComponentName, "OMX.NOKIA.IMAGE.FILESOURCE");
_LIT8(KNokiaOMXFileSourceRole, "image_reader.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILImageFileSource);

#elif defined(OMXIL_OTHER_FILESOURCE)
#include "omxilotherfilesourceopb0port.h"
_LIT8(KNokiaOMXFileSourceComponentName, "OMX.NOKIA.OTHER.FILESOURCE");
_LIT8(KNokiaOMXFileSourceRole, "other_reader.binary");
OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidNokiaOmxILOtherFileSource);

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
	TRAPD(err, COmxILFileSource::CreateComponentL(aComponent));
	if (err == KErrNone)
		{
		return OMX_ErrorNone;
		}
	else
		{
		return err == KErrNoMemory ? OMX_ErrorInsufficientResources : OMX_ErrorUndefined;
		}
	}

void COmxILFileSource::CreateComponentL(OMX_HANDLETYPE aComponent)
	{
	COmxILFileSource* self = new (ELeave) COmxILFileSource();
	CleanupStack::PushL(self);
	self->ConstructL(aComponent);
	CleanupStack::Pop(self);
	}

COmxILFileSource::COmxILFileSource()
	{
	// nothing to do
	}

COmxILFileSource::~COmxILFileSource()
	{
	delete ipCallbackManager;
	delete ipProcessingFunction;
	delete ipPortManager;
	delete iOPB0Port;
	delete ipConfigManager;
	delete ipFsm;
	}

void COmxILFileSource::ConstructL(OMX_HANDLETYPE aComponent)
	{
	// STEP 1: Initialize the data received from the IL Core
	ipHandle	= static_cast<OMX_COMPONENTTYPE*>(aComponent);
	ipAppData	= 0;
	ipCallbacks = 0;

	// STEP 2: Create the call backs manager...
	ipCallbackManager = COmxILCallbackManager::NewL(ipHandle, ipAppData, ipCallbacks);

	// STEP 3: Create the file source-specific Processing Function...
	ipProcessingFunction = COmxILFileSourceProcessingFunction::NewL(*ipCallbackManager);

	// STEP 4: Create Port manager...

#ifdef OMXIL_AUDIO_FILESOURCE
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

#elif defined(OMXIL_VIDEO_FILESOURCE)
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

#elif defined(OMXIL_IMAGE_FILESOURCE)
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


#elif defined(OMXIL_OTHER_FILESOURCE)
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

	// STEP 5: Create the File Source component port...
	iOPB0Port = ConstructOPB0PortL();

	// STEP 6: Add to the port manager...
	User::LeaveIfError(ipPortManager->AddPort(iOPB0Port, OMX_DirOutput));

	// STEP 7: Create the non-port related configuration manager...
	RPointerArray<TDesC8> componentRoles;
	CleanupClosePushL(componentRoles);
	componentRoles.AppendL(&KNokiaOMXFileSourceRole);
	ipConfigManager = COmxILFileSourceConfigManager::NewL(
		*ipPortManager,
		KNokiaOMXFileSourceComponentName,
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

COmxILPort* COmxILFileSource::ConstructOPB0PortL() const
	{
	OMX_U32 thisPortIndex = 0;
#ifdef OMXIL_AUDIO_FILESOURCE
	RArray<OMX_AUDIO_CODINGTYPE> supportedAudioFormats;
	CleanupClosePushL(supportedAudioFormats);
	supportedAudioFormats.AppendL(OMX_AUDIO_CodingUnused);
	COmxILAudioFileSourceOPB0Port* opb0Port = COmxILAudioFileSourceOPB0Port::NewL(
			TOmxILCommonPortData (
			TOmxILSpecVersion(),	// OMX specification version information
			thisPortIndex, 			// Port number the structure applies to
			OMX_DirOutput, 			// Direction of this port
			KMinBuffers,			// The minimum number of buffers this port requires
			KMinBufferSize,			// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainAudio,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,		                 // Buffer aligment requirements
			OMX_BufferSupplyUnspecified,	// supplier preference when tunneling between two ports
			COmxILPort::KBufferMarkPropagationPortNotNeeded),
			supportedAudioFormats,
			*ipProcessingFunction);
	CleanupStack::PopAndDestroy(&supportedAudioFormats);
	return opb0Port;
#elif defined(OMXIL_VIDEO_FILESOURCE)
    RArray<OMX_VIDEO_CODINGTYPE> supportedVideoFormats;
    CleanupClosePushL(supportedVideoFormats);
    RArray<OMX_COLOR_FORMATTYPE> supportedColourFormats;
    CleanupClosePushL(supportedColourFormats);
    COmxILVideoFileSourceOPB0Port* opb0Port = COmxILVideoFileSourceOPB0Port::NewL(
            TOmxILCommonPortData (
            TOmxILSpecVersion(),    // OMX specification version information
            thisPortIndex,          // Port number the structure applies to
            OMX_DirOutput,           // Direction of this port
            KMinBuffers,            // The minimum number of buffers this port requires
            KMinBufferSize,         // Minimum size, in bytes, for buffers to be used for this port
            OMX_PortDomainVideo,    // Domain of the port
            OMX_FALSE,//OMX_TRUE,               // Buffers contiguous requirement (true or false)
            0,//KBufferAlignment,       // Buffer aligment requirements
            OMX_BufferSupplyUnspecified, // supplier preference when tunneling between two ports
            COmxILPort::KBufferMarkPropagationPortNotNeeded),
            supportedVideoFormats,
            supportedColourFormats,
            *ipProcessingFunction);
    CleanupStack::PopAndDestroy(2);
    return opb0Port;
#elif defined(OMXIL_IMAGE_FILESOURCE)

    RArray<OMX_IMAGE_CODINGTYPE> supportedImageFormats;
     CleanupClosePushL(supportedImageFormats);
     RArray<OMX_COLOR_FORMATTYPE> supportedColourFormats;
     CleanupClosePushL(supportedColourFormats);
     COmxILImageFileSourceOPB0Port* opb0Port = COmxILImageFileSourceOPB0Port::NewL(
             TOmxILCommonPortData (
             TOmxILSpecVersion(),    // OMX specification version information
             thisPortIndex,          // Port number the structure applies to
             OMX_DirOutput,           // Direction of this port
             KMinBuffers,            // The minimum number of buffers this port requires
             KMinBufferSize,         // Minimum size, in bytes, for buffers to be used for this port
             OMX_PortDomainImage,    // Domain of the port
             OMX_FALSE,              // Buffers contiguous requirement (true or false)
             0,                      // Buffer aligment requirements
             OMX_BufferSupplyUnspecified, // supplier preference when tunneling between two ports
             COmxILPort::KBufferMarkPropagationPortNotNeeded),
             supportedImageFormats,
             supportedColourFormats,
             *ipProcessingFunction);
     CleanupStack::PopAndDestroy(2);
     return opb0Port;

#elif defined(OMXIL_OTHER_FILESOURCE)
	RArray<OMX_OTHER_FORMATTYPE> supportedOtherFormats;

	CleanupClosePushL(supportedOtherFormats);
	supportedOtherFormats.AppendL(OMX_OTHER_FormatBinary);

	COmxILOtherFileSourceOPB0Port* opb0Port = COmxILOtherFileSourceOPB0Port::NewL(
			TOmxILCommonPortData (
			TOmxILSpecVersion(),	// OMX specification version information
			thisPortIndex, 			// Port number the structure applies to
			OMX_DirOutput, 			// Direction of this port
			KMinBuffers,			// The minimum number of buffers this port requires
			KMinBufferSize,			// Minimum size, in bytes, for buffers to be used for this port
			OMX_PortDomainOther,	// Domain of the port
			OMX_FALSE,				// Buffers contiguous requirement (true or false)
			0,		                 // Buffer aligment requirements
			OMX_BufferSupplyUnspecified,	// supplier preference when tunneling between two ports
			COmxILPort::KBufferMarkPropagationPortNotNeeded),
			supportedOtherFormats,
			*ipProcessingFunction);

	CleanupStack::PopAndDestroy(&supportedOtherFormats);
	return opb0Port;
#endif
	}
