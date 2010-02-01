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

#include "ImageTransformFramework.h"
#include "ImageTransform.h"
#include "ImageTransform.hrh"
#include "ImageTransformMain.h"
#include "../../src/ImageUtils.h"
#include "ImageTransformResolverAPI.h"
#include "ImageTransformResolvrUtils.h"

/*
* NewL
* The function NewL constructs a CImageTransformFramework
*
* @return	CImageTransformFramework*
*
*/
CImageTransformFramework* CImageTransformFramework::NewL(CImageTransform& aHandle, RFs& aFs)
	{
	CImageTransformFramework* self = new(ELeave) CImageTransformFramework(aHandle, aFs);
	return self;
	}


/*
*
* CImageTransformFramework
*
*/
CImageTransformFramework::CImageTransformFramework(CImageTransform& aHandle, RFs& aFs) :
	iHandle(aHandle), iFs(aFs)
	{
	Reset();
	}

/*
*
* ~CImageTransformFramework
* This is the destructor for the CImageTransformFramework
* and is resposible for deallocating all resources
* allocated by the CImageTransformFramework
*
*/
CImageTransformFramework::~CImageTransformFramework()
	{
	// make sure any existing transformation is cancelled
	if (iPlugin)
		{
		iPlugin->CancelTransform();
		DeletePlugin();
		}
	}

TBool CImageTransformFramework::SourceIsFilename() const
	{
	return iSourceImage.IsFilename();
	}

TBool CImageTransformFramework::SourceIsData() const
	{
	return iSourceImage.IsData();
	}

const TDesC& CImageTransformFramework::SourceFilename() const
	{
	if (!iSourceImage.IsFilename())
		{
		Panic(EImageWrongType);
		}
	return iSourceImage.Filename();
	}

const TDesC8& CImageTransformFramework::SourceData() const
	{
	if (!iSourceImage.IsData())
		{
		Panic(EImageWrongType);
		}
	return iSourceImage.Data();
	}

void CImageTransformFramework::SetPluginInstanceUidL(TUid aInstanceUid)
	{
	if (aInstanceUid == KNullUid)
		{
		User::Leave(KErrArgument);
		}
	iPluginInstanceUid = aInstanceUid;
	}

void CImageTransformFramework::SetSourceFilenameL(const TDesC& aFilename)
	{
	TEntry entry;
	User::LeaveIfError(iFs.Entry(aFilename, entry));

	iSourceImage.SetFilename(aFilename);
	}

void CImageTransformFramework::SetSourceDataL(const TDesC8& aData)
	{
	if (aData.Length() == 0)
		{
		User::Leave(KErrArgument);
		}

	iSourceImage.SetData(aData);
	}

void CImageTransformFramework::SetSourceRect(const TRect& aRect)
	{
	iSourceRect = aRect;
	SetFlag(EFlagSourceRectSet);
	}

void CImageTransformFramework::ClearSourceRect()
	{
	ClearFlag(EFlagSourceRectSet);
	}

void CImageTransformFramework::SetDestFilenameL(const TDesC& aFilename)
	{
	if (aFilename.Length() == 0)
		{
		User::Leave(KErrArgument);
		}

	iDestImage.SetFilename(aFilename);
	}

void CImageTransformFramework::SetDestDataL(HBufC8*& aData)
	{
	if (aData != NULL)
		{
		User::Leave(KErrArgument);
		}

	iDestImage.SetDataPtr(aData);
	}

