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

#ifndef OMXILPCMPROCESSOR_H
#define OMXILPCMPROCESSOR_H
#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/omxilcomponent.h>
#else
#include "omxilcomponent.h"
#endif

class COmxILPcmProcessorPort;


NONSHARABLE_CLASS(COmxILPcmProcessor) : public COmxILComponent
	{
public:
	static const TUint8 iComponentVersionMajor	  = 1;
	static const TUint8 iComponentVersionMinor	  = 0;
	static const TUint8 iComponentVersionRevision = 0;
	static const TUint8 iComponentVersionStep	  = 0;

public:

	static TInt CreateComponent(OMX_HANDLETYPE aComponent, const TDesC8& aComponentName, const TDesC8& aComponentRole);
	~COmxILPcmProcessor();

private:

	COmxILPcmProcessor();
	void ConstructL(OMX_HANDLETYPE aComponent, const TDesC8& aComponentName, const TDesC8& aComponentRole);
	void ConstructPortsL();

private:

	const OMX_VERSIONTYPE iOmxILVersion;
	COmxILPcmProcessorPort* ipb0Port;
	COmxILPcmProcessorPort* ipb1Port;
	};

#endif
