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
//

/**    @file
   @internalComponent */
#include <caf/data.h>
#include <caf/content.h>
#include "ImageDisplayFramework.h"
#include "ImageDisplay.h"
#include "ImageDisplay.hrh"
#include "ImageDisplayMain.h"
#include "../../src/ImageUtils.h"
#include "ImageDisplayResolverAPI.h"
#include "ImageDisplayResolvrUtils.h"
using namespace ContentAccess;

/**
* NewL
* The function NewL constructs a CImageDisplayFramework
*
* @return	CImageDisplayFramework*
*
*/
CImageDisplayFramework* CImageDisplayFramework::NewL(CImageDisplay& aHandle, MIclImageDisplayObserver& aCallback, RFs& aFs)
	{
	CImageDisplayFramework* self = new(ELeave) CImageDisplayFramework(aHandle, aCallback, aFs);
	return self;
	}

/**
*
* CImageDisplayFramework
*
*/
CImageDisplayFramework::CImageDisplayFramework(CImageDisplay& aHandle, MIclImageDisplayObserver& aCallback, RFs& aFs) : 
	iHandle(aHandle), iCallback(aCallback), iFs(aFs)
	{
	Reset();
	}

/**
*
* ~CImageDisplayFramework
* This is the destructor for the CImageDisplayFramework 
* and is resposible for deallocating all resources 
* allocated by the CImageDisplayFramework
*
*/
CImageDisplayFramework::~CImageDisplayFramework()
	{
	// make sure any existing displayation is cancelled
	if (iPlugin && (EStateProcessing == iState || iState == EStatePluginLoaded
						|| iState == EStatePaused))
		{
		iPlugin->StopPlay();
		}
	DeletePlugin();
	}

CImageDisplayPlugin::TImageSourceType CImageDisplayFramework::SourceType() const
	{
	return iSourceImage.ImageSourceType();
	}

const TDesC& CImageDisplayFramework::SourceFilename() const
	{
	if (CImageDisplayPlugin::EImgSrcFileName != iSourceImage.ImageSourceType())
		{
		Panic(EImageWrongType);
		}
	return iSourceImage.Filename();
	}

const TDesC8& CImageDisplayFramework::SourceData() const
	{
	if (CImageDisplayPlugin::EImgSrcDescriptor != iSourceImage.ImageSourceType())
		{
		Panic(EImageWrongType);
		}
	return iSourceImage.Data();
	}

RFile& CImageDisplayFramework::SourceFileHandle() const
	{
	if (CImageDisplayPlugin::EImgSrcFileHandle != iSourceImage.ImageSourceType())
		{
		Panic(EImageWrongType);
		}
	return iSourceImage.FileHandle();
	}

TInt CImageDisplayFramework::SetImageSource(const TMMSource& aImageSource)
	{
	if (EStatePluginLoaded == iState)
		{
		DeletePlugin();
		iState = EStateOpened;
		}
	if (iState != EStateOpened)
		{
		Panic(EIllegalCallSequence);
		}
	iState = EStateConfigured;
	if (aImageSource.SourceType() == KUidMMFileSource)
		{
		iSourceImage.SetFilename(static_cast<const TMMFileSource&>(aImageSource).Name());
		}
	else if (aImageSource.SourceType() == KUidMMFileHandleSource)
		{
		iSourceImage.SetFileHandle(static_cast<const TMMFileHandleSource&>(aImageSource).Handle());
		}
	else if (aImageSource.SourceType() == KUidDescDataSource)
		{
		iSourceImage.SetData(static_cast<const TDescriptorDataSource&>(aImageSource).DataBuf());
		}
	else
		{
		return KErrNotSupported;
		}
	iSourceImage.SetIntent(aImageSource.Intent());
	iSourceImage.SetContentId(aImageSource.UniqueId());
	return KErrNone;
	}

