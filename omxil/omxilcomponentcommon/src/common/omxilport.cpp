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

#include <mmf/server/mmfbuffer.h>
#include <mmf/server/mmfdatabuffer.h>

#include "log.h"
#include "omxilport.h"
#include "omxilutil.h"


const TInt COmxILPort::KMaxBufferMarksQueueSize;
const TInt COmxILPort::KBufferMarkPropagationPortNotNeeded;


EXPORT_C
TOmxILCommonPortData::TOmxILCommonPortData(
	OMX_VERSIONTYPE aOmxVersion,
	OMX_U32 aPortIndex,
	OMX_DIRTYPE aDirection,
	OMX_U32 aBufferCountMin,
	OMX_U32 aBufferSizeMin,
	OMX_PORTDOMAINTYPE aPortDomain,
	OMX_BOOL aBuffersContiguous,
	OMX_U32 aBufferAlignment,
	OMX_BUFFERSUPPLIERTYPE aBufferSupplier,
	OMX_U32 aBufferMarkPropagationPortIndex)
	:
	iOmxVersion(aOmxVersion),
	iPortIndex(aPortIndex),
	iDirection(aDirection),
	iBufferCountMin(aBufferCountMin),
	iBufferSizeMin(aBufferSizeMin),
	iPortDomain(aPortDomain),
	iBuffersContiguous(aBuffersContiguous),
	iBufferAlignment(aBufferAlignment),
	iBufferSupplier(aBufferSupplier),
	iBufferMarkPropagationPortIndex(aBufferMarkPropagationPortIndex)
	{
	}


EXPORT_C
COmxILPort::COmxILPort(const TOmxILCommonPortData& aCommonPortData)
	:
	iTunnelledComponent(0),
	iTunnelledPort(0),
	iBufferHeaders(),
	iBufferMarks(_FOFF(TBufferMarkInfo, iLink)),
	iTransitionState(EPortNotTransitioning),
	iBufferMarkPropagationPortIndex(
		aCommonPortData.iBufferMarkPropagationPortIndex)
	{
    DEBUG_PRINTF(_L8("COmxILPort::COmxILPort"));

	__ASSERT_DEBUG(aCommonPortData.iDirection == OMX_DirInput ||
				   aCommonPortData.iDirection == OMX_DirOutput,
				   User::Panic(KOmxILPortPanicCategory, 1));

	// From Section 3.1.2.12.1 .
	//
	// nBufferCountMin : "The component shall define this non-zero default
	// value."
	//
	// nBufferCountActual : "The component shall set a default value no less
	// than nBufferCountMin for this field"
	__ASSERT_DEBUG(aCommonPortData.iBufferCountMin > 0,
				   User::Panic(KOmxILPortPanicCategory, 1));

	// Filll in OMX_PARAM_PORTDEFINITIONTYPE
	iParamPortDefinition.nSize				= sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
	iParamPortDefinition.nVersion			= aCommonPortData.iOmxVersion;
	iParamPortDefinition.nPortIndex			= aCommonPortData.iPortIndex;
	iParamPortDefinition.eDir				= aCommonPortData.iDirection;
	iParamPortDefinition.nBufferCountActual = aCommonPortData.iBufferCountMin;
	iParamPortDefinition.nBufferCountMin	= aCommonPortData.iBufferCountMin;
	iParamPortDefinition.nBufferSize		= aCommonPortData.iBufferSizeMin;
	iParamPortDefinition.bEnabled			= OMX_TRUE;
	iParamPortDefinition.bPopulated			= OMX_FALSE;
	iParamPortDefinition.eDomain			= aCommonPortData.iPortDomain;
	// NOTE: iParamPortDefinition.format must be finished up by concrete ports
	iParamPortDefinition.bBuffersContiguous = aCommonPortData.iBuffersContiguous;
	iParamPortDefinition.nBufferAlignment	= aCommonPortData.iBufferAlignment;

	// Fill in OMX_PARAM_BUFFERSUPPLIERTYPE
	iParamCompBufferSupplier.nSize			 = sizeof(OMX_PARAM_BUFFERSUPPLIERTYPE);
	iParamCompBufferSupplier.nVersion		 = aCommonPortData.iOmxVersion;
	iParamCompBufferSupplier.nPortIndex		 = aCommonPortData.iPortIndex;
	iParamCompBufferSupplier.eBufferSupplier = aCommonPortData.iBufferSupplier;

	}

