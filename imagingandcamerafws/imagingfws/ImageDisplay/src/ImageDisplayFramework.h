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
// These are internal structures used by the Image Display Library
// 
//

#ifndef __IMAGEDISPLAYFRAMEWORK_H__
#define __IMAGEDISPLAYFRAMEWORK_H__

#include <ecom/ecom.h>

#include "ImageDisplay.h"
#include "ImageDisplayPlugin.h"

/** @file
	@internalComponent */

/*
@internalComponent
  
   Class used to shadow the descriptor supplying the image data.
   It maintains a flag to determine whether the descriptor contains a
   filename or the image data itself.
   This class is not for public use
  
 */

class TImageParameterData
	{
public:
	TImageParameterData();
	void Reset();
	void SetFilename(const TDesC& aFilenamePtr);
	void SetData(const TDesC8& aData);
	void SetFileHandle(RFile& aFileHandle);

	void SetMimeType(const TDesC8& aMimeType);
	void SetIntent(ContentAccess::TIntent aIntent);
	void SetContentId(const TDesC& aContentId);
	void SetImageType(TUid aImageType, TUid aImageSubType = KNullUid);
	void SetEnableUI(TBool aEnableUI);

	CImageDisplayPlugin::TImageSourceType ImageSourceType() const;
	const TDesC& Filename() const;
	const TDesC8& Data() const;
	RFile& FileHandle() const;

	ContentAccess::TIntent Intent() const;
	const TDesC& ContentId() const;

	const TDesC8& MimeType() const;
	const TUid ImageType() const;
	const TUid ImageSubType() const;
	TBool IsDataTypeDefined() const;
	TBool IsUIEnabled() const;

private:
	CImageDisplayPlugin::TImageSourceType iImageParameterDataFlag;
	TPtrC iFilename;	// not owned
	TPtrC8 iData;	// not owned
	RFile* iFileHandle;// not owned
	ContentAccess::TIntent iIntent;
	TPtrC iContentId;
	TPtrC8  iMimeType;	// not owned
	TUid iImageType;
	TUid iImageSubType;
	TBool iEnableUI;
	};

/*
@internalComponent

  Body of CImageDisplay

  This class is not for public use
*/
class CImageDisplayFramework;
NONSHARABLE_CLASS(CImageDisplayFramework) : public CBase
	{
	friend class CImageDisplay;
	friend class CImageDisplayPlugin;

private:

	enum TImageDisplayInternalFlags
		{
		EFlagMaintainAspectRatio	=0x00000001,
		EFlagSourceRectSet			=0x00000002
		};

	static CImageDisplayFramework* NewL(CImageDisplay& aHandle, MIclImageDisplayObserver& aCallback, RFs& aFs);
	CImageDisplayFramework(CImageDisplay& aHandle, MIclImageDisplayObserver& aCallback, RFs& aFs);
	~CImageDisplayFramework();
	
	// setter methods - called from client API (CImageDisplay)
	void SetPluginUid(TUid aPluginUid);
	TInt SetImageSource(const TMMSource& aImageSource);

	void SetSourceMimeType(const TDesC8& aMIMEType);
	void SetSourceImageType(TUid aImageType, TUid aImageSubType);
	void SetSourceRect(const TRect& aRect);
	void ResetSourceRect();
	void SetSizeInPixels(const TSize& aDestinationSize, TBool aMaintainAspectRatio);
	TInt SetOptions(TUint aOptions);
	void SetDisplayMode(TDisplayMode aDisplayMode);

	// getters called from client API
	TInt ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr);
	void GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const;
	TBool ValidBitmap() const;
	
	// getter methods - called from plugin (CImageDisplayPlugin)
	CImageDisplayPlugin::TImageSourceType SourceType() const;

	const TDesC& SourceFilename() const;
	RFile& SourceFileHandle() const;
	const TDesC8& SourceData() const;
	const TDesC8& SourceMimeType() const;
	ContentAccess::TIntent SourceDataIntent() const;
	const TDesC& SourceDataId() const;

	const TUid SourceImageType() const;
	const TUid SourceImageSubType() const;
	TBool SourceRect(TRect& aRect) const;
	const TSize& DestinationSizeInPixels() const;
	TUint Options() const;
	TBool MaintainAspectRatio() const;
	TDisplayMode DisplayMode() const;
	const CImageDisplay::RImageSizeArray& RecommendedImageSizes() const;
	TUint ImageStatus() const;
	TInt NumFrames(TInt& aNumFrames) const;

	// callback methods - called from plugin (CImageDisplayPlugin)
	void CallbackImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError);

	// "action" methods
	void SetupL();
	void Play();
	void StopPlay();
	void Reset();
	void Pause();
	
		// "internal" methods called only from client API (CImageDisplay)
	CImageDisplayPlugin* FileNewImplL(
		const TDesC& aSourceFilename, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);

	CImageDisplayPlugin* FileHandleNewImplL(
		RFile& aFileHandle, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);

	CImageDisplayPlugin* NewImplL(
		const TDesC8& aMIMEType);

	CImageDisplayPlugin* DataNewL(
		const TDesC8& aSourceData, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);

	void MimeTypeGetDisplayerListL(
		RImplInfoPtrArray& aDisplayerList, 
		const TDesC8& aMIMEType);
	void ImageTypeGetDisplayerListL(
		RImplInfoPtrArray& aDisplayerList, 
		const TDesC8& aImageHeader, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);
	void SuffixTypeGetDisplayerListL(
		RImplInfoPtrArray& aDisplayerList, 
		const TDesC& aFileName);
	CImageDisplayPlugin* FindDisplayerNewL(
		const RImplInfoPtrArray& aDisplayerList);

	void DeletePlugin();
	inline void SetFlag(TInt aFlag);
	inline void ClearFlag(TInt aFlag);
	inline TBool FlagSet(TInt aFlag) const;

private:
	CImageDisplay& iHandle;
	MIclImageDisplayObserver& iCallback;
	RFs& iFs;

	TImageParameterData iSourceImage;

	TSize iDestinationSize;

	TUid iPluginUid;
	
	TDisplayMode iDisplayMode;

	CImageDisplayPlugin* iPlugin;
	TUint iOptions;

	TRect iSourceRect;

	TInt iFlags;
	enum TFrameworkState 
		{
		EStateOpened=0, 
		EStateConfigured, 
		EStatePluginLoaded, 
		EStateProcessing, 
		EStatePaused
		}			iState;
	};


#include "Imagedisplayframework.inl"

#endif  // __IMAGEDISPLAYFRAMEWORK_H__

