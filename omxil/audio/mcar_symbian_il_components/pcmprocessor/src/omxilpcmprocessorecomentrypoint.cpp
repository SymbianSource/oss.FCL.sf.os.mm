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
#include <ecom/ecom.h>

#include <openmax/il/loader/omxilcomponentif.h>

#include "omxilpcmprocessor.h"
#include "omxilpcmprocessor.hrh"


_LIT8(KSymbianOmxILPcmDecoderName, "OMX.SYMBIAN.AUDIO.DECODER.PCM");
_LIT8(KSymbianOmxILPcmDecoderRole, "audio_decoder.pcm");

_LIT8(KSymbianOmxILPcmEncoderName, "OMX.SYMBIAN.AUDIO.ENCODER.PCM");
_LIT8(KSymbianOmxILPcmEncoderRole, "audio_encoder.pcm");



//
// This component doesn't use the OMXIL_COMPONENT_ECOM_ENTRYPOINT.
// The macro isn't used because it restricts the ECOM plugin DLL to only implementing a single named component.
// This plugin is intended to act the same whether it's used as if it were a "pcm decoder" or a "pcm encoder" and
// implements two interfaces so that it can be used as either of those roles.
// 









// This is a clone of the non-sharable COmxILSymbianComponentIf that allows parameterisation of component name and role
class COmxILPcmTranscoderComponentIf : public COmxILComponentIf
	{
public:
	static COmxILPcmTranscoderComponentIf* NewL(const TDesC8& aComponentName, const TDesC8& aComponentRole)
		{
		COmxILPcmTranscoderComponentIf* self = new (ELeave) COmxILPcmTranscoderComponentIf;
		CleanupStack::PushL(self);
		self->ConstructL(aComponentName, aComponentRole);
		CleanupStack::Pop(self);
	    return self;
		}
	
	~COmxILPcmTranscoderComponentIf()
		{
		delete ipHandle;
		}

	// from COmxILComponentIf
	OMX_HANDLETYPE Handle()
		{
		return ipHandle;
		}

private:
	COmxILPcmTranscoderComponentIf()
		{
		}
	
	
	void ConstructL(const TDesC8& aComponentName, const TDesC8& aComponentRole)
		{
		ipHandle = new(ELeave) OMX_COMPONENTTYPE;
		User::LeaveIfError(COmxILPcmProcessor::CreateComponent(ipHandle, aComponentName, aComponentRole));
		}

private:
	OMX_COMPONENTTYPE* ipHandle;
	};


#if 0

// Disabled. Since this isn't exported, and the 

// Component Entry Point
OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE aComponent)
	{
	TInt err = COmxILPcmProcessor::CreateComponent(aComponent, KSymbianOmxILPcmDecoderName, KSymbianOmxILPcmDecoderRole );

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

#endif

COmxILComponentIf* ConstructPcmDecoderL()
	{
	return COmxILPcmTranscoderComponentIf::NewL(KSymbianOmxILPcmDecoderName, KSymbianOmxILPcmDecoderRole);
	}

COmxILComponentIf* ConstructPcmEncoderL()
	{
	return COmxILPcmTranscoderComponentIf::NewL(KSymbianOmxILPcmEncoderName, KSymbianOmxILPcmEncoderRole);
	}


const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(KUidSymbianOmxILPcmDecoder,	ConstructPcmDecoderL),
	IMPLEMENTATION_PROXY_ENTRY(KUidSymbianOmxILPcmEncoder,	ConstructPcmEncoderL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}


