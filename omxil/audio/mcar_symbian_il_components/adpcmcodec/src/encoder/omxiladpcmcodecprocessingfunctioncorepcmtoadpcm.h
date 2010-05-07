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

#ifndef OMXILADPCMDECODERPROCESSINGFUNCTIONCOREPCMTOADPCM_H
#define OMXILADPCMDECODERPROCESSINGFUNCTIONCOREPCMTOADPCM_H

#include "OmxILAdPcmCodecConsts.h"
#include "omxiladpcmcodecprocessingfunctioncore.h"
#include "omxiladpcmcodecport.h"
#include "OmxILPcmPortAdPcmVariant.h"

#include <e32base.h>
#include <openmax/il/khronos/v1_x/OMX_Audio.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>

const TInt KImaAdpcmCodecIndexTableSize = 16;
const TInt KImaAdpcmCodecStepTableSize = 89;

/*
* 
* class TOmxILAdPcmCodecState
*
*/
class TOmxILAdPcmCodecState
	{
public:
	TInt16 iPredicted; // Previous output value
	TUint8 iIndex; // Index into stepsize table
	};

class COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm : public COmxILAdPcmCodecProcessingFunctionCore
	{
public:

	static COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm* NewL( COmxILPcmPortAdPcmVariant& aPb0Port, COmxILAdPcmCodecPort& aPb1Port );
	~COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm();
	
private:
	COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm( COmxILPcmPortAdPcmVariant& aPb0Port, COmxILAdPcmCodecPort& aPb1Port );
	
	//from COmxILAdPcmCodecProcessingFunctionCore
	virtual void Convert( const TAny* aIn, TAny* aOut, TInt aSamples);
	virtual void Reset();
	virtual TInt CheckConversion();
	virtual TUint BitsPerSample() const;
	virtual TUint SampleRate() const;
	virtual TUint Channels() const;
	virtual TUint InternalOutputBufferSize() const;
	virtual TUint HowMuchOneConvertEats() const;
	virtual TInt Configure();
	virtual TBool ForgetNotAlignedInputBuffers() const;
	virtual OMX_ERRORTYPE SetBlockAlign(TInt aBlockAlign);
	
	//original from the MMF folder
	void Convert(const TUint8* aSrc, TUint8* aDst, TInt aSamples);
	
private:
	TOmxILAdPcmCodecState iState;	//supports mono only //original comment from the MMF folder
	static const TInt KIndexTable[KImaAdpcmCodecIndexTableSize];
	static const TInt KStepSizeTable[KImaAdpcmCodecStepTableSize];
	TBool iBufferStep;
	TInt iBuffer;
	
	//not owned
	COmxILPcmPortAdPcmVariant& ipb0Port;
	COmxILAdPcmCodecPort& ipb1Port;
	};

#endif// OMXILADPCMDECODERPROCESSINGFUNCTIONCOREPCMTOADPCM_H_
