// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/** 	@file
	@internalComponent */
#include <iclexif.h>
#include <exifutility.h>
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif
#include "ExifImageDisplayPanic.h"

#include "ExifImageDisplay.h"

_LIT(KExifPanicCat, "ExifImageDisplay");

CExifImageDisplayPlugin* CExifImageDisplayPlugin::NewL()
	{
	CExifImageDisplayPlugin* self = new(ELeave) CExifImageDisplayPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CExifImageDisplayPlugin::~CExifImageDisplayPlugin()
	{
	}

CExifImageDisplayPlugin::CExifImageDisplayPlugin():
							CGenericImageDisplayPlugin(KExifPanicCat)
	{
	iMaxUnscaledSize.SetSize(160,120);
	iMaxReductionFactor = KMaxReductionFactor;
	}

void CExifImageDisplayPlugin::AfterOpenL()
	{
	GetExifMetadataReader(iExifReader);
	}

TInt CExifImageDisplayPlugin::ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr)
	{
	if (KErrNone == CGenericImageDisplayPlugin::ExtensionInterface(aIFaceUid, aIFacePtr))
		{
		return KErrNone;
		}
	if (aIFaceUid != KExifImageDisplayExtensionUid)
		{
		return KErrNotSupported;
		}
	aIFacePtr = static_cast<MExifImageDisplayExtension*>(this);
	return KErrNone;
	}

TUint CExifImageDisplayPlugin::ImageStatus() const
	{
	return CImageDisplay::EImageSingleFrame | (ThumbnailExists()?CImageDisplay::EImageHasThumbnail:0);
	}

TInt CExifImageDisplayPlugin::GetExifMetadataReader(MExifMetadataReader*& aPtr)
	{
	// it should be safe since we've asked for EXIF decoder during decoder creation
	aPtr=static_cast<CJPEGExifDecoder*>(iDecoder)->ExifMetadata();
	return (aPtr==NULL)? KErrNotSupported : KErrNone;
	}

const TUid CExifImageDisplayPlugin::Uid() const
	{
	return KExifImageDisplayExtensionUid;
	}

void CExifImageDisplayPlugin::Release()
	{
	}
	
TInt CExifImageDisplayPlugin::SetScaleQuality(TInt aQualityLevel)
	{
	return DoSetScaleQuality(aQualityLevel);
	}
	
TInt CExifImageDisplayPlugin::SetMaximumReductionFactor(TInt aMaxReductionFactor)
	{
	return DoSetMaximumReductionFactor(aMaxReductionFactor);
	}

TBool CExifImageDisplayPlugin::ThumbnailExists() const
	{
	if (iExifReader==NULL)
		{
		return EFalse;
		}
	TExifReaderUtility util(iExifReader);
	TUint16 dummy;
	return (KErrNone == util.GetThumbCompression(dummy));
	}

TUint CExifImageDisplayPlugin::EffectiveRotation() const
	{
	TUint rotation=Options() & ( CImageDisplay::EOptionMirrorVertical |
			(CImageDisplay::EOptionMirrorVertical - CImageDisplay::EOptionRotateCw90) );

// calculate autorotation option for main image
	if ( (Options() & CImageDisplay::EOptionAutoRotate) && iExifReader)
		{
		TExifReaderUtility utility(iExifReader);
		TUint16 orientation(0);
		TInt err=KErrNotFound;
		if (ThumbnailRequested())
			{
			err=utility.GetThumbOrientation(orientation);
			}
		if (err!=KErrNone)
			{
			err = utility.GetOrientation(orientation);
			}
		if (KErrNone == err && orientation>0 && orientation<9)
			{
			// auto-rotation table using the EXIF 2.2 page 18
			static const TUint 
			KOrientation2Rotation[8]=	{
										0, // 1
										CImageDisplay::EOptionMirrorVertical, // 2
										CImageDisplay::EOptionRotateCw180, // 3
										CImageDisplay::EOptionMirrorHorizontal,	// 4
										CImageDisplay::EOptionRotateCw90|CImageDisplay::EOptionMirrorVertical,		// 5
										CImageDisplay::EOptionRotateCw90,		// 6
										CImageDisplay::EOptionRotateCw270|CImageDisplay::EOptionMirrorVertical,		// 7
										CImageDisplay::EOptionRotateCw270,		// 8
										};
			rotation = KOrientation2Rotation[orientation-1];
			}
		}
	return rotation;
	}

TUid CExifImageDisplayPlugin::RequiredImageClass() const
	{
	return TUid::Uid(KUidICLJpegEXIFInterface);
	}

