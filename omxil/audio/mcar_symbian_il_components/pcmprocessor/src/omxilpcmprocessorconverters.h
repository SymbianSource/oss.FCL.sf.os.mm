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

#ifndef OMXILPCMPROCESSORCONVERTERS_H_
#define OMXILPCMPROCESSORCONVERTERS_H_


namespace NOmxILPcmProcessorConverters
	{
	
	// All functions have the common prototype:
	//
	// void function(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, 
	//					TUint& aInConsumed, TUint& aOutUsed	);
	//
	// where aIn and aOut are the input and output buffers
	// 		aInLength is the length in bytes of the input buffer data
	//		aOutLength is the maximum length in bytes of the output data buffer
	// and on return
	//		aInConsumed is the number of bytes from the input buffer consumed
	//		aOutUsed is the number of bytes from the output buffer used
	
	void PCMConverterNoConversion(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterU8toS16(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterU16LEtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16LEtoU8(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterU16BEtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16LEtoS8(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS8toS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16LEtoU16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16LEtoU16BE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16SwapEndianness(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterALawtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16LEtoALaw(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterMuLawtoS16LE(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	void PCMConverterS16LEtoMuLaw(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed);
	}

#include "omxilpcmprocessorconverters.inl"
#endif