EXPORT_C
COmxILPort::~COmxILPort()
	{
    DEBUG_PRINTF(_L8("COmxILPort::~COmxILPort"));

#ifdef _DEBUG
	const TInt headerCount = iBufferHeaders.Count();
	if (headerCount > 0)
		{
		DEBUG_PRINTF(_L8("COmxILPort::~COmxILPort :: ------------------------------- WARNING ---------------------------------------  "));
		DEBUG_PRINTF2(_L8("COmxILPort::~COmxILPort :: [%d] Buffer headers still exist"), headerCount);
		DEBUG_PRINTF(_L8("COmxILPort::~COmxILPort :: CleanUpPort() may be used from the most derived port class to delete them"));
		DEBUG_PRINTF(_L8("COmxILPort::~COmxILPort :: ------------------------------- WARNING ---------------------------------------  "));
		}
#endif

	iBufferHeaders.Close();
	iBufferMarks.ResetAndDestroy();

	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILPort::GetLocalOmxParamIndexes"));

	TInt err = aIndexArray.InsertInOrder(OMX_IndexParamPortDefinition);

	// Note that index duplication is OK.
	if (KErrNone == err || KErrAlreadyExists == err)
		{
		err = aIndexArray.InsertInOrder(OMX_IndexParamCompBufferSupplier);
		}

	if (KErrNone != err && KErrAlreadyExists != err)
		{
		return OMX_ErrorInsufficientResources;
		}

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::GetLocalOmxConfigIndexes(RArray<TUint>& /*aIndexArray*/) const
	{
    DEBUG_PRINTF(_L8("COmxILPort::GetLocalOmxConfigIndexes"));

	return OMX_ErrorNone;

	}


EXPORT_C OMX_ERRORTYPE
COmxILPort::GetParameter(OMX_INDEXTYPE aParamIndex,
						 TAny* apComponentParameterStructure) const
	{
    DEBUG_PRINTF(_L8("COmxILPort::GetParameter"));

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aParamIndex)
		{
	case OMX_IndexParamPortDefinition:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_PARAM_PORTDEFINITIONTYPE))))
			{
			return omxRetValue;
			}

		OMX_PARAM_PORTDEFINITIONTYPE* pPortDefinition
			= static_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(
				apComponentParameterStructure);

		*pPortDefinition = iParamPortDefinition;
		}
		break;

	case OMX_IndexParamCompBufferSupplier:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  apComponentParameterStructure,
								  sizeof(OMX_PARAM_BUFFERSUPPLIERTYPE))))
			{
			return omxRetValue;
			}

		OMX_PARAM_BUFFERSUPPLIERTYPE* pBufferSupplier
			= static_cast<OMX_PARAM_BUFFERSUPPLIERTYPE*>(
				apComponentParameterStructure);

		pBufferSupplier->eBufferSupplier =
			iParamCompBufferSupplier.eBufferSupplier;
		}
		break;

	default:
		{
		return OMX_ErrorUnsupportedIndex;
		}
		};

	return OMX_ErrorNone;

	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::SetParameter(OMX_INDEXTYPE aParamIndex,
						 const TAny* apComponentParameterStructure,
						 TBool& aUpdateProcessingFunction)
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetParameter"));

	aUpdateProcessingFunction = EFalse;

	OMX_ERRORTYPE omxRetValue = OMX_ErrorNone;
	switch(aParamIndex)
		{
	case OMX_IndexParamPortDefinition:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_PARAM_PORTDEFINITIONTYPE))))
			{
			return omxRetValue;
			}

		const OMX_PARAM_PORTDEFINITIONTYPE* pPortDefinition
			= static_cast<const OMX_PARAM_PORTDEFINITIONTYPE*>(
				apComponentParameterStructure);

		// Port Format must be set by the concrete port...
		if (OMX_ErrorNone !=
			(omxRetValue =
			 SetFormatInPortDefinition(*pPortDefinition,
									   aUpdateProcessingFunction)))
			{
			return omxRetValue;
			}

		// Set here only the additional read-write parameters of
		// OMX_PARAM_PORTDEFINITIONTYPE
		if (iParamPortDefinition.nBufferCountActual !=
			pPortDefinition->nBufferCountActual)
			{
			if (pPortDefinition->nBufferCountActual <
				iParamPortDefinition.nBufferCountMin)
				{
				return OMX_ErrorBadParameter;
				}
			iParamPortDefinition.nBufferCountActual =
				pPortDefinition->nBufferCountActual;
			aUpdateProcessingFunction = ETrue;
			}

		}
		break;

	case OMX_IndexParamCompBufferSupplier:
		{
		if (OMX_ErrorNone != (omxRetValue =
							  TOmxILUtil::CheckOmxStructSizeAndVersion(
								  const_cast<OMX_PTR>(apComponentParameterStructure),
								  sizeof(OMX_PARAM_BUFFERSUPPLIERTYPE))))
			{
			return omxRetValue;
			}

		const OMX_PARAM_BUFFERSUPPLIERTYPE* pBufferSupplier
			= static_cast<const OMX_PARAM_BUFFERSUPPLIERTYPE*>(
				apComponentParameterStructure);

		// OMX_BufferSupplyOutput is the last of the supported values as of
		// v1.1.1
		if (iParamCompBufferSupplier.eBufferSupplier > OMX_BufferSupplyOutput)
			{
			return OMX_ErrorBadParameter;
			}

		if (iParamCompBufferSupplier.eBufferSupplier !=
			pBufferSupplier->eBufferSupplier)
			{
			// The component providing the input port is responsible for
			// signalling the tunnelled component about the buffer supplier
			// override...
			if (iTunnelledComponent &&
				iParamPortDefinition.eDir == OMX_DirInput)
				{
				OMX_PARAM_BUFFERSUPPLIERTYPE bufferSupplierType;
				bufferSupplierType.nSize		   = sizeof(OMX_PARAM_BUFFERSUPPLIERTYPE);
				bufferSupplierType.nVersion		   = iParamPortDefinition.nVersion;
				bufferSupplierType.nPortIndex	   = iTunnelledPort;
				bufferSupplierType.eBufferSupplier = pBufferSupplier->eBufferSupplier;
				OMX_ERRORTYPE retValue = OMX_ErrorUndefined;
				if (OMX_ErrorNone !=
					(retValue =
					 OMX_SetParameter(iTunnelledComponent,
									  OMX_IndexParamCompBufferSupplier,
									  &bufferSupplierType)) )
					{
					return retValue;
					}
				}
			iParamCompBufferSupplier.eBufferSupplier =
				pBufferSupplier->eBufferSupplier;
			}

		}
		break;
	default:
		{
		return OMX_ErrorUnsupportedIndex;
		}
		};

	return OMX_ErrorNone;

	}


