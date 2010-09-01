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

#include <e32cmn.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>

#include "log.h"
#include "omxilconfigmanager.h"
#include "omxilportmanager.h"
#include "omxilspecversion.h"
#include "omxilutil.h"


// NOTE: OMX_UUIDTYPE[128] is defined in OMX_Types.h
#define KMAX_UUIDTYPE_SIZE 128

EXPORT_C COmxILConfigManager*
COmxILConfigManager::NewL(
	COmxILPortManager& aPortManager,
	const TDesC8& aComponentName,
	const OMX_VERSIONTYPE& aComponentVersion,
	const RPointerArray<TDesC8>& aComponentRoleList)
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::NewL"));
	__ASSERT_DEBUG(aComponentName.Length() && aComponentRoleList.Count(),
				   User::Panic(KOmxILConfigManagerPanicCategory, 1));

	COmxILConfigManager* self = new (ELeave)COmxILConfigManager(aPortManager);
	CleanupStack::PushL(self);
	self->ConstructL(aComponentName,
					 aComponentVersion,
					 aComponentRoleList);
	CleanupStack::Pop(self);
	return self;

	}

EXPORT_C void
COmxILConfigManager::ConstructL(const TDesC8& aComponentName,
								const OMX_VERSIONTYPE& aComponentVersion,
								const RPointerArray<TDesC8>& aComponentRoleList)
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::ConstructL"));

	iComponentName.CreateL(aComponentName, OMX_MAX_STRINGNAME_SIZE);
	iComponentName.PtrZ();
	iComponentVersion.s = aComponentVersion.s;

	// Note that the first role in the list of roles becomes the default role
	// assumed by the component

	const TUint rolesCount = aComponentRoleList.Count();
	for (TUint i=0; i<rolesCount; ++i)
		{
		iComponentRoleList.AppendL(aComponentRoleList[i]->AllocLC());
		CleanupStack::Pop();
		}


	InsertParamIndexL(OMX_IndexParamDisableResourceConcealment);
	InsertParamIndexL(OMX_IndexParamSuspensionPolicy);
	InsertParamIndexL(OMX_IndexParamStandardComponentRole);
	InsertParamIndexL(OMX_IndexParamPriorityMgmt);
	InsertConfigIndexL(OMX_IndexConfigPriorityMgmt);

	}

EXPORT_C
COmxILConfigManager::COmxILConfigManager(COmxILPortManager& aPortManager)
	:
	iPortManager(aPortManager),
	iComponentName(),
	iComponentRoleList(),
	iCurrentRoleIndex(0)

	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::COmxILConfigManager"));

	iParamDisableResourceConcealment.nSize						   = sizeof(OMX_RESOURCECONCEALMENTTYPE);
	iParamDisableResourceConcealment.nVersion					   = TOmxILSpecVersion();
	iParamDisableResourceConcealment.bResourceConcealmentForbidden = OMX_TRUE;

	iParamSuspensionPolicy.nSize	= sizeof(OMX_PARAM_SUSPENSIONPOLICYTYPE);
	iParamSuspensionPolicy.nVersion = TOmxILSpecVersion();
	iParamSuspensionPolicy.ePolicy	= OMX_SuspensionDisabled;

	iConfigPriorityMgmt.nSize		   = sizeof(OMX_PRIORITYMGMTTYPE);
	iConfigPriorityMgmt.nVersion	   = TOmxILSpecVersion();
	iConfigPriorityMgmt.nGroupPriority = 0;
	iConfigPriorityMgmt.nGroupID	   = 0;

	}

