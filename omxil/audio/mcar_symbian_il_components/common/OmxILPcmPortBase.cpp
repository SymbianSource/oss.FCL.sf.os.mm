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

// Self
#include "OmxILPcmPortBase.h"

// Project (MMP)
/* None */

// Solution (INF)
#include <openmax/il/khronos/v1_x/OMX_Component.h>
#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/log.h>
#include <openmax/il/common/omxilutil.h>
#else
#include "log.h"
#include "omxilutil.h"
#endif

// System
/* None */

_LIT8(KMimeTypeAudioPcm, "audio/pcm");

/**
 * Constructor.
 * Derived classes must call this function.
 */
COmxILPcmPortBase::COmxILPcmPortBase(const TOmxILCommonPortData& aCommonPortData, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType)
	:	COmxILAudioPort(aCommonPortData), iParamAudioPcm(aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILPcmDecoderPort::NewL")); 
	}
	 
/**
 * Second stage constructor.
 * Note: Derived classes must call this function as part of construction.
 */
void COmxILPcmPortBase::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	DEBUG_PRINTF(_L8("COmxILPcmDecoderPort::ConstructL"));
	// Debug checks (release will leave in ConstructL instead)
	ASSERT(aSupportedAudioFormats.Count() == 1);
	ASSERT(aSupportedAudioFormats[0] == OMX_AUDIO_CodingPCM);
	if (aSupportedAudioFormats.Count() != 1 || aSupportedAudioFormats[0] != OMX_AUDIO_CodingPCM)
		{
		User::Leave(KErrNotSupported);
		}
	
	COmxILAudioPort::ConstructL(aSupportedAudioFormats);
	//
	// Setup the iParamPortDefinition (output info)
	
	// Before constructL completes, we must have set (and be finished with) the iParamPortDefinition
	iParamPortDefinition.eDomain = OMX_PortDomainAudio;
	
	TUint8* mimeptr = const_cast<TUint8*>(KMimeTypeAudioPcm().Ptr());	// KMimeTypeAudioPcm is already null-terminated since it's made from a literal C-style string
	iParamPortDefinition.format.audio.cMIMEType = reinterpret_cast<OMX_STRING>(mimeptr);
	
	iParamPortDefinition.format.audio.bFlagErrorConcealment = OMX_FALSE;
	iParamPortDefinition.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
	}

/**
 * Destructor.
 */
COmxILPcmPortBase::~COmxILPcmPortBase()
	{
	DEBUG_PRINTF(_L8("COmxILPcmDecoderPort::~COmxILPcmDecoderAPB0Port"));
	// nothing to do
	}

/**
 * From COmxILPort.
 * Note: Derived classes should call this function if implementing it themselves.
 * @param aIndexArray
 * @return
 */ 
OMX_ERRORTYPE COmxILPcmPortBase::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILPcmDecoderPort::GetLocalOmxParamIndexes"));

	// Always collect param indexes from parent
	OMX_ERRORTYPE omxRetValue = COmxILAudioPort::GetLocalOmxParamIndexes(aIndexArray);
	if (OMX_ErrorNone != omxRetValue)
		{
		return omxRetValue;
		}

	TInt err = aIndexArray.InsertInOrder(OMX_IndexParamAudioPcm);
	if (KErrNone != err && KErrAlreadyExists != err)	// Note that index duplication is OK.
		{
		return OMX_ErrorInsufficientResources;
		}

	return OMX_ErrorNone;
	}

/**
 * From COmxILPort.
 * Note: Derived classes should call this function if implementing it themselves.
 * @param aIndexArray
 * @return
 */
OMX_ERRORTYPE COmxILPcmPortBase::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("OmxILPcmPortBase::GetLocalOmxConfigIndexes"));
    
    // Call parent
    return COmxILAudioPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

/**
 * From COmxILPort.
 * Note: Derived classes should call this function if implementing it themselves.
 * @param aParamIndex
 * @param apComponentParameterStructure
 * @return
 */
OMX_ERRORTYPE COmxILPcmPortBase::GetParameter(OMX_INDEXTYPE aParamIndex, TAny* apComponentParameterStructure) const
	{
    DEBUG_PRINTF(_L8("OmxILPcmPortBase::GetParameter"));

    // We only have one parameter - OMX_IndexParamAudioPcm - pass all other requests to our parent
    if (aParamIndex != OMX_IndexParamAudioPcm)
		{
		return COmxILAudioPort::GetParameter(aParamIndex, apComponentParameterStructure);
		}
    
    OMX_ERRORTYPE err = TOmxILUtil::CheckOmxStructSizeAndVersion( apComponentParameterStructure, sizeof iParamAudioPcm );
    if (err==OMX_ErrorNone)
    	{
    	// export our PCM mode settings
    	OMX_AUDIO_PARAM_PCMMODETYPE* pcmmode = static_cast<OMX_AUDIO_PARAM_PCMMODETYPE*>(apComponentParameterStructure);
    	*pcmmode = iParamAudioPcm;
    	}
    
	return err;
	}

/**
 * From COmxILPort.
 * Note: Derived classes should call this function if implementing it themselves.
 * @param aParamIndex
 * @param apComponentParameterStructure
 * @param aUpdateProcessingFunction
 * @return
 */
