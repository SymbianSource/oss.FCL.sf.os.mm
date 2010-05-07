/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

COmxILPcmProcessorProcessingFunction::TPcmEncodingType::TPcmEncodingType() : iEncoding(EPcmEncodingLinear), iWidth(EPcmWidth16bitLE), iSignedness(EPcmSigned)
			{
			}
		
COmxILPcmProcessorProcessingFunction::TPcmEncodingType::TPcmEncodingType(TEncoding aEncoding, TWidthAndEndianness aWidth, TSignedness aSignedness)
	: iEncoding(aEncoding), iWidth(aWidth), iSignedness(aSignedness)
	{
	}

TInt COmxILPcmProcessorProcessingFunction::TPcmEncodingType::SetFromPcmMode(OMX_AUDIO_PCMMODETYPE aPCMMode, OMX_U32 aBitsPerSample, OMX_ENDIANTYPE aEndian, OMX_NUMERICALDATATYPE aSigned)
	{
	TInt err = KErrNone;
	
	// set the encoding type
	if (aPCMMode == OMX_AUDIO_PCMModeLinear) 
		{
		iEncoding = EPcmEncodingLinear;
		}
	else if (aPCMMode == OMX_AUDIO_PCMModeMULaw) 
		{
		iEncoding = EPcmEncodingMuLaw;
		}
	else if (aPCMMode == OMX_AUDIO_PCMModeALaw) 
		{
		iEncoding = EPcmEncodingALaw;
		}
	else
		{
		err = KErrNotSupported;
		}

	// if this is alaw or mulaw encoded PCM, set the width
	if (aPCMMode == OMX_AUDIO_PCMModeALaw || aPCMMode == OMX_AUDIO_PCMModeMULaw )
		{
		if (aBitsPerSample==8)
			{
			iWidth = EPcmWidth8bit;
			}
		else
			{
			err = KErrNotSupported;
			}
		}

	// if this is linear encoded PCM, set width and endianness
	if ( aPCMMode == OMX_AUDIO_PCMModeLinear )
		{
		if (aBitsPerSample == 8)
			{
			iWidth = EPcmWidth8bit;
			}
		else if (aBitsPerSample==16 && aEndian==OMX_EndianBig)
			{
			iWidth = EPcmWidth16bitBE;
			}
		else if (aBitsPerSample==16 && aEndian==OMX_EndianLittle)
			{
			iWidth = EPcmWidth16bitLE;
			}
		else
			{
			err = KErrNotSupported;
			}
		}
	
	// set the signedness
	if (iEncoding==EPcmEncodingLinear)
		{
		iSignedness = (aSigned == OMX_NumericalDataSigned ? EPcmSigned : EPcmUnsigned );
		}
	else
		{
		iSignedness = EPcmSigned;
		}
	
	return err;
	}

TUint32 COmxILPcmProcessorProcessingFunction::TPcmEncodingType::Hash() const
	{
	return TPcmEncodingType::PcmEncodingHash(iEncoding, iWidth, iSignedness);
	}

// The maximum value of a PCM encoding hash value
TUint COmxILPcmProcessorProcessingFunction::TPcmEncodingType::PcmMaxHash()
	{
	return static_cast<TUint>(TPcmEncodingType::EPcmEncodingMax)		
		* static_cast<TUint>(TPcmEncodingType::EPcmWidthMax)		
		* static_cast<TUint>(TPcmEncodingType::EPcmSignednessMax);	
	}
	
// Combines two valid PCM encoding hash values into a single value
TUint COmxILPcmProcessorProcessingFunction::TPcmEncodingType::PcmPairHash(TUint aLeft, TUint aRight )
			{
			return aLeft + (TPcmEncodingType::PcmMaxHash() * aRight);
			}