void CImageTransformFramework::SetDestSizeInPixelsL(const TSize& aDestinationSize, TBool aMaintainAspectRatio)
	{
	if (aDestinationSize == TSize(0,0))
		{
		User::Leave(KErrArgument);
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

void CImageTransformFramework::SetOptionsL(TUint aOptions)
	{
	if (aOptions >= CImageTransform::EEnumBoundary)
		{
		User::Leave(KErrArgument);
		}
	if ((aOptions & CImageTransform::EThumbnail) && (aOptions & CImageTransform::EIgnoreExifMetadataProcessing))
		{
		// if both flags are set, then EThumbnail is used,
		// as setting both does not make sense in this context. 
		iOptions = iOptions & ~CImageTransform::EIgnoreExifMetadataProcessing; // clear the EIgnoreExifMetadataProcessing flag.
		}
	else
		{
		iOptions = aOptions;
		}
	}

void CImageTransformFramework::SetTransformationsL(TUint aTransformations)
	{
	if (aTransformations >= CImageTransform::ETransformEnumBoundary)
		{
		User::Leave(KErrArgument);
		}
	iTransformations = aTransformations;
	}

void CImageTransformFramework::SetPreserveImageData(TBool aPreserveImageData)
	{
	if (aPreserveImageData)
		{
		SetFlag(EFlagPreserveImageData);
		}
	else
		{
		ClearFlag(EFlagPreserveImageData);
		}
	}

void CImageTransformFramework::SetSourceMimeTypeL(const TDesC8& aMimeType)
	{
	if (aMimeType.Length() == 0)
		{
		User::Leave(KErrArgument);
		}

	iSourceImage.SetMimeType(aMimeType);
	}

void CImageTransformFramework::SetSourceImageTypeL(TUid aImageType, TUid aImageSubType)
	{
	if (aImageType == KNullUid)
		{
		User::Leave(KErrArgument);
		}

	iSourceImage.SetImageType(aImageType, aImageSubType);
	}

void CImageTransformFramework::SetPluginUidL(TUid aPluginUid)
	{
	if (aPluginUid == TUid::Null())
		{
		User::Leave(KErrArgument);
		}

	iPluginUid = aPluginUid;
	}

const TDesC8& CImageTransformFramework::SourceMimeType() const
	{
	// this will return KNullDesC8 if not defined
	return iSourceImage.MimeType();
	}

const TUid CImageTransformFramework::SourceImageType() const
	{
	return iSourceImage.ImageType();
	}

const TUid CImageTransformFramework::SourceImageSubType() const
	{
	return iSourceImage.ImageSubType();
	}

TBool CImageTransformFramework::SourceRect(TRect& aRect) const
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

TBool CImageTransformFramework::DestIsFilename() const
	{
	return iDestImage.IsFilename();
	}

TBool CImageTransformFramework::DestIsData() const
	{
	return iDestImage.IsData();
	}

const TSize& CImageTransformFramework::DestinationSizeInPixels() const
	{
	return iDestinationSize;
	}

const TDesC& CImageTransformFramework::DestFilename() const
	{
	if (!iDestImage.IsFilename())
		{
		Panic(EImageWrongType);
		}
	return iDestImage.Filename();
	}

HBufC8*& CImageTransformFramework::DestData() const
	{
	if (!iDestImage.IsData())
		{
		Panic(EImageWrongType);
		}
	return iDestImage.DataPtr();
	}

TUint CImageTransformFramework::Options() const
	{
	return iOptions;
	}

TBool CImageTransformFramework::MaintainAspectRatio() const
	{
	return FlagSet(EFlagMaintainAspectRatio);
	}

TBool CImageTransformFramework::PreserveImageData() const
	{
	return FlagSet(EFlagPreserveImageData);
	}

void CImageTransformFramework::SetupL()
	{
	if (!iSourceImage.IsDataTypeDefined())
		{
		User::Leave(KErrNotReady);
		}
	
	if (!iDestImage.IsDataTypeDefined())
		{
		User::Leave(KErrNotReady);
		}
	
	if (iTransformations & CImageTransform::ESqueeze)
		{
		// Destination size should not be set for squeeze
		if (iDestinationSize != TSize(0, 0))
			{
			User::Leave(KErrArgument);
			}
		}
	else if ((iTransformations == CImageTransform::ETransformationNone) || 
			 (iTransformations & CImageTransform::EScale))
		{
		// Destination size should be set
		if (iDestinationSize == TSize(0, 0))
			{
			User::Leave(KErrArgument);
			}
		}
	else if (iTransformations == CImageTransform::EOverlay)
		{
		if (iDestinationSize != iSourceRect.Size())
			{
			User::Leave(KErrNotSupported);
			}
		}

	//delete existing plugin if present
	if (iPlugin)
		{
		DeletePlugin();
		}

	if (iSourceImage.IsFilename())
		{
		if (SourceMimeType() == KNullDesC8)
			{
			iPlugin = FileNewImplL(
				iFs,
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
	else
		{
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
	}

void CImageTransformFramework::Transform(TRequestStatus& aStatus)
	{
	if (!iPlugin)
		{
		Panic(EIllegalCallSequence);
		}
	iPlugin->Transform(aStatus);
	}

void CImageTransformFramework::CancelTransform()
	{
	if (iPlugin)
		{
		iPlugin->CancelTransform();
		}
	}

void CImageTransformFramework::Reset()
	{
	// Reset state to "just created" state
	if (iPlugin)
		{
		DeletePlugin();
		}
	iSourceImage.Reset();
	iDestImage.Reset();
	iDestinationSize = TSize(0, 0);
	ClearFlag(EFlagSourceRectSet);
	iTransformations = CImageTransform::ETransformationNone;
	}


/*
 * Create a transformer for the image in the named file. The client supplies a
 * MIME type which will be used to try and select an appropriate plugin
 * Transformer. If it finds a Transformer it creates it and then goes on to use that
 * transformer to scan the beginning of the image file.
 * @internalTechnology
 * @param	"aFs"
 *			A reference to a file server session for the transformer to use.
 * @param	"aSourceFilename"
 *			The name of the file to be convertd.
 * @param	"aMIMEType"
 *			The MIME type of the image in the file.
 * @return	Returns a pointer to the newly created transformer.
 * @leave   KEComErrNoInterfaceIdentified
 *			No plugin transformers were found.
 */
CImageTransformPlugin* CImageTransformFramework::NewImplL(const TDesC8& aMIMEType)
	{
	//Get a sorted list of transformers that will convert the image
	RImplInfoPtrArray transformerList;
	CleanupResetAndDestroyPushL(transformerList);
	MimeTypeGetTransformerListL(transformerList, aMIMEType);

	CImageTransformPlugin* transformer = NULL;
	transformer = FindTransformerNewL(transformerList);
	ASSERT(transformer!=NULL);

	CleanupStack::PopAndDestroy(&transformerList);
	return transformer;
	}


/*
 *
 * Create a transformer for the image in the named file. If the client supplies an
 * image type (and sub-type, if applicable) or transformer uid, these will be used
 * to try and select an appropriate plugin transformer. If not, then the selection
 * will be done by matching the image header in the file. If it finds a transformer
 * it creates it and then goes on to use that transformer to scan the beginning of
 * the image file.
 * @internalTechnology
 * @param	"aFs"
 *			A reference to a file server session for the transformer to use.
 * @param	"aSourceFilename"
 *			The name of the file to be converted.
 * @param	"aImageType"
 *			The image type of the image in the file (optional, defaults to KNullUid).
 * @param	"aImageSubType"
 *			The image sub-type of the image in the file (optional, defaults to KNullUid).
 * @param	"aPluginUid"
 *			The implementation uid for a specific codec (optional, defaults to KNullUid).
 *			If this option is selected the image type and
 *			image sub type for the transformer must be supplied.
 * @return	Returns a pointer to the newly created transformer.
 * @leave	KErrUnderflow
 *			Not enough data in file to identify which plugin transformer to use.
 * @leave   KEComErrNoInterfaceIdentified
 *			No plugin transformers were found.
 * @panic   EIllegalImageSubType
 *			No base type given for sub-type.
 * @panic   EIllegalImageType
 *			No base type given for transformer implementation.
 */
CImageTransformPlugin* CImageTransformFramework::FileNewImplL(
	RFs& aFs,
	const TDesC& aSourceFilename,
	const TUid aImageType,
	const TUid aImageSubType,
	const TUid aPluginUid)
	{
	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType);
		}

	RImplInfoPtrArray transformerList;
	CleanupResetAndDestroyPushL(transformerList);

	TBuf8<KImageHeaderSize> imageHeader;
	RFile file;
	CleanupClosePushL(file);

	TInt err = file.Open(aFs, aSourceFilename, EFileShareReadersOnly);
	if(err != KErrNone && err != KErrNoMemory)
		{
		err = file.Open(aFs, aSourceFilename, EFileShareAny);
		}
	User::LeaveIfError(err);

	User::LeaveIfError(file.Read(imageHeader, KImageHeaderSize));
	CleanupStack::PopAndDestroy(&file);

	ImageTypeGetTransformerListL(
		transformerList,
		imageHeader,
		aImageType,
		aImageSubType,
		aPluginUid);

	//Try to match by file extension only
	//1) If no plugin was found and
	//2) No specific transformer or format was specified
	const TBool formatSpecified = (aImageType!=KNullUid || aImageSubType!=KNullUid || aPluginUid!=KNullUid);
	if(transformerList.Count()==0 && !formatSpecified)
		{
		SuffixTypeGetTransformerListL(
			transformerList,
			aSourceFilename);
		}

	CImageTransformPlugin* transformer = NULL;
	transformer = FindTransformerNewL(transformerList);

	ASSERT(transformer!=NULL);

	CleanupStack::PopAndDestroy(&transformerList);
	return transformer;
	}


/*
Creates a transformer for the image in the source buffer.

If the client supplies an image type (and sub-type, if applicable) or transformer UID,
these will be used to try and select an appropriate plugin transformer. If not, then
the selection will be done by matching the image header from the buffer. If it
finds a transformer, it is created and then used to scan the beginning of the image
buffer.

@param  aSourceData
        The buffer containing the image to be converted.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param	aPluginUid
        The implementation UID for a specific codec (optional, defaults to KNullUid).
        If this option is selected the image type and
        image sub type for the transformer must be supplied.

@return	Returns a pointer to the newly created transformer.

@leave  KErrUnderflow
        Not enough data in descriptor to identify which plugin transformer to use.
@leave  KEComErrNoInterfaceIdentified
        No plugin transformers were found.
@panic  EIllegalImageSubType
        No base type given for sub-type.

*/
CImageTransformPlugin* CImageTransformFramework::DataNewL(
	const TDesC8& aSourceData,
	const TUid aImageType,
	const TUid aImageSubType,
	const TUid aPluginUid)
	{

	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType);
		}

	//Get a list of transformers that will convert the image
	RImplInfoPtrArray transformerList;
	CleanupResetAndDestroyPushL(transformerList);
	ImageTypeGetTransformerListL(transformerList, aSourceData, aImageType, aImageSubType, aPluginUid);

	CImageTransformPlugin* transformer = NULL;
	transformer = FindTransformerNewL(transformerList);
	ASSERT(transformer!=NULL);

	CleanupStack::PopAndDestroy(&transformerList);
	return transformer;
	}



/*
 * Create a list of converts that support the specified MIME type.
 *
 * @param	"aTransformerList"
 *			Create a list of converts that support the given MIME type.
 * @param	"aMIMEType"
 *			The MIME type to convert.
 */
void CImageTransformFramework::MimeTypeGetTransformerListL(RImplInfoPtrArray& aTransformerList, const TDesC8& aMIMEType)
	{
	if (aMIMEType.Length() == 0)
		{ // Get out, empty MIME type string
		Panic(EUndefinedMIMEType);
		}

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchMIMEType);
	customMatchData->SetMatchStringL(aMIMEType);

	TUint flagsNeeded = NeededPluginFlags();
	// Set the required transformations
	customMatchData->SetPluginFlagsNeeded(flagsNeeded);
	
	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(
		TUid::Uid(KUidTransformInterface),
		resolverParams,
		TUid::Uid(KUidTransformImplementationResolver),
		aTransformerList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}


/*
 * Create a list of transformers that support the specified image type.
 * @param	"aTransformerList"
 *			A list of transformers that support the specified image type.
 * @param	"aImageHeader"
 *			The header of the image file.
 * @param	"aImageType"
 *			The image base type.
 * @param	"aImageSubType"
 *			The image sub type.
 * @param	"aPluginUid"
 *			A specific transformer to convert the image.
 */
void CImageTransformFramework::ImageTypeGetTransformerListL(
	RImplInfoPtrArray& aTransformerList,
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

	TUint flagsNeeded = NeededPluginFlags();	
	// Set the required transformations
	customMatchData->SetPluginFlagsNeeded(flagsNeeded);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(
		TUid::Uid(KUidTransformInterface),
		resolverParams,
		TUid::Uid(KUidTransformImplementationResolver),
		aTransformerList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}

/*
 * Create a list of transformers that support the specified file extension.
 *
 * @param	"aTransformerList"
 *			Create a list of transformers that support the given file extension.
 * @param	"aFileName"
 *			The file name from which the file extension will be taken.
 * @internalComponent
 */
void CImageTransformFramework::SuffixTypeGetTransformerListL(
	RImplInfoPtrArray& aTransformerList,
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

	TUint flagsNeeded = NeededPluginFlags();	
	// Set the required transformations
	customMatchData->SetPluginFlagsNeeded(flagsNeeded);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(
		TUid::Uid(KUidTransformInterface),
		resolverParams,
		TUid::Uid(KUidTransformImplementationResolver),
		aTransformerList);

	CleanupStack::PopAndDestroy(2,customMatchData); // package, customMatchData
	}


/*
 * Scan a sorted list of transformers for the first one that can convert the image.
 * @internalTechnology
 * @param	"aTransformerList"
 *			A list of transformers that support the image format.
 * @return	A pointer to the transformer.
 */
CImageTransformPlugin* CImageTransformFramework::FindTransformerNewL(
	const RImplInfoPtrArray& aTransformerList)
	{
	TInt noOfTransformers = aTransformerList.Count();

	if(noOfTransformers == 0)
		{
		User::Leave(KErrNotFound);
		}

	CImageTransformPlugin* transformer = NULL;
	TInt transformerNo = 0;
	TInt error = KErrNone;
	do
		{
		const CImplementationInformation& transformerInfo = *(aTransformerList[transformerNo++]);

		TRAP(error, transformer = CImageTransformPlugin::NewL(transformerInfo.ImplementationUid(), *this));

		if (error != KErrCorrupt && error != KErrNotSupported && error != KErrNotFound)
			{
			break;
			}
		}
	while(transformerNo < noOfTransformers);

	if(error!=KErrNone)
		{
		ASSERT(transformer==NULL);
		if (error == KErrCorrupt || error == KErrNotSupported)
			{
			error = KErrNotFound;
			}
		User::Leave(error);
		}

	return transformer;
	}

void CImageTransformFramework::DeletePlugin()
	{
	delete iPlugin;
	iPlugin = NULL;
	REComSession::DestroyedImplementation(iPluginInstanceUid);
	}

/**
Gets a pointer to a plugin extension

@panic	EIllegalCallSequence
		No plugin loaded.
@return	A pointer to a fully constructed CImageTransformPluginExtension
		NULL, if there is no extension to the plugin
*/
CImageTransformPluginExtension* CImageTransformFramework::Extension() const
	{
	if (!iPlugin)
		{
		Panic(EIllegalCallSequence);
		}
	return iPlugin->Extension();
	}

/**
Gets a pointer to a plugin extension

@panic	EIllegalCallSequence
		No plugin loaded.
@param  "aExtensionUid"		
		Uid of the required extension
@param	"aError"
		System wide error
@return	A pointer to a fully constructed CImageTransformPluginExtension
		NULL, if there is no extension to the plugin
*/
CImageTransformPluginExtension* CImageTransformFramework::Extension(TUid aExtensionUid, TInt& aError) const
	{
	if (!iPlugin)
		{
		Panic(EIllegalCallSequence);
		}
	return iPlugin->Extension(aExtensionUid, aError);
	}

/**
Gets the flags needed in the plugin
@return	Bitwise ORed flags
*/
TUint CImageTransformFramework::NeededPluginFlags()
	{
	TUint flagsNeeded = 0;

	// Check if scaling is requested
	if ((iTransformations == CImageTransform::ETransformationNone) || 
		(iTransformations & CImageTransform::EScale))
		{
		flagsNeeded |= ESetScaleSupportNeeded;
		}
	// Check if cropping is requested
	if (FlagSet(EFlagSourceRectSet) || 
		(iTransformations & CImageTransform::ECrop))
		{
		flagsNeeded |= ESetSourceRectSupportNeeded;
		}
	// Check if Squeeze transform is requested. 	
	if (iTransformations & CImageTransform::ESqueeze)
		{
		flagsNeeded |= ESetSqueezeSupportNeeded;
		}
	// Check if Orientations transform is requested.	
	if (iTransformations & CImageTransform::EOrientation)
		{
		flagsNeeded |= ESetOrientationSupportNeeded;
		}
	// Check if Overlay transform is requested.	
	if (iTransformations & CImageTransform::EOverlay)
		{
		flagsNeeded |= ESetOverlaySupportNeeded;
		}
	// Check if exif support is requested.
	if (iTransformations & CImageTransform::EExif)
		{
		flagsNeeded |= ESetExifSupportNeeded;
		}
	return flagsNeeded;
	}