void CImageDisplayFramework::SetSourceRect(const TRect& aRect)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState || EStatePluginLoaded == iState))
		{
		Panic(EIllegalCallSequence);
		}
	iSourceRect = aRect;
	SetFlag(EFlagSourceRectSet);
	}

void CImageDisplayFramework::ResetSourceRect()
	{
	if (!(EStateOpened == iState || EStateConfigured == iState || EStatePluginLoaded == iState))
		{
		Panic(EIllegalCallSequence);
		}
	ClearFlag(EFlagSourceRectSet);
	}

void CImageDisplayFramework::SetSizeInPixels(const TSize& aDestinationSize, TBool aMaintainAspectRatio)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState || EStatePluginLoaded == iState))
		{
		Panic(EIllegalCallSequence);
		}
	iDestinationSize = aDestinationSize;
	if (aMaintainAspectRatio)
		{
		SetFlag(EFlagMaintainAspectRatio);
		}
	else
		{
		ClearFlag(EFlagMaintainAspectRatio);
		}
	}

TInt CImageDisplayFramework::SetOptions(TUint aOptions)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState))
		{
		Panic(EIllegalCallSequence);
		}
	if ((aOptions & CImageDisplay::EOptionMainImage) != CImageDisplay::EOptionMainImage  
			&& (aOptions & CImageDisplay::EOptionThumbnail) != CImageDisplay::EOptionThumbnail)
		{
		return KErrArgument;
		}
	iOptions = aOptions;
	return KErrNone;
	}

void CImageDisplayFramework::SetSourceMimeType(const TDesC8& aMimeType)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState))
		{
		Panic(EIllegalCallSequence);
		}
	iSourceImage.SetMimeType(aMimeType);
	}

void CImageDisplayFramework::SetSourceImageType(TUid aImageType, TUid aImageSubType)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState))
		{
		Panic(EIllegalCallSequence);
		}
	iSourceImage.SetImageType(aImageType, aImageSubType);
	}

void CImageDisplayFramework::SetPluginUid(TUid aPluginUid)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState))
		{
		Panic(EIllegalCallSequence);
		}
	iPluginUid = aPluginUid;
	}

void CImageDisplayFramework::SetDisplayMode(TDisplayMode aDisplayMode)
	{
	if (!(EStateOpened == iState || EStateConfigured == iState))
		{
		Panic(EIllegalCallSequence);
		}
	iDisplayMode = aDisplayMode;	
	}

const TDesC8& CImageDisplayFramework::SourceMimeType() const
	{
	// this will return KNullDesC8 if not defined
	return iSourceImage.MimeType();
	}

const TUid CImageDisplayFramework::SourceImageType() const
	{
	return iSourceImage.ImageType();
	}

const TUid CImageDisplayFramework::SourceImageSubType() const
	{
	return iSourceImage.ImageSubType();
	}

