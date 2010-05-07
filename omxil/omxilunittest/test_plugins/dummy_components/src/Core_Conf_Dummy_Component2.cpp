// Core_Conf_Dummy_Component2.cpp

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

#include <openmax/il/loader/omxilsymbiancomponentif.h>
#include "Core_Conf_Dummy_Component.hrh"

extern "C" OMX_ERRORTYPE TestCompComponentInit(OMX_HANDLETYPE hComponent);

OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidCoreDummyTestComponent2);

OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE hComponent)
	{
	return TestCompComponentInit(hComponent);
	}
