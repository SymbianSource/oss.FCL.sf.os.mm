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

#ifndef OMXILADPCMDECODERPROCESSINGFUNCTIONCOREADPCMTOPCM_H
#define OMXILADPCMDECODERPROCESSINGFUNCTIONCOREADPCMTOPCM_H

#include "OmxILAdPcmCodecConsts.h"
#include "omxiladpcmcodecprocessingfunctioncore.h"
#include "omxiladpcmcodecport.h"
#include "OmxILPcmPortAdPcmVariant.h"


#include <e32base.h>
#include <openmax/il/khronos/v1_x/OMX_Audio.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>


const TInt KIndexTableSize = 16;
const TInt KStepTableSize = 89;

class COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm : public COmxILAdPcmCodecProcessingFunctionCore
	{
public:

	static COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm* NewL( COmxILAdPcmCodecPort& aPb0Port, COmxILPcmPortAdPcmVariant& aPb1Port );
	~COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm();
	
	//from COmxILAdPcmCodecProcessingFunctionCore
	virtual TInt Configure();
	virtual TInt CheckConversion();
	virtual TUint BitsPerSample() const;
	virtual TUint SampleRate() const;
	virtual TUint Channels() const;
	virtual TUint InternalOutputBufferSize() const;
	virtual TUint HowMuchOneConvertEats() const;
	virtual TBool ForgetNotAlignedInputBuffers() const;
	virtual OMX_ERRORTYPE SetBlockAlign(TInt aBlockAlign);

private:
	COmxILAdPcmCodecProcessingFunctionCoreAdPcmToPcm(COmxILAdPcmCodecPort& aPb0Port, COmxILPcmPortAdPcmVariant& CPb1Port );
	virtual void Convert( const TAny* aIn, TAny* aOut, TInt aSamples );
	void Convert( const TUint8* aSrc, TUint8* aDst, TInt aSamples);
	
private:
	static const TInt KIndexTable[KIndexTableSize];
	static const TInt KStepSizeTable[KStepTableSize];
	
	//not owned
	COmxILAdPcmCodecPort& ipb0Port;
	COmxILPcmPortAdPcmVariant& ipb1Port;
	
    TUint iCustomBlockAlign;
	};

#endif// OMXILADPCMDECODERPROCESSINGFUNCTIONCOREADPCMTOPCM_H_
