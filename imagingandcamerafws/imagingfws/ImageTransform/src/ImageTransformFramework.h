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
// These are internal structures used by the Image Transform Library
// 
//

#ifndef __IMAGETRANSFORMFRAMEWORK_H__
#define __IMAGETRANSFORMFRAMEWORK_H__

#include <e32base.h>
#include <ecom/ecom.h>

#include "ImageTransform.h"
#include "ImageTransformPlugin.h"

/*
 * Class used to shadow the descriptor supplying the image data.
 * It maintains a flag to determine whether the descriptor contains a
 * filename or the image data itself.
 * This class is not for public use
 *
 */
class TImageParameterData
	{
public:

	/**
	Flag used to determine what this class contains.
	*/
	enum TImageParameterDataFlag
		{
		/** Field not set
		*/
		ENull,

		/** class contains a filename
		*/
		EFilename,

		/** class contains a const source descriptor
		*/
		EData,

		/** class contains a pointer to a HBufC8
		*/
		EDataPtr
		};

	TImageParameterData();
	void Reset();
	void SetFilename(const TDesC& aFilenamePtr);
	void SetData(const TDesC8& aData);
	void SetDataPtr(HBufC8*& aBufferPtr);
	void SetMimeType(const TDesC8& aMimeType);
	void SetImageType(TUid aImageType, TUid aImageSubType = KNullUid);

	TBool IsFilename() const;
	TBool IsData() const;
	const TDesC& Filename() const;
	const TDesC8& Data() const;
	HBufC8*& DataPtr() const;
	const TDesC8& MimeType() const;
	const TUid ImageType() const;
	const TUid ImageSubType() const;
	TBool IsDataTypeDefined() const;

private:
	TImageParameterDataFlag iImageParameterDataFlag;
	union
		{
		const TDesC* iFilename;	// not owned
		const TDesC8* iData;	// not owned
		HBufC8** iDataPtr;		// not owned
		};
	const TDesC8* iMimeType;	// not owned
	TUid iImageType;
	TUid iImageSubType;
	};

/*
 * Body of CImageTransform
 */
NONSHARABLE_CLASS(CImageTransformFramework) : public CBase
	{
	friend class CImageTransform;
	friend class CImageTransformPlugin;

private:

	enum TImageTransformInternalFlags
		{
		EFlagMaintainAspectRatio	=0x00000001,
		EFlagPreserveImageData		=0x00000002,
		EFlagSourceRectSet			=0x00000004
		};

	static CImageTransformFramework* NewL(CImageTransform& aHandle, RFs& aFs);
	CImageTransformFramework(CImageTransform& aHandle, RFs& aFs);
	~CImageTransformFramework();
	
	void SetPluginInstanceUidL(TUid aInstanceUid);
	
	// setter methods - called from client API (CImageTransform)
	void SetPluginUidL(TUid aPluginUid);
	void SetSourceFilenameL(const TDesC& aFilename);
	void SetSourceDataL(const TDesC8& aData);
	void SetSourceMimeTypeL(const TDesC8& aMIMEType);
	void SetSourceImageTypeL(TUid aImageType, TUid aImageSubType);
	void SetSourceRect(const TRect& aRect);
	void ClearSourceRect();
	void SetDestFilenameL(const TDesC& aFilename);
	void SetDestDataL(HBufC8*& aDestinationData);
	void SetDestSizeInPixelsL(const TSize& aDestinationSize, TBool aMaintainAspectRatio);
	void SetOptionsL(TUint aOptions);
	void SetTransformationsL(TUint aTransformations);
	void SetPreserveImageData(TBool aPreserveImageData);
	// getter called from client API
	CImageTransformPluginExtension* Extension() const;
	CImageTransformPluginExtension* Extension(TUid aExtensionUid, TInt& aError) const;

	// getter methods - called from plugin (CImageTransformPlugin)
	TBool SourceIsFilename() const;
	TBool SourceIsData() const;
	const TDesC& SourceFilename() const;
	const TDesC8& SourceData() const;
	const TDesC8& SourceMimeType() const;
	const TUid SourceImageType() const;
	const TUid SourceImageSubType() const;
	TBool SourceRect(TRect& aRect) const;
	TBool DestIsFilename() const;
	TBool DestIsData() const;
	const TDesC& DestFilename() const;
	HBufC8*& DestData() const;
	const TSize& DestinationSizeInPixels() const;
	TUint Options() const;
	TBool MaintainAspectRatio() const;
	TBool PreserveImageData() const;

	// "action" methods
	void SetupL();
	void Transform(TRequestStatus& aStatus);
	void CancelTransform();
	void Reset();
	
	// "internal" methods called only from client API (CImageTransform)
	CImageTransformPlugin* FileNewImplL(
		RFs& aFs, 
		const TDesC& aSourceFilename, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);
	CImageTransformPlugin* NewImplL(
		const TDesC8& aMIMEType);
	CImageTransformPlugin* DataNewL(
		const TDesC8& aSourceData, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);

	void MimeTypeGetTransformerListL(
		RImplInfoPtrArray& aTransformerList, 
		const TDesC8& aMIMEType);
	void ImageTypeGetTransformerListL(
		RImplInfoPtrArray& aTransformerList, 
		const TDesC8& aImageHeader, 
		const TUid aImageType, 
		const TUid aImageSubType, 
		const TUid aPluginUid);
	void SuffixTypeGetTransformerListL(
		RImplInfoPtrArray& aTransformerList, 
		const TDesC& aFileName);
	CImageTransformPlugin* FindTransformerNewL(
		const RImplInfoPtrArray& aTransformerList);

	void DeletePlugin();
	inline void SetFlag(TInt aFlag);
	inline void ClearFlag(TInt aFlag);
	inline TBool FlagSet(TInt aFlag) const;
	TUint NeededPluginFlags();
private:
	CImageTransform& iHandle;
	RFs& iFs;

	TImageParameterData iSourceImage;

	TSize iDestinationSize;
	TImageParameterData iDestImage;
	
	TUid iPluginUid;

	CImageTransformPlugin* iPlugin;
	TUid iPluginInstanceUid;
	TUint iOptions;
	TUint iTransformations;

	TRect iSourceRect;

	TInt iFlags;
	};

inline void CImageTransformFramework::SetFlag(TInt aFlag)
    {
    iFlags|=aFlag;
    }

inline void CImageTransformFramework::ClearFlag(TInt aFlag)
    {
    iFlags&=~aFlag;
    }

inline TBool CImageTransformFramework::FlagSet(TInt aFlag) const
    {
    return iFlags&aFlag;
    }

#endif  // __IMAGETRANSFORMFRAMEWORK_H__

