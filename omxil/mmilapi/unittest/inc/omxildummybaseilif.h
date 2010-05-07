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

#ifndef OMXILDUMMYBASEILIF_H
#define OMXILDUMMYBASEILIF_H

#include <e32base.h>
#include <openmax/il/khronos/v1_x/OMX_Component.h>

#include "ilcomponentif.h"
#include "momxilcomponentifobserver.h"

/**
Test component. 
Implementation of the generic parts of the MILIF API specific to OpenMAX IL specification when implemented as a plugin.
*/
class COmxILDummyBaseILIF	 : 	public CILComponentIf,
 								public MOmxILComponentIfObserver
	{
friend class COmxILDummyBasePortILIF; 

public:
	class CBody;

public:
	
	// from MILComponentIf
	IMPORT_C void CreateComponentL(const TDesC8& aComponentName, MILIfObserver& aComponentIfObserver);
	IMPORT_C void CreateComponentL(const TUid& aUid, MILIfObserver& aComponentIfObserver);
	IMPORT_C void ReleaseComponent(); 
	IMPORT_C TInt GetComponentInputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const;
	IMPORT_C TInt GetComponentOutputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const;
	IMPORT_C TInt SetConfig(const TILStruct& aConfig, const MILComponentPortIf* aComponentPort);
	IMPORT_C TInt GetConfig(TILStruct& aConfig, const MILComponentPortIf* aComponentPort) const;	
	IMPORT_C TInt Initialize();
	IMPORT_C TInt Execute(); 
	IMPORT_C TInt Pause();
	IMPORT_C TInt Stop();
	IMPORT_C TInt UnInitialize();
	IMPORT_C TInt GetState(TILComponentState& aState) const;
	IMPORT_C TInt SendCommand(const TILCommand& aCommand);
	IMPORT_C TInt FlushAllPorts();
	IMPORT_C TInt ComponentRoleEnum(TPtr8& aComponentRole, TUint32 aIndex) const;
	IMPORT_C TInt GetComponentVersion(TILComponentVersion& aVersion) const;
	IMPORT_C TInt SetComponentIfRole(const TUid& aComponentRole);
	IMPORT_C TInt SetComponentIfRole(const TDesC8& aComponentRole);
	IMPORT_C TInt GetComponentIfRole(TUid& aComponentRole) const;
	IMPORT_C TInt GetComponentIfRole(TDes8& aComponentRole) const;
	IMPORT_C TInt SetExtConfig(const TDesC8& aParameterName, const TILStruct& aConfig, const MILComponentPortIf* aComponentPort);
	IMPORT_C TInt GetExtConfig(const TDesC8& aParameterName, TILStruct& aConfig, const MILComponentPortIf* aComponentPort) const;
	IMPORT_C TAny* CustomInterface(TUid aUid);

	// from MOmxILComponentIfObserver
	IMPORT_C TInt FillBufferDone(CMMFBuffer* aBuffer,TInt aPortIndex);
	IMPORT_C TInt EmptyBufferDone(const CMMFBuffer* aBuffer,TInt aPortIndex);
	IMPORT_C TInt EventHandler(OMX_EVENTTYPE aEvent, TUint32 aData1, TUint32 aData2, TAny* aExtraInfo);

	IMPORT_C ~COmxILDummyBaseILIF();

protected:
	IMPORT_C COmxILDummyBaseILIF();

	IMPORT_C TInt OmxGetParameter(OMX_INDEXTYPE aParamIndex, TAny* aComponentParameterStructure) const;
	IMPORT_C TInt OmxSetParameter(OMX_INDEXTYPE aParamIndex, TAny* aComponentParameterStructure);
	IMPORT_C TInt OmxGetConfig(OMX_INDEXTYPE aConfigIndex, TAny* aValue) const;
	IMPORT_C TInt OmxSetConfig(OMX_INDEXTYPE aConfigIndex, TAny* aValue);
	IMPORT_C TInt OmxGetExtensionIndex(const TDesC8& aParameterName, OMX_INDEXTYPE* aIndexType) const;

private:
	// Internal implementation. Only meant to be used by the component's port interface.
	TInt OmxSendCommand(OMX_COMMANDTYPE aCmd, TUint32 aParam1, TAny* aCmdData);
	TInt OmxComponentTunnelRequest(TUint32 aPortInput, OMX_HANDLETYPE aOutput, TUint32 aPortOutput);
	TInt OmxComponentDisconnectTunnel(TUint32 aPortInput, OMX_HANDLETYPE aOutput, TUint32 aPortOutput);
	TInt OmxUseBuffer(CMMFBuffer* aBuffer, TUint32 aPortIndex);
	CMMFBuffer* OmxAllocateBufferL(TUint32 aPortIndex, TUint32 aSizeBytes);
	TInt OmxFreeBuffer(CMMFBuffer* aBuffer);
	TInt OmxEmptyThisBuffer(const CMMFBuffer* aBuffer, MOmxILComponentIfObserver* aObserver);
	TInt OmxFillThisBuffer(CMMFBuffer* aBuffer, MOmxILComponentIfObserver* aObserver);
	OMX_COMPONENTTYPE* OmxHandle() const;

private:
	CBody*	iBody;
	};

#endif // OMXILDUMMYBASEILIF_H

