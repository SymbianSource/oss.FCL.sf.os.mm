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

#ifndef OMXILPCMPORTADPCMVARIANT_H
#define OMXILPCMPORTADPCMVARIANT_H

#include "OmxILPcmPortBase.h"

class COmxILPcmPortAdPcmVariant : public COmxILPcmPortBase
	{
	public:
		/**
		 * Constructor.
		 * @return 	Newly created COmxILPcmPortAdPcmVariant.
		 */
		static COmxILPcmPortAdPcmVariant* NewL(const TOmxILCommonPortData& aCommonPortData, const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType); 
		
		~COmxILPcmPortAdPcmVariant();


	private:
		COmxILPcmPortAdPcmVariant(const TOmxILCommonPortData& aCommonPortData, const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType);
		
		/**
		 * Second stage constructor.
		 * See @C NewL for details.
		 */
		void ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats);
		
		/**
		 * From OmxILPcmPortBase.
		 */
		TBool ValidatePcmModeParams(const OMX_AUDIO_PARAM_PCMMODETYPE& aPcmModeType) const;
	};


#endif
