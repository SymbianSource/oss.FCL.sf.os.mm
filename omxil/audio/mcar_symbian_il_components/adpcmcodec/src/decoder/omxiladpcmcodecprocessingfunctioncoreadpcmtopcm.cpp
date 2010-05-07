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


#include "omxiladpcmcodecprocessingfunctioncoreadpcmtopcm.h"



// IMA-ADPCM step variation table 
const TInt COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::KIndexTable[] =
 	{
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8
	};

const TInt COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::KStepSizeTable[89] = 
	{
   7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
   19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
   50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
   130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
   337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
   876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
   2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
   5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
   15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
	};


COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm* COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::NewL(COmxILAdPcmCodecPort& aPb0Port, COmxILPcmPortAdPcmVariant& aPb1Port )
	{
	COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm* self 
		= new(ELeave)COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm(aPb0Port, aPb1Port);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm(COmxILAdPcmCodecPort& aPb0Port, COmxILPcmPortAdPcmVariant& aPb1Port )
	: ipb0Port( aPb0Port )
	, ipb1Port( aPb1Port )
	{
	}

COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::~COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm()
	{
	}

TUint COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::BitsPerSample() const 
	{
	return ipb0Port.ModeType().nBitsPerSample;
	}

TUint COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::Channels() const 
	{
	return ipb0Port.ModeType().nChannels;
	}

TUint COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::SampleRate() const 
	{
	return ipb0Port.ModeType().nSampleRate;
	}

TUint COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::InternalOutputBufferSize() const 
	{
	return (Channels() == 1 ) ? iInternalOutputBufferSize : KImaAdpcmStereoTempBufferSize ;
	}

TUint COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::HowMuchOneConvertEats() const 
	{
	return iBlockAlign;
	}

TBool COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::ForgetNotAlignedInputBuffers() const 
	{
	return ETrue;
	}

TInt COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::CheckConversion()
	{
	//SetParameter checks if number bits per sample is 4

	TInt err = KErrNone;
	// verify that sample rates and channel assignments match
	if ( (ipb0Port.ModeType().nSampleRate != ipb1Port.ModeType().nSamplingRate)
		|| (ipb0Port.ModeType().nChannels != ipb1Port.ModeType().nChannels) 
		//|| (ipb0Port.ModeType().nBitsPerSample != ipb1Port.ModeType().nBitPerSample)
		||	!ipb1Port.ModeType().bInterleaved
		) 
		{
		err = KErrNotSupported;
		}
	
	return err;
	}

/**
*
* Convert
* @param aSrc
* @param aDst
* @param aSamples
*
*/
void COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::Convert(const TAny* aSrc, TAny* aDst, TInt aSamples )
	{
	//const TInt samples = ipb0Port.ModeType().nChannels == 2 ? KImaAdpcmStereoSamplesPerBlock : KImaAdpcmSamplesPerBlock;
	Convert( static_cast<const TUint8*>(aSrc), static_cast<TUint8*>(aDst), aSamples );
	}

/**
*
* Convert
* This is the orginial version from 
* //EPOC/DV3/team/2008/multimedia/personal/extianwoods2/EmccOmxILProject/MMF/Codecs/Src/MMFCodecCommon/
* Changes: aSrc was made const
* @param aSrc
* @param aDst
* @param aSamples
*
*/
void COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::Convert(const TUint8* aSrc, TUint8* aDst, TInt aSamples)
	{
	TInt delta;			// Current adpcm output value 
    TInt step;			// Stepsize
    TInt valpred;		// Predicted value 
    TInt vpdiff;		// Current change to valpred 
    TInt index;			// Current step change index 

	//[Read first sample and index from block header
	// we do not need to store the information across calls 
	//since we process the entire block here]
	valpred = (*aSrc++) & KAndMask8bit;
	valpred |= STATIC_CAST(TInt16, ((*aSrc++) << 8));
	index = *aSrc++;
	TUint8* dst=aDst;

	aSrc++; //skip reserved header byte

	//Write first sample to dest
	*aDst++ = STATIC_CAST( TUint8, valpred);
	*aDst++ = STATIC_CAST( TUint8, valpred >> 8);
	dst += 2;
	aSamples --;

	TBool theBufferStep = ETrue;
    TInt  bufferValue = 0;
	for ( ; aSamples > 0 ; aSamples-- ) 
		{ 
		// Step 1 - get the delta value
		if ( theBufferStep) 
			{
			bufferValue = *aSrc++;
			delta = bufferValue & 0xf;
			} 
		else 
			{
			delta = (bufferValue >> 4) & 0xf;
			}

		theBufferStep = !theBufferStep;

		ASSERT(index >= 0);
		step = KStepSizeTable[index];

		vpdiff = step>>3;
		if ( delta & 4 )
			{
			vpdiff += step;
			}
		if ( delta & 2 )
			{
			vpdiff += step>>1;
			}
		if ( delta & 1 ) 
			{
			vpdiff += step>>2;
			}

		if ( delta & 8 )
			{
			valpred -= vpdiff;
			}
		else
			{
			valpred += vpdiff;
			}

		if ( valpred > (KClamp - 1) )
			{
			valpred = (KClamp - 1);
			}
		else if ( valpred < -KClamp )
			{
			valpred = -KClamp;
			}

		index += KIndexTable[delta];
		if ( index < 0 ) 
			{
			index = 0;
			}
		if ( index > KMaxImaAdpcmTableEntries ) 
			{
			index = KMaxImaAdpcmTableEntries;
			}

		*dst++ = STATIC_CAST( TUint8, valpred&KAndMask8bit);
		*dst++ = STATIC_CAST( TUint8, (valpred>>8) );
		}
	}


//from TInt CMMFImaAdpcmToPcm16Codec::Configure(TUint aChannels, TUint aSampleRate, TUint aBlockAlign)
TInt COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::Configure()
    {
    TUint channels = Channels();
    TUint sampleRate = SampleRate();
    
	// TODO: The logic below should be revised when refactoring the whole encoder/decoder code.
	// This logic doesn't allow for setting blocks smaller than 256. However, it seems this limitation
	// doesn't adhere to the standard that allows for smaller blocks. Therefore, the logic has been
	// overwritten for the SetBlockAlign() calls.
    if (iCustomBlockAlign > 0)
        {
        iBlockAlign = iCustomBlockAlign;
        }
    else
        {
        if (iBlockAlign < 256)  
            {
            switch (sampleRate * channels)
                {
                case 8000: // fall through, same as 11025
                case 11025:
                case 16000:
                    iBlockAlign = 256;
                    break;
                case 22050:
                    iBlockAlign = 512;
                    break;
                    
                case 44100:
                    iBlockAlign = 1024;
                    break;
                    
                case 88200:
                    iBlockAlign = 2048;
                    break;
                    
                default:
                    return KErrArgument;
                }
            }
        }
    
    const TUint KImaAdpcmBitsPerSample = 4;
    iSamplesPerBlock = (iBlockAlign - 4 * channels) * 8 / (KImaAdpcmBitsPerSample * channels) + 1;
    
	if (iInternalOutputBuffer == NULL || (iInternalOutputBufferSize != iSamplesPerBlock*2))
		{
		iInternalOutputBufferSize = iSamplesPerBlock*2;
		delete [] iInternalOutputBuffer;
		iInternalOutputBuffer = new OMX_U8[iInternalOutputBufferSize];
		if (iInternalOutputBuffer == NULL)
			{
			iInternalOutputBufferSize = 0;
			return KErrNoMemory;
			}
		
		}
    
    return KErrNone;
    }

OMX_ERRORTYPE COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm::SetBlockAlign(TInt aBlockAlign)
    {
    if (aBlockAlign < KImaAdpcmMinBlockAlign ||
        aBlockAlign > KImaAdpcmMaxBlockAlign ||
        aBlockAlign % KImaAdpcmMinBlockAlign != 0)
        {
        return OMX_ErrorBadParameter; 
        }
    
    iCustomBlockAlign = aBlockAlign;
    
    return OMX_ErrorNone;
    }