TBool CImageDisplayFramework::SourceRect(TRect& aRect) const
	{
	if (FlagSet(EFlagSourceRectSet))
		{
		aRect = iSourceRect;
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}

const TSize& CImageDisplayFramework::DestinationSizeInPixels() const
	{
	return iDestinationSize;
	}

TUint CImageDisplayFramework::Options() const
	{
	return iOptions;
	}

TBool CImageDisplayFramework::MaintainAspectRatio() const
	{
	return FlagSet(EFlagMaintainAspectRatio);
	}

TDisplayMode CImageDisplayFramework::DisplayMode() const
	{
	return iDisplayMode;
	}

void CImageDisplayFramework::CallbackImageReady(const CFbsBitmap* aBitmap, TUint aStatus, const TRect& aUpdatedArea, TInt aError)
	{
	if (CImageDisplayPlugin::EStatusNoMoreToDecode & aStatus)
		{
		iState = EStatePluginLoaded;
		}
	else if (CImageDisplayPlugin::EStatusPaused & aStatus)
		{
		iState = EStatePaused;
		}

	iCallback.MiidoImageReady(aBitmap, aStatus, aUpdatedArea, aError);
	}

void CImageDisplayFramework::SetupL()
	{
	if (EStateConfigured != iState )
		{
		Panic(EUndefinedSourceType);
		}
// pre-conditions
	if (iOptions == CImageDisplay::EOptionsUndefined)
		{
		User::Leave(KErrArgument);
		}
	if (!iSourceImage.IsDataTypeDefined())
		{
		Panic(EUndefinedSourceType);
		}

	DeletePlugin();

	if (CImageDisplayPlugin::EImgSrcFileName == iSourceImage.ImageSourceType())
		{
		// check arguments
		TEntry entry;
		User::LeaveIfError(iFs.Entry(iSourceImage.Filename(), entry));
		if (SourceMimeType() == KNullDesC8)
			{
			iPlugin = FileNewImplL(
				iSourceImage.Filename(),
				iSourceImage.ImageType(), 
				iSourceImage.ImageSubType(),
				iPluginUid);
			}
		else
			{
			iPlugin = NewImplL(SourceMimeType());
			}
		}
	else if (CImageDisplayPlugin::EImgSrcFileHandle == iSourceImage.ImageSourceType())
			{
			iPlugin = FileHandleNewImplL(
						iSourceImage.FileHandle(),
						iSourceImage.ImageType(), 
					iSourceImage.ImageSubType(),
					iPluginUid
				);
			}
	else
		{
		if (iSourceImage.Data().Length() == 0)
			{
			User::Leave(KErrArgument);
			}
		if (SourceMimeType() == KNullDesC8)
			{
			iPlugin = DataNewL(
				iSourceImage.Data(), 
				iSourceImage.ImageType(), 
				iSourceImage.ImageSubType(),
				iPluginUid);
			}
		else
			{
			iPlugin = NewImplL(SourceMimeType());
			}
		}

	ASSERT(iPlugin);
	
	// if the plugin leaves in the OpenL() call, then we should 
	// delete the plugin in case SetupL() is called again (with no Reset())
	TRAPD(err, iPlugin->OpenL());
	if (err != KErrNone)
		{
		DeletePlugin();
		User::Leave(err);
		}
	iState = EStatePluginLoaded;
	}

void CImageDisplayFramework::Play()
	{
	if (!(iState == EStateProcessing || iState == EStatePaused || iState == EStatePluginLoaded))
		{
		Panic(EIllegalCallSequence);
		}
	if (iDestinationSize == TSize(0, 0))
		{
		Panic(EUndefinedDestSize);
		}
	iState = EStateProcessing;
	iPlugin->Play();
	}

void CImageDisplayFramework::Pause()
	{
	if (!(iState == EStateProcessing || iState == EStatePaused))
		{
		Panic(EIllegalCallSequence);
		}
	iState = EStatePaused;
	iPlugin->Pause();
	}

void CImageDisplayFramework::StopPlay()
	{
	if (EStateProcessing != iState && iState != EStatePluginLoaded && iState != EStatePaused)
		{
		Panic(EIllegalCallSequence);
		}
	iState = EStatePluginLoaded;
	iPlugin->StopPlay();
	}

void CImageDisplayFramework::Reset()
	{
	// Reset state to "just created" state
	DeletePlugin();

	iSourceImage.Reset();
	iDestinationSize = TSize(0, 0);
	iDisplayMode = ENone;
	ClearFlag(EFlagSourceRectSet);
	iOptions = CImageDisplay::EOptionsUndefined;
	iState = EStateOpened;
	}


/**
 * Create a display plugin for the image in the named file. The client supplies a
 * MIME type which will be used to try and select an appropriate plugin
 * Displayer. If it finds a Displayer it creates it and then goes on to use that
 * displayer to scan the beginning of the image file.
 * @internalTechnology
 * @param	"aFs"
 *			A reference to a file server session for the displayer to use.
 * @param	"aSourceFilename"
 *			The name of the file to be convertd.
 * @param	"aMIMEType"
 *			The MIME type of the image in the file.
 * @return	Returns a pointer to the newly created displayer.
 * @leave   KEComErrNoInterfaceIdentified
 *			No plugin displayers were found.
 */
CImageDisplayPlugin* CImageDisplayFramework::NewImplL(const TDesC8& aMIMEType)
	{
	//Get a sorted list of displayers that will handle the image
	RImplInfoPtrArray displayerList;
	CleanupResetAndDestroyPushL(displayerList);
	MimeTypeGetDisplayerListL(displayerList, aMIMEType);

	CImageDisplayPlugin* displayer = NULL;
	displayer = FindDisplayerNewL(displayerList);
	ASSERT(displayer!=NULL);

	CleanupStack::PopAndDestroy(&displayerList);
	return displayer;
	}


/**
 *
 * Create a displayer for the image in the named file. If the client supplies an
 * image type (and sub-type, if applicable) or displayer uid, these will be used 
 * to try and select an appropriate plugin displayer. If not, then the selection 
 * will be done by matching the image header in the file. If it finds a displayer
 * it creates it and then goes on to use that displayer to scan the beginning of
 * the image file.
 * @internalTechnology
 * @param	"aFs"
 *			A reference to a file server session for the displayer to use.
 * @param	"aSourceFilename"
 *			The name of the file to be converted.
 * @param	"aImageType"
 *			The image type of the image in the file (optional, defaults to KNullUid).
 * @param	"aImageSubType"
 *			The image sub-type of the image in the file (optional, defaults to KNullUid).
 * @param	"aPluginUid"
 *			The implementation uid for a specific codec (optional, defaults to KNullUid).
 *			If this option is selected the image type and
 *			image sub type for the displayer must be supplied.
 * @return	Returns a pointer to the newly created displayer.
 * @leave	KErrUnderflow
 *			Not enough data in file to identify which plugin displayer to use.
 * @leave   KEComErrNoInterfaceIdentified
 *			No plugin displayers were found.
 * @panic   EIllegalImageSubType
 *			No base type given for sub-type.
 * @panic   EIllegalImageType
 *			No base type given for displayer implementation.
 */
CImageDisplayPlugin* CImageDisplayFramework::FileNewImplL(
	const TDesC& aSourceFilename, 
	const TUid aImageType, 
	const TUid aImageSubType, 
	const TUid aPluginUid)
	{
	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	RImplInfoPtrArray displayerList;
	CleanupResetAndDestroyPushL(displayerList);

	TBuf8<KImageHeaderSize> imageHeader;

	ContentAccess::CData* data = ContentAccess::CData::NewLC(TVirtualPathPtr(aSourceFilename, iSourceImage.ContentId()), ContentAccess::EContentShareReadOnly);
	TInt err = data->SetProperty(EAgentPropertyAgentUI, iSourceImage.IsUIEnabled());
	// KErrCANotSupported doesn't prevent us from continuing so eat the error code.
	if (err != KErrNone && err != KErrCANotSupported)
		{
		User::Leave(err);
		}
	User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));

	TRAP(err, ImageTypeGetDisplayerListL(
					displayerList, 
					imageHeader, 
					aImageType, 
					aImageSubType, 
					aPluginUid)
		);

	if (!(err==KErrNone || err==KErrUnderflow))
		{
		User::Leave(err);
		}
	
	//Try to match by file extension only
	//1) If no plugin was found and 
	//2) No specific displayer or format was specified
	const TBool formatSpecified = (aImageType!=KNullUid || aImageSubType!=KNullUid || aPluginUid!=KNullUid);
	if(displayerList.Count()==0 && !formatSpecified)
		{
		SuffixTypeGetDisplayerListL(
			displayerList, 
			aSourceFilename);
		}

	CImageDisplayPlugin* displayer = NULL;
	displayer = FindDisplayerNewL(displayerList);

	ASSERT(displayer!=NULL);

	CleanupStack::PushL(displayer);
	User::LeaveIfError( data->EvaluateIntent(iSourceImage.Intent()) );
	CleanupStack::Pop(displayer);
	CleanupStack::PopAndDestroy(2, &displayerList);
	return displayer;
	}

