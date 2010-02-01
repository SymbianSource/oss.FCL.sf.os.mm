// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __IMAGECONVERSIONRESOLVER_H__
#define __IMAGECONVERSIONRESOLVER_H__

#include <e32std.h>
#include <ecom/resolver.h>
#include <ecom/publicregistry.h>
#include <ecom/ecomresolverparams.h>
#include "ImageResolverAPI.h"

/**
 * Help sorting of the plugin list for EMatchString and EMatchMIMEType
 * by saving the extra sorting criteria.
 *
 */
class TSortImplInfo
	{
public:
	TSortImplInfo(CImplementationInformation *const aImplInfo, TInt aMatchLevel);
	TSortImplInfo(CImplementationInformation *const aImplInfo, TInt aMatchLevel, TInt aOptionsMatchLevel);
public:
	CImplementationInformation *const iImplInfo; //Not owned
	TInt iMatchLevel;
	TInt iOptionsMatchLevel;
	};

/**
 *
 * This is the custom resolver for the Image Conversion Library
 *
 * It selects a plugin by either match string or by image type and sub-type
 * or can be used to return a list of available types or sub-types. In all
 * cases it ignores Disabled plugins and also those that have been superceded
 * by a later version.
 *
 * @since 7.0s
 */
class CImageConversionResolver : public CResolver
	{
public:
	static CImageConversionResolver* NewL(MPublicRegistry& aRegistry);

	~CImageConversionResolver();
	// from CResolver
	TUid IdentifyImplementationL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const;
	RImplInfoArray* ListAllL(TUid aInterfaceUid, const TEComResolverParams& aAdditionalParameters) const;

private:
	explicit CImageConversionResolver(MPublicRegistry& aRegistry);
	TUid ResolveL(const RImplInfoArray& aImplementationsInfo, const TEComResolverParams& aAdditionalParameters) const;
	static TInt SortAsending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2);
	static TInt SortDesending(const TSortImplInfo& aImpInfo1, const TSortImplInfo& aImpInfo2);
	};



#endif // __IMAGECONVERSIONRESOLVER_H__

