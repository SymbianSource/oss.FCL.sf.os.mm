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
 @file
 @internalComponent
*/

#include "omxilclientclockport.h"
#include "log.h"


EXPORT_C COmxILClientClockPort* COmxILClientClockPort::NewL(const TOmxILCommonPortData& aCommonPortData, 
							    							const RArray<OMX_OTHER_FORMATTYPE>& aSupportedOtherFormats)
	{
	COmxILClientClockPort* self = new(ELeave) COmxILClientClockPort(aCommonPortData);
	CleanupStack::PushL(self);
	self->COmxILOtherPort::ConstructL(aSupportedOtherFormats);
	CleanupStack::Pop();
	return self;
	}

COmxILClientClockPort::COmxILClientClockPort(const TOmxILCommonPortData& aCommonPortData) 
  : COmxILOtherPort(aCommonPortData),
    iSpecVersion(TOmxILSpecVersion())
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::COmxILClientClockPort"));
	}

OMX_ERRORTYPE COmxILClientClockPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetLocalOmxParamIndexes"));
	return COmxILOtherPort::GetLocalOmxParamIndexes(aIndexArray);
	}


OMX_ERRORTYPE COmxILClientClockPort::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetLocalOmxParamIndexes"));
	return COmxILOtherPort::GetLocalOmxConfigIndexes(aIndexArray);
	}


OMX_ERRORTYPE COmxILClientClockPort::GetParameter(OMX_INDEXTYPE aParamIndex,
						     							TAny* apComponentParameterStructure) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetParameter"));
	return COmxILOtherPort::GetParameter(aParamIndex, apComponentParameterStructure);
	}


OMX_ERRORTYPE COmxILClientClockPort::SetParameter(OMX_INDEXTYPE aParamIndex,
						     							const TAny* apComponentParameterStructure,
						     							TBool& aUpdateProcessingFunction)
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::SetParameter"));
	return COmxILOtherPort::SetParameter(aParamIndex, apComponentParameterStructure, aUpdateProcessingFunction);
	}


OMX_ERRORTYPE COmxILClientClockPort::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& /*aPortDefinition*/,
								  										TBool& /*aUpdateProcessingFunction*/)
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::SetFormatInPortDefinition"));
	return OMX_ErrorNone;
	}


TBool COmxILClientClockPort::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::IsTunnelledPortCompatible"));
	// Domain check
	if(aPortDefinition.eDomain != iParamPortDefinition.eDomain)
		{
		return EFalse;
		}
	
	// Format check
	if (aPortDefinition.format.other.eFormat != iParamPortDefinition.format.other.eFormat)
		{
		return EFalse;
		}
	
	return ETrue;
	}


EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::GetMediaTime(OMX_TICKS& aMediaTime) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetMediaTime"));
	return GetTime(aMediaTime, OMX_IndexConfigTimeCurrentMediaTime);
	}


EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::GetWallTime(OMX_TICKS& aWallTime) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetWallTime"));
	return GetTime(aWallTime, OMX_IndexConfigTimeCurrentWallTime);
	}


EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::GetClockState(OMX_TIME_CONFIG_CLOCKSTATETYPE& aClockState) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetClockState"));

	if (IsClockComponentAvailable())
		{
		aClockState.nSize = sizeof(OMX_TIME_CONFIG_CLOCKSTATETYPE);
		aClockState.nVersion = iSpecVersion;
		aClockState.eState = OMX_TIME_ClockStateMax;
		aClockState.nStartTime = 0;
		aClockState.nOffset = 0;
		aClockState.nWaitMask = 0;
		return OMX_GetConfig(iTunnelledComponent, OMX_IndexConfigTimeClockState, &aClockState);
		}

	return OMX_ErrorIncorrectStateOperation;

	}

EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::MediaTimeRequest(const OMX_PTR apPrivate, 
															const OMX_TICKS aMediaTime,
															const OMX_TICKS aOffset) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::MediaTimeRequest"));
	OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE timeInfo;
	timeInfo.nSize = sizeof(timeInfo);
	timeInfo.nVersion = iSpecVersion;
	timeInfo.nPortIndex = iTunnelledPort;
	timeInfo.pClientPrivate = apPrivate;
	timeInfo.nMediaTimestamp = aMediaTime;
	timeInfo.nOffset = aOffset;

	if (IsClockComponentAvailable())
		{
		return OMX_SetConfig(iTunnelledComponent, OMX_IndexConfigTimeMediaTimeRequest, &timeInfo);
		}
	
	return OMX_ErrorIncorrectStateOperation;
	}


EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::SetStartTime(const OMX_TICKS aStartTime) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::SetStartTime"));
	return SetTime(aStartTime, OMX_IndexConfigTimeClientStartTime);
	}


EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::SetVideoReference(const OMX_TICKS aVideoRef) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::SetVideoReference"));
	return SetTime(aVideoRef, OMX_IndexConfigTimeCurrentVideoReference);
	}


EXPORT_C OMX_ERRORTYPE COmxILClientClockPort::SetAudioReference(const OMX_TICKS aAudioRef) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::SetAudioReference"));
	return SetTime(aAudioRef, OMX_IndexConfigTimeCurrentAudioReference);
	}

EXPORT_C OMX_BOOL COmxILClientClockPort::IsClockComponentAvailable() const
{
	if (iTunnelledComponent != NULL && iParamPortDefinition.bEnabled)
		{
		return OMX_TRUE;
		}
	
	return OMX_FALSE;
}

OMX_ERRORTYPE COmxILClientClockPort::GetTime(OMX_TICKS& aWallTime, const OMX_INDEXTYPE aTimeIndex) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::GetTime"));
	OMX_TIME_CONFIG_TIMESTAMPTYPE timeInfo;
	timeInfo.nSize = sizeof(timeInfo);
	timeInfo.nVersion = iSpecVersion;
	timeInfo.nPortIndex = iTunnelledPort;

	if (!IsClockComponentAvailable())
		{
		return OMX_ErrorIncorrectStateOperation;
		}
		
	OMX_ERRORTYPE error = OMX_GetConfig(iTunnelledComponent, aTimeIndex, &timeInfo);

	if (error == OMX_ErrorNone)
		{
		aWallTime = timeInfo.nTimestamp;
		}
	
	return error;
	}


OMX_ERRORTYPE COmxILClientClockPort::SetTime(const OMX_TICKS aStartTime, const OMX_INDEXTYPE aTimeIndex) const
	{
	DEBUG_PRINTF(_L8("COmxILClientClockPort::SetTime"));
	OMX_TIME_CONFIG_TIMESTAMPTYPE timeInfo;
	timeInfo.nSize = sizeof(timeInfo);
	timeInfo.nVersion = iSpecVersion;
	timeInfo.nPortIndex = iTunnelledPort;
	timeInfo.nTimestamp = aStartTime;
	
	if (IsClockComponentAvailable())
		{
		return OMX_SetConfig(iTunnelledComponent, aTimeIndex, &timeInfo);
		}
		
	return OMX_ErrorIncorrectStateOperation;
	}