/**
 *
 * Create a displayer for the image using the given file handle. If the client supplies an
 * image type (and sub-type, if applicable) or displayer uid, these will be used 
 * to try and select an appropriate plugin displayer. If not, then the selection 
 * will be done by matching the image header in the file. If it finds a displayer
 * it creates it and then goes on to use that displayer to scan the beginning of
 * the image file.
 * @internalTechnology
 * @param	"aFileHandle"
 *			A reference to a file handle that contains an image data 
 * @param	"aImageType"
 *			The image type of the image in the file (optional, defaults to KNullUid).
 * @param	"aImageSubType"
 *			The image sub-type of the image in the file (optional, defaults to KNullUid).
 * @param	"aPluginUid"
 *			The implementation uid for a specific codec (optional, defaults to KNullUid).
 *			If this option is selected the image type and
 *			image sub type for the displayer must be supplied.
 * @return	Returns a pointer to the newly created displayer.
 * @leave	KErrUnderflow
 *			Not enough data in file to identify which plugin displayer to use.
 * @leave   KEComErrNoInterfaceIdentified
 *			No plugin displayers were found.
 * @panic   EIllegalImageSubType
 *			No base type given for sub-type.
 * @panic   EIllegalImageType
 *			No base type given for displayer implementation.
 */
CImageDisplayPlugin* CImageDisplayFramework::FileHandleNewImplL(
	RFile& aFileHandle, 
	const TUid aImageType, 
	const TUid aImageSubType, 
	const TUid aPluginUid)
	{
	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	RImplInfoPtrArray displayerList;
	CleanupResetAndDestroyPushL(displayerList);

	TBuf8<KImageHeaderSize> imageHeader;

	ContentAccess::CData* data = ContentAccess::CData::NewLC(aFileHandle, iSourceImage.ContentId());
	TInt err = data->SetProperty(EAgentPropertyAgentUI, iSourceImage.IsUIEnabled());
	// KErrCANotSupported doesn't prevent us from continuing so eat the error code.
	if (err != KErrNone && err != KErrCANotSupported)
		{
		User::Leave(err);
		}
	User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));

	ImageTypeGetDisplayerListL(
		displayerList, 
		imageHeader, 
		aImageType, 
		aImageSubType, 
		aPluginUid);	

	CImageDisplayPlugin* displayer = NULL;
	displayer = FindDisplayerNewL(displayerList);
	ASSERT(displayer!=NULL);

	CleanupStack::PushL(displayer);
	User::LeaveIfError( data->EvaluateIntent(iSourceImage.Intent()) );
	CleanupStack::Pop(displayer);
	CleanupStack::PopAndDestroy(2, &displayerList);
	return displayer;
	}


