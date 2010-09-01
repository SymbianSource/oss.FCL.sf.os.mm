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

#include "omxilutil.h"
#include "omxilsymbianaudiopcmextensions.h"
#include "log.h"
#include "omxilpcmrendererconfigmanager.h"
#include "omxilpcmrendererprocessingfunction.h"



COmxILPcmRendererConfigManager*
COmxILPcmRendererConfigManager::NewL(
	COmxILPortManager& aPortManager,
	COmxILPcmRendererProcessingFunction& aProcessingFunction,
	const TDesC8& aComponentName,
	const OMX_VERSIONTYPE& aComponentVersion,
	const RPointerArray<TDesC8>& aComponentRoleList)
	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::NewL"));
	ASSERT(aComponentName.Length() && aComponentRoleList.Count());

	COmxILPcmRendererConfigManager* self = new (ELeave)COmxILPcmRendererConfigManager(aPortManager, aProcessingFunction);
	CleanupStack::PushL(self);
	self->ConstructL(aComponentName,
					 aComponentVersion,
					 aComponentRoleList);
	CleanupStack::Pop(self);
	return self;

	}

void
COmxILPcmRendererConfigManager::ConstructL(const TDesC8& aComponentName,
								const OMX_VERSIONTYPE& aComponentVersion,
								const RPointerArray<TDesC8>& aComponentRoleList)
	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::ConstructL"));

	COmxILConfigManager::ConstructL(aComponentName, aComponentVersion, aComponentRoleList);
	
	ManagedConfigIndexes().InsertInOrderL(OMX_SymbianIndexConfigAudioBytesPlayed);
	}

COmxILPcmRendererConfigManager::COmxILPcmRendererConfigManager(COmxILPortManager& aPortManager,
															   COmxILPcmRendererProcessingFunction& aProcessingFunction)
	:
	COmxILConfigManager(aPortManager),
	iProcessingFunction(aProcessingFunction)
	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::COmxILPcmRendererConfigManager"));

	}

COmxILPcmRendererConfigManager::~COmxILPcmRendererConfigManager()
	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::~COmxILPcmRendererConfigManager"));

	}

OMX_ERRORTYPE
COmxILPcmRendererConfigManager::GetConfig(OMX_INDEXTYPE aConfigIndex,
							   TAny* apComponentConfigStructure) const
	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::GetConfig"));

	TInt index = FindConfigIndex(aConfigIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aConfigIndex)
		{
	case OMX_SymbianIndexConfigAudioBytesPlayed:
		{
		if (OMX_ErrorNone !=
			(omxRetValue =
			 TOmxILUtil::CheckOmxStructSizeAndVersion(
				 apComponentConfigStructure,
				 sizeof(OMX_SYMBIAN_AUDIO_CONFIG_PCM_BYTESPLAYED))))
			{
			return omxRetValue;
			}

		OMX_SYMBIAN_AUDIO_CONFIG_PCM_BYTESPLAYED*
			pPcmBytesPlayed
			= static_cast<
			OMX_SYMBIAN_AUDIO_CONFIG_PCM_BYTESPLAYED*>(
				apComponentConfigStructure);
			
		pPcmBytesPlayed->nBytesPlayed = iProcessingFunction.GetBytesPlayed();
		}
		break;
		
	default:
		{
		omxRetValue = COmxILConfigManager::GetConfig(aConfigIndex, apComponentConfigStructure);
		}
		};

	return omxRetValue;
	}

OMX_ERRORTYPE
COmxILPcmRendererConfigManager::SetConfig(OMX_INDEXTYPE aConfigIndex,
							   const TAny* apComponentConfigStructure)

	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::SetConfig"));

	TInt index = FindConfigIndex(aConfigIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aConfigIndex)
		{
	case OMX_SymbianIndexConfigAudioBytesPlayed:
		{
		omxRetValue = OMX_ErrorUnsupportedSetting;
		}
		break;
				
	default:
		{
		omxRetValue = COmxILConfigManager::SetConfig(aConfigIndex, apComponentConfigStructure);
		}
		};

	return omxRetValue;

	}

OMX_ERRORTYPE
COmxILPcmRendererConfigManager::GetExtensionIndex(
	OMX_STRING aParameterName,
	OMX_INDEXTYPE* apIndexType) const
	{
    DEBUG_PRINTF(_L8("COmxILPcmRendererConfigManager::GetExtensionIndex"));

	TPtrC8 requestedParameterNamePtr(
		const_cast<const TUint8*>(
			reinterpret_cast<TUint8*>(aParameterName)));

	TPtrC8 parameterNamePtr(
		reinterpret_cast<const TUint8*>(sOmxSymbianPcmBytesPlayed));

	if (requestedParameterNamePtr == parameterNamePtr)
		{
		*apIndexType =
			static_cast<OMX_INDEXTYPE>(
					OMX_SymbianIndexConfigAudioBytesPlayed);
			
		return OMX_ErrorNone;
		}

	*apIndexType = OMX_IndexMax;
	return OMX_ErrorUnsupportedIndex;
	}
