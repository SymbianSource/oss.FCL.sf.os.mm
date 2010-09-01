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
// TestTransformerExt.H
// 
//

#ifndef __TestTRANSFORMEXT_H__
#define __TestTRANSFORMEXT_H__

#include <ICL\ImageTransformPluginExt.h>

class CTestTransformerBase;

class CTestTransformerExtension : public CImageTransformPluginExtension
	{
public:
	IMPORT_C static CTestTransformerExtension* NewL(CTestTransformerBase* aOwner);
	IMPORT_C CTestTransformerExtension(CTestTransformerBase* aOwner);
	IMPORT_C const TUid Uid() const;
	IMPORT_C TBool PreserveImageData() const;
private:
	CTestTransformerBase* iOwner;
	};

#endif //__TestTRANSFORMEXT_H__
