// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalTechnology 
*/

#include <icl/imageplugin.h>
#include "MngDecodeConstruct.h"

#include "MngIclDecoder.h"


/*static*/
CMngDecodeConstruct* CMngDecodeConstruct::NewL()
	{
	CMngDecodeConstruct* self = new (ELeave) CMngDecodeConstruct;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// from CImageDecodeConstruct
CImageDecoderPlugin* CMngDecodeConstruct::NewPluginL() const
	{
	return CMngIclDecoder::NewL();
	}

