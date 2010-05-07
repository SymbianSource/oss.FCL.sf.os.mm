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

#include "comxilvideoscheduleroutputport.h"


COmxILVideoSchedulerOutputPort* COmxILVideoSchedulerOutputPort::NewL(const TOmxILCommonPortData& aCommonPortData)
	{
	// TODO these arrays must left empty, to be removed from the video port constructor
	RArray<OMX_VIDEO_CODINGTYPE> supportedCodings;
	RArray<OMX_COLOR_FORMATTYPE> supportedColorFormats;
	CleanupClosePushL(supportedCodings);
	CleanupClosePushL(supportedColorFormats);

	COmxILVideoSchedulerOutputPort* self = new(ELeave) COmxILVideoSchedulerOutputPort(aCommonPortData, supportedCodings, supportedColorFormats);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	CleanupStack::PopAndDestroy(2, &supportedCodings);
	
	return self;
	}

COmxILVideoSchedulerOutputPort::COmxILVideoSchedulerOutputPort(const TOmxILCommonPortData& aCommonPortData, 
	                                             const RArray<OMX_VIDEO_CODINGTYPE>& aSupportedCodings,
	                                             const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColourFormats)
 : COmxILVideoPort(aCommonPortData, aSupportedCodings, aSupportedColourFormats)
	{
	}

COmxILVideoSchedulerOutputPort::~COmxILVideoSchedulerOutputPort()
	{
	iMimeTypeBuf.Close();
	}

void COmxILVideoSchedulerOutputPort::ConstructL()
	{
	// Port definition mime type. Mime type is not relevant for uncompressed video frames
 	iMimeTypeBuf.CreateL(KNullDesC8(), KNullDesC8().Length() + 1);
 	TUint8* pTUint = const_cast<TUint8*>(iMimeTypeBuf.PtrZ());
 	iParamPortDefinition.format.video.cMIMEType = reinterpret_cast<OMX_STRING>(pTUint);

	iSupportedVideoFormats.AppendL(OMX_VIDEO_CodingUnused);
	iSupportedColorFormats.AppendL(OMX_COLOR_FormatUnused);
	}

OMX_ERRORTYPE COmxILVideoSchedulerOutputPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILVideoPort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILVideoSchedulerOutputPort::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILVideoPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILVideoSchedulerOutputPort::GetParameter(OMX_INDEXTYPE aParamIndex,
                                               TAny* apComponentParameterStructure) const
	{				
	return COmxILVideoPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILVideoSchedulerOutputPort::SetParameter(OMX_INDEXTYPE aParamIndex,
		                                       const TAny* apComponentParameterStructure,
		                                       TBool& aUpdateProcessingFunction)
	{		
	return COmxILVideoPort::SetParameter(aParamIndex, apComponentParameterStructure, aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILVideoSchedulerOutputPort::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
													TBool& /*aUpdateProcessingFunction*/)
	{
	iParamPortDefinition.format.video = aPortDefinition.format.video;
 	TUint8* pTUint = const_cast<TUint8*>(iMimeTypeBuf.PtrZ());
 	iParamPortDefinition.format.video.cMIMEType = reinterpret_cast<OMX_STRING>(pTUint);
 	iParamPortDefinition.format.video.nSliceHeight = iParamPortDefinition.format.video.nFrameHeight;
 	iParamPortDefinition.nBufferSize = iParamPortDefinition.format.video.nStride * iParamPortDefinition.format.video.nSliceHeight;
	return OMX_ErrorNone;
	}

TBool COmxILVideoSchedulerOutputPort::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& /*aPortDefinition*/) const
	{
	return ETrue;	
	}

/** Returns the number of buffers configured on this port. */
TUint32 COmxILVideoSchedulerOutputPort::BufferCount() const
	{
	return iParamPortDefinition.nBufferCountActual;
	}