EXPORT_C
COmxILConfigManager::~COmxILConfigManager()
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::~COmxILConfigManager"));

	iComponentName.Close();
	iComponentRoleList.ResetAndDestroy();

	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::GetComponentVersion(OMX_STRING aComponentName,
										 OMX_VERSIONTYPE* apComponentVersion,
										 OMX_VERSIONTYPE* apSpecVersion,
										 OMX_UUIDTYPE* apComponentUUID) const
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::GetComponentVersion"));

	TPtr8 name(reinterpret_cast<TUint8*>(aComponentName),
			   OMX_MAX_STRINGNAME_SIZE);
	name.Copy(iComponentName);
	name.PtrZ();

	(*apComponentVersion) = iComponentVersion;
	(*apSpecVersion)	  = TOmxILSpecVersion();

	// Generate a component uuid
	TUint32 uid = reinterpret_cast<TUint32>(this);
	TPtr8 uidPtr(reinterpret_cast<TUint8*>(*apComponentUUID),
				  KMAX_UUIDTYPE_SIZE);
	uidPtr = TPtr8(reinterpret_cast<TUint8*>(uid), sizeof(uid));

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::GetParameter(OMX_INDEXTYPE aParamIndex,
								  TAny* apComponentParameterStructure) const
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::GetParameter"));

	TInt index = FindParamIndex(aParamIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aParamIndex)
		{
	case OMX_IndexParamStandardComponentRole:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_PARAM_COMPONENTROLETYPE))))
			{
			return omxRetValue;
			}

		OMX_PARAM_COMPONENTROLETYPE* pComponentRole
			= static_cast<OMX_PARAM_COMPONENTROLETYPE*>(
				apComponentParameterStructure);

		// Here, the role returned must be the role that this component is
		// currently assuming
		TPtr8 role(reinterpret_cast<TUint8*>(pComponentRole->cRole),
				   OMX_MAX_STRINGNAME_SIZE);

		role = *(iComponentRoleList[iCurrentRoleIndex]);
		role.PtrZ();

		}
		break;

	case OMX_IndexParamDisableResourceConcealment:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_RESOURCECONCEALMENTTYPE))))
			{
			return omxRetValue;
			}

		OMX_RESOURCECONCEALMENTTYPE* pResConceal
			= static_cast<OMX_RESOURCECONCEALMENTTYPE*>(
				apComponentParameterStructure);

		*pResConceal = iParamDisableResourceConcealment;

		}
		break;
	case OMX_IndexParamSuspensionPolicy:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_PARAM_SUSPENSIONPOLICYTYPE))))
			{
			return omxRetValue;
			}

		OMX_PARAM_SUSPENSIONPOLICYTYPE* pSuspensionPolicy
			= static_cast<OMX_PARAM_SUSPENSIONPOLICYTYPE*>(
				apComponentParameterStructure);

		*pSuspensionPolicy = iParamSuspensionPolicy;

		}
		break;

	case OMX_IndexParamPriorityMgmt:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_PRIORITYMGMTTYPE))))
			{
			return omxRetValue;
			}

		OMX_PRIORITYMGMTTYPE* pPriorityMgmt
			= static_cast<OMX_PRIORITYMGMTTYPE*>(
				apComponentParameterStructure);

		*pPriorityMgmt = iConfigPriorityMgmt;

		}
		break;

	default:
		{
		__ASSERT_ALWAYS(EFalse,
						User::Panic(KOmxILConfigManagerPanicCategory, 1));
		}
		};

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::SetParameter(OMX_INDEXTYPE aParamIndex,
								  const TAny* apComponentParameterStructure,
								  OMX_BOOL aInitTime /*  = OMX_TRUE */)
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::SetParameter"));

	TInt index = FindParamIndex(aParamIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aParamIndex)
		{
	case OMX_IndexParamStandardComponentRole:
		{
		if (!aInitTime)
			{
			return OMX_ErrorIncorrectStateOperation;
			}

		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_PARAM_COMPONENTROLETYPE))))
			{
			return omxRetValue;
			}

		const OMX_PARAM_COMPONENTROLETYPE* pComponentRole
			= static_cast<const OMX_PARAM_COMPONENTROLETYPE*>(
				apComponentParameterStructure);

		TPtrC8 roleToFindPtr(
			static_cast<const TUint8*>(pComponentRole->cRole));

		HBufC8* pRoleToFind = HBufC8::New(OMX_MAX_STRINGNAME_SIZE);
		if (!pRoleToFind)
			{
			return OMX_ErrorInsufficientResources;
			}
		*pRoleToFind = roleToFindPtr;

		TInt newRoleIndex = 0;
		if (KErrNotFound ==
			(newRoleIndex =
			 iComponentRoleList.Find(pRoleToFind,
									 TIdentityRelation<HBufC8>(
										 &COmxILConfigManager::CompareRoles))))
			{
			delete pRoleToFind;
			return OMX_ErrorBadParameter;
			}

		if (*pRoleToFind != *(iComponentRoleList[iCurrentRoleIndex]))
			{
			// At this point, it is mandated that the component populates all
			// defaults according to the new role that has just been set by the
			// IL Client
			if (OMX_ErrorNone !=
				(omxRetValue =
				 iPortManager.ComponentRoleIndication(newRoleIndex)))
				{
				delete pRoleToFind;
				return omxRetValue;
				}

			iCurrentRoleIndex = newRoleIndex;

			}

		delete pRoleToFind;

		}
		break;

	case OMX_IndexParamDisableResourceConcealment:
		{
		if (!aInitTime)
			{
			return OMX_ErrorIncorrectStateOperation;
			}

		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_RESOURCECONCEALMENTTYPE))))
			{
			return omxRetValue;
			}

		const OMX_RESOURCECONCEALMENTTYPE* pResConceal
			= static_cast<const OMX_RESOURCECONCEALMENTTYPE*>(
				apComponentParameterStructure);

		iParamDisableResourceConcealment = *pResConceal;

		}
		break;

	case OMX_IndexParamSuspensionPolicy:
		{
		if (!aInitTime)
			{
			return OMX_ErrorIncorrectStateOperation;
			}

		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_PARAM_SUSPENSIONPOLICYTYPE))))
			{
			return omxRetValue;
			}

		const OMX_PARAM_SUSPENSIONPOLICYTYPE* pSuspensionPolicy
			= static_cast<const OMX_PARAM_SUSPENSIONPOLICYTYPE*>(
				apComponentParameterStructure);

		// OMX_SuspensionEnabled is the last of the supported values as of
		// v1.1.1
		if (pSuspensionPolicy->ePolicy > OMX_SuspensionEnabled)
			{
			return OMX_ErrorBadParameter;
			}

		iParamSuspensionPolicy = *pSuspensionPolicy;

		}
		break;

	case OMX_IndexParamPriorityMgmt:
		{
		if (!aInitTime)
			{
			return OMX_ErrorIncorrectStateOperation;
			}

		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_PRIORITYMGMTTYPE))))
			{
			return omxRetValue;
			}


		const OMX_PRIORITYMGMTTYPE* pPriorityMgmt
			= static_cast<const OMX_PRIORITYMGMTTYPE*>(
				apComponentParameterStructure);

		iConfigPriorityMgmt = *pPriorityMgmt;

		}
		break;

	default:
		{
		__ASSERT_ALWAYS(EFalse,
						User::Panic(KOmxILConfigManagerPanicCategory, 1));
		}
		};

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::GetConfig(OMX_INDEXTYPE aConfigIndex,
							   TAny* apComponentConfigStructure) const
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::GetConfig"));

	TInt index = FindConfigIndex(aConfigIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aConfigIndex)
		{
	case OMX_IndexConfigPriorityMgmt:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentConfigStructure,
								  sizeof(OMX_PRIORITYMGMTTYPE))))
			{
			return omxRetValue;
			}

		OMX_PRIORITYMGMTTYPE* pPriorityMgmt
			= static_cast<OMX_PRIORITYMGMTTYPE*>(
				apComponentConfigStructure);

		*pPriorityMgmt = iConfigPriorityMgmt;

		}
		break;
	default:
		{
		__ASSERT_ALWAYS(EFalse,
						User::Panic(KOmxILConfigManagerPanicCategory, 1));
		}
		};

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::SetConfig(OMX_INDEXTYPE aConfigIndex,
							   const TAny* apComponentConfigStructure)

	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::SetConfig"));

	TInt index = FindConfigIndex(aConfigIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aConfigIndex)
		{
	case OMX_IndexConfigPriorityMgmt:
		{
		if (OMX_ErrorNone !=
			(omxRetValue =
			 TOmxILUtil::CheckOmxStructSizeAndVersion(
				 const_cast<OMX_PTR>(apComponentConfigStructure),
				 sizeof(OMX_PRIORITYMGMTTYPE))))
			{
			return omxRetValue;
			}

		const OMX_PRIORITYMGMTTYPE* pPriorityMgmt
			= static_cast<const OMX_PRIORITYMGMTTYPE*>(
				apComponentConfigStructure);

		iConfigPriorityMgmt = *pPriorityMgmt;

		}
		break;
	default:
		{
		__ASSERT_ALWAYS(EFalse,
						User::Panic(KOmxILConfigManagerPanicCategory, 1));
		}
		};

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::GetExtensionIndex(
	OMX_STRING /*aParameterName*/,
	OMX_INDEXTYPE* /*apIndexType*/) const
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::GetExtensionIndex"));

	// No custom index here for now...
	return OMX_ErrorUnsupportedIndex;
	}

EXPORT_C OMX_ERRORTYPE
COmxILConfigManager::ComponentRoleEnum(OMX_U8* aRole,
									   OMX_U32 aIndex) const
	{
    DEBUG_PRINTF(_L8("COmxILConfigManager::ComponentRoleEnum"));

    // TWC:OpenmaxIL requires error code OMX_ErrorNoMore to be returned when no more roles
    if (aIndex >= iComponentRoleList.Count())
		{
		return OMX_ErrorNoMore;
		}

	HBufC8* pRole = iComponentRoleList[aIndex];

	TPtr8 role(reinterpret_cast<TUint8*>(aRole),
			   OMX_MAX_STRINGNAME_SIZE);
	role = *pRole;
	role.PtrZ();

	return OMX_ErrorNone;

	}

TBool
COmxILConfigManager::CompareRoles(
	const HBufC8& aRole1, const HBufC8& aRole2)
	{
	return (aRole1 == aRole2);
	}