EXPORT_C OMX_ERRORTYPE
COmxILPort::GetConfig(OMX_INDEXTYPE /*aConfigIndex*/,
					  TAny* /*apComponentConfigStructure*/) const
	{
    DEBUG_PRINTF(_L8("COmxILPort::GetConfig"));
	return OMX_ErrorUnsupportedIndex;
	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::SetConfig(OMX_INDEXTYPE /*aConfigIndex*/,
					  const TAny* /*apComponentConfigStructure*/,
					  TBool& /*aUpdateProcessingFunction*/)
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetConfig"));
	return OMX_ErrorUnsupportedIndex;
	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::GetExtensionIndex(OMX_STRING /* aParameterName */,
							  OMX_INDEXTYPE* /* apIndexType */) const
	{
	// This method is intended to be overridden by the concrete ports when
	// needed...
	return OMX_ErrorUnsupportedIndex;
	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::PopulateBuffer(OMX_BUFFERHEADERTYPE** appBufferHdr,
						   const OMX_PTR apAppPrivate,
						   OMX_U32 aSizeBytes,
						   OMX_U8* apBuffer,
						   TBool& portPopulationCompleted)
	{
    DEBUG_PRINTF(_L8("COmxILPort::PopulateBuffer"));

	portPopulationCompleted = EFalse;

	if(aSizeBytes < iParamPortDefinition.nBufferSize)
		{
		return OMX_ErrorBadParameter;
		}

	// Allocate the buffer header...
	OMX_BUFFERHEADERTYPE* pHeader = new OMX_BUFFERHEADERTYPE;
	*appBufferHdr = pHeader;
	if (!pHeader)
		{
		return OMX_ErrorInsufficientResources;
		}

	// Here, lets discriminate between apBuffer == 0 (AllocateBuffer) and
	// apBuffer != 0 (UseBuffer)
	TUint8* pPortSpecificBuffer = 0;
	OMX_PTR pPortPrivate = 0;
	OMX_PTR pPlatformPrivate = 0;
	OMX_ERRORTYPE portSpecificErr = OMX_ErrorNone;
	if (apBuffer)
		{
		//... (UseBuffer) Do any port-specific wrapping of the received buffer,
		// if needed by the port....
		portSpecificErr = DoBufferWrapping(aSizeBytes,
										   apBuffer,
										   pPortPrivate,
										   pPlatformPrivate,
										   apAppPrivate);
		}
	else
		{
		// ... (AllocateBuffer) Do the port-specific buffer allocation ...
		portSpecificErr = DoBufferAllocation(aSizeBytes,
											 pPortSpecificBuffer,
											 pPortPrivate,
											 pPlatformPrivate,
											 apAppPrivate);
		}

	if (OMX_ErrorNone != portSpecificErr)
		{
		delete *appBufferHdr; *appBufferHdr = 0;
		return portSpecificErr;
		}


	// Add to local list of buffer headers...
	if (KErrNone !=
		iBufferHeaders.Append(
			TBufferInfo(pHeader,
						(apBuffer ?
						 TBufferInfo::EBufferAway :
						 TBufferInfo::EBufferAtHome),
						(apBuffer ?
						 TBufferInfo::EBufferNotOwned :
						 TBufferInfo::EBufferOwned),
						apBuffer,
						apAppPrivate,
						pPlatformPrivate,
						pPortPrivate)))
		{
		// Undo custom buffer allocation/wrapping
		if (apBuffer)
			{
			DoBufferUnwrapping(
				apBuffer,
				pPortPrivate,
				pPlatformPrivate,
				apAppPrivate);

			}
		else
			{
			DoBufferDeallocation(
				pPortSpecificBuffer,
				pPortPrivate,
				pPlatformPrivate,
				apAppPrivate);
			}
		delete *appBufferHdr; *appBufferHdr = NULL;
		return OMX_ErrorInsufficientResources;
		}


	// Fill in the header...
	pHeader->nSize				  = sizeof(OMX_BUFFERHEADERTYPE);
	pHeader->nVersion			  = iParamPortDefinition.nVersion;
	pHeader->pBuffer			  = apBuffer ? apBuffer : pPortSpecificBuffer;
	pHeader->nAllocLen			  = aSizeBytes;
	pHeader->nFilledLen			  = 0;
	pHeader->nOffset			  = 0;
	pHeader->pAppPrivate		  = apAppPrivate;
	pHeader->pPlatformPrivate	  = pPlatformPrivate;
	pHeader->hMarkTargetComponent = 0;
	pHeader->pMarkData			  = 0;
	pHeader->nTickCount			  = 0;
	pHeader->nTimeStamp			  = 0;
	pHeader->nFlags				  = 0;


	if (OMX_DirInput == iParamPortDefinition.eDir)
		{
		pHeader->pInputPortPrivate	= pPortPrivate;
		pHeader->pOutputPortPrivate = 0;
		pHeader->nInputPortIndex	= iParamPortDefinition.nPortIndex;
		pHeader->nOutputPortIndex	= 0;
		}
	else
		{
		pHeader->pInputPortPrivate	= 0;
		pHeader->pOutputPortPrivate = pPortPrivate;
		pHeader->nInputPortIndex	= 0;
		pHeader->nOutputPortIndex	= iParamPortDefinition.nPortIndex;
		}

	if (iParamPortDefinition.nBufferCountActual == iBufferHeaders.Count())
		{
		iParamPortDefinition.bPopulated = OMX_TRUE;
		portPopulationCompleted			= ETrue;
		}

	return OMX_ErrorNone;

	}


EXPORT_C OMX_ERRORTYPE
COmxILPort::FreeBuffer(OMX_BUFFERHEADERTYPE* apBufferHeader,
					   TBool& portDepopulationCompleted)
	{
    DEBUG_PRINTF2(_L8("COmxILPort::FreeBuffer : BUFFER [%X]"), apBufferHeader);

	portDepopulationCompleted = EFalse;

	TInt headerIndex = 0;
	if (KErrNotFound ==
		(headerIndex =
		 iBufferHeaders.Find(TBufferInfo(apBufferHeader),
							 TIdentityRelation<TBufferInfo>(
								 &TBufferInfo::Compare))))
		{
		return OMX_ErrorBadParameter;
		}


	OMX_PTR pPortPrivate =
		OMX_DirInput == iParamPortDefinition.eDir ?
		apBufferHeader->pInputPortPrivate :
		apBufferHeader->pOutputPortPrivate;

	if (iBufferHeaders[headerIndex].IsBufferOwned())
		{
		DoBufferDeallocation(
			apBufferHeader->pBuffer,
			pPortPrivate,
			apBufferHeader->pPlatformPrivate,
			apBufferHeader->pAppPrivate);
		}
	else
		{
		DoBufferUnwrapping(
			apBufferHeader->pBuffer,
			pPortPrivate,
			apBufferHeader->pPlatformPrivate,
			apBufferHeader->pAppPrivate);
		}

	delete apBufferHeader;
	iBufferHeaders.Remove(headerIndex);

	if (iBufferHeaders.Count() < iParamPortDefinition.nBufferCountActual)
		{
		iParamPortDefinition.bPopulated = OMX_FALSE;
		}

	if (0 == iBufferHeaders.Count())
		{
		portDepopulationCompleted = ETrue;
		}

	return OMX_ErrorNone;

	}


EXPORT_C OMX_ERRORTYPE
COmxILPort::TunnelRequest(OMX_HANDLETYPE aTunneledComp,
						  OMX_U32 aTunneledPort,
						  OMX_TUNNELSETUPTYPE* apTunnelSetup)
	{
    DEBUG_PRINTF(_L8("COmxILPort::TunnelRequest"));

	// Check whether the tunnel is being torn down
	if (!aTunneledComp)
		{
		// Cancel existing tunnel setup, if any
		iTunnelledComponent = 0;
		return OMX_ErrorNone;
		}

	// Check that we are receiving a valid tunnel setup structure
	if (!apTunnelSetup)
		{
		return OMX_ErrorBadParameter;
		}

	// STEP 0: Retrieve the port definition from the tunnelled component...
	OMX_PARAM_PORTDEFINITIONTYPE paramPortDef;
	paramPortDef.nSize		= sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
	paramPortDef.nVersion	= iParamPortDefinition.nVersion;
	paramPortDef.nPortIndex = aTunneledPort;
	if (OMX_ErrorNone !=
		OMX_GetParameter(aTunneledComp,
						 OMX_IndexParamPortDefinition,
						 &paramPortDef) )
		{
		return OMX_ErrorUndefined;
		}

	if (OMX_DirOutput == iParamPortDefinition.eDir)
		{
		// OMX_DirOutput
		//

		// Step 1: Check that this output port is being tunnelled to an input
		// port...
		if (paramPortDef.eDir != OMX_DirInput)
			{
			return OMX_ErrorPortsNotCompatible;
			}

		// Step 2: Fill in the tunnel setup structure received...
		apTunnelSetup->nTunnelFlags = 0;
		apTunnelSetup->eSupplier	= iParamCompBufferSupplier.eBufferSupplier;

		iTunnelledComponent	= aTunneledComp;
		iTunnelledPort		= aTunneledPort;
		}
	else
		{
		// OMX_DirInput
		//

		// Check that this input port is being tunnelled to an output
		// port...
		if (paramPortDef.eDir != OMX_DirOutput)
			{
			return OMX_ErrorPortsNotCompatible;
			}

		// Check that there is something consistent in the tunnel setup data
		// received...
		if ((apTunnelSetup->eSupplier != OMX_BufferSupplyUnspecified) &&
			(apTunnelSetup->eSupplier != OMX_BufferSupplyInput) &&
			(apTunnelSetup->eSupplier != OMX_BufferSupplyOutput))
			{
			return OMX_ErrorBadParameter;
			}

		// Set tunnelled component and port as they will be needed by
		// IsTunnelledPortCompatible...
		iTunnelledComponent	= aTunneledComp;
		iTunnelledPort		= aTunneledPort;

		// Check domain-specific parameter compatibility (this is delegated
		// to derived port classes)...
		if (!IsTunnelledPortCompatible(paramPortDef))
			{
			iTunnelledComponent	= 0;
			return OMX_ErrorPortsNotCompatible;
			}

		// Now, try to get to an understanding here...Work out which port will
		// be buffer supplier...
		OMX_BUFFERSUPPLIERTYPE bufferSupplierDecision =
			OMX_BufferSupplyUnspecified;
		if (apTunnelSetup->nTunnelFlags & OMX_PORTTUNNELFLAG_READONLY	||
			( (apTunnelSetup->eSupplier == OMX_BufferSupplyInput) &&
			  (iParamCompBufferSupplier.eBufferSupplier ==
			   OMX_BufferSupplyInput ||
			   iParamCompBufferSupplier.eBufferSupplier ==
			   OMX_BufferSupplyUnspecified) ) ||
			( (apTunnelSetup->eSupplier == OMX_BufferSupplyUnspecified) &&
			  (iParamCompBufferSupplier.eBufferSupplier ==
			   OMX_BufferSupplyInput)) )
			{
			bufferSupplierDecision = OMX_BufferSupplyInput;
			}
		else
			{
			bufferSupplierDecision = OMX_BufferSupplyOutput;
			}

		// Set buffer supplier param in tunnelled port...
		OMX_PARAM_BUFFERSUPPLIERTYPE bufferSupplierType;
		bufferSupplierType.nSize		   = sizeof(OMX_PARAM_BUFFERSUPPLIERTYPE);
		bufferSupplierType.nVersion		   = iParamPortDefinition.nVersion;
		bufferSupplierType.nPortIndex	   = aTunneledPort;
		bufferSupplierType.eBufferSupplier = bufferSupplierDecision;
		if (OMX_ErrorNone !=
			OMX_SetParameter(aTunneledComp,
							 OMX_IndexParamCompBufferSupplier,
							 &bufferSupplierType) )
			{
			iTunnelledComponent = 0;
			return OMX_ErrorPortsNotCompatible;
			}

		apTunnelSetup->eSupplier = bufferSupplierDecision;
		iParamCompBufferSupplier.eBufferSupplier = bufferSupplierDecision;

		}

	return OMX_ErrorNone;

	}


EXPORT_C OMX_ERRORTYPE
COmxILPort::PopulateTunnel(TBool& portPopulationCompleted)
	{
    DEBUG_PRINTF(_L8("COmxILPort::PopulateTunnel"));

	__ASSERT_DEBUG(iBufferHeaders.Count() == 0,
				   User::Panic(KOmxILPortPanicCategory, 1));
	__ASSERT_DEBUG(IsTunnelledAndBufferSupplier(),
				   User::Panic(KOmxILPortPanicCategory, 1));

	portPopulationCompleted = EFalse;

	// STEP 1: Obtain the number of buffers that the tunnelled port requires to
	// be populated...  Retrieve the port definition from the tunnelled
	// component
	OMX_PARAM_PORTDEFINITIONTYPE paramPortDef;
	paramPortDef.nSize		= sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
	paramPortDef.nVersion	= iParamPortDefinition.nVersion;
	paramPortDef.nPortIndex = iTunnelledPort;
	if (OMX_ErrorNone !=
		OMX_GetParameter(iTunnelledComponent,
						 OMX_IndexParamPortDefinition,
						 &paramPortDef) )
		{
		return OMX_ErrorUndefined;
		}

	// Step 2: Both ports must have the same threshold value (number of buffers
	// to be populated) before completing the transition to
	// OMX_StateIdle...Otherwise the population process would complete earlier
	// or never...
	TUint numOfBuffersToPopulate = iParamPortDefinition.nBufferCountActual;
	if(paramPortDef.nBufferCountActual !=
	   iParamPortDefinition.nBufferCountActual)
		{
		numOfBuffersToPopulate =
			Max(iParamPortDefinition.nBufferCountActual,
				paramPortDef.nBufferCountActual);
		if (iParamPortDefinition.nBufferCountActual <
			numOfBuffersToPopulate)
			{
			// Update own buffer count requirements
			DEBUG_PRINTF3(_L8("COmxILPort::PopulateTunnel : Updated own nBufferCountActual - Old Value [%d] New Value [%d] "),
						  iParamPortDefinition.nBufferCountActual, numOfBuffersToPopulate);
			iParamPortDefinition.nBufferCountActual = numOfBuffersToPopulate;
			}
		else
			{
			// Update peer's buffer count requirements
			DEBUG_PRINTF3(_L8("COmxILPort::PopulateTunnel : Updated peer's nBufferCountActual - Old Value [%d] New Value [%d] "),
						  paramPortDef.nBufferCountActual, numOfBuffersToPopulate);
			paramPortDef.nBufferCountActual = numOfBuffersToPopulate;
			if (OMX_ErrorNone != OMX_SetParameter(iTunnelledComponent,
												  OMX_IndexParamPortDefinition,
												  &paramPortDef))
				{
				DEBUG_PRINTF(_L8("COmxILPort::PopulateTunnel : Error setting nBufferCountActual in tunnelled component "));
				return OMX_ErrorUndefined;
				}
			}
		}

	// STEP 3: Start population of the tunnel...
	TUint sizeOfBuffersToPopulate =
		iParamPortDefinition.nBufferSize >= paramPortDef.nBufferSize  ?
		iParamPortDefinition.nBufferSize :
		paramPortDef.nBufferSize;

	OMX_BUFFERHEADERTYPE* pHeader = 0;
	TUint8* pPortSpecificBuffer = 0;
	OMX_PTR pPortPrivate = 0;
	OMX_PTR pPlatformPrivate = 0;
	OMX_ERRORTYPE portSpecificErr = OMX_ErrorNone;
	for (TUint i=0; i<numOfBuffersToPopulate; i++)
		{
		// Allocate the buffer...
		if (OMX_ErrorNone !=
			(portSpecificErr = DoBufferAllocation(
				sizeOfBuffersToPopulate,
				pPortSpecificBuffer,
				pPortPrivate,
				pPlatformPrivate)))
			{
			// There's no point on continuing here... the tunnel will never
			// be completely populated now...
			return portSpecificErr;
			}
		
		OMX_ERRORTYPE useBufRes = DoOmxUseBuffer(iTunnelledComponent,
												 &pHeader,
												 iTunnelledPort,
												 pPortPrivate,
												 pPlatformPrivate,
												 sizeOfBuffersToPopulate,
												 pPortSpecificBuffer);

		if ((OMX_ErrorNone != useBufRes) || !pHeader)
			{
			DoBufferDeallocation(
				pPortSpecificBuffer,
				pPortPrivate,
				pPlatformPrivate);

			if (pHeader)
				{
				switch(useBufRes)
					{
				case OMX_ErrorIncorrectStateOperation:
					{
					// Here, the tunnelled component is not ready. Probably,
					// the IL Client has not commanded yet the component to go
					// to OMX_StateIdle. Out-of-context implementations could
					// do here a backoff-and-retry procedure. This
					// implementation can just return and expect that the IL
					// Client will detect the situation of this component not
					// transitioning to OMX_StateIdle.
					DEBUG_PRINTF(_L8("COmxILPort::PopulateTunnel : OMX_ErrorIncorrectStateOperation received from non-supplier component"));
					}
					break;
					};
				}
				
			if (OMX_ErrorInsufficientResources == useBufRes)
				{
				return OMX_ErrorInsufficientResources; 
				}
			// This is a gotcha. Here there is some problem with the tunnelled
			// component. If we return the received error, this component may
			// be sending back some error code that is not allowed in
			// OMX_SendCommand. Example: The component conformance suite
			// expects here OMX_ErrorNone if the tunnelled component does not
			// support OMX_UseBuffer or some other problem. Also, we don't send
			// and error event here as there's no appropriate error for this
			// situation (OMX_ErrorPortUnresponsiveDuringAllocation is for
			// non-supplier ports). Therefore, the IL Client should recover
			// from this situation after some time by detecting that this
			// component didn't transition to OMX_StateIdle.
					
			return OMX_ErrorNone;
			}
		// Fill the data in the received header so we can correctly use it when
		// the header is at this side of the tunnel...
		if (OMX_DirInput == iParamPortDefinition.eDir)
			{
			pHeader->pInputPortPrivate = pPortPrivate;
			pHeader->nInputPortIndex   = iParamPortDefinition.nPortIndex;
			}
		else	// OMX_DirOutput == iParamPortDefinition.eDir
			{
			pHeader->pOutputPortPrivate = pPortPrivate;
			pHeader->nOutputPortIndex	= iParamPortDefinition.nPortIndex;
			}

		// Add to local list of buffer headers... return if not sucessful...
		if (KErrNone !=
			iBufferHeaders.Append(
				TBufferInfo(pHeader,
							TBufferInfo::EBufferAtHome,
							TBufferInfo::EBufferOwned,
							pPortSpecificBuffer,
							0,
							pPlatformPrivate,
							pPortPrivate)))
			{
			DoBufferDeallocation(
				pPortSpecificBuffer,
				pPortPrivate,
				pPlatformPrivate);

			return OMX_ErrorInsufficientResources;
			}
		}

	iParamPortDefinition.bPopulated = OMX_TRUE;
	portPopulationCompleted			= ETrue;

	__ASSERT_DEBUG(iBufferHeaders.Count()	  ==
				   iParamPortDefinition.nBufferCountActual,
				   User::Panic(KOmxILPortPanicCategory, 1));

	return OMX_ErrorNone;

	}


EXPORT_C OMX_ERRORTYPE
COmxILPort::FreeTunnel(TBool& portDepopulationCompleted)
	{
    DEBUG_PRINTF(_L8("COmxILPort::FreeTunnel"));

  	__ASSERT_DEBUG(iBufferHeaders.Count() ==
  				   iParamPortDefinition.nBufferCountActual,
  				   User::Panic(KOmxILPortPanicCategory, 1));	
	
	__ASSERT_DEBUG(IsTunnelledAndBufferSupplier(),
				   User::Panic(KOmxILPortPanicCategory, 1));

	const TUint numBuffersToDepopulate = iBufferHeaders.Count();
	for (TUint i=0; i<numBuffersToDepopulate; ++i)
		{
		OMX_BUFFERHEADERTYPE* pBufferHeader = iBufferHeaders[i].GetHeader();

		// Take some things from the header, before it gets deleted by the
		// tunnelled port...
		OMX_U8* pBuffer = pBufferHeader->pBuffer;
		OMX_PTR pPortPrivate =
			OMX_DirInput == iParamPortDefinition.eDir ?
			pBufferHeader->pInputPortPrivate :
			pBufferHeader->pOutputPortPrivate;
		OMX_PTR pAppPrivate = pBufferHeader->pAppPrivate;
		OMX_PTR pPlatformPrivate = pBufferHeader->pPlatformPrivate;

		DEBUG_PRINTF2(_L8("COmxILPort::FreeTunnel : BUFFER [%X]"), iBufferHeaders[i].GetHeader());

		OMX_ERRORTYPE freeBufRes = OMX_FreeBuffer(
			iTunnelledComponent,
			iTunnelledPort,
			pBufferHeader);

		// At this point, the actual buffer header should no longer exist...
		pBufferHeader = 0;

		// NOTE that we don't check OMX_FreeBuffer returned error here. If
		// something wrong happens at the tunnelled component side we'll
		// continue here and try to free as many buffers as possible.... at
		// least the state of this component will remain valid.... (we don't
		// report errors coming from the tunnelled component as that is its
		// responsibility....)

		DoBufferDeallocation(
			pBuffer,
			pPortPrivate,
			pPlatformPrivate,
			pAppPrivate);
		}

	// Clear the local list of headers. Note that there's no need to delete the
	// header as these have been allocated by the tunnelled port...

	iBufferHeaders.Reset();
	iParamPortDefinition.bPopulated = OMX_FALSE;
	portDepopulationCompleted = ETrue;

	__ASSERT_DEBUG(iBufferHeaders.Count() == 0,
				   User::Panic(KOmxILPortPanicCategory, 1));

	return OMX_ErrorNone;

	}



EXPORT_C TBool
COmxILPort::SetBufferSent(OMX_BUFFERHEADERTYPE* apBufferHeader,
						  TBool& aBufferMarkedWithOwnMark)
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetBufferSent"));

	__ASSERT_DEBUG(apBufferHeader, User::Panic(KOmxILPortPanicCategory, 1));

	aBufferMarkedWithOwnMark = EFalse;

	TInt index = 0;
	if (KErrNotFound ==
		(index =
		 iBufferHeaders.Find(TBufferInfo(apBufferHeader),
							 TIdentityRelation<TBufferInfo>(
								 &TBufferInfo::Compare))))
		{
		return EFalse;
		}

	iBufferHeaders[index].SetBufferAway();

	if (!iBufferMarks.IsEmpty())
		{
		// Check for existing marks in the buffer header...
		if (apBufferHeader->hMarkTargetComponent)
			{
			// We queue the mark received within the buffer header if there are
			// marks already to be delivered... it is mandatory to give a FIFO
			// preference to the marks received by a port..
			if (iBufferMarks.Elements() < KMaxBufferMarksQueueSize)
				{
				// The buffer is marked already. Store the current mark at the end
				// of the buffer mark list...
				StoreBufferMark(apBufferHeader->hMarkTargetComponent,
								apBufferHeader->pMarkData);
				}
			}

		// Use the first mark in the queue...
		TBufferMarkInfo* pMark = iBufferMarks.First();
		apBufferHeader->hMarkTargetComponent = pMark->ipMarkTargetComponent;
		apBufferHeader->pMarkData			 = pMark->ipMarkData;
		aBufferMarkedWithOwnMark			 = pMark->iOwnMark;
		iBufferMarks.Remove(*pMark);
		delete pMark;
		}

	return ETrue;

	}

EXPORT_C TBool
COmxILPort::SetBufferReturned(OMX_BUFFERHEADERTYPE* apBufferHeader)
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetBufferReturned"));

	__ASSERT_DEBUG(apBufferHeader, User::Panic(KOmxILPortPanicCategory, 1));

	TInt index = 0;
	if (KErrNotFound ==
		(index =
		 iBufferHeaders.Find(TBufferInfo(apBufferHeader),
							 TIdentityRelation<TBufferInfo>(
								 &TBufferInfo::Compare))))
		{
		return EFalse;
		}

	iBufferHeaders[index].SetBufferAtHome();

	return ETrue;

	}

