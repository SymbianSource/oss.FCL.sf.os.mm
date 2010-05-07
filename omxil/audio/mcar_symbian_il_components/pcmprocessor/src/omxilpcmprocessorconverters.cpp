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

#include <e32base.h>
#include "omxilpcmprocessorconverters.h"

//
// PCM encoding conversion functions
//

namespace NOmxILPcmProcessorConverters
{

	const TInt KMaskSign16bit = 0x8000;
	const TInt KMaskSign8bit = 0x80;
	const TInt KAndMask8bit = 0xff;

	// PCM converter for cases where no conversion is necessary
	void PCMConverterNoConversion(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		TUint maxcopy = Min(aInLength, aOutLength);
		Mem::Copy(aOut, aIn, maxcopy);
		
		aInConsumed = aOutUsed = maxcopy;
		}


	// Convert from unsigned 8 to signed 16. This conversion is regardless of endianness!
	void PCMConverterU8toS16(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);
		
		// samples is the number of input samples. Each input byte produces 2 output bytes.
		TUint samples = Min(aInLength, aOutLength/2);
		
		aInConsumed = samples;
		aOutUsed = samples*2;
		
		while (samples--)
			{
			*dest++ = STATIC_CAST( TUint8, *src^KMaskSign8bit );
			*dest++ = STATIC_CAST( TUint8, *src++^KMaskSign8bit );
			}
		}

	// Convert from unsigned 16 LE to signed 16 LE
	void PCMConverterU16LEtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TInt8* dest = static_cast<TInt8*>(aOut);
		
		// samples is the number of input or output samples. Each input byte produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength/2);
		aInConsumed = aOutUsed = samples*2;
		
		TInt s16;
		while (samples--)
			{ 
			s16 = (src[0]+(src[1]<<8))-KMaskSign16bit;
			*dest++ = STATIC_CAST( TInt8, s16&KAndMask8bit);
			*dest++ = STATIC_CAST( TInt8, (s16>>8)&KAndMask8bit);
			src+=2;
			}
		}

	// Convert from Signed 16bit LE to Unsigned 8bit
	void PCMConverterS16LEtoU8(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);

		// samples is the number of output samples. Each 2 input bytes produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength);
		
		aInConsumed = samples*2;
		aOutUsed = samples;
		
		while (samples--)
			{
			src++;
			*dest++ = STATIC_CAST( TUint8, (*src++) - KMaskSign8bit);
			}
		}
	
	// Convert from Unsigned 16 BE to Signed 16 LE
	void PCMConverterU16BEtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TInt8* dest = static_cast<TInt8*>(aOut);
		
		// samples is the number of input or output samples. Each input byte produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength/2);
		aInConsumed = aOutUsed = samples*2;
		
		TInt s16;
		while (samples--)
			{ 
			s16 = ((src[0]<<8)+src[1])-KMaskSign16bit;
			*dest++ = STATIC_CAST( TInt8, s16&KAndMask8bit);
			*dest++ = STATIC_CAST( TInt8, (s16>>8)&KAndMask8bit);
			src+=2;
			}
		}

	// Convert from Signed 16 LE to Signed 8 bit
	void PCMConverterS16LEtoS8(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TInt8* dest = static_cast<TInt8*>(aOut);

		// samples is the number of output samples. Each 2 input bytes produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength);
		
		aInConsumed = samples*2;
		aOutUsed = samples;
		
		while (samples--)
			{ 
			*dest++ = STATIC_CAST( TInt8, (src[0]+(src[1]<<8))>>8);
			src += 2;
			}
		}

	// Convert from Signed 8bit to Signed 16bit LE
	void PCMConverterS8toS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TInt8* src = static_cast<const TInt8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);
		
		// samples is the number of input samples. Each input byte produces 2 output bytes.
		TUint samples = Min(aInLength, aOutLength/2);
		
		aInConsumed = samples;
		aOutUsed = samples*2;
		
		TInt16 s16;
		while (samples--)
			{ 
			s16 = static_cast<TInt16>((*src++)<<8);
			*dest++ = STATIC_CAST( TUint8, s16&KAndMask8bit);
			*dest++ = STATIC_CAST( TUint8, (s16>>8)&KAndMask8bit);		
			}
		}

	// Convert from Signed 16bit LE to Unsigned 16bit LE
	void PCMConverterS16LEtoU16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TInt8* dest = static_cast<TInt8*>(aOut);
		
		// samples is the number of input or output samples. Each input byte produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength/2);
		aInConsumed = aOutUsed = samples*2;
		
		TInt s16;
		while (samples--)
			{ 
			s16 = ((src[1]<<8)+src[0])-KMaskSign16bit;
			*dest++ = STATIC_CAST( TInt8, s16&KAndMask8bit);
			*dest++ = STATIC_CAST( TInt8, (s16>>8)&KAndMask8bit);
			src += 2;
			}
		}

	// Convert from Signed 16bit LE to Unsigned 16bit BE
	void PCMConverterS16LEtoU16BE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TInt8* dest = static_cast<TInt8*>(aOut);
		
		// samples is the number of input or output samples. Each input byte produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength/2);
		aInConsumed = aOutUsed = samples*2;
		
		TInt s16;
		while (samples--)
			{ 
			s16 = ((src[1]<<8)+src[0])+KMaskSign16bit;
			*dest++ = STATIC_CAST( TInt8, (s16>>8)&KAndMask8bit);
			*dest++ = STATIC_CAST( TInt8, s16&KAndMask8bit);
			src += 2;
			}
		}

	// Convert from Signed 16 bit toggling endiannes (i.e. LE to BE, BE to LE) 
	void PCMConverterS16SwapEndianness(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);
		
		// samples is the number of input or output samples. Each input byte produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength/2);
		aInConsumed = aOutUsed = samples*2;
		
		while (samples--)
			{ 
			*dest++ = src[1];
			*dest++ = src[0];
			src += 2;
			}
		}
	
	//
	// ALaw conversions
	//
	
	// unnamed namespace to make Alaw snigle sample functions private
	namespace
	{
		TInt AlawSampleToPcm(TUint8 aAlaw)
			{
			const TInt KExpLutSize = 8;
			const TInt KExpLut[KExpLutSize] = 
				{
				0, 264, 528, 1056, 2112, 4224, 8448, 16896
				};
	
			TInt sign, exponent, mantissa, sample;
			
		    aAlaw ^= 0x55;	//will leave this as is, more readable
		    sign = ( aAlaw & KMaskSign8bit );
		    aAlaw &= 0x7f;			/* get magnitude */
		    if (aAlaw >= 16)
				{
				exponent = (aAlaw >> 4 ) & 0x07;
				mantissa = aAlaw & 0x0F;
				sample = KExpLut[exponent] + ( mantissa << ( exponent + 3 ) );
				}
		    else
				sample = (aAlaw << 4) + 8;
			
		    if ( sign == 0 )
				{
				sample = -sample;
				}
			
		    return sample;
			}
	
		TUint8 PcmSampleToAlaw(TInt aPcm)
			{
			const TInt KMda16PcmToAlawClip       = 31744;

			//[ conversion look up table pcm16 to alaw ]
			const TInt KExpLutSize = 128;
			const TInt8 KExpLut[KExpLutSize] = 
				{
				1,1,2,2,3,3,3,3,
				4,4,4,4,4,4,4,4,
				5,5,5,5,5,5,5,5,
				5,5,5,5,5,5,5,5,
				6,6,6,6,6,6,6,6,
				6,6,6,6,6,6,6,6,
				6,6,6,6,6,6,6,6,
				6,6,6,6,6,6,6,6,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7
				};
	
			TInt sign;
			TInt exponent;
			TInt mantissa;
			TUint8 alawbyte;
	
		    sign = ((~aPcm) >> 8) & KMaskSign8bit;		
		    if (sign == 0)
				{
				aPcm = -aPcm;
				}
		    if (aPcm > KMda16PcmToAlawClip) 
				{
				aPcm = KMda16PcmToAlawClip;
				}
	
		    if (aPcm >= 256)
				{
				exponent = KExpLut[( aPcm >> 8 ) & 0x7F];
				mantissa = ( aPcm >> ( exponent + 3 ) ) & 0x0F;
				alawbyte = STATIC_CAST( TUint8, (( exponent << 4 ) | mantissa));
				}
		    else
				{
				alawbyte = STATIC_CAST( TUint8, aPcm >> 4);
				}
	
		    alawbyte ^= (sign ^ 0x55);
	
		    return alawbyte;
			}
	}
	
	
	
	// Convert from 8 bit ALaw to Signed 16 LE 
	void PCMConverterALawtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);
		
		// samples is the number of input samples. Each input byte produces 2 output bytes.
		TUint samples = Min(aInLength, aOutLength/2);
		
		aInConsumed = samples;
		aOutUsed = samples*2;
		
		TInt pcm;
		while (samples--)
			{
			pcm = AlawSampleToPcm(*src++);
			*dest++ = STATIC_CAST( TUint8, pcm&KAndMask8bit);
			*dest++ = STATIC_CAST( TUint8, pcm>>8);
			}
		}

	// Convert from Signed 16 LE to 8 bit ALaw 
	void PCMConverterS16LEtoALaw(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		TUint8* dest = static_cast<TUint8*>(aOut);
		TInt16* src = (TInt16*)aIn;

		// samples is the number of output samples. Each 2 input bytes produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength);
		
		aInConsumed = samples*2;
		aOutUsed = samples;
		
		while (samples--)
			{
			*dest++ = PcmSampleToAlaw(*src++);
			}
		}

	
	
	//
	// MuLaw conversions
	//
	
	namespace
	{

		TInt MulawSampleToPcm(TUint8 aMulaw)
			{
			const TInt KExpLutSize = 8;
			const TInt KExpLut[KExpLutSize] = 
				{
				0, 132, 396, 924, 1980, 4092, 8316, 16764
				};
	
			TInt sign, exponent, mantissa, sample;
			
		    aMulaw = STATIC_CAST( TUint8, ~aMulaw);
		    sign = ( aMulaw & KMaskSign8bit );
		    exponent = ( aMulaw >> 4 ) & 0x07;
		    mantissa = aMulaw & 0x0F;
		    sample = KExpLut[exponent] + ( mantissa << ( exponent + 3 ) );
		    if ( sign != 0 )
				sample = -sample;
			
		    return sample;
			}

		
		
		TUint8 PcmSampleToMuLaw(TInt aPcm)
			{
			const TInt KMda16PcmToMulawClip		  = 32635;
			const TInt KMda16PcmToMulawBias		  = 0x84;			
			
			//[ conversion look up table pcm16 to Mulaw ]
			const TInt KExpLutSize = 256;
			const TInt8 KExpLut[KExpLutSize] = 
				{
				0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
				4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
				5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
				5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
				6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
				6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
				6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
				6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
				7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
				};

			TInt sign, exponent, mantissa;
		    TUint8 ulawbyte;

		    sign = (aPcm >> 8) & KMaskSign8bit;		
		    if ( sign != 0 )
				{
				aPcm = STATIC_CAST( TUint16,-aPcm);
				}
			if ( STATIC_CAST(TUint , aPcm) > STATIC_CAST(TUint , KMda16PcmToMulawClip ))
				{
				aPcm = KMda16PcmToMulawClip;
				}

		    aPcm = STATIC_CAST(TInt16 , aPcm + KMda16PcmToMulawBias);
		    exponent = KExpLut[( aPcm >> 7 ) & KAndMask8bit];
		    mantissa = ( aPcm >> ( exponent + 3 ) ) & 0x0F;
		    ulawbyte = STATIC_CAST( TUint8, ~ ( sign | ( exponent << 4 ) | mantissa ));

		    return ulawbyte;
			}
		
	}
	
	
	// Convert from 8 bit MuLaw to Signed 16 LE 
	void PCMConverterMuLawtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);
		
		// samples is the number of input samples. Each input byte produces 2 output bytes.
		TUint samples = Min(aInLength, aOutLength/2);
		
		aInConsumed = samples;
		aOutUsed = samples*2;
		
		TInt pcm;
		while (samples--)
			{
			pcm = MulawSampleToPcm(*src++);
			*dest++ = STATIC_CAST( TUint8, pcm&KAndMask8bit);
			*dest++ = STATIC_CAST( TUint8, pcm>>8);
			}
		}

	
	// Convert from Signed 16 LE to 8 bit MuLaw 
	void PCMConverterS16LEtoMuLaw(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed)
		{
		const TUint8* src = static_cast<const TUint8*>(aIn);
		TUint8* dest = static_cast<TUint8*>(aOut);

		// samples is the number of output samples. Each 2 input bytes produces 1 output byte.
		TUint samples = Min(aInLength/2, aOutLength);
		
		aInConsumed = samples*2;
		aOutUsed = samples;

		TInt pcm;
		while (samples--)
			{
			pcm = (src[0]+(src[1]<<8));
			*dest++ = PcmSampleToMuLaw(STATIC_CAST(TInt16 , pcm));
			src+=2;
			}
		}

	
	
}
