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
// Not for public use
// 
//

#ifndef __IMAGEDISPLAYRESOLVER_H__
#define __IMAGEDISPLAYRESOLVER_H__

#include <e32std.h>
#include <ecom/resolver.h>
#include <ecom/publicregistry.h>
#include <ecom/ecomresolverparams.h>
#include "ImageDisplayResolverAPI.h"

/** @file
	@internalComponent */


/**
 * Help sorting of the plugin list for EMatchString and EMatchMIMEType
 * by saving the extra sorting criteria.
 *
 */
class TSortImplInfo
	{
public:
	TSortImplInfo(CImplementationInformation *const aImplInfo, TInt aMatchLevel);
public:
	CImplementationInformation *const iImplInfo; //Not owned
	TInt iMatchLevel;
	};

/**
 *
 *
 * It selects a plugin by either match string or by image type and sub-type
 * or can be used to return a list of available types or sub-types. In all
 * cases it ignores Disabled plugins and also those that have been superceded
 * by a later version.
 *
 */
class CImageDisplayResolver : public CResolver
	{
public:
	static CImageDisplayResolver* NewL(MPublicRegistry& aRegistry);

	~CImageDisplayResolver();
	// from CResolver
	TUid IdentifyImplementationL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const;
	RImplInfoArray* ListAllL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const;

private:
	explicit CImageDisplayResolver(MPublicRegistry& aRegistry);
	static TInt SortAsending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2);
	static TInt SortDesending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2);
	};



#endif // __IMAGEDISPLAYRESOLVER_H__

