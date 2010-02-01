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

#ifndef __TestTRANSFORMBASE_H__
#define __TestTRANSFORMBASE_H__

#include <icl/imagetransformplugin.h>

class CTestTransformerBase: public CImageTransformPlugin
	{
public:
	virtual TBool GetPreserveImageData() const = 0;
	};

#endif //__TestTRANSFORMBASE_H__
