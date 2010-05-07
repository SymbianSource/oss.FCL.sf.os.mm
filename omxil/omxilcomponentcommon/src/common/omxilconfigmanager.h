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

#ifndef OMXILCONFIGMANAGER_H
#define OMXILCONFIGMANAGER_H

#include <e32base.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>

#include "omxilindexmanager.h"

/**
   Config Manager Panic category
*/
_LIT(KOmxILConfigManagerPanicCategory, "OmxILConfigManager");


// Forward declarations
class MOmxILPortManagerIf;


/**
   This class is a placeholder for those OpenMAX IL parameters and configs that
   apply to the component as a whole and not to an specific port in the
   component. The design of this class should be revisited as the OpenMAX IL
   resource management logic is added. For now resource management-related
   params/configs values can only be set or queried but the associated
   functionality is missing.
 */
class COmxILConfigManager : public COmxILIndexManager
	{

public:

	IMPORT_C static COmxILConfigManager* NewL(
		MOmxILPortManagerIf& aPortManager,
		const TDesC8& aComponentName,
		const OMX_VERSIONTYPE& aComponentVersion,
		const RPointerArray<TDesC8>& aComponentRoleList);

	IMPORT_C ~COmxILConfigManager();

	IMPORT_C virtual OMX_ERRORTYPE GetComponentVersion(
		OMX_STRING aComponentName,
		OMX_VERSIONTYPE* apComponentVersion,
		OMX_VERSIONTYPE* apSpecVersion,
		OMX_UUIDTYPE* apComponentUUID) const;

	IMPORT_C virtual OMX_ERRORTYPE GetParameter(
		OMX_INDEXTYPE aParamIndex,
		TAny* apComponentParameterStructure) const;

	IMPORT_C virtual OMX_ERRORTYPE SetParameter(
		OMX_INDEXTYPE aParamIndex,
		const TAny* apComponentParameterStructure,
		OMX_BOOL aInitTime = OMX_TRUE);

	IMPORT_C virtual OMX_ERRORTYPE GetConfig(
		OMX_INDEXTYPE aConfigIndex,
		TAny* apComponentConfigStructure) const;

	IMPORT_C virtual OMX_ERRORTYPE SetConfig(
		OMX_INDEXTYPE aConfigIndex,
		const TAny* apComponentConfigStructure);

	IMPORT_C virtual OMX_ERRORTYPE GetExtensionIndex(
		OMX_STRING aParameterName,
		OMX_INDEXTYPE* apIndexType) const;

	IMPORT_C OMX_ERRORTYPE ComponentRoleEnum(
		OMX_U8* aRole,
		OMX_U32 aIndex) const;

protected:

	IMPORT_C explicit COmxILConfigManager(MOmxILPortManagerIf& aPortManager);

	IMPORT_C void ConstructL(const TDesC8& aComponentName,
					const OMX_VERSIONTYPE& aComponentVersion,
					const RPointerArray<TDesC8>& aComponentRoleList);


	static TBool CompareRoles(const HBufC8& aRole1, const HBufC8& aRole2);


protected:

	// Reference to the component's port manager
	MOmxILPortManagerIf& iPortManager;

	// Reference to the component's port manager
	RBuf8 iComponentName;

	// The list of OpenMAX IL roles supported by the component
	RPointerArray<HBufC8> iComponentRoleList;

	// The current OpenMAX IL role
	TUint iCurrentRoleIndex;

	// The current version of this component (this is different to the spec
	// version)
	OMX_VERSIONTYPE iComponentVersion;

	// OpenMAX IL resource concealment structure (only set/get, resource
	// concealment logic not implemented)
	OMX_RESOURCECONCEALMENTTYPE iParamDisableResourceConcealment;

	// OpenMAX IL component suspension policy structure (only set/get,
	// component suspension logic not implemented)
	OMX_PARAM_SUSPENSIONPOLICYTYPE iParamSuspensionPolicy;

	// OpenMAX IL component priority structure (only set/get, component
	// priority logic not implemented)
	OMX_PRIORITYMGMTTYPE iConfigPriorityMgmt;

	};

#endif // OMXILCONFIGMANAGER_H
