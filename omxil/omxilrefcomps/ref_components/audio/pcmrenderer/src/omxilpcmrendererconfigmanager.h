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

#ifndef OMXILPCMRENDERERCONFIGMANAGER_H
#define OMXILPCMRENDERERCONFIGMANAGER_H

#include "omxilconfigmanager.h"

// class forwarding
class COmxILPcmRendererProcessingFunction;

/**
   This class is a placeholder for those PCM Renderer parameters and configs that
   apply to the component as a whole and not to a specific port in the
   component.
 */
NONSHARABLE_CLASS(COmxILPcmRendererConfigManager) : public COmxILConfigManager
	{

public:

	static COmxILPcmRendererConfigManager* NewL(
		MOmxILPortManagerIf& aPortManager,
		COmxILPcmRendererProcessingFunction& aProcessingFunction,
		const TDesC8& aComponentName,
		const OMX_VERSIONTYPE& aComponentVersion,
		const RPointerArray<TDesC8>& aComponentRoleList);

	~COmxILPcmRendererConfigManager();

	OMX_ERRORTYPE GetConfig(
		OMX_INDEXTYPE aConfigIndex,
		TAny* apComponentConfigStructure) const;

	OMX_ERRORTYPE SetConfig(
		OMX_INDEXTYPE aConfigIndex,
		const TAny* apComponentConfigStructure);

	OMX_ERRORTYPE GetExtensionIndex(
		OMX_STRING aParameterName,
		OMX_INDEXTYPE* apIndexType) const;

protected:

	COmxILPcmRendererConfigManager(MOmxILPortManagerIf& aPortManager,
								   COmxILPcmRendererProcessingFunction& aProcessingFunction);

	void ConstructL(const TDesC8& aComponentName,
					const OMX_VERSIONTYPE& aComponentVersion,
					const RPointerArray<TDesC8>& aComponentRoleList);

private:
	COmxILPcmRendererProcessingFunction& iProcessingFunction;
	};

#endif // OMXILPCMRENDERERCONFIGMANAGER_H
