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


#include "omxutil.h"

void OMXUtil::ErrorDes(OMX_ERRORTYPE aError, TBuf<64>& aErrorDes)
	{
	switch(aError)
		{
#define DEFERR(x) case x: aErrorDes = _L(#x); break;
		DEFERR(OMX_ErrorNone);
		DEFERR(OMX_ErrorUndefined);
		DEFERR(OMX_ErrorComponentNotFound);
		DEFERR(OMX_ErrorInsufficientResources);
		DEFERR(OMX_ErrorInvalidComponentName);
		DEFERR(OMX_ErrorPortsNotCompatible);
		DEFERR(OMX_ErrorUnsupportedIndex);
		DEFERR(OMX_ErrorBadParameter);
		DEFERR(OMX_ErrorIncorrectStateTransition);
		DEFERR(OMX_ErrorIncorrectStateOperation);
		DEFERR(OMX_ErrorInvalidState);
		DEFERR(OMX_ErrorInvalidComponent);
		DEFERR(OMX_ErrorBadPortIndex);
		DEFERR(OMX_ErrorContentPipeOpenFailed);
		DEFERR(OMX_ErrorUnsupportedSetting);
#undef DEFERR
	default:
		aErrorDes.Format(_L("%u"), aError);
		break;
		}	
	}
