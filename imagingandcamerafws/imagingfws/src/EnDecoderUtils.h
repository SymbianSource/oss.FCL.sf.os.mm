// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Utilities used by Encoders and Decoders
// 
//

#ifndef __ENDECODER_UTILS_H__
#define __ENDECODER_UTILS_H__

const TUid KImageDecoderInterfaceUid = {0x101F45AD};
const TUid KImageEncoderInterfaceUid = {0x101F45B4};
const TUid KImageConvertResolverUid = {0x101F45DC};

/**
 *
 * This class is not for public use
 *
 */
class ImageEnDecoderUtils
	{
public:
	static void DoGetImageTypesL(TUid aInterfaceUid, RImageTypeDescriptionArray& aImageTypeArray, TUid aBaseType = KNullUid);
	static void DoGetFileTypesL(TUid aInterfaceUid, RFileExtensionMIMETypeArray& aFileTypeArray);
	static void DoGetImplementationInformationL(const TUid aInterfaceUid, CImplementationInformationType& aImplementationInformation,const TUid aImplementationUid);
	static TInt SortAsending(const CFileExtensionMIMEType& aItem1, const CFileExtensionMIMEType& aItem2);
	static void DoGetPluginPropertiesL(const TUid aInterfaceUid, const TUid aImplementationUid, RUidDataArray& aPropertiesArray, const TUint aOptions = 0);
	static void DoGetInterfaceImplementationsL(const TUid aInterfaceUid, const RUidDataArray& aRequiredUids, RUidDataArray& aImplArray);
	};

#endif // __ENDECODER_UTILS_H__
