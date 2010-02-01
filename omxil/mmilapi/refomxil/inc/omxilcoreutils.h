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

#ifndef OMXILCOREUTILS_H
#define OMXILCOREUTILS_H

#include "ilifbase.h"
#include <e32std.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>


const TInt KSymbianErrors[] =
	{
	KErrNoMemory, /*OMX_ErrorInsufficientResources*/
	KErrUnknown, /*OMX_ErrorUndefined*/
	KErrBadName, /*OMX_ErrorInvalidComponentName*/
	KErrNotFound, /*OMX_ErrorComponentNotFound*/
	KErrGeneral, /*OMX_ErrorInvalidComponent*/
	KErrArgument, /*OMX_ErrorBadParameter*/
	KErrNotSupported, /*OMX_ErrorNotImplemented*/
	KErrUnderflow, /*OMX_ErrorUnderflow*/
	KErrOverflow, /*OMX_ErrorOverflow*/
	KErrHardwareNotAvailable, /* OMX_ErrorHardware*/
	KErrGeneral, /*OMX_ErrorInvalidState*/
  	KErrCorrupt, /*OMX_ErrorStreamCorrupt*/
  	KErrArgument, /*OMX_ErrorPortsNotCompatible*/
  	KErrHardwareNotAvailable, /*OMX_ErrorResourcesLost*/
  	KErrCompletion, /*OMX_ErrorNoMore*/ 
  	KErrGeneral, /*OMX_ErrorVersionMismatch*/
  	KErrNotReady, /*OMX_ErrorNotReady*/
  	KErrTimedOut, /*OMX_ErrorTimeout*/
  	KErrNone, /*OMX_ErrorSameState*/
  	KErrNotReady, /*OMX_ErrorResourcesPreempted*/
  	KErrNotReady, /*OMX_ErrorPortUnresponsiveDuringAllocation*/
  	KErrNotReady, /*OMX_ErrorPortUnresponsiveDuringDeallocation*/
  	KErrNotReady, /*OMX_ErrorPortUnresponsiveDuringStop*/
  	KErrNotReady, /*OMX_ErrorIncorrectStateTransition*/
  	KErrNotReady, /*OMX_ErrorIncorrectStateOperation*/
  	KErrNotSupported, /*OMX_ErrorUnsupportedSetting*/
  	KErrNotSupported, /*OMX_ErrorUnsupportedIndex*/
  	KErrArgument, /*OMX_ErrorBadPortIndex*/
  	KErrNotReady, /*OMX_ErrorPortUnpopulated*/
  	KErrNotReady, /*OMX_ErrorComponentSuspended*/
  	KErrNotReady, /*OMX_ErrorDynamicResourcesUnavailable*/
  	KErrCorrupt, /*OMX_ErrorMbErrorsInFrame*/
  	KErrNotSupported, /*OMX_ErrorFormatNotDetected*/
  	KErrGeneral, /*OMX_ErrorContentPipeOpenFailed*/
  	KErrGeneral, /*OMX_ErrorContentPipeCreationFailed*/
  	KErrCorrupt, /*OMX_ErrorSeperateTablesUsed*/
  	KErrNotSupported, /*OMX_ErrorTunnelingUnsupported*/
	};


TInt ConvertOmxErrorType(OMX_ERRORTYPE aErrorType);
OMX_ERRORTYPE ConvertSymbianErrorType(TInt aError);
TILComponentState ConvertOmxState(OMX_STATETYPE aState);
OMX_COMMANDTYPE ConvertSymbianCommandType(TILCommandTypes aCmd);
	
#endif // OMXILCOREUTILS_H

