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

/** @file
	@internalComponent */
inline
TImageParameterData::TImageParameterData()
	{
	Reset();
	}

inline void CImageDisplayFramework::SetFlag(TInt aFlag)
    {
    iFlags|=aFlag;
    }

inline void CImageDisplayFramework::ClearFlag(TInt aFlag)
    {
    iFlags&=~aFlag;
    }

inline TBool CImageDisplayFramework::FlagSet(TInt aFlag) const
    {
    return iFlags&aFlag;
    }

inline
const TDesC8& TImageParameterData::MimeType() const
	{
	return iMimeType;
	}

inline
const TUid TImageParameterData::ImageType() const
	{
	return iImageType;
	}

inline
const TUid TImageParameterData::ImageSubType() const
	{
	return iImageSubType;
	}

inline
TBool TImageParameterData::IsDataTypeDefined() const
	{
	return (iImageParameterDataFlag == CImageDisplayPlugin::EImgSrcNotDefined)?EFalse:ETrue;
	}

inline
CImageDisplayPlugin::TImageSourceType TImageParameterData::ImageSourceType() const
	{
	return iImageParameterDataFlag;
	}

inline
void TImageParameterData::SetContentId(const TDesC& aContentId)
	{
	iContentId.Set(aContentId);
	}

inline
void TImageParameterData::SetIntent(ContentAccess::TIntent aIntent)
	{
	iIntent = aIntent;
	}

inline
void TImageParameterData::SetMimeType(const TDesC8& aMimeType)
	{
	iMimeType.Set(aMimeType);
	}

inline
ContentAccess::TIntent TImageParameterData::Intent() const
	{
	return iIntent;
	}

inline
const TDesC& TImageParameterData::ContentId() const
	{
	return iContentId;
	}

inline
ContentAccess::TIntent CImageDisplayFramework::SourceDataIntent() const
	{
	return iSourceImage.Intent();
	}


inline
const TDesC& CImageDisplayFramework::SourceDataId() const
	{
	return iSourceImage.ContentId();
	}

inline
TBool TImageParameterData::IsUIEnabled() const
	{
	return iEnableUI;
	}
	
inline
void TImageParameterData::SetEnableUI(TBool aEnableUI)
	{
	iEnableUI = aEnableUI;
	}
