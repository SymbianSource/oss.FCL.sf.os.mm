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


#include <ecom/ecom.h>
#include <ecom/ecomerrorcodes.h>
#include <ecom/implementationinformation.h>

#include "testiclresolver.h"

/** 
 * Constructs and initialises a new instance of CImageConversionResolver.
 *
 * @param     "MPublicRegistry& aRegistry"
 *            A reference to the registry on which it will operate
 * @return    "CImageConversionResolver*"
 *            A pointer to the newly constructed resolver
 */
CTestIclResolver* CTestIclResolver::NewL(MPublicRegistry& aRegistry)
	{
	return new(ELeave) CTestIclResolver(aRegistry);
	}

/** 
 *
 * Default destructor
 *
 */
CTestIclResolver::~CTestIclResolver()
	{
	}

/** 
 *
 * Default constructor
 *
 * @param     "MPublicRegistry& aRegistry"
 *            A reference to the registry on which it will operate
 */
CTestIclResolver::CTestIclResolver(MPublicRegistry& aRegistry)
	: CResolver(aRegistry)
	{
	}

/** 
 *
 * Request that the resolver identify the most appropriate interface
 * implementation.
 *
 * @param     "TUid aInterfaceUid"
 *            The Uid of the interface you want to match against
 * @param     "const TEComResolverParams& aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "TUid"
 *            The implementation Uid of the single best match found
 */
TUid CTestIclResolver::IdentifyImplementationL(TUid /*aInterfaceUid*/, const TEComResolverParams& /*aAdditionalParameters*/) const
	{
	TUid found = KNullUid;
	return found;
	}

/* 
 *
 * List all the implementations which satisfy the specified interface, ignoring 
 * any implementations that have their Disabled flag set or are not of the 
 * current framework version.
 *
 * The list is sorted:
 * 1) For EMatchString in desending order of number of bytes matched and version number.
 * 2) For EMatchMIMEType in asending order of the MIME position that matched and
 *    desending version number.
 * 3) All others - desending version numbers.
 *
 * @param     "TUid aInterfaceUid"
 *            The Uid of the interface you want to match against
 * @param     "const TEComResolverParams& aAdditionalParameters"
 *            A passed in reference to the parameters on which to match
 * @return    "RImplInfoArray*"
 *            The list of matches found
 */
 RImplInfoArray* CTestIclResolver::ListAllL(TUid /*aInterfaceUid*/, const TEComResolverParams& /*aAdditionalParameters*/) const
	{
	return NULL;
	}