OMX_ERRORTYPE COmxILPcmPortBase::SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure, TBool& aUpdateProcessingFunction)
	{
    DEBUG_PRINTF(_L8("OmxILPcmPortBase::SetParameter"));

    // We only have one parameter - OMX_IndexParamAudioPcm - pass all other requests to our parent
    if (aParamIndex != OMX_IndexParamAudioPcm)
		{
		return COmxILAudioPort::SetParameter(aParamIndex, apComponentParameterStructure, aUpdateProcessingFunction);
		}
    
    OMX_ERRORTYPE err = TOmxILUtil::CheckOmxStructSizeAndVersion( const_cast<TAny*>(apComponentParameterStructure), sizeof iParamAudioPcm );
    if (err!=OMX_ErrorNone)
    	{
    	return err;
    	}

    // get and sanity check the new settings
	const OMX_AUDIO_PARAM_PCMMODETYPE* pcmmode = static_cast<const OMX_AUDIO_PARAM_PCMMODETYPE*>(apComponentParameterStructure);
	if (!CheckPcmModeParamsAreSane(*pcmmode))
		{
		return OMX_ErrorBadParameter;
		}

	// preserve the new settings and update the processing function
	iParamAudioPcm = *pcmmode;
	aUpdateProcessingFunction = ETrue;
	
	return OMX_ErrorNone;
	}

/**
 * From COmxILPort.
 * @param aPortDefinition
 * @param aUpdateProcessingFunction
 * @return
 */
OMX_ERRORTYPE COmxILPcmPortBase::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, TBool& aUpdateProcessingFunction)
	{
	DEBUG_PRINTF(_L8("OmxILPcmPortBase::SetFormatInPortDefinition"));

	// Currently not changing cMIMEType type of this port
	// Same thing for pNativeRender and eEncoding
	iParamPortDefinition.format.audio.bFlagErrorConcealment = aPortDefinition.format.audio.bFlagErrorConcealment;

	// Error concealment not currently needed at the processing function
	aUpdateProcessingFunction = EFalse;

	return OMX_ErrorNone;
	}

/**
 * From COmxILPort.
 * @param aPortDefinition
 * @return
 */
TBool COmxILPcmPortBase::IsTunnelledPortCompatible( const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
	DEBUG_PRINTF(_L8("OmxILPcmPortBase::IsTunnelledPortCompatible"));
	
	#ifdef OMXIL_PERMISSIVE_PORT_TUNNELING
		
		// TODO: Enable correct compatibility check
		(void) aPortDefinition;
		return ETrue;
		
	#else

		// Check the domain and encodings match
		// Note: OMX_AUDIO_CodingUnused is permitted for interoperability with the test filesource and filesink components
		
		if(aPortDefinition.eDomain != iParamPortDefinition.eDomain
			|| ( aPortDefinition.format.audio.eEncoding != OMX_AUDIO_CodingUnused && aPortDefinition.format.audio.eEncoding != iParamPortDefinition.format.audio.eEncoding))
			{
			return EFalse;
			}
		
	#ifdef OMXIL_VERIFY_MIME_TYPE
		// TODO: Consider removing this check completely
		// check that the MIME types match
		// Note: cMIMEType==NULL is permitted for interoperability with the test filesource and filesink components
		if (aPortDefinition.format.audio.cMIMEType)
			{
			TPtrC8 tunnelMimeType( reinterpret_cast<TUint8*>(aPortDefinition.format.audio.cMIMEType), User::StringLength(reinterpret_cast<TUint8*>(aPortDefinition.format.audio.cMIMEType)) );
			TPtrC8 myMimeType( reinterpret_cast<TUint8*>(iParamPortDefinition.format.audio.cMIMEType), User::StringLength(reinterpret_cast<TUint8*>(iParamPortDefinition.format.audio.cMIMEType)) );
			
			if (tunnelMimeType != myMimeType)
				{
				return EFalse;
				}
			}
	#endif
	
		return ETrue;
		
	#endif
	}

/**
 * Validates that parameters are valid symanticly.
 * Any further checks should be called in
 * @param aPcmModeType
 * @return
 */
TBool COmxILPcmPortBase::CheckPcmModeParamsAreSane(const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType) const
	{
	// Verify enums are below their maximum values ( as of the v1.1.1 )
	if (!(     aPcmModeType.nPortIndex        == iParamAudioPcm.nPortIndex
			&& aPcmModeType.eNumData 		  <= OMX_NumericalDataUnsigned
			&& aPcmModeType.eEndian  	      <= OMX_EndianLittle
			&& (aPcmModeType.nBitPerSample    == 8
				|| aPcmModeType.nBitPerSample == 16)
			&& aPcmModeType.ePCMMode 		  <= OMX_AUDIO_PCMModeMULaw))
		{
		return EFalse;
		}
	
	// Verify anything more specific in the derived class
	if (!ValidatePcmModeParams(aPcmModeType))
		{
		return EFalse;
		}
	
	// All OK
	return ETrue;
	}

/**
 * Non-operational in base class @C COmxILPcmPortBase.
 * 
 * A derived class can (though is not required to) override this function.
 * Use to implement any further checking of parameters beyond that
 * provided in @C CheckPcmModeParamsAreSane.
 * 
 * Called from @C CheckPcmModeParamsAreSane.
 * 
 * @param aPcmModeType	Parameters to check
 * @return				Should return @C ETrue if valid, else @C EFalse.
 */
TBool COmxILPcmPortBase::ValidatePcmModeParams(const OMX_AUDIO_PARAM_PCMMODETYPE& /*aPcmModeType*/) const
	{
	// Perform no further validation in this class.
	return ETrue;
	}
