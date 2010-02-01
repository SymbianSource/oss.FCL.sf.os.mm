// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef OMXILDUMMYAUDIODECILIF_H
#define OMXILDUMMYAUDIODECILIF_H

#include "omxildummybaseilif.h"

/**
Test component. 
Concrete implementation of the MILIF API when implemented as a plugin 
and specific to the AAC and Vorbis decoder and the PCM renderer OMX IL component specifications.

*/
class COmxILDummyAudioDecILIF	 : 	public COmxILDummyBaseILIF
	{
public:
	 static COmxILDummyAudioDecILIF* NewL();
	
	// from COmxILDummyBaseILIF
	TInt SetConfig(const TILStruct& aConfig, const MILComponentPortIf* aComponentPort);
	TInt GetConfig(TILStruct& aConfig, const MILComponentPortIf* aComponentPort) const;	
	TInt SetExtConfig(const TDesC8& aParameterName, const TILStruct& aConfig, const MILComponentPortIf* aComponentPort);
	TInt GetExtConfig(const TDesC8& aParameterName, TILStruct& aConfig, const MILComponentPortIf* aComponentPort) const;
	TInt SetComponentIfRole(const TUid& aComponentRole);

private:
	COmxILDummyAudioDecILIF();
	~COmxILDummyAudioDecILIF();
	};

#endif // OMXILDUMMYAUDIODECILIF_H

