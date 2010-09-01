// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef _PNGDECODER_FACTORY_H

#define _PNGDECODER_FACTORY_H

#include "StreamDecoderIFace.h"
/** 	@file
	@internalComponent */

class CPngImageStreamDecoderFactory:public CBase, public MImageStreamDecoderFactory 
	{
public:
	static MImageStreamDecoderFactory* NewL();

protected:
	CPngImageStreamDecoderFactory();
	~CPngImageStreamDecoderFactory();

// from the MImageStreamDecoderFactory // 
	void CreatePngDecoderL(MImageStreamDecoder*& aPtr);
	void SetDtorKey(TUid aUid);
	void Release();
private:
	TUid iDtorKey;
	};

#endif // ndef _PNGDECODER_FACTORY_H