EXPORT_C void
COmxILPort::SetTransitionToEnabled()
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetTransitionToEnabled"));

	iTransitionState = EPortTransitioningToEnabled;
	iParamPortDefinition.bEnabled = OMX_TRUE;

	}

EXPORT_C void
COmxILPort::SetTransitionToDisabled()
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetTransitionToDisabled"));

	iTransitionState = EPortTransitioningToDisabled;
	iParamPortDefinition.bEnabled = OMX_FALSE;

	}

EXPORT_C void
COmxILPort::SetTransitionToDisabledCompleted()
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetTransitionToDisabledCompleted"));

	iTransitionState = EPortNotTransitioning;
	iParamPortDefinition.bEnabled = OMX_FALSE;

	}

EXPORT_C void
COmxILPort::SetTransitionToEnabledCompleted()
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetTransitionToEnabledCompleted"));

	iTransitionState = EPortNotTransitioning;
	iParamPortDefinition.bEnabled = OMX_TRUE;

	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::StoreBufferMark(const OMX_MARKTYPE* apMark)
	{
    DEBUG_PRINTF(_L8("COmxILPort::StoreBufferMark"));
	TBufferMarkInfo* pTBufferMarkInfo = new TBufferMarkInfo(apMark);
	if (!pTBufferMarkInfo)
		{
		return OMX_ErrorInsufficientResources;
		}

	iBufferMarks.AddLast(*pTBufferMarkInfo);

	return OMX_ErrorNone;

	}