/**
Creates a displayer for the image in the source buffer. 

If the client supplies an image type (and sub-type, if applicable) or displayer UID, 
these will be used to try and select an appropriate plugin displayer. If not, then 
the selection will be done by matching the image header from the buffer. If it
finds a displayer, it is created and then used to scan the beginning of the image
buffer.

@param  aSourceData
        The buffer containing the image to be displayed.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param	aPluginUid
        The implementation UID for a specific codec (optional, defaults to KNullUid).
        If this option is selected the image type and
        image sub type for the displayer must be supplied.

@return	Returns a pointer to the newly created displayer.

@leave  KErrUnderflow
        Not enough data in descriptor to identify which plugin displayer to use.
@leave  KEComErrNoInterfaceIdentified
        No plugin displayers were found.
@panic  EIllegalImageSubType
        No base type given for sub-type.

*/
CImageDisplayPlugin* CImageDisplayFramework::DataNewL(
	const TDesC8& aSourceData, 
	const TUid aImageType, 
	const TUid aImageSubType, 
	const TUid aPluginUid)
	{

	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	//Get a list of displayers that will convert the image
	RImplInfoPtrArray displayerList;
	CleanupResetAndDestroyPushL(displayerList);
	ImageTypeGetDisplayerListL(displayerList, aSourceData, aImageType, aImageSubType, aPluginUid);

	CImageDisplayPlugin* displayer = NULL;
	displayer = FindDisplayerNewL(displayerList);
	ASSERT(displayer!=NULL);

	CleanupStack::PopAndDestroy(&displayerList);
	return displayer;
	}



