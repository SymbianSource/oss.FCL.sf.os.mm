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

#include "omxilcoreutils.h"


/**
 Converts an OpenMAX error code to a Symbian error code.
 @param		aErrorType The OpenMAX error code.
 @return	The Symbian error code.
 */
TInt ConvertOmxErrorType(OMX_ERRORTYPE aErrorType)
	{
	if (aErrorType == OMX_ErrorNone)
		{
		return KErrNone;
		}
	else if (aErrorType >= OMX_ErrorInsufficientResources &&
			  aErrorType <= OMX_ErrorTunnelingUnsupported)
		{
		return KSymbianErrors[aErrorType - OMX_ErrorInsufficientResources];
		}
	else
		{
		return KErrGeneral;
		}
	}

/**
 Converts a Symbian error code to an OpenMAX error code.
 @param		aError The Symbian error code.
 @return	The OpenMAX error code.
 */
OMX_ERRORTYPE ConvertSymbianErrorType(TInt aError)
	{
 	// In the current implementation this function is only used for the return code in the 
 	// callback methods. Currently the only expected errors KErrNone and KErrOverflow.
 	
	OMX_ERRORTYPE err = OMX_ErrorNone;
	switch (aError)
		{
	case KErrNone:
		err = OMX_ErrorNone;
		break;
	case KErrOverflow:
	case KErrNoMemory:
		err = OMX_ErrorInsufficientResources;
		break;
	case KErrNotSupported:
		err = OMX_ErrorNotImplemented;
		break;
	case KErrNotReady:
		err = OMX_ErrorNotReady;
		break;
	case KErrGeneral:
	default:
		err = OMX_ErrorUndefined;
		}
	return err;
	}

/**
 Converts an OpenMAX state to a Symbian state.
 @param		aState
 			The OpenMAX error code.
 @return	The Symbian state.
 @see TILComponentState
 */
TILComponentState ConvertOmxState(OMX_STATETYPE aState)
	{
	TILComponentState state = EComponentInvalid;
	switch (aState)
		{
	case OMX_StateInvalid:
		state = EComponentInvalid;
		break;
	case OMX_StateLoaded:
		state = EComponentLoaded;
		break;
	case OMX_StateIdle:
		state = EComponentIdle;
		break;
	case OMX_StateExecuting:
		state = EComponentExecuting;
		break;
	case OMX_StatePause:
		state = EComponentPaused;
		break;
	case OMX_StateWaitForResources:
		state = EComponentWaitingForResources;
		break;
	default:
		state = EComponentInvalid;
		}
	return state;
	}
	
	
OMX_COMMANDTYPE ConvertSymbianCommandType(TILCommandTypes aCmd)
	{
	OMX_COMMANDTYPE omxcommand = OMX_CommandMax;
	switch (aCmd)
		{
	case ECommandStateSet:
		omxcommand = OMX_CommandStateSet;
		break;
	case ECommandFlush:
		omxcommand = OMX_CommandFlush;
		break;
	case ECommandPortDisable:
		omxcommand = OMX_CommandPortDisable;
		break;
	case ECommandPortEnable:
		omxcommand = OMX_CommandPortEnable;
		break;
	case ECommandMarkBuffer:
		omxcommand = OMX_CommandMarkBuffer;
		break;
	default:
		omxcommand = OMX_CommandMax;
		}
	return omxcommand;
	}