/**
   This utility method may be called from the most derived port
   class' destructor to delete any buffer header and or buffer that may remain
   allocated in the port. This typically happens when the component is unloaded
   without being properly transitioned from OMX_StateIdle to OMX_StateLoaded.

 */
EXPORT_C void
COmxILPort::CleanUpPort()
	{

	// Do the clean-up here in case something went wrong and the component is
	// being unloaded in a failure scenario...
	const TInt headerCount = iBufferHeaders.Count();
	if (headerCount > 0)
		{
		if (!IsTunnelled())
			{
			// A non-tunnelled port needs to delete always the header and needs
			// to deallocate/unwrap the buffer depending on whether the buffer
			// is owned or not...

			RPointerArray<OMX_BUFFERHEADERTYPE> tempHeadersArray;
			for (TInt i=0; i<headerCount; ++i)
				{
				tempHeadersArray.Append(iBufferHeaders[i].GetHeader());
				}

			TBool portDepopulationCompleted = EFalse;
			for (TInt i=0; i<headerCount; ++i)
				{
				// Errors are ignored here ...
				FreeBuffer(tempHeadersArray[i], portDepopulationCompleted);
				}
			tempHeadersArray.Close();

			}
		else
			{
			if (IsTunnelledAndBufferSupplier())
				{
				// A tunnelled supplier only needs to delete the buffers, not
				// the buffer headers... Also, we cannot use the pointer to the
				// buffer header, as it may not exist anymore...
				for (TInt i=0; i<headerCount; ++i)
					{
					DoBufferDeallocation(
						iBufferHeaders[i].GetBufferPointer(),
						iBufferHeaders[i].GetPortPointer(),
						iBufferHeaders[i].GetPlatformPointer(),
						iBufferHeaders[i].GetAppPointer());
					}

				}
			else
				{
				// A tunnelled non-supplier needs to remove buffer headers and
				// undo the buffer wrapping, if any. We can use FreeBuffer for
				// that purpose...
				RPointerArray<OMX_BUFFERHEADERTYPE> tempHeadersArray;
				for (TInt i=0; i<headerCount; ++i)
					{
					tempHeadersArray.Append(iBufferHeaders[i].GetHeader());
					}

				TBool portDepopulationCompleted = EFalse;
				for (TInt i=0; i<headerCount; ++i)
					{
					//  errors here...
					FreeBuffer(tempHeadersArray[i], portDepopulationCompleted);
					}
				tempHeadersArray.Close();
				}
			}
		}

	}

