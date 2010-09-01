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

#ifndef __TestHelperIface_H__
#define __TestHelperIface_H__

#include <e32base.h>
#include <icl/imagedisplaypluginext.h>
#include "TestImageDisplay.hrh"


class MTestHelper:public MImageDisplayPluginExtension {
public:
	enum	{
			KIFaceUidValue=KUidImageDisplayTestPluginImplUid
			};
	virtual TInt CurrentFrameNumber(TInt& aFrameParam)=0;
	virtual void TriggerPanic_4()=0;
};
// fwd refs



#endif // ndef __TestHelperIface_H__
