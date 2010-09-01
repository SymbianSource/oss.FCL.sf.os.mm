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
// Not for public use
// 
//

#ifndef __TESTICLRESOLVER_H__
#define __TESTICLRESOLVER_H__

#include <e32std.h>
#include <ecom/resolver.h>
#include <ecom/publicregistry.h>
#include <ecom/ecomresolverparams.h>

/**
 * This is the test resolver *
 */
class CTestIclResolver : public CResolver
	{
public:
	static CTestIclResolver* NewL(MPublicRegistry& aRegistry);
	~CTestIclResolver();
	// from CResolver
	TUid IdentifyImplementationL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const;
	RImplInfoArray* ListAllL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const;
private:
	explicit CTestIclResolver(MPublicRegistry& aRegistry);
	};

#endif // __TESTICLRESOLVER_H__

