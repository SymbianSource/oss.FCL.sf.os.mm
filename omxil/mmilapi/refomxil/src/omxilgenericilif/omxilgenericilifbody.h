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

#ifndef OMXILGENERICILIFBODY_H
#define OMXILGENERICILIFBODY_H


#include "omxilgenericilif.h"
#include "omxilgenericportilif.h"
#include "omxilgenericilifbodyimpl.h"
#include <e32base.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>


/**
Body implementation of the COmxILGenericILIF specific to OpenMAX specification.
*/
class COmxILGenericILIF::CBody : public CBase
	{
public:
	static CBody* NewL(COmxILGenericILIF& aParent,
					   MILIfObserver& aComponentIfObserver,
					   const TDesC8& aComponentName);
	~CBody();

	OMX_COMPONENTTYPE* Handle() const;
	
	TInt OmxGetComponentVersion(TPtr8 aComponentName, OMX_VERSIONTYPE* aComponentVersion, OMX_VERSIONTYPE* aSpecVersion, OMX_UUIDTYPE* aComponentUUID);
	TInt OmxSendCommand(OMX_COMMANDTYPE aCmd, TUint32 aParam1, TAny* aCmdData);
	TInt OmxGetParameter(OMX_INDEXTYPE aParamIndex, TAny* aComponentParameterStructure);
	TInt OmxSetParameter(OMX_INDEXTYPE aIndex, TAny* aComponentParameterStructure);
	TInt OmxGetConfig(OMX_INDEXTYPE aIndex, TAny* aValue);
	TInt OmxSetConfig(OMX_INDEXTYPE aIndex, TAny* aValue);
	TInt OmxGetExtensionIndex(const TDesC8& aParameterName, OMX_INDEXTYPE* aIndexType);
	TInt OmxGetState(OMX_STATETYPE*  aState);
	TInt OmxComponentTunnelRequest(TUint32 aPortInput, OMX_HANDLETYPE aOutput, TUint32 aPortOutput);
	TInt OmxComponentDisconnectTunnel(TUint32 aPortInput, OMX_HANDLETYPE aOutput, TUint32 aPortOutput);
	TInt OmxUseBuffer(CMMFBuffer* aBuffer, TUint32 aPortIndex);
	CMMFBuffer* OmxAllocateBufferL(TUint32 aPortIndex, TUint32 aSizeBytes);
	TInt OmxFreeBuffer(CMMFBuffer* aBuffer);
	TInt OmxEmptyThisBuffer(const CMMFBuffer* aBuffer, MOmxILComponentIfObserver* aObserver);
	TInt OmxFillThisBuffer(CMMFBuffer* aBuffer, MOmxILComponentIfObserver* aObserver);
	TInt OmxComponentRoleEnum(TPtr8& aComponentRole, TUint32 aIndex);
	TInt SetIfToRole(const TUid& aComponentRole);
	TInt GetIfRole(TUid& aComponentRole) const;
	TInt GetComponentPorts(RPointerArray<MILComponentPortIf>& aComponentPorts, OMX_DIRTYPE);
	TInt SetPortsL();

	TInt FillBufferDone(CMMFBuffer* aBuffer, TInt aPortIndex);
	TInt EmptyBufferDone(const CMMFBuffer* aBuffer, TInt aPortIndex);
	TInt EventHandler(OMX_EVENTTYPE aEvent, TUint32 aData1, TUint32 aData2, TAny* aExtraInfo);
	 
private:
	CBody(COmxILGenericILIF& aParent, MILIfObserver& aComponentIfObserver);
	void ConstructL(const TDesC8& aComponentName);

private:

	OMX_COMPONENTTYPE* iHandle;
	
	RPointerArray<COmxILGenericPortILIF> iInputPorts;
	RPointerArray<COmxILGenericPortILIF> iOutputPorts;

	COmxBufferManager* iBufferManager;
	COmxCallbacks* iCallbacks;

	COmxILGenericILIF& iParent;
	MILIfObserver& iIfObserver;

	TUid iFormat;	
	};

#endif // OMXILGENERICILIFBODY_H