/**
 * Create a list of converts that support the specified MIME type.
 *
 * @param	"aDisplayerList"
 *			Create a list of converts that support the given MIME type.
 * @param	"aMIMEType"
 *			The MIME type to convert.
 * @panic EUndefinedMIMEType
 */
void CImageDisplayFramework::MimeTypeGetDisplayerListL(RImplInfoPtrArray& aDisplayerList, const TDesC8& aMIMEType)
	{
	if (aMIMEType.Length() == 0)
		{ // Get out, empty MIME type string
		Panic(EUndefinedMIMEType);
		}

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchMIMEType);
	customMatchData->SetMatchStringL(aMIMEType);
	if (FlagSet(EFlagSourceRectSet))
		{
		customMatchData->SetPluginFlagsNeeded(ESetSourceRectSupportNeeded);
		}

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(
		TUid::Uid(KUidDisplayInterface), 
		resolverParams, 
		TUid::Uid(KUidDisplayImplementationResolver), 
		aDisplayerList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}


/**
 * Create a list of displayers that support the specified image type.
 * @param	"aDisplayerList"
 *			A list of displayers that support the specified image type.
 * @param	"aImageHeader"
 *			The header of the image file.
 * @param	"aImageType"
 *			The image base type.
 * @param	"aImageSubType"
 *			The image sub type.
 * @param	"aPluginUid"
 *			A specific displayer to convert the image.
 */
void CImageDisplayFramework::ImageTypeGetDisplayerListL(
	RImplInfoPtrArray& aDisplayerList, 
	const TDesC8& aImageHeader, 
	const TUid aImageType, 
	const TUid aImageSubType, 
	const TUid aPluginUid)
	{
	TBuf8<KImageHeaderSize> imageHeader;

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();

	if ((aImageType != KNullUid) || (aPluginUid != KNullUid))
		{ // We have a specific image type we are trying to convert
		customMatchData->SetMatchType(EMatchUids);
		customMatchData->SetBaseType(aImageType);
		customMatchData->SetSubType(aImageSubType);
		customMatchData->SetImplementationType(aPluginUid);
		}
	else
		{
		if (aImageHeader.Length() < KImageHeaderSize) // There is not enough data in the header
			{ // Get out - clean up and leave
			User::Leave(KErrUnderflow); 
			}
		imageHeader = aImageHeader.Left(KImageHeaderSize);
		customMatchData->SetMatchType(EMatchString);
		customMatchData->SetMatchStringL(imageHeader);
		}

	if (FlagSet(EFlagSourceRectSet))
		{
		customMatchData->SetPluginFlagsNeeded(ESetSourceRectSupportNeeded);
		}

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(
		TUid::Uid(KUidDisplayInterface), 
		resolverParams, 
		TUid::Uid(KUidDisplayImplementationResolver), 
		aDisplayerList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}

/**
 * Create a list of displayers that support the specified file extension.
 *
 * @param	"aDisplayerList"
 *			Create a list of displayers that support the given file extension.
 * @param	"aFileName"
 *			The file name from which the file extension will be taken.
 * @internalComponent
 */
void CImageDisplayFramework::SuffixTypeGetDisplayerListL(
	RImplInfoPtrArray& aDisplayerList, 
	const TDesC& aFileName)
	{
	TParse fileName;
	fileName.Set(aFileName,NULL,NULL);

	//No file extension
	if (!fileName.ExtPresent())	
		{
		User::Leave(KErrNotFound);
		}
		
	//Get the suffix
	TPtrC suffix(fileName.Ext());

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchFileSuffix);
	customMatchData->SetFileSuffixL(suffix);
	if (FlagSet(EFlagSourceRectSet))
		{
		customMatchData->SetPluginFlagsNeeded( ESetSourceRectSupportNeeded );
		}
	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(
		TUid::Uid(KUidDisplayInterface),
		resolverParams, 
		TUid::Uid(KUidDisplayImplementationResolver),
		aDisplayerList);

	CleanupStack::PopAndDestroy(2,customMatchData); // package, customMatchData
	}