/**
   This is to be overriden by concrete ports that either support more than
   one role or that want to let the IL Client to reset the component to
   some default state...

   @param aComponentRoleIndex The index of the role that is to be assumed by
   the component.

   @return OMX_ERRORTYPE
 */
EXPORT_C OMX_ERRORTYPE
COmxILPort::SetComponentRoleDefaults(TUint /*aComponentRoleIndex*/)
	{
    DEBUG_PRINTF(_L8("COmxILPort::SetComponentRoleDefaults"));

	return OMX_ErrorNotImplemented;
	}

/**
   This is to be overriden by concrete ports that either support more than one
   role or that want to let the IL Client to reset the component to some
   default state...

   @param aPortSettingsIndex An implementation-specific identifier that the
   implementation associates to the setting(s) that need(s) updating in the
   port.

   @param aPortSettings A buffer descriptor that contains an
   implementation-specific structure with the new setting(s) that need to
   be updated in the port.

   @param [output] aEventForILClient An event that the port may choose to
   deliver to the IL Client. This should be one of OMX_EventPortSettingsChanged
   or OMX_EventPortFormatDetected. Use OMX_EventMax if no event need to be
   sent.
 */
EXPORT_C OMX_ERRORTYPE
COmxILPort::DoPortReconfiguration(TUint /* aPortSettingsIndex */,
								  const TDesC8& /* aPortSettings */,
								  OMX_EVENTTYPE& aEventForILClient)
	{
    DEBUG_PRINTF(_L8("COmxILPort::DoPortReconfiguration"));

	// This means no need to notify the IL Client...
	aEventForILClient = OMX_EventMax;

	return OMX_ErrorNotImplemented;
	}

