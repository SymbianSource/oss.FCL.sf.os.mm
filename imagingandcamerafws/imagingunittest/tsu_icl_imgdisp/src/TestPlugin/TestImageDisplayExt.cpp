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
// TestTransformerExt.CPP
// 
//

#include "TestTransformerExt.h"
#include "TestTransformerBase.h"

const TUid KUidTestTransformerExt = {0x101F7C44};

EXPORT_C CTestTransformerExtension* CTestTransformerExtension::NewL(CTestTransformerBase* aOwner)
	{
	CTestTransformerExtension* self = new (ELeave) CTestTransformerExtension(aOwner);
	return self;	
	}

EXPORT_C const TUid CTestTransformerExtension::Uid() const
	{
	return KUidTestTransformerExt;
	}

EXPORT_C CTestTransformerExtension::CTestTransformerExtension(CTestTransformerBase* aOwner)
	:iOwner(aOwner) 
	{
	}

EXPORT_C TBool CTestTransformerExtension::PreserveImageData() const
	{
	return iOwner->GetPreserveImageData();
	}