/**
 * Scan a sorted list of displayers for the first one that can convert the image.
 * @internalTechnology
 * @param	"aDisplayerList"
 *			A list of displayers that support the image format.
 * @return	A pointer to the displayer.
 */
CImageDisplayPlugin* CImageDisplayFramework::FindDisplayerNewL(
	const RImplInfoPtrArray& aDisplayerList)
	{
	TInt noOfDisplayers = aDisplayerList.Count();

	if(noOfDisplayers == 0)
		{
		User::Leave(KErrNotFound);
		}

	CImageDisplayPlugin* displayer = NULL;
	TInt displayerNo = 0;
	TInt error = KErrNone;
	do
		{
		const CImplementationInformation& displayerInfo = *(aDisplayerList[displayerNo++]);
		
		TRAP(error, displayer = CImageDisplayPlugin::NewL(displayerInfo.ImplementationUid(), *this));

		if (error != KErrCorrupt && error != KErrNotSupported && error != KErrNotFound)
			{
			break;
			}
		}
	while(displayerNo < noOfDisplayers);

	if(error!=KErrNone)
		{
		ASSERT(displayer==NULL);
		if (error == KErrCorrupt || error == KErrNotSupported)
			{
			error = KErrNotFound;
			}
		User::Leave(error);
		}

	return displayer;
	}

inline
void CImageDisplayFramework::DeletePlugin()
	{
	delete iPlugin;
	iPlugin = NULL;
	}
	
/**
Gets a pointer to a plugin extension

@panic	EIllegalCallSequence
		No plugin loaded.
@return	A pointer to a fully constructed  Image Display Plugin Extension interface
		NULL, if there is no extension to the plugin
*/
TInt CImageDisplayFramework::ExtensionInterface(TUid aIFaceUid, TAny*& aIFacePtr)
	{
	if (!iPlugin)
		{
		Panic(EIllegalCallSequence);
		}
		
	return iPlugin->ExtensionInterface(aIFaceUid, aIFacePtr);
	}
	
void CImageDisplayFramework::GetBitmap(const CFbsBitmap*& aBitmap, const CFbsBitmap*& aMask) const
	{
	if (EStateOpened == iState || EStateConfigured == iState )
		{
		Panic(EIllegalCallSequence);
		}
	iPlugin->GetBitmap(aBitmap, aMask);	
	}

TBool CImageDisplayFramework::ValidBitmap() const
	{
	if (!iPlugin)
		{
		Panic(EIllegalCallSequence);
		}
	return iPlugin->ValidBitmap();
	}
	
const CImageDisplay::RImageSizeArray& CImageDisplayFramework::RecommendedImageSizes() const
	{
	if (!iPlugin)
		{
		Panic(EIllegalCallSequence);
		}
	return iPlugin->RecommendedImageSizes();
	}

TUint CImageDisplayFramework::ImageStatus() const
	{
	if (EStateOpened == iState || EStateConfigured == iState )
		{
		Panic(EIllegalCallSequence);
		}
	return iPlugin->ImageStatus();
	}

TInt CImageDisplayFramework::NumFrames(TInt& aNumFrames) const
	{
	aNumFrames = -1;
	if (EStateOpened == iState || EStateConfigured == iState )
		{
		Panic(EIllegalCallSequence);
		}
	return iPlugin->NumFrames(aNumFrames);
	}