EXPORT_C OMX_ERRORTYPE
COmxILPort::StoreBufferMark(OMX_HANDLETYPE& ipMarkTargetComponent,
							OMX_PTR& ipMarkData)
	{
    DEBUG_PRINTF(_L8("COmxILPort::StoreBufferMark"));

	TBufferMarkInfo* pTBufferMarkInfo =
		new TBufferMarkInfo(ipMarkTargetComponent,
							ipMarkData, EFalse);
	if (!pTBufferMarkInfo)
		{
		return OMX_ErrorInsufficientResources;
		}

	iBufferMarks.AddLast(*pTBufferMarkInfo);

	return OMX_ErrorNone;

	}

EXPORT_C TBool
COmxILPort::HasAllBuffersAtHome() const
	{

	__ASSERT_ALWAYS(IsTunnelledAndBufferSupplier(),
					User::Panic(KOmxILPortPanicCategory, 1));

	const TInt headerCount = iBufferHeaders.Count();
	for (TInt i=0; i<headerCount; ++i)
		{
		if (!iBufferHeaders[i].IsBufferAtHome())
			{
			DEBUG_PRINTF(_L8("COmxILPort::HasAllBuffersAtHome : [NO]"));
			return EFalse;
			}
		}

	DEBUG_PRINTF(_L8("COmxILPort::HasAllBuffersAtHome : [YES]"));
	return ETrue;

	}

EXPORT_C TBool
COmxILPort::IsBufferAtHome(OMX_BUFFERHEADERTYPE* apBufferHeader) const
	{
	DEBUG_PRINTF2(_L8("COmxILPort::IsBufferAtHome : [%X]"), apBufferHeader);

	TInt headerIndex = 0;
	if (KErrNotFound ==
		(headerIndex =
		 iBufferHeaders.Find(TBufferInfo(apBufferHeader),
							 TIdentityRelation<TBufferInfo>(
								 &TBufferInfo::Compare))))
		{
		User::Panic(KOmxILPortPanicCategory, 1);
		}

	DEBUG_PRINTF2(_L8("COmxILPort::IsBufferAtHome : [%s]"), iBufferHeaders[headerIndex].IsBufferAtHome() ? "YES" : "NO");

	return iBufferHeaders[headerIndex].IsBufferAtHome();

	}


/**
   This method provides a mechanism for ports to place the port-specific
   buffer allocation logic (typically used in an OMX_AllocateBuffer
   scenario).

   @param aSizeBytes The size in bytes of the buffer to be allocated.

   @param [OUT] apPortSpecificBuffer A pointer to the memory area allocated by
   the port.

   @param [OUT] apPortPrivate A pointer that refers to an optional
   port-specific structure.

   @param apPlatformPrivate[OUT] A pointer to an optional platform-specific
   structure.

   @param apAppPrivate A pointer that refers to a buffer supplier-specific
   structure.

   @return OMX_ERRORTYPE
*/
EXPORT_C OMX_ERRORTYPE
COmxILPort::DoBufferAllocation(OMX_U32 aSizeBytes,
							   OMX_U8*& apPortSpecificBuffer,
							   OMX_PTR& apPortPrivate,
							   OMX_PTR& /* apPlatformPrivate */,
							   OMX_PTR /* apAppPrivate = 0 */)
	{
	DEBUG_PRINTF2(_L8("COmxILPort::DoBufferAllocation : aSizeBytes[%u]"), aSizeBytes);

	__ASSERT_DEBUG(aSizeBytes > 0, User::Panic(KOmxILPortPanicCategory, 1));

	CMMFDescriptorBuffer* pDescBuffer = 0;
	TRAPD(allocRes, pDescBuffer = CMMFDescriptorBuffer::NewL(aSizeBytes));
	if (KErrNone != allocRes)
		{
		return OMX_ErrorInsufficientResources;
		}

	apPortSpecificBuffer = const_cast<TUint8*>(pDescBuffer->Data().Ptr());
	apPortPrivate		 = static_cast<CMMFBuffer*>(pDescBuffer);

	return OMX_ErrorNone;

	}

