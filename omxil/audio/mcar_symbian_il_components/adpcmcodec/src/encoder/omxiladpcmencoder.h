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

#ifndef OMXILADPCMENCODER_H
#define OMXILADPCMENCODER_H
#include "omxilcomponent.h"


// Forward declarations
class COmxILAdPcmCodecPort;
class COmxILPcmPortAdPcmVariant;
class COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm;


NONSHARABLE_CLASS(COmxILAdPcmEncoder) : public COmxILComponent
	{

public:

	static const TUint8 iComponentVersionMajor	  = 1;
	static const TUint8 iComponentVersionMinor	  = 0;
	static const TUint8 iComponentVersionRevision = 0;
	static const TUint8 iComponentVersionStep	  = 0;

public:

	static  TInt CreateComponent(OMX_HANDLETYPE aComponent);

	~COmxILAdPcmEncoder();

private:

	COmxILAdPcmEncoder();
	void ConstructL(OMX_HANDLETYPE aComponent);
	void ConstructPortsL();
	TInt AddPorts();
	void ConstructAdPcmPortL( COmxILAdPcmCodecPort*& aPort, OMX_DIRTYPE aDirection, TInt aPortIndex );
	void ConstructPcmPortL( COmxILPcmPortAdPcmVariant*& aPort, OMX_DIRTYPE aDirection, TInt aPortIndex );


private:
	const OMX_VERSIONTYPE iOmxILVersion;
	COmxILPcmPortAdPcmVariant* ipb0Port;
	COmxILAdPcmCodecPort* ipb1Port;
	COmxILAdPcmCodecProcessingFunctionCorePcmToAdPcm* iCore;
	};

#endif /*OMXILADPCMENCODER_H_*/