/**
   This method provides a mechanism for ports to place the port-specific
   buffer deallocation logic (typically used in an OMX_FreeBuffer
   scenario).

   @param apPortSpecificBuffer A pointer to the memory area to be deallocated
   by the port.

   @param apPortPrivate A pointer that refers to a port-specific structure.

   @param apPlatformPrivate A pointer to an optional platform-specific
   structure.

   @param apAppPrivate A pointer that refers to a buffer supplier-specific
   structure.

*/
EXPORT_C void
COmxILPort::DoBufferDeallocation(OMX_PTR /*apPortSpecificBuffer */,
								 OMX_PTR apPortPrivate,
								 OMX_PTR /* apPlatformPrivate */,
								 OMX_PTR /* apAppPrivate = 0 */)
	{
	DEBUG_PRINTF(_L8("COmxILPort::DoBufferDeallocation"));

	__ASSERT_DEBUG(apPortPrivate, User::Panic(KOmxILPortPanicCategory, 1));

	delete reinterpret_cast<CMMFBuffer*>(apPortPrivate);

	}

/**
   This method provides a mechanism for ports to place the port-specific buffer
   wrapping logic (typically used in an OMX_UseBuffer scenario).

   @param aSizeBytes The size in bytes of the buffer to be wrapped.

   @param apBuffer A pointer received from the IL Client or another
   component that references the allocated memory area .

   @param [OUT] apPortPrivate A pointer that refers to a port-specific
   structure.

   @param [OUT] apPlatformPrivate A pointer to an optional platform-specific
   structure.

   @param [OUT] apAppPrivate A pointer that refers to a buffer
   supplier-specific structure.

   @return OMX_ERRORTYPE
*/
EXPORT_C OMX_ERRORTYPE
COmxILPort::DoBufferWrapping(OMX_U32 aSizeBytes,
							 OMX_U8* apBuffer,
							 OMX_PTR& apPortPrivate,
							 OMX_PTR& /* apPlatformPrivate */,
							 OMX_PTR /* apAppPrivate = 0 */)
	{
	DEBUG_PRINTF(_L8("COmxILPort::DoBufferWrapping"));

	__ASSERT_DEBUG(aSizeBytes > 0 && apBuffer,
				   User::Panic(KOmxILPortPanicCategory, 1));

	CMMFBuffer* pMmfBuffer = 0;
	TPtr8 ptr(apBuffer, aSizeBytes, aSizeBytes);
	TRAPD(allocRes, pMmfBuffer = CMMFPtrBuffer::NewL(ptr));
	if (KErrNone != allocRes)
		{
		return OMX_ErrorInsufficientResources;
		}

	apPortPrivate = pMmfBuffer;

	return OMX_ErrorNone;

	}

/**
   This method provides a mechanism for ports to place the port-specific buffer
   unwrapping logic (typically used in an OMX_FreeBuffer scenario).

   @param apPortSpecificBuffer A pointer to the allocated memory area.

   @param apPortPrivate A pointer that refers to a port-specific structure.

   @param apPlatformPrivate A pointer to an optional platform-specific
   structure.

   @param apAppPrivate A pointer that refers to a buffer supplier-specific
   structure.

*/
EXPORT_C void
COmxILPort::DoBufferUnwrapping(OMX_PTR /* apBuffer*/,
							   OMX_PTR appPortPrivate,
							   OMX_PTR /* apPlatformPrivate */,
							   OMX_PTR /* apAppPrivate = 0 */)
	{

	DEBUG_PRINTF(_L8("COmxILPort::DoBufferUnwrapping"));

	__ASSERT_DEBUG(appPortPrivate, User::Panic(KOmxILPortPanicCategory, 1));

	delete reinterpret_cast<CMMFBuffer*>(appPortPrivate);

	}

/**
   This method gets called during tunnelled buffer allocation (i.e.,
   PopulateTunnel). This is to allow component implementations to override this
   default implementation in a way that the pAppPrivate pointer parameter of
   OMX_UseBuffer can be used to convey pPortPrivate or pPlatformPrivate to the
   tunnelled component.

   @param aTunnelledComponent Handle to the tunnelled component

   @param [OUT] appBufferHdr The buffer header that will be allocated by the
   tunnelled component

   @param aTunnelledPortIndex The index of the tunnelled port

   @param apPortPrivate pPortPrivate pointer as returned by DoBufferAllocation

   @param apPlatformPrivate pPlatformPrivate pointer as returned by
   DoBufferAllocation

   @param aSizeBytes The size in bytes of the to be allocated buffer

   @param apBuffer A pointer to the allocated buffer

   @return OMX_ERRORTYPE

*/
EXPORT_C OMX_ERRORTYPE
COmxILPort::DoOmxUseBuffer(OMX_HANDLETYPE aTunnelledComponent,
						   OMX_BUFFERHEADERTYPE** appBufferHdr,
						   OMX_U32 aTunnelledPortIndex,
						   OMX_PTR /* apPortPrivate */,
						   OMX_PTR /* apPlatformPrivate */,
						   OMX_U32 aSizeBytes,
						   OMX_U8* apBuffer)
	{

	DEBUG_PRINTF(_L8("COmxILPort::DoOmxUseBuffer"));

	return OMX_UseBuffer(
		aTunnelledComponent,
		appBufferHdr,
		aTunnelledPortIndex,
		0,
		aSizeBytes,
		apBuffer);

	}

EXPORT_C TBool
COmxILPort::TBufferInfo::Compare(const TBufferInfo& aBi1,
								 const TBufferInfo& aBi2)
	{
	return (aBi1.GetHeader() == aBi2.GetHeader() ? ETrue : EFalse);
	}
