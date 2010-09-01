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
//

#include <ecom/ecom.h>
#include <bitdev.h>
#include <bautils.h>
#include <barsc.h>
#include <caf/caf.h>
using namespace ContentAccess;

#include "ImageConversion.h"
#include "ImageClientMain.h"
#include "ImageResolverAPI.h"
#include "ImageUtils.h"
#include "ImageConvResolvrUtils.h"
#include "ImageRelay.h"
#include "icl/ImageConstruct.h"
#include "icl/ImagePlugin.h"
#include "ImageConversion.inl"
#include "ImageCodecBody.h"
#include "EnDecoderUtils.h"
#include "icl/imageconversionextension.h"
#include "fwextconstants.h"

const TInt KMaxMimeLength = 256;
const TInt KMinimumHeaderLength = 2;


//
// CImageTypeDescription
//

/**
Constructs a new image type description entry.

A leave occurs if there is insufficient memory available.

@param  aDescription
        A description of the plugin decoder/encoder.
@param  aImageType
        The plugin decoder/encoder type UID.
@param  aSubType
        The plugin decoder/encoder sub-type UID.

@return A pointer to the new image type description entry.
*/
CImageTypeDescription* CImageTypeDescription::NewL(const TDesC& aDescription, const TUid aImageType, const TUid aSubType)
	{
	CImageTypeDescription* self = CImageTypeDescription::NewLC(aDescription, aImageType, aSubType);
	CleanupStack::Pop();
	return self;
	}

/**
Constructs a new image type description entry. The new object created on the clean-up stack.

A leave occurs if there is insufficient memory available.

@param  aDescription
        A description of the plugin decoder/encoder.
@param  aImageType
        The plugin decoder/encoder type UID.
@param  aSubType
        The plugin decoder/encoder sub-type UID.

@return A pointer to the new image type description entry.
*/
CImageTypeDescription* CImageTypeDescription::NewLC(const TDesC& aDescription, const TUid aImageType, const TUid aSubType)
	{
	CImageTypeDescription* self = new(ELeave) CImageTypeDescription(aImageType, aSubType);
	CleanupStack::PushL(self);
	self->ConstructL(aDescription);
	return self;
	}


/** 
Default constructor. 
@param aImageType 
           The plugin decoder/encoder type UID. 
@param aSubType 
           The plugin decoder/encoder sub-type UID 
*/ 

CImageTypeDescription::CImageTypeDescription(const TUid aImageType, const TUid aSubType)
	: iImageType(aImageType), iSubType(aSubType)
	{
	}

/** 
Second phase constructor. 
@param aDescription 
           A description of the plugin decoder/encoder. 
*/ 

void CImageTypeDescription::ConstructL(const TDesC& aDescription)
	{
	iDescription = aDescription.AllocL();
	}

/**
Destructor.
*/
EXPORT_C CImageTypeDescription::~CImageTypeDescription()
	{
	delete iDescription;
	}

/**
Returns the image description info.

@return  A reference to the descriptor containing the description.
*/
EXPORT_C const TDesC& CImageTypeDescription::Description() const
	{
	return *iDescription;
	}

/**
Returns the image type info.

@return The image type info as a TUid object.
*/
EXPORT_C TUid CImageTypeDescription::ImageType() const
	{
	return iImageType;
	}

/**
Returns the image sub-type information.

@return The image sub-type information as a TUid object.
*/
EXPORT_C TUid CImageTypeDescription::SubType() const
	{
	return iSubType;
	}

//
// CFileExtensionMIMEType
//

/**
Constructs a new file extension/MIME type entry.

A leave occurs if there is insufficient memory available.

@param  aExtn
        The file extension.
@param  aMIMEType
        The associated MIME type.
@param  aDisplayName The display name of the implementation 
@param  aImageType The plugin decoder/encoder type UID. 
@param  aImageSubType The plugin decoder/encoder sub-type UID. 
@param  aImplementationUid The unique ID of the implementation. 
@return A pointer to the new file extension/MIME type entry.
*/
CFileExtensionMIMEType* CFileExtensionMIMEType::NewL(const TDesC8& aExtn, const TDesC8& aMIMEType, const TDesC& aDisplayName, TUid aImageType, TUid aImageSubType, TUid aImplementationUid)
	{
	CFileExtensionMIMEType* self = CFileExtensionMIMEType::NewLC(aExtn, aMIMEType, aDisplayName, aImageType, aImageSubType, aImplementationUid);
	CleanupStack::Pop();
	return self;
	}

/**
Constructs a new file extension/MIME type entry. The new object is left on the clean-up stack.

A leave occurs if there is insufficient memory available.

@param  aExtn
        The file extension.
@param  aMIMEType 
        The associated MIME type.
@param  aDisplayName The display name of the implementation 
@param  aImageType The plugin decoder/encoder type UID. 
@param  aImageSubType The plugin decoder/encoder sub-type UID. 
@param  aImplementationUid The unique ID of the implementation. 
@return A pointer to the new file extension/MIME type entry.
*/
EXPORT_C CFileExtensionMIMEType* CFileExtensionMIMEType::NewLC(const TDesC8& aExtn, const TDesC8& aMIMEType, const TDesC& aDisplayName, TUid aImageType, TUid aImageSubType, TUid aImplementationUid)
	{
	CFileExtensionMIMEType* self = new(ELeave) CFileExtensionMIMEType;
	CleanupStack::PushL(self);
	self->ConstructL(aExtn, aMIMEType, aDisplayName, aImageType, aImageSubType, aImplementationUid);
	return self;
	}
/**
Default constructor. 
*/
CFileExtensionMIMEType::CFileExtensionMIMEType()
	{
	}

/**
Second phase constructor. 
@param aExtn The file extension 
@param aMIMEType The associated MIME type. 
@param aDisplayName The display name of the implementation 
@param aImageType The plugin decoder/encoder type UID. 
@param aImageSubType The plugin decoder/encoder sub-type UID. 
@param aImplementationUid The unique ID of the implementation. 
*/
void CFileExtensionMIMEType::ConstructL(const TDesC8& aExtn, const TDesC8& aMIMEType, const TDesC& aDisplayName, TUid aImageType, TUid aImageSubType, TUid aImplementationUid)
	{
	iFileExtension = HBufC::NewL(aExtn.Length());
	iFileExtension->Des().Copy(aExtn); // Create a 16 bit copy of the 8 bit original
	ASSERT(iFileExtension->Length() == aExtn.Length());
	iMIMEType = aMIMEType.AllocL();
	ASSERT(iMIMEType->Length() == aMIMEType.Length());
	iDisplayName = aDisplayName.AllocL();
	iImageTypeUid = aImageType;
	iImageSubTypeUid = aImageSubType;
	iImplementationUid = aImplementationUid;
	}

/**
Destructor.

Frees all resources owned by the object prior to its destruction.
*/
EXPORT_C CFileExtensionMIMEType::~CFileExtensionMIMEType()
	{
	delete iFileExtension;
	delete iMIMEType;
	delete iDisplayName;
	}

/**
Returns the image file extension info.

@return A reference to the descriptor containing the file extension.
*/
EXPORT_C const TDesC& CFileExtensionMIMEType::FileExtension() const
	{
	return *iFileExtension;
	}

/**
Returns the image MIME type info.

@return A reference to the descriptor containing the MIME type.
*/
EXPORT_C const TDesC8& CFileExtensionMIMEType::MIMEType() const
	{
	return *iMIMEType;
	}

/**
Returns the human-readable plugin description.

@return A reference to the display descriptor.
*/
EXPORT_C const TDesC& CFileExtensionMIMEType::DisplayName() const
	{
	return *iDisplayName;
	}

/**
Returns the image type associated with the plugin.

@return The image type associated with the plugin
*/
EXPORT_C TUid CFileExtensionMIMEType::ImageType() const
	{
	return iImageTypeUid;
	}

/**
Returns the image sub-type associated with the plugin.

@return The image sub-type associated with the plugin.
*/
EXPORT_C TUid CFileExtensionMIMEType::ImageSubType() const
	{
	return iImageSubTypeUid;
	}

/**
Returns the implementation UID of the plugin.

@return	The implementation UID of the plugin.
*/
EXPORT_C TUid CFileExtensionMIMEType::ImplementationUid() const
	{
	return iImplementationUid;
	}

//
// CImplementationInformationType
//

/**
Constructs a new implementation information object.

@return A pointer to the new implementation information object.
*/

CImplementationInformationType* CImplementationInformationType::NewL()
	{
	CImplementationInformationType* self = CImplementationInformationType::NewLC();
	CleanupStack::Pop();
	return self;
	}

// CImplementationInformationType transfer ownership of iDisplyName, iDataType and iOpaqueData

/**
Sets the contents of the CImplementationInformationType object.

Use this variant of SetDataL() if the plugin is installed on a drive other than the default, which is zero.

@param  aImplementationUid
        The unique ID of the implementation.
@param  aVersion
        The version number of the implementation.
@param  aDisplayName
        The display name of the implementation.
@param  aDataType
        The data type supported by the implementation.
@param  aOpaqueData
        Additional data for this implementation. This data is not used by the ECom framework.
@param  aDrive
        The drive on which the plugin is installed.
*/
void CImplementationInformationType::SetDataL(TUid aImplementationUid, TInt aVersion, const TDesC& aDisplayName, const TDesC8& aDataType, const TDesC8& aOpaqueData, TDriveUnit aDrive)
	{
	iImplementationUid = aImplementationUid;
	iVersion = aVersion;

	delete iDisplayName; iDisplayName = NULL;
	iDisplayName = aDisplayName.AllocL();

	delete iDataType; iDataType = NULL;
	iDataType = aDataType.AllocL();

	delete iOpaqueData; iOpaqueData = NULL;
	iOpaqueData = aOpaqueData.AllocL();

	iDrive = aDrive;
	}

/**
Default constructor.

This member is internal and not intended for use.
*/
CImplementationInformationType::CImplementationInformationType()
	{
	}

/**
Constructs a new implementation information object. The new object is left on the clean-up stack.

@return A pointer to the new implementation information object.
*/
CImplementationInformationType* CImplementationInformationType::NewLC()
	{
	CImplementationInformationType* self = new(ELeave) CImplementationInformationType;
	CleanupStack::PushL(self);
	return self;
	}

/**
Destructor.

Frees all resources owned by the object prior to its destruction.
*/
EXPORT_C CImplementationInformationType::~CImplementationInformationType()
	{
	delete iDisplayName;
	delete iDataType;
	delete iOpaqueData;
	}


/**
Returns the implementation UID.

@return The implementation UID as a TUid object.
*/
EXPORT_C TUid CImplementationInformationType::ImplementationUid() const
	{
	return iImplementationUid;
	}

/**
Returns the version number of the implementation.

@return The version number of the implementation.
*/
EXPORT_C TInt CImplementationInformationType::Version() const
	{
	return iVersion;
	}

/**
Returns the display name of the implementation.

@return A reference to the display name of the implementation.
*/
EXPORT_C const TDesC& CImplementationInformationType::DisplayName() const
	{
	return *iDisplayName;
	}

/**
Returns the data type supported by the implementation.

@return A reference to the descriptor containing the data type.
*/
EXPORT_C const TDesC8& CImplementationInformationType::DataType() const
	{
	return *iDataType;
	}

/**
Returns the opaque binary data for the implementation.

@return A reference to the descriptor containing the data.
*/
EXPORT_C const TDesC8& CImplementationInformationType::OpaqueData() const
	{
	return *iOpaqueData;
	}

/**
Returns the drive location information for the implementation.

@return The drive location information as a TDriveUnit object.
*/
EXPORT_C TDriveUnit CImplementationInformationType::Drive()
	{
	return iDrive;
	}

/**
Function to sort an array of CFileExtensionMIMEType in ascending
order of file extesion and MIME type.

@param  aItem1
        The first element.
@param  aItem2
        The second element.

@return	An indication of the element swapping order.
*/
TInt ImageEnDecoderUtils::SortAsending(const CFileExtensionMIMEType& aItem1, const CFileExtensionMIMEType& aItem2)
	{
	TInt result = aItem1.FileExtension().Compare(aItem2.FileExtension());
	if(result==0)
		result = aItem1.MIMEType().Compare(aItem2.MIMEType());
	return result;
	}

// Used by the 4 decoder and encoder functions that return arrays of Uids and plugin descriptions
void ImageEnDecoderUtils::DoGetImageTypesL(TUid aInterfaceUid, RImageTypeDescriptionArray& aImageTypeArray, TUid aBaseType)
	{
	RImplInfoPtrArray pluginArray; // Array to return matching plugins in
	CleanupResetAndDestroyPushL(pluginArray);

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EListImageTypes);
	customMatchData->SetBaseType(aBaseType);
	customMatchData->SetSubType(KNullUid);
	customMatchData->SetImplementationType(KNullUid);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(aInterfaceUid, resolverParams, KImageConvertResolverUid, pluginArray);
	if (pluginArray.Count() == 0)
		User::Leave(KErrNotFound);

	CleanupStack::PopAndDestroy(2); // package, customMatchData

	// Transfer data to our array
	for (TInt index = 0; index < pluginArray.Count(); index++)
		{
		COpaqueDataParse* parser = NULL;
		TRAPD(error, parser = COpaqueDataParse::NewL(pluginArray[index]->OpaqueData()));
		if (error!=KErrNone)
			{
			if (error==KErrNotSupported)
				{
				// means that the resource entry was not valid
				continue;
				}
			else
				User::Leave(error);
			}

		CleanupStack::PushL(parser);
		CImageTypeDescription* imageTypeDescription = CImageTypeDescription::NewLC(pluginArray[index]->DisplayName(), parser->ImageTypeUid(), parser->ImageSubTypeUid());

		TBool mustAppend = ETrue;
		for(TInt i=0; i< aImageTypeArray.Count(); i++)
			{
			if(	aImageTypeArray[i]->ImageType().iUid == imageTypeDescription->ImageType().iUid &&
				aImageTypeArray[i]->SubType().iUid == imageTypeDescription->SubType().iUid &&
				(aImageTypeArray[i]->Description().Compare(imageTypeDescription->Description())) == 0)
				{
				// If here - image type implementation with same description is already in array.
				mustAppend = EFalse;
				CleanupStack::PopAndDestroy(imageTypeDescription); 
				break;
				}
			}
		if(mustAppend)
			{
			//A new image type implementation, add it to array.
			User::LeaveIfError(aImageTypeArray.Append(imageTypeDescription));		
			CleanupStack::Pop(imageTypeDescription); 
			}
		CleanupStack::PopAndDestroy(parser);
		}
	CleanupStack::PopAndDestroy(&pluginArray); 
	}

// Used by both the decoder and encoder functions that return arrays of file extensions and MIME types.
// This function creates an array of each valid file extension along with the associated "primary"
// (ie. first) MIME type plus the "secondary" MIME type(s).
void ImageEnDecoderUtils::DoGetFileTypesL(TUid aInterfaceUid, RFileExtensionMIMETypeArray& aFileTypeArray)
	{
	RImplInfoPtrArray pluginArray; // Array to return matching plugins in
	CleanupResetAndDestroyPushL(pluginArray);

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EListFileTypes);

	HBufC8* package = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(aInterfaceUid, resolverParams, KImageConvertResolverUid, pluginArray);
	if (pluginArray.Count() == 0)
		{
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(2); // package, customMatchData

	// Transfer data to our array
	for (TInt index = 0; index < pluginArray.Count(); index++)
		{
		CImplementationInformation* implInfo = pluginArray[index];
		COpaqueDataParse* parser = NULL;
		TRAPD(error, parser = COpaqueDataParse::NewL(implInfo->OpaqueData()));
		if (error != KErrNone)
			{
			if (error==KErrNotSupported)
				{
				// means that the resource entry was not valid
				continue;
				}
			else
				{
				User::Leave(error);
				}				
			}

		CleanupStack::PushL(parser);
  		if (parser->OnlyUidsAvail() || parser->IsSubCodec()) // ignore simple descriptions
			{
			CleanupStack::PopAndDestroy(); // parser
			continue;
			}
		parser->EnsureMIMETypesReadL();

		// Return all the MIME types specified in the resource file, not just the primary
		// one by filling the file type array with elements created multiplicatively
		// for each extension and each MIME type.

		// use a blank MIME type if none provided by the plugin
		const TBool blankMIMEType = (parser->MIMETypesCount() == 0);
		const TInt numMIMETypes = blankMIMEType ? 1 : parser->MIMETypesCount();

		//use a blank extension if not provided by plugin
		const TBool blankExtn = (parser->ExtnsCount() == 0);
		const TInt numExtns = blankExtn ? 1 : parser->ExtnsCount();

		TLinearOrder<CFileExtensionMIMEType> sortFunction(SortAsending);
		for (TInt index2 = 0; index2 < numExtns; index2++)
			{
			const TDesC8& extn = blankExtn ? KNullDesC8() : parser->Extn(index2);
			for (TInt index3 = 0; index3 < numMIMETypes; index3++)
				{
				const TDesC8& MIMEType = blankMIMEType ? KNullDesC8() : parser->MIMEType(index3);
				CFileExtensionMIMEType* fileExtensionMIMEType = CFileExtensionMIMEType::NewLC(  extn, MIMEType,
																								implInfo->DisplayName(),
																								parser->ImageTypeUid(),
																								parser->ImageSubTypeUid(),
																								implInfo->ImplementationUid());
				//InsertInOrder will return KErrAlreadyExists if an entry already exists that
				//has the same MIME type and file extension.  Thus, aFileTypeArray may not
				//contain all implementation UIDs that are present. 
				//We assume the resolver CImageConversionResolver::ListAllL() return highest version number first
				//if calls succeed ownership is transferred
				TInt error = aFileTypeArray.InsertInOrder(fileExtensionMIMEType, sortFunction);
				if (error != KErrNone)
					{
					CleanupStack::PopAndDestroy(fileExtensionMIMEType);
					if (error != KErrAlreadyExists)
						{
						User::Leave(error);
						}
					}
				else
					{
					CleanupStack::Pop(fileExtensionMIMEType);				
					}
				}
			}
		CleanupStack::PopAndDestroy(parser);
		}
	CleanupStack::PopAndDestroy(&pluginArray); 
	}

// Used by the decoder and encoder functions to obtain the implementation information a specific codec
void ImageEnDecoderUtils::DoGetImplementationInformationL(const TUid aInterfaceUid, CImplementationInformationType& aImplementationInformation, const TUid aImplementationUid)
	{
	RImplInfoPtrArray pluginArray; // Array to return matching plugins in
	CleanupResetAndDestroyPushL(pluginArray);

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetImplementationType(aImplementationUid);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(aInterfaceUid, resolverParams, KImageConvertResolverUid, pluginArray);
	if (pluginArray.Count() == 0)
		User::Leave(KErrNotFound);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	
	// Transfer data (if more than one is found take first)
	CImplementationInformation* impInfo = pluginArray[0];
	aImplementationInformation.SetDataL(impInfo->ImplementationUid(),
										impInfo->Version(),
										impInfo->DisplayName(),
										impInfo->DataType(),
										impInfo->OpaqueData(),
										impInfo->Drive()
										);

	CleanupStack::PopAndDestroy(); // pluginArray
	}

// Used by the decoder and encoder functions to obtain the implementation properties of a specific codec
// Since this function can be called by both CImageEncoder and CImageDecoder functions we must
// use the generic TUint data type for the aOptions parameter rather than TOptions.
void ImageEnDecoderUtils::DoGetPluginPropertiesL(const TUid aInterfaceUid, const TUid aImplementationUid, RUidDataArray& aPropertiesArray, const TUint aOptions)
	{
	RImplInfoPtrArray pluginArray; // Array to return matching plugins in
	CleanupResetAndDestroyPushL(pluginArray);

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetImplementationType(aImplementationUid);
	customMatchData->SetOptions(CImageDecoder::TOptions(aOptions));
	customMatchData->SetExtensionOptions(aOptions);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(aInterfaceUid, resolverParams, KImageConvertResolverUid, pluginArray);
	if (pluginArray.Count() == 0)
		{
		User::Leave(KErrNotFound);
		}
	
	CleanupStack::PopAndDestroy(2); // package, customMatchData
	
	// Transfer data to our array
	COpaqueDataParse* parser = NULL;
	TRAPD(error, parser = COpaqueDataParse::NewL((pluginArray[0])->OpaqueData()));
	if (error != KErrNone)
		{
		User::Leave(error);
		}

	CleanupStack::PushL(parser);
	parser->EnsureBinaryPropertiesReadL();
	parser->GetBinaryPropertiesArrayL(aPropertiesArray);
	
	CleanupStack::PopAndDestroy(2); // parser, &pluginArray
	}

// Used by the decoder and encoder functions to obtain the implementations (array of uids) that have the required properties and UIDs
void ImageEnDecoderUtils::DoGetInterfaceImplementationsL(const TUid aInterfaceUid, const RUidDataArray& aRequiredUids, RUidDataArray& aImplArray)
	{
	RImplInfoPtrArray pluginArray; // Array to return matching plugins in
	CleanupResetAndDestroyPushL(pluginArray);

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	TEComResolverParams resolverParams; // Parameters on which to match

	customMatchData->SetMatchType(EMatchReqUids);
	customMatchData->SetMatchReqUidsL(aRequiredUids);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(aInterfaceUid, resolverParams, KImageConvertResolverUid, pluginArray);
	if (pluginArray.Count() == 0)
		{
		User::Leave(KErrNotFound);
		}
	
	CleanupStack::PopAndDestroy(2); // package, customMatchData
	
	// Transfer data to our array
	aImplArray.Reset();
	for (TInt index = 0; index < pluginArray.Count(); index++)
		{
		User::LeaveIfError(aImplArray.Append((pluginArray[index])->ImplementationUid()));
		}
	
	CleanupStack::PopAndDestroy(&pluginArray);
	}

/**
Static factory function for creating instances of CFrameImageData.
The managed list of image data is created/destroyed internally.

@return A pointer to a fully constructed CFrameImageData.
*/
EXPORT_C CFrameImageData* CFrameImageData::NewL()
	{
	CImageDataArray* newImageData = new(ELeave) CImageDataArray;
	CleanupStack::PushL(newImageData);

	CFrameImageData* newFrameImageData = new(ELeave) CFrameImageData(*newImageData, ETrue);
	CleanupStack::Pop(); // newImageData

	return newFrameImageData;
	}

/**
Static factory function for creating instances of CFrameImageData.

The managed list of image data is created internally. If a data owner is specified then the responsibility
for destroying the managed list is that of the owner. If no owner is specified the managed list is
destroyed internally.

@param  aImageData
        A reference to an externally created CImageDataArray.
@param  aImageDataOwner
        If set to true, responsibility for deleting CImageDataArray
        is passed to the CFrameImageData object.

@return A pointer to a fully constructed CFrameImageData.
*/
EXPORT_C CFrameImageData* CFrameImageData::NewL(CImageDataArray& aImageData, TBool aImageDataOwner)
	{
	CFrameImageData* newFrameImageData = new(ELeave) CFrameImageData(aImageData, aImageDataOwner);
	return newFrameImageData;
	}

CFrameImageData::CFrameImageData(CImageDataArray& aImageData, TBool aImageDataOwner)
	: iImageData(aImageData),
	iImageDataOwner(aImageDataOwner)
	{
	}

/**
Destructor for this class.

If ownership of the image data was transfered, it is now destroyed.
*/
EXPORT_C CFrameImageData::~CFrameImageData()
	{
	if (iImageDataOwner)
		delete &iImageData;

	iFrameData.ResetAndDestroy();
	}

/**
Intended for future proofing - will panic if called.

@panic  ImageConversion 30
*/
EXPORT_C void CFrameImageData::Reserved_1()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called.

@panic  ImageConversion 30
*/
EXPORT_C void CFrameImageData::Reserved_2()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called.

@panic  ImageConversion 30
*/
EXPORT_C void CFrameImageData::Reserved_3()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called.

@panic  ImageConversion 30
*/
EXPORT_C void CFrameImageData::Reserved_4()
	{
	Panic(EReservedCall);
	}

/**
@see     CFrameImageData::ImageDataCount().

Inserts a pointer to an image data block into the internally held list in front
of the item at position aPos.

@param  aEntry
        The image data block pointer to be inserted.
@param  aPos
        The position at which to insert the pointer into the internal list.
        Must not be less than 0 or greater than the value returned by
        CFrameImageData::ImageDataCount().

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
EXPORT_C TInt CFrameImageData::InsertImageData(const TImageDataBlock* aEntry, TInt aPos)
	{
	ASSERT(aEntry != NULL);
	return iImageData.InsertImageData(aEntry, aPos);
	}

/**
Adds a pointer to an image data block to the end of the internally held list.

@param  aEntry
        The image data block pointer to be inserted.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
EXPORT_C TInt CFrameImageData::AppendImageData(const TImageDataBlock* aEntry)
	{
	ASSERT(aEntry != NULL);
	return iImageData.AppendImageData(aEntry);
	}

/**
Removes a pointer to an image data block at position aIndex from the
internally held list.

@param  aIndex
        The index into the list. Must not be less than 0 or greater
        than the value returned by CFrameImageData::ImageDataCount().

@see    CFrameImageData::ImageDataCount().
*/
EXPORT_C void CFrameImageData::RemoveImageData(TInt aIndex)
	{
	iImageData.RemoveImageData(aIndex);
	}

/**
Inserts a pointer to a frame data block into the internally held list in front
of the item at position aPos.

@param  aEntry
        The frame data block pointer to be inserted.
@param  aPos
        The position at which to insert the pointer into the internal list. Must not be
		less than 0 or greater than the value returned by CFrameImageData::FrameDataCount().

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.

@see    CFrameImageData::FrameDataCount()
*/
EXPORT_C TInt CFrameImageData::InsertFrameData(const TFrameDataBlock* aEntry, TInt aPos)
	{
	ASSERT(aEntry != NULL);
	return iFrameData.Insert(aEntry, aPos);
	}

/**
Adds a pointer to a frame data block to the end of the internally held list.

@param  aEntry
        The frame data block pointer to be inserted.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
EXPORT_C TInt CFrameImageData::AppendFrameData(const TFrameDataBlock* aEntry)
	{
	ASSERT(aEntry != NULL);
	return iFrameData.Append(aEntry);
	}

/**
Removes a pointer to a frame data block at position aIndex from the
internally held list.

@param  aIndex
        Position of the pointer to delete. Must not be less than 0 or greater
        than the value returned by CFrameImageData::ImageDataCount().

@see    CFrameImageData::FrameDataCount().
*/
EXPORT_C void CFrameImageData::RemoveFrameData(TInt aIndex)
	{
	iFrameData.Remove(aIndex);
	}

/**
Creates a full copy of the objects data.
Ownership is transferred to the caller.

@return  A pointer to the new copy of the frame data.
*/
EXPORT_C CFrameImageData* CFrameImageData::AllocL() const
	{
	// Make copies of the TImageDataBlock arrays.
	CImageDataArray* newImageData = new (ELeave) CImageDataArray;
	CleanupStack::PushL(newImageData);

	CFrameImageData* newFrameData = new (ELeave) CFrameImageData(*newImageData, ETrue);
	CleanupStack::Pop(newImageData);
	CleanupStack::PushL(newFrameData);

	// Copy the contents.
	TInt index;
	TInt count = ImageDataCount();
	for (index = 0 ; index<count ; index++)
		{
		const TImageDataBlock* imageData = GetImageData(index);
		TImageDataBlock* tmpImageData = imageData->DuplicateL(*newFrameData);
		CleanupDeletePushL(tmpImageData);

		User::LeaveIfError(newFrameData->AppendImageData(tmpImageData));
		CleanupStack::Pop(); // tmpImageData
		}

	count = FrameDataCount();
	for (index = 0 ; index<count ; index++)
		{
		const TFrameDataBlock* frameData = GetFrameData(index);
		TFrameDataBlock* tmpFrameData = frameData->DuplicateL(*newFrameData);
		CleanupDeletePushL(tmpFrameData);

		User::LeaveIfError(newFrameData->AppendFrameData(tmpFrameData));
		CleanupStack::Pop(); // tmpFrameData
		}

	CleanupStack::Pop(newFrameData); // newFrameData
	return newFrameData;
	}

/**
Returns the image data block at the given position from the
internally held list.

@param  aIndex
        The position of the image data block to retrieve. Must be in the range 0 to ImageDataCount().

@return A pointer to the image data block.
*/
EXPORT_C const TImageDataBlock* CFrameImageData::GetImageData(TInt aIndex) const
	{
	return iImageData.GetImageData(aIndex);
	}

/**
Returns the image data block at the given position from the
internally held list.

@param  aIndex
        The position of the image data block to retrieve. Must be in the range 0 to ImageDataCount().

@return A pointer to the image data block.
*/
EXPORT_C TImageDataBlock* CFrameImageData::GetImageData(TInt aIndex)
	{
	return iImageData.GetImageData(aIndex);
	}

/**
Const version.

Returns the frame data block at the given position from the
internally held list.

@param  aIndex
        The position of the frame data block to retrieve. Must be in the range 0 to FrameDataCount().

@return A pointer to the image data block.
*/
EXPORT_C const TFrameDataBlock* CFrameImageData::GetFrameData(TInt aIndex) const
	{
	return iFrameData[aIndex];
	}

/**
Non const version.

Returns the frame data block at the given position from the
internally held list.

@param  aIndex
        The position of the frame data block to retrieve. Must be in the range 0 to FrameDataCount().

@return A pointer to the image data block.
*/
EXPORT_C TFrameDataBlock* CFrameImageData::GetFrameData(TInt aIndex)
	{
	return iFrameData[aIndex];
	}

/**
Returns the number of image data blocks in the internally held list.

@return The number of image data blocks.
*/
EXPORT_C TInt CFrameImageData::ImageDataCount() const
	{
	return iImageData.ImageDataCount();
	}

/**
Returns the number of frame data blocks in the internally held list.

@return The number of image data blocks.
*/
EXPORT_C TInt CFrameImageData::FrameDataCount() const
	{
	return iFrameData.Count();
	}

/**
Stores the supplied image data in a newly allocated entry in the image buffer array.

@param  aImageBuffer
        The HBufC8 pointer to take ownership of.

@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        another of the system-wide error codes.
*/
EXPORT_C TInt CFrameImageData::AppendImageBuffer(const HBufC8* aImageBuffer)
	{
	return iImageData.AppendImageBuffer(aImageBuffer);
	}

// Codec virtual fns.

// CImageReadCodec

/**
Default constructor.
*/
EXPORT_C CImageReadCodec::CImageReadCodec()
	{
	}

/**
Second phase constructor.
*/
EXPORT_C void CImageReadCodec::ConstructL()
	{
	// NO-OP - reserved in case we need to do Cheshire Cat at a later state. Should always be called.
	}

/**
Destructor.
*/
EXPORT_C CImageReadCodec::~CImageReadCodec()
	{
	}

/**
Used to initialise the frame header data structures. 

The default version of this function does nothing. It should be implemented by
the codec to at least update the appropriate processing state of the current
frame using its TFrameInfo structure.

@param  aFrameInfo
        A reference to a TFrameInfo object which will contain the current frame's header info
@param  aFrameData
        A reference to a TFrameInfo object which will contain the current frame's header data
*/
EXPORT_C void CImageReadCodec::InitFrameHeader(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameData*/)
	{
	}

/**
Processes the header for one frame.

The default version of this function and simply returns EFrameComplete.  It
should be implemented by the codec to at least update the appropriate
processing state of the current frame using its TFrameInfo structure.

@param  aData
        A reference to a TBufPtr8 that contains the frame data.
@return The completion status of this frame's processing
*/
EXPORT_C TFrameState CImageReadCodec::ProcessFrameHeaderL(TBufPtr8& /*aData*/)
	{
	return EFrameComplete;
	}

/**
Processes/displays converted image data.

This function is called on frame completion and on underflow. The default version of this function
does nothing. It should be implemented by the codec if required. 

If it is called on underflow for example, it can enable display of a partially decoded image. In such
cases this function could display all the image data up to the point of the underflow.
*/
EXPORT_C void CImageReadCodec::Complete()
	{
	}

/**
Returns a new position and number of bytes to read for the data stream.

The default version of this function does nothing.
It should be implemented by the codec, if required.

@param  aPosition
        A reference to the returned new position.
@param  aLength
        A reference to the number of bytes to read.
*/
EXPORT_C void CImageReadCodec::GetNewDataPosition(TInt& /*aPosition*/, TInt& /*aLength*/)
	{
	}

/**
@internalComponent

Sets the current frame number.
Called by the framework before InitFrameHeader() and InitFrameL()

@param  aFrameNumber
        The current frame number
*/
void CImageReadCodec::SetCurrentFrame(TInt aFrameNumber)
	{
	iCurrentFrame = aFrameNumber;
	}

/**
Return the current frame number

@return	The current frame number
*/
EXPORT_C TInt CImageReadCodec::CurrentFrame() const
	{
	return iCurrentFrame;
	}

/**
Calculates reduction factor based on the input parameters.
The default Implementation is given here. It should be implemented by the Codecs, If required.

@param  aOriginalSize
        A reference to the original size of an image.
@param  aReducedSize
        A reference to the new size of an image.
@return The reduction factor.
*/

EXPORT_C TInt CImageReadCodec::ReductionFactor(const TSize& aOriginalSize,const TSize& aReducedSize) const
   {
   if( (aReducedSize.iWidth<=0) || (aReducedSize.iHeight<=0))
      {
      return 0;
      }
   TInt reductionFactor = 0;
   TInt roundup=0;
   while( ((aOriginalSize.iWidth+roundup)>>reductionFactor) > aReducedSize.iWidth || 
         ((aOriginalSize.iHeight+roundup)>>reductionFactor) > aReducedSize.iHeight)
      {
      reductionFactor++;
      roundup=(1<<reductionFactor)-1;
      }
   return reductionFactor;
   }

/**
Calculates reduced size of the decoded bitmap based on the input parameters and updates aReducedSize with this value.
The default Implementation is given here. It should be implemented by the Codecs, If required.

@param  aOriginalSize
        A reference to the original size of an image.
@param  aReducedFactor
        The reduction factor to be applied
@param  aReducedSize
        A reference to the new size of an image.
@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        KErrArgument
*/

EXPORT_C TInt CImageReadCodec::ReducedSize(const TSize& aOriginalSize,TInt aReductionFactor, TSize& aReducedSize) const
   {
   aReducedSize = aOriginalSize;
   if (aReductionFactor<0)
   	   {
   	   return KErrArgument;
   	   }
   TInt roundup = aReductionFactor>0? (1<<aReductionFactor)-1: 0;
   aReducedSize.SetSize(((aOriginalSize.iWidth+roundup)>>aReductionFactor),
               ((aOriginalSize.iHeight+roundup)>>aReductionFactor) );
   return KErrNone;            
   }

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageReadCodec::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageReadCodec::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageReadCodec::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageReadCodec::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

// CImageProcessorReadCodec

/**
Default constructor.
*/
EXPORT_C CImageProcessorReadCodec::CImageProcessorReadCodec()
	{
	}

/**
Second phase constructor.
*/
EXPORT_C void CImageProcessorReadCodec::ConstructL()
	{
	CImageReadCodec::ConstructL();

	iBody = CImageProcessorReadCodecBody::NewL();
	}

/**
Destructor.
*/
EXPORT_C CImageProcessorReadCodec::~CImageProcessorReadCodec()
	{
	delete iBody;
	}

/**
Returns a pointer to the codec's CImageProcessor.

@return	A pointer to the codec's CImageProcessor derived object.
*/
EXPORT_C CImageProcessor* CImageProcessorReadCodec::ImageProcessor() const
	{
	return iBody->ImageProcessor();
	}

/**
Sets the codec's CImageProcessor.

Ownership is transferred.

@param  aImageProc
        A pointer to a full constructed CImageProcessor derived object.
*/
EXPORT_C void CImageProcessorReadCodec::SetImageProcessor(CImageProcessor *aImageProc)
	{
	iBody->SetImageProcessor(aImageProc);
	}

/**
Returns the current position within the bitmap (const version).

@return	A reference to a TPoint object specifying the location.
*/
EXPORT_C const TPoint& CImageProcessorReadCodec::Pos() const
	{
	return iBody->Pos();
	}

/**
Returns the current position within the bitmap (non const version).

@return	A reference to a TPoint object specifying the location.
*/
EXPORT_C TPoint& CImageProcessorReadCodec::Pos()
	{
	return iBody->Pos();
	}

/**
Sets the current position within the bitmap.

@param  aPos
        A reference to a TPoint object specifying the location.
*/
EXPORT_C void CImageProcessorReadCodec::SetPos(const TPoint& aPos)
	{
	iBody->SetPos(aPos);
	}

// CImageMaskProcessorReadCodec

/**
Default constructor.
*/
EXPORT_C CImageMaskProcessorReadCodec::CImageMaskProcessorReadCodec()
	{
	}

/**
Second phase constructor.
*/
EXPORT_C void CImageMaskProcessorReadCodec::ConstructL()
	{
	CImageProcessorReadCodec::ConstructL();

	iBody = CImageMaskProcessorReadCodecBody::NewL();
	}

/**
Destructor.
*/
EXPORT_C CImageMaskProcessorReadCodec::~CImageMaskProcessorReadCodec()
	{
	delete iBody;
	}

/**
Returns a pointer to the codec's CImageProcessor used when decoding
the image mask.

@return	A pointer to the codec's CImageProcessor derived object.
*/
EXPORT_C CImageProcessor* CImageMaskProcessorReadCodec::MaskProcessor() const
	{
	return iBody->MaskProcessor();
	}

/**
Replaces a codec's existing CImageProcessor with a new fully constructed instance of the same
object. 

Ownership of the new instance is transferred.

@param  aMaskProc
        A pointer to a full constructed CImageProcessor derived object.
*/
EXPORT_C void CImageMaskProcessorReadCodec::SetMaskProcessor(CImageProcessor *aMaskProc)
	{
	iBody->SetMaskProcessor(aMaskProc);
	}

/**
Constructor for this class.
*/
EXPORT_C CImageProcessorReadCodecExtension::CImageProcessorReadCodecExtension()
	{
	}

EXPORT_C void CImageProcessorReadCodecExtension::ConstructL()
	{
	CImageProcessorReadCodec::ConstructL();
	}

/**
Destructor for this class.
*/
EXPORT_C CImageProcessorReadCodecExtension::~CImageProcessorReadCodecExtension()
	{
	}

EXPORT_C TInt CImageProcessorReadCodecExtension::ScalingCoefficient(const TSize& aOriginalSize, const TSize& aDesiredSize) const
	{
	TInt reductionFactor = ReductionFactor(aOriginalSize, aDesiredSize);
	return -(reductionFactor+1);
	}

EXPORT_C TInt CImageProcessorReadCodecExtension::GetReducedSize(const TSize& aOriginalSize, TInt aScalingCoeff, TSize& aReducedSize) const
	{
	return ReducedSize(aOriginalSize, -(aScalingCoeff+1), aReducedSize);
	}

EXPORT_C CImageMaskProcessorReadCodecExtension::CImageMaskProcessorReadCodecExtension()
	{
	}

EXPORT_C void CImageMaskProcessorReadCodecExtension::ConstructL()
	{
	CImageMaskProcessorReadCodec::ConstructL();
	}

EXPORT_C CImageMaskProcessorReadCodecExtension::~CImageMaskProcessorReadCodecExtension()
	{
	}

EXPORT_C TInt CImageMaskProcessorReadCodecExtension::ScalingCoefficient(const TSize& aOriginalSize, const TSize& aDesiredSize) const
	{
	TInt reductionFactor = ReductionFactor(aOriginalSize, aDesiredSize);
	return -(reductionFactor+1);
	}

EXPORT_C TInt CImageMaskProcessorReadCodecExtension::GetReducedSize(const TSize& aOriginalSize, TInt aScalingCoeff, TSize& aReducedSize) const
	{
	return ReducedSize(aOriginalSize, -(aScalingCoeff+1), aReducedSize);
	}

// CImageWriteCodec

/**
Default constructor.
*/
EXPORT_C CImageWriteCodec::CImageWriteCodec()
	{
	}

/**
Second phase constructor.
*/
EXPORT_C void CImageWriteCodec::ConstructL()
	{
	// NO-OP - reserved in case we need to do Cheshire Cat at a later state. Should always be called.
	}

/**
Destructor.
*/
EXPORT_C CImageWriteCodec::~CImageWriteCodec()
	{
	}

/**
Performs initial processing of image data from an internally held buffer.

Used to initialise the frame header. The default version of this function does nothing.
It should be implemented by the codec, if required.

@param  aDst
        The destination buffer.
@param  aSource
        The source internally held buffer.
*/
EXPORT_C void CImageWriteCodec::InitFrameL(TBufPtr8& /*aDst*/, const CFbsBitmap& /*aSource*/)
	{
	}

/**
Returns the codec's source bitmap.

@return	A pointer to the codec's source bitmap.
*/
EXPORT_C const CFbsBitmap* CImageWriteCodec::Source() const
	{
	return iSource;
	}

/**
Sets the codec's source bitmap.

Use this function if you need to process more than one internally held buffer. This 
will be necessary if, for example, you need to add a bitmap mask to the destination buffer. 

@param  aSource
        A pointer to the codec's source bitmap.
*/
EXPORT_C void CImageWriteCodec::SetSource(const CFbsBitmap* aSource)
	{
	iSource = aSource;
	}


/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageWriteCodec::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageWriteCodec::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageWriteCodec::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageWriteCodec::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

// CFrameInfoStrings fns.

CFrameInfoStrings::CFrameInfoStrings()
	{
	}

/**
Static factory function for creating instances of CFrameInfoStrings.
Leaves the newly allocated object on the cleanup stack.

@return A pointer to a fully constructed CFrameInfoStrings.
*/
EXPORT_C CFrameInfoStrings* CFrameInfoStrings::NewLC()
	{
	CFrameInfoStrings* result = new (ELeave) CFrameInfoStrings;
	CleanupStack::PushL(result);
	result->ConstructL();
	return result;
	}

/**
Static factory function for creating instances of CFrameInfoStrings.

@return A pointer to a fully constructed CFrameInfoStrings.
*/
EXPORT_C CFrameInfoStrings* CFrameInfoStrings::NewL()
	{
	CFrameInfoStrings* result = NewLC();
	CleanupStack::Pop(result);
	return result;
	}

/**
Destructor.

Frees all resources owned by the object prior to its destruction.
*/
EXPORT_C CFrameInfoStrings::~CFrameInfoStrings()
	{
	delete iFrameInfoStrings;
	}

void CFrameInfoStrings::ConstructL()
	{
	iFrameInfoStrings = new (ELeave) CDesC16ArrayFlat(6);
	}

void CFrameInfoStrings::SetStringL(TInt aIndex, const TDesC& aString)
	{
	// ensure we have enough to just append to - as standard case
	const TInt minRequired = aIndex;
	while (iFrameInfoStrings->Count() < minRequired)
		iFrameInfoStrings->AppendL(KNullDesC);

	if (iFrameInfoStrings->Count()==minRequired)
		iFrameInfoStrings->AppendL(aString);
	else
		{
		// delete the original and insert
		iFrameInfoStrings->Delete(aIndex);
		iFrameInfoStrings->InsertL(aIndex, aString);
		}

	ASSERT(String(aIndex)==aString);
	}

/**
Returns the string at position aIndex in the string table.
The index runs from 0 to CFrameInfoStrings::Count() - 1.

@param  aIndex
        The position of the string to retrieve.

@return	A pointer to the string at the given index.
*/
EXPORT_C const TPtrC CFrameInfoStrings::String(TInt aIndex) const
	{
	ASSERT(aIndex < iFrameInfoStrings->Count());
	return (*iFrameInfoStrings)[aIndex];
	}

/**
Returns the number of strings in the string table.

@return	Returns the number of entries in the string table.
*/
EXPORT_C TInt CFrameInfoStrings::Count() const
	{
	return iFrameInfoStrings->Count();
	}

/**
Returns a pointer to the 'decoder' entry string from this object.

@return	A read-only pointer to the returned string.
*/
EXPORT_C const TPtrC CFrameInfoStrings::Decoder() const
	{
	return String(EDecoder);
	}

/**
Adds the supplied string to this object as the 'decoder' entry.

@param  aString
        A descriptor containing the string to add.
*/
EXPORT_C void CFrameInfoStrings::SetDecoderL(const TDesC& aString)
	{
	SetStringL(EDecoder, aString);
	}

/**
Retruns a pointer to the 'format' entry string from this object.

@return	A pointer to the returned read-only string.
*/
EXPORT_C const TPtrC CFrameInfoStrings::Format() const
	{
	return String(EFormat);
	}

/**
Adds the supplied string to this object as the 'format' entry.

@param  aString
        A descriptor containing the string to add.
*/
EXPORT_C void CFrameInfoStrings::SetFormatL(const TDesC& aString)
	{
	SetStringL(EFormat, aString);
	}

/**
Returns a pointer to the 'dimensions' entry string from this object.

@return	A pointer to the returned read-only string.
*/
EXPORT_C const TPtrC CFrameInfoStrings::Dimensions() const
	{
	return String(EDimensions);
	}

/**
Adds the supplied string to this object as the 'dimensions' entry.

@param  aString
        A descriptor containing the string to add.
*/
EXPORT_C void CFrameInfoStrings::SetDimensionsL(const TDesC& aString)
	{
	SetStringL(EDimensions, aString);
	}

/**
Returns a pointer to the 'depth' entry string from this object.

@return	A pointer to the returned read-only string.
*/
EXPORT_C const TPtrC CFrameInfoStrings::Depth() const
	{
	return String(EDepth);
	}

/**
Adds the supplied string to this object as the 'depth' entry.

@param  aString
        A descriptor containing the string to add.
*/
EXPORT_C void CFrameInfoStrings::SetDepthL(const TDesC& aString)
	{
	SetStringL(EDepth, aString);
	}

/**
Returns a pointer to the 'details' entry string from this object.

@return	A pointer to the returned read-only string.
*/
EXPORT_C const TPtrC CFrameInfoStrings::Details() const
	{ 
	return String(EDetails);
	}

/**
Adds the supplied string to this object as the 'details' entry.

@param  aString
        A descriptor containing the string to add.
*/
EXPORT_C void CFrameInfoStrings::SetDetailsL(const TDesC& aString)
	{
	SetStringL(EDetails, aString);
	}

/**
Get a list of the basic image types that can be decoded, based on the
currently available decoder plugins.

@param  aImageTypeArray
        An empty array, into which this function will put a list of
        entries. Each entry will consist of the "display string" from
        the registry entry for a plugin that has been found and that is
        a decoder for a basic image type, accompanied by the Uids for
        that image type. Since we asked for basic types the second Uid,
        for the image sub-type, will always be zero.

        Ownership of the array is passed to the caller so, before the
        array goes out of scope in the client, the caller must call
        ResetAndDestroy() on it to free the entries.
*/
EXPORT_C void CImageDecoder::GetImageTypesL(RImageTypeDescriptionArray& aImageTypeArray)
	{
	ImageEnDecoderUtils::DoGetImageTypesL(KImageDecoderInterfaceUid, aImageTypeArray);
	}

/**
For a given basic image type, get a list of the sub image types that can
be decoded, based on the currently available decoder plugins.

@param  aImageType
        The basic image type for which you want a list of sub-types.
@param  aSubTypeArray
        An empty array, into which this function will put a list of
        entries. Each entry will consist of the "display string" from
        the registry entry for a plugin that has been found and that is
        a decoder for a sub-type of the given basic image type,
        accompanied by the Uids for the sub type. The first Uid, for
        the basic type, will always correspond to aImageType.

        Ownership of the array is passed to the caller so, before the
        array goes out of scope in the client, the caller must call
        ResetAndDestroy() on it to free the entries.
*/
EXPORT_C void CImageDecoder::GetImageSubTypesL(const TUid aImageType, RImageTypeDescriptionArray& aSubTypeArray)
	{
	ImageEnDecoderUtils::DoGetImageTypesL(KImageDecoderInterfaceUid, aSubTypeArray, aImageType);
	}

/**
Get a list of the file extensions that can be decoded and their corresponding
MIME types, based on the currently available decoder plugins.

@param  aFileTypeArray
        An empty array, into which this function will put a list of
        entries. Each entry will consist of a file extension string for
        which a decoder plugin has been found, accompanied by the
        primary MIME type and then any secondary MIME types
        (if present).
        
        Ownership of the array is passed to the caller so, before the
        array goes out of scope in the client, the caller must call
        ResetAndDestroy() on it to free the entries.
*/
EXPORT_C void CImageDecoder::GetFileTypesL(RFileExtensionMIMETypeArray& aFileTypeArray)
	{
	ImageEnDecoderUtils::DoGetFileTypesL(KImageDecoderInterfaceUid, aFileTypeArray);
	}

/**
Get the primary MIME type of the decoder that will be used to
decode a file.
Some file types (like OTA or WBPM), which do not have unique 
pattern in their header may not be recognised, in case when 
the source file name doesn't have extension or, extension is 
not common to that file type. Such files are not supported by this API.

@param  aFs
        A reference to a file server session to use.
@param  aFileName
        The name of the file for which a MIME type has to be determined
@param	aMimeType
        An empty descriptor in which the MIME type assosiated with the file
        will be returned. Ownership is passed to the caller.
*/
EXPORT_C void CImageDecoder::GetMimeTypeFileL(RFs& /*aFs*/, const TDesC& aFileName, TDes8& aMimeType)
	{
	CContent* content = CContent::NewLC(aFileName, EContentShareReadWrite);
	CData* data = content->OpenContentL(EPeek);
	
	CleanupStack::PushL(data);
	TBool mimeTypeKnown = data->GetMimeTypeL(aMimeType);

	if (!mimeTypeKnown)
		{
		TBuf8<KImageHeaderSize> imageHeader;
		User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));

		if (imageHeader.Length() < KImageHeaderSize) // There is not enough data in the file
			{ // Get out - clean up and leave
			User::Leave(KErrUnderflow);
			}

		DoGetMimeTypeL(aFileName, imageHeader, aMimeType);
		}
	CleanupStack::PopAndDestroy(2, content); // content, data
	}

/**
Get the primary MIME type of the decoder that will be used to
decode a descriptor.
Some file types (like OTA or WBPM), which do not have unique 
pattern in their header may not be recognised.
Such files are not supported by this API

@param  aImageData
        A descriptor containing the image data for which a MIME
        type has to be determined.
@param  aMimeType
        An empty descriptor in which the MIME type assosiated with the file
        will be returned. Ownership is passed to the caller.
*/
EXPORT_C void CImageDecoder::GetMimeTypeDataL(const TDesC8& aImageData, TDes8& aMimeType)
	{
	TBuf8<KImageHeaderSize> imageHeader;

	if (aImageData.Length() < KImageHeaderSize) // There is not enough data in this source
		{ // Get out - clean up and leave
		User::Leave(KErrUnderflow);
		}

	imageHeader = aImageData.Left(KImageHeaderSize);

	DoGetMimeTypeL(KNullDesC, imageHeader, aMimeType);
	}

/**
Gets the implementation information for a specific decoder plugin

Ownership of the implementation information is passed to the caller.

@param	aImplementationUid
        The decoder implementation UID for which to retrieve implementation information
@return	A pointer to the implementation information.
*/
EXPORT_C CImplementationInformationType* CImageDecoder::GetImplementationInformationL(TUid aImplementationUid)
	{
	CImplementationInformationType* implementationInformation;
	implementationInformation = CImplementationInformationType::NewLC();
	ImageEnDecoderUtils::DoGetImplementationInformationL(KImageDecoderInterfaceUid, *implementationInformation, aImplementationUid);
	CleanupStack::Pop(implementationInformation);
	return implementationInformation;
	}

/**
Gets a list of the properties of a specific decoder plugin.

@publishedAll
@released
@param	aImplementationUid
        The decoder implementation UID for which the plugin properties need to be retrieved.
@param	aPropertiesArray
		The array of plugin properties owned by the specified decoder.
		The caller has the ownership of the array.
*/
EXPORT_C void CImageDecoder::GetPluginPropertiesL(const TUid aImplementationUid, RUidDataArray& aPropertiesArray)
	{
	ImageEnDecoderUtils::DoGetPluginPropertiesL(KImageDecoderInterfaceUid, aImplementationUid, aPropertiesArray);
	}

/**
@publishedAll
@released

Gets a list of decoder implementations UIDs which have a set of specific capabilities defined by UIDs.

@param	aRequiredUids
        The array containing the required UIDs (properties, image type, image sub-type or class UIDs).
@param	aImplArray
		The array containing the implementation UIDs of the available decoder plugins with the required UIDs.
		The caller has the ownership of the array.
*/
EXPORT_C void CImageDecoder::GetInterfaceImplementationsL(const RUidDataArray& aRequiredUids, RUidDataArray& aImplArray)
	{
	ImageEnDecoderUtils::DoGetInterfaceImplementationsL(KImageDecoderInterfaceUid, aRequiredUids, aImplArray);
	}

/**
@publishedAll
@released

Gets a list of decoder implementations UIDs which have a set of specific capabilities defined by UIDs.

@param	aRequiredUids
        The array containing the required UIDs (properties, image type, image sub-type or class UIDs).
@param	aLength
		The length of aRequiredUids (number of required UIDs).
@param	aImplArray
		The array containing the implementation UIDs of the available decoder plugins with the required UIDs.
		The caller has the ownership of the array.
*/	
EXPORT_C void CImageDecoder::GetInterfaceImplementationsL(const TUid* aRequiredUids, const TInt aLength, RUidDataArray& aImplArray)
	{
	RUidDataArray requiredUids;
	CleanupClosePushL(requiredUids);
	for(TInt index = 0 ; index < aLength ; index++)
		{
		User::LeaveIfError(requiredUids.Append(aRequiredUids[index]));
		}
	ImageEnDecoderUtils::DoGetInterfaceImplementationsL(KImageDecoderInterfaceUid, requiredUids, aImplArray);	
	CleanupStack::PopAndDestroy(1); //requiredUids
	}
	
/**
Create a list of decoders that support the specified MIME type.

@param  aDecoderList
        A list of decoders that support the given MIME type.
@param  aMIMEType
        The MIME type to decode.
@param	aOptions
		Extension options which must be supported by the plugin.
*/
void CImageDecoder::MimeTypeGetDecoderListL(RImplInfoPtrArray& aDecoderList, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	if (aMIMEType.Length() == 0)
		{ // Get out, empty MIME type string
		User::Leave(KErrArgument);
		}

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchMIMEType);
	customMatchData->SetMatchStringL(aMIMEType);
	customMatchData->SetOptions(aOptions);
	customMatchData->SetExtensionOptions(aOptions);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(KImageDecoderInterfaceUid, resolverParams, KImageConvertResolverUid, aDecoderList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}


/**
Create a list of decoders that support the specified image type.

@param  aDecoderList
        A list of decoders that support the specified image type.
@param  aImageHeader
        The header of the image file.
@param  aImageType
        The image base type.
@param  aImageSubType
        The image sub type.
@param  aDecoderUid
        The implementation UID for a specific codec or a decoder/encoder class UID.
@param	aOptions
		Extension options which must be supported by the plugin.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface
*/
void CImageDecoder::ImageTypeGetDecoderListL(RImplInfoPtrArray& aDecoderList, const TDesC8& aImageHeader, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid, const TOptions aOptions)
	{
	if (aDecoderUid != KNullUid)
		{
		RImplInfoPtrArray list;
		CleanupResetAndDestroyPushL(list);
		
		REComSession::ListImplementationsL(KImageDecoderInterfaceUid, list);
		
		for (TInt i = 0; i < list.Count(); i++)
			{
			if (list[i]->ImplementationUid() == aDecoderUid)
				{
				User::LeaveIfError(aDecoderList.Append(list[i]));
				}
			else
				{
				delete list[i];
				}
			list[i] = NULL;
			}
		
		CleanupStack::PopAndDestroy(&list);
		if (aDecoderList.Count() > 0)
			{
			return;
			}
		}
		
	TBuf8<KImageHeaderSize> imageHeader;

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();

	if ((aImageType != KNullUid) || (aDecoderUid != KNullUid))
		{ // We have a specific image type we are trying to convert
		customMatchData->SetMatchType(EMatchUids);
		customMatchData->SetBaseType(aImageType);
		customMatchData->SetSubType(aImageSubType);
		customMatchData->SetImplementationType(aDecoderUid);
		}
	else
		{
		if (aImageHeader.Length() < KMinimumHeaderLength) // There is not enough data in the header
			{ // Get out - clean up and leave
			User::Leave(KErrUnderflow);
			}
		imageHeader = aImageHeader.Left(KImageHeaderSize);
		customMatchData->SetMatchType(EMatchString);
		customMatchData->SetMatchStringL(imageHeader);
		}

	customMatchData->SetOptions(aOptions);
	customMatchData->SetExtensionOptions(aOptions);
	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	#if defined(__ICL_PROFILING)
	// intended for use with TProfImage only
	RDebug::ProfileStart(2);
	TRAPD(err,REComSession::ListImplementationsL(KImageDecoderInterfaceUid, resolverParams, KImageConvertResolverUid, aDecoderList));
	RDebug::ProfileEnd(2);
	User::LeaveIfError(err);
	#else
	REComSession::ListImplementationsL(KImageDecoderInterfaceUid, resolverParams, KImageConvertResolverUid, aDecoderList);
	#endif  // defined(__ICL_PROFILING)

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}

/**
@internalComponent

Creates a list of decoders that support the specified file extension.

@param  aDecoderList
        A list of decoders that support the given file extension.
@param  aFileName
        The file name from which the file extension will be taken.
@param	aOptions
		Extension options which must be supported by the plugin.
*/
void CImageDecoder::SuffixTypeGetDecoderListL(RImplInfoPtrArray& aDecoderList, const TDesC& aFileName, const TOptions aOptions)
	{
	TParse fileName;
	fileName.Set(aFileName,NULL,NULL);

	//No file extension
	if (!fileName.ExtPresent())
		User::Leave(KErrNotFound);

	//Get the suffix
	TPtrC suffix(fileName.Ext());

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchFileSuffix);
	customMatchData->SetFileSuffixL(suffix);
	customMatchData->SetOptions(aOptions);
	customMatchData->SetExtensionOptions(aOptions);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(KImageDecoderInterfaceUid, resolverParams, KImageConvertResolverUid, aDecoderList);

	CleanupStack::PopAndDestroy(2,customMatchData); // package, customMatchData
	}

/**
@internalTechnology

Scans a sorted list of decoders for the first one that can decode the image.

@param  aDecoderList
        A list of decoders that support the image format.
@param  aFs
        A file server session for the decoder to use.
@param  aSourceFilename
        The filename of the file to decode.
@param  aOptions
        The options to use during decoding.

@return A pointer to the decoder.
*/

CImageDecoder* CImageDecoder::FileFindDecoderNewL(const RImplInfoPtrArray& aDecoderList, RFs& aFs, const TDesC& aSourceFilename, const TOptions aOptions, const TDesC& aUniqueId)
	{
	TInt noOfDecoders = aDecoderList.Count();

	if (noOfDecoders == 0)
		{
		User::Leave(KErrNotFound);
		}

	CImageDecoder* decoder = NULL;
	TInt decoderNo = 0;
	TInt error = KErrNone;

	do
		{
		const CImplementationInformation& decoderInfo = *(aDecoderList[decoderNo++]);

		TRAP(error,decoder=FileDecoderNewL(decoderInfo, aFs, aSourceFilename, aOptions, aUniqueId));

		if (error != KErrCorrupt && error != KErrNotSupported && error != KErrNotFound)
			break;
		}
	while(decoderNo < noOfDecoders);

	if(error!=KErrNone)
		{
		ASSERT(decoder==NULL);
		if (error == KErrCorrupt || error == KErrNotSupported)
			error = KErrNotFound;
		User::Leave(error);
		}

	return decoder;
	}
	
/**
@internalTechnology

Create a construct that can create a decoder and call
functions to initialise the decoder with the image data.

@param  aDecoderInfo
        Implementation information for the decoder to be created.
@param  aFs
        A file server session for the decoder to use.
@param  aSourceFilename
        The filename of the file to decode.
@param  aOptions
        Options the decoder must use.

@return	A pointer to the decoder.
*/

CImageDecoder* CImageDecoder::FileDecoderNewL(const CImplementationInformation& aDecoderInfo, RFs& aFs, const TDesC& aSourceFilename, const TOptions aOptions, const TDesC& aUniqueId)
	{
	CImageDecodeConstruct* construct = NULL;
	construct = NewDecodeConstructL(aDecoderInfo, aOptions);
	ASSERT(construct!= NULL);

	CImageDecoder* decoder = NewL(construct, aOptions); // note NewL takes ownership of construct - don't push on stack
	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);

	decoder->iRelay->SetUniqueIdL(aUniqueId);

	decoder->iRelay->SetFileL(aFs, aSourceFilename, aOptions);
	decoder->iRelay->HandleNewlyOpenedImageL();

	CleanupStack::Pop(decoder);
	return decoder;
	}

/**
@internalTechnology
@released
Scan a sorted list of decoders for the first one that can decode the image.

@param  aDecoderList
        A list of decoders that support the image format.
@param  aFs
        A file server session for the decoder to use.
@param  aSourceData
        The data to decode.
@param  aOptions
        Options to use during decoding.

@return A pointer to the decoder.
 */
CImageDecoder* CImageDecoder::DataFindDecoderNewL(const RImplInfoPtrArray& aDecoderList, RFs& aFs, const TDesC8& aSourceData, const TOptions aOptions)
	{
	TInt noOfDecoders = aDecoderList.Count();

	if(noOfDecoders == 0)
		User::Leave(KErrNotFound);

	CImageDecoder* decoder = NULL;
	TInt decoderNo = 0;
	TInt error = KErrNone;
	do
		{
		const CImplementationInformation& decoderInfo = *(aDecoderList[decoderNo++]);
		TRAP(error,decoder=DataDecoderNewL(decoderInfo, aFs, aSourceData, aOptions));
		if (error != KErrCorrupt && error != KErrNotSupported && error != KErrNotFound)
			break;
		}
	while(decoderNo < noOfDecoders);

	if(error!=KErrNone)
		{
		ASSERT(decoder==NULL);
		if (error == KErrCorrupt || error == KErrNotSupported)
			error = KErrNotFound;
		User::Leave(error);
		}

	return decoder;
	}

/**
@internalTechnology
@released
Create a construct that can create a decoder and call
functions to initialise the decoder with the image data.

@param  aDecoderInfo
        Implementation information for the decoder to be created.
@param  aFs
        A file server session for the decoder to use.
@param  aSourceData
        The data to decode.
@param  aOptions
        Options the decoder must use.

@return	A pointer to the decoder.
*/
CImageDecoder* CImageDecoder::DataDecoderNewL(const CImplementationInformation& aDecoderInfo, RFs& aFs, const TDesC8& aSourceData, const TOptions aOptions)
	{
	CImageDecodeConstruct* construct = NULL;
	construct = NewDecodeConstructL(aDecoderInfo, aOptions);
	ASSERT(construct!= NULL);

	CImageDecoder* decoder = NewL(construct, aOptions); // note NewL takes ownership of construct - don't push on stack
	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);

	decoder->iRelay->SetDataL(aFs, aSourceData, aOptions);
	decoder->iRelay->HandleNewlyOpenedImageL();

	CleanupStack::Pop(decoder);
	return decoder;
	}

/**
@internalTechnology

Create a construct object for the specified decoder.

@param  aDecoderInfo
        Implementation information for the decoder to be created.
@param	aOptions
		Extension options which must be supported by the plugin.
@return	A construct object that can create the decoder.
*/
CImageDecodeConstruct* CImageDecoder::NewDecodeConstructL(const CImplementationInformation& aDecoderInfo, const TOptions aOptions)
	{
	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	COpaqueDataParse* parse = COpaqueDataParse::NewLC(aDecoderInfo.OpaqueData());

	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetBaseType(parse->ImageTypeUid());
	customMatchData->SetSubType(parse->ImageSubTypeUid());
	customMatchData->SetImplementationType(aDecoderInfo.ImplementationUid());

	CleanupStack::PopAndDestroy(parse);

	customMatchData->SetOptions(aOptions);
	customMatchData->SetExtensionOptions(aOptions);
	HBufC8* package = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams;
	resolverParams.SetDataType(packageDes);

	CImageDecodeConstruct* construct = NULL;
#if defined(__ICL_PROFILING)
	// intended for use with TProfImage only
	RDebug::ProfileStart(3);
	TRAPD(err, construct = STATIC_CAST(CImageDecodeConstruct*,
			REComSession::CreateImplementationL(KImageDecoderInterfaceUid,
			_FOFF(CImageDecodeConstruct, iDtorIDKey),
			resolverParams,
			KImageConvertResolverUid)));
	RDebug::ProfileEnd(3);
	User::LeaveIfError(err);
#else
	construct = STATIC_CAST(CImageDecodeConstruct*,
			REComSession::CreateImplementationL(KImageDecoderInterfaceUid,
			_FOFF(CImageDecodeConstruct, iDtorIDKey),
			resolverParams,
			KImageConvertResolverUid));
#endif  // defined(__ICL_PROFILING)

	ASSERT(construct!=NULL);

	CleanupStack::PopAndDestroy(2, customMatchData); //package, customMatchData

	return construct;
	}

/**
@internalTechnology

Get the MIME type for a given match string.

@param  aFileName
        The file name of the image file.
@param  aMatchString
        An image header of an image file.
@param  aMimeType
        The primary MIME type returned.
*/
void CImageDecoder::DoGetMimeTypeL(const TDesC& aFileName, const TDesC8& aMatchString, TDes8& aMimeType)
	{
	// Get a list of decoders that will decode the image.
	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);
	CImageDecoder::ImageTypeGetDecoderListL(decoderList, aMatchString, KNullUid, KNullUid, KNullUid);

	// Try to match by file extension.
	// aFileName will be KNullDesC when called from GetMimeTypeDataL()
	if(aFileName!=KNullDesC && decoderList.Count() == 0)
		CImageDecoder::SuffixTypeGetDecoderListL(decoderList, aFileName);

	if(decoderList.Count() == 0)
		User::Leave(KErrNotFound);

    TInt decoderNo = 0;

#if defined (__GET_MIME_TYPE_THOROUGH)
	RFs fs;
	CleanupClosePushL(fs);
    User::LeaveIfError(fs.Connect());
	TInt noOfDecoders = decoderList.Count();

    CImageDecoder* decoder = NULL;
    TInt error = KErrNotFound;
	while(decoderNo < noOfDecoders)
		{
		const CImplementationInformation& decoderInfo = *(decoderList[decoderNo]);
        TRAP(error,decoder=DataDecoderNewL(decoderInfo, fs, aMatchString, EOptionAllowZeroFrameOpen));
  		// if decoder didn't match, then it should return KErrCorrupt,
		// but we also accept KErrNotSupported & KErrNotFound
		if (error != KErrCorrupt && error != KErrNotSupported && error != KErrNotFound)
			break;
		decoderNo++;
		}

	if (error!=KErrNone)
		{
		ASSERT(decoder==NULL);
		if (error == KErrCorrupt || error == KErrNotSupported)
			error = KErrNotFound;
		User::Leave(error);
		}
    delete decoder;
	CleanupStack::PopAndDestroy(&fs); 
#endif  // defined (__GET_MIME_TYPE_THOROUGH)

	//Use the highest rated decoder.
	CImageDecodeConstruct* construct = NewDecodeConstructL(*decoderList[decoderNo]);
	CleanupStack::PopAndDestroy(&decoderList);
	if (construct == NULL)
		{ // We didn't get a match - leave
		User::Leave(KErrNotFound); 
		}
	CleanupStack::PushL(construct);

	// Determine the primary Mime type of the decoder	
	CImplementationInformationType* implementationInformation = NULL;
	implementationInformation = GetImplementationInformationL(construct->ImplementationUid());
	CleanupStack::PushL(implementationInformation);

	TPtrC8 opaqueDataPtr = implementationInformation->OpaqueData();
	COpaqueDataParse* parse = COpaqueDataParse::NewLC(opaqueDataPtr);

	if(parse->OnlyUidsAvail())
		{
		User::Leave(KErrNotFound);
		}

	parse->EnsureMIMETypesReadL();

	aMimeType = parse->MIMEType(0);

	CleanupStack::PopAndDestroy(3); // parse, ImplementationInformation, decoderPtr
	}

/**
Create a decoder for the image in the named file. The client supplies a
MIME type which will be used to try and select an appropriate plugin
decoder. If it finds a decoder it creates it and then goes on to use that
decoder to scan the beginning of the image file.

If any file related errors are encountered opening the specified file, this 
function leaves with an appropriate file related leave code.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceFilename
        The name of the file to be decoded.
@param  aMIMEType
        The MIME type of the image in the file.
@param	aOptions
        Decoder options to use.

@return	Returns a pointer to the newly created decoder.

@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        Either the specific plugin decoder for this file hasn't been found, or the file itself is missing.

@see    TOptions
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFs& aFs, const TDesC& aSourceFilename, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	return CImageDecoder::FileNewImplL(aFs, aSourceFilename, aMIMEType, KDefaultContentObject, EPeek, aOptions);
	}

/**
Create a decoder for the image in the named file. The client supplies a
MIME type which will be used to try and select an appropriate plugin
decoder. If it finds a decoder it creates it and then goes on to use that
decoder to scan the beginning of the image file.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceFilename
        The name of the file to be decoded.
@param  aMIMEType
        The MIME type of the image in the file.
@param  aIntent
        The DRM Intent for image conversion.
@param  aOptions
        The decoder options to use.

@return A pointer to the newly created decoder.

@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        Either the specific plugin decoder for this file hasn't been found, or the file itself is missing.
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFs& aFs, const TDesC& aSourceFilename, const TDesC8& aMIMEType, TIntent aIntent, const TOptions aOptions)
	{
	return CImageDecoder::FileNewImplL(aFs, aSourceFilename, aMIMEType, KDefaultContentObject, aIntent, aOptions);
	}

/**
Create a decoder for the image in the named file. The client supplies a
MIME type which will be used to try and select an appropriate plugin
decoder. If it finds a decoder it creates it and then goes on to use that
decoder to scan the beginning of the image file.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSource
        An interface between filename based and file handle.
@param  aMIMEType
        The MIME type of the image in the file.
@param  aOptions
        The decoder options to use. Specifying one of more extension options (for example EOptionExtCrop) can be used to cause the 
		to cause a plugin to load which supports the image type and supports the requested extensions.

@return A pointer to the newly created decoder.

@leave  KErrNotSupported
        A matching decoder could not be found for the MIME type.
@leave	KErrNotFound
		Either the specific plugin decoder for this source image hasn't been found, or the source image itself is missing, or
		a plugin with the requested extensions cannot be found.
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFs& aFs, const TMMSource& aSource, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	if (aSource.SourceType()==KUidMMFileHandleSource)
		{
		const TMMFileHandleSource& source = static_cast<const TMMFileHandleSource&>(aSource);
		return CImageDecoder::FileNewImplL(source.Handle(), aMIMEType, source.UniqueId(), source.Intent(), aOptions);
		}
	else if (aSource.SourceType()==KUidMMFileSource)
		{
		const TMMFileSource& source = static_cast<const TMMFileSource&>(aSource);
		return CImageDecoder::FileNewImplL(aFs, source.Name(), aMIMEType, source.UniqueId(), source.Intent(), aOptions);
		}
	else
		{
		// unknown source type
		User::Leave(KErrNotSupported);
		return NULL;
		}
	}

/**
Create a decoder for the image in the named source.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSource
        An interface between filename based and file handle.
@param  aOptions
        The decoder options to use. Specifying one of more extension options (for example EOptionExtCrop) can be used to cause the 
		to cause a plugin to load which supports the image type and supports the requested extensions.
@param  aImageType
        The image type of the image in the file.
@param  aImageSubType
        The image sub-type of the image in the file.
@param  aDecoderUid
        The implementation UID for a specific codec or a decoder/encoder class UID.        
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface
@return A pointer to the newly created decoder.

@leave  KErrNotSupported
        A matching decoder could not be found for the MIME type.
@leave  KErrNotFound
        Either the specific plugin decoder for this source image hasn't been found, or the source image itself is missing.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFs& aFs, const TMMSource& aSource, 
                                                const TOptions aOptions, const TUid aImageType, 
                                                const TUid aImageSubType, const TUid aDecoderUid)
	{
	if (aSource.SourceType()==KUidMMFileHandleSource)
		{
		const TMMFileHandleSource& source = static_cast<const TMMFileHandleSource&>(aSource);
		return CImageDecoder::FileNewImplL(source.Handle(), source.UniqueId(), source.Intent(), aOptions, aImageType, aImageSubType, aDecoderUid);
		}
	else if (aSource.SourceType()==KUidMMFileSource)
		{
		const TMMFileSource& source = static_cast<const TMMFileSource&>(aSource);
		return CImageDecoder::FileNewImplL(aFs, source.Name(), source.UniqueId(), source.Intent(), aOptions, aImageType, aImageSubType, aDecoderUid);
		}
	else
		{
		// unknown source type
		User::Leave(KErrNotSupported);
		return NULL;
		}
	}

/**

Sets the properties for the Image decoder.

@param  aProperty
        The property to set.
@param  aValue
        The value of the property.
               
@return KErrNone if successful, otherwise one of the system-wide errors.
*/
EXPORT_C TInt CImageDecoder::SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)
	{
	ASSERT(ValidProperties());
	return iRelay->SetAgentProperty(aProperty, aValue);
	}

/**
@internalTechnology

Creates a decoder for the image in the named file. The client supplies a
MIME type which will be used to try and select an appropriate plugin
decoder. If it finds a decoder it creates it and then goes on to use that
decoder to scan the beginning of the image file.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceFilename
        The name of the file to be decoded.
@param  aMIMEType
        The MIME type of the image in the file.
@param  aUniqueId
        The object to open for reading. If the UniqueId is set to KNullDesC the entire file will be opened for reading with no transformation.
@param  aIntent
        The DRM Intent for image conversion.
@param  aOptions
        The decoder options to use.

@return	A pointer to the newly created decoder.

@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        Either the specific plugin decoder for this file hasn't been found, or the file itself is missing.
*/

CImageDecoder* CImageDecoder::FileNewImplL(RFs& aFs, const TDesC& aSourceFilename, const TDesC8& aMIMEType, const TDesC& aUniqueId, TIntent aIntent, const TOptions aOptions)
	{
	CContent* content = NULL;
	content = GetContentLC(aSourceFilename);
	CData* data = content->OpenContentL(aIntent, aUniqueId); // check file presence, evaluate (not execute) intent
	CleanupStack::PopAndDestroy(content);

	delete data; // close file
	

	//Get a sorted list of decoders that will decode the image
	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);
	CImageDecoder::MimeTypeGetDecoderListL(decoderList, aMIMEType, aOptions);

	CImageDecoder* decoder = NULL;
	decoder = CImageDecoder::FileFindDecoderNewL(decoderList, aFs, aSourceFilename, aOptions, aUniqueId);

	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);
	decoder->iRelay->SetIntent(aIntent);

	CleanupStack::Pop(decoder);
	CleanupStack::PopAndDestroy(&decoderList);
	return decoder;
	}

/**
Create a decoder for the image in the source buffer. The client supplies a
MIME type which will be used to try and select an appropriate plugin
decoder. If a decoder is found it is created and then used to scan
the beginning of the image file.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceData
        The buffer containing the image to be decoded. Note that the framework 
        doesn't take a copy of the actual data, therefore both the descriptor
        object and the data must persist during decoding.
@param  aMIMEType
        The MIME type of the image in the file(used to determine the plugin
		to create).
@param  aOptions
        The decoder options to use. Specifying one of more extension options (for example EOptionExtCrop) can be used to cause the 
		to cause a plugin to load which supports the image type and supports the requested extensions.

@return	Returns a pointer to the newly created decoder.

@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        No appropriate plugin decoder for this image has been found.

@see    TOptions
*/
EXPORT_C CImageDecoder* CImageDecoder::DataNewL(RFs& aFs, const TDesC8& aSourceData, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	//Get a list of decoders that will decode the image
	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);
	CImageDecoder::MimeTypeGetDecoderListL(decoderList, aMIMEType, aOptions);

	CImageDecoder* decoder = NULL;
	decoder = CImageDecoder::DataFindDecoderNewL(decoderList, aFs, aSourceData, aOptions);
	ASSERT(decoder!=NULL);

	CleanupStack::PopAndDestroy(&decoderList);
	return decoder;
	}

/**
Create a decoder for the image in the named file. 

If the client supplies an image type (and sub-type, if applicable) or decoder 
UID, these will be used to try and select an appropriate plugin decoder. If 
not, then the selection will be done by matching the image header in the file. 
If it finds a decoder, it will be created and then used to scan the beginning 
of the image file.

Note: Every image format has two IDs, known as the type and the sub-type (although 
generally the sub-type is KNullUid). To retrieve a list of supported types and 
sub-types that can be decoded, use the static functions GetImageTypesL() and 
GetImageSubTypesL().

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceFilename
        The name of the file to be decoded.
@param	aOptions
        Decoder options to use.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param  aDecoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface
@return	Returns a pointer to the newly created decoder.

@leave  KErrUnderflow
        Not enough data in file to identify which plugin decoder to use.
@leave  KErrNotFound
        Either the appropriate plugin decoder for this file hasn't been found, or the file itself is missing.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.

@see    TOptions
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFs& aFs, const TDesC& aSourceFilename, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	return CImageDecoder::FileNewImplL(aFs, aSourceFilename, KDefaultContentObject, EPeek, aOptions, aImageType, aImageSubType, aDecoderUid);
	}

/**
Creates a decoder for the image in the named file. If the client supplies an
image type (and sub-type, if applicable) or decoder UID, these will be used
to try and select an appropriate plugin decoder. If not, then the selection
will be done by matching the image header in the file. If it finds a decoder
it creates it and then goes on to use that decoder to scan the beginning of
the image file.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceFilename
        The name of the file to be decoded.
@param  aIntent
        The DRM Intent for image conversion.
@param  aOptions
        The decoder options to use. See TOptions.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param  aDecoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return	A pointer to the newly created decoder.

@leave  KErrUnderflow
        Not enough data in file to identify which plugin decoder to use.
@leave  KErrNotFound
        Either the appropriate plugin decoder for this file hasn't been found, or the file itself is missing.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.

@see     TOptions
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFs& aFs, const TDesC& aSourceFilename, TIntent aIntent, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	return CImageDecoder::FileNewImplL(aFs, aSourceFilename, KDefaultContentObject, aIntent, aOptions, aImageType, aImageSubType, aDecoderUid);
	}

/**
@internalTechnology

Creates a decoder for the image in the named file. If the client supplies an
image type (and sub-type, if applicable) or decoder UID, these will be used
to try and select an appropriate plugin decoder. If not, then the selection
will be done by matching the image header in the file. If it finds a decoder
it creates it and then goes on to use that decoder to scan the beginning of
the image file.


@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceFilename
        The name of the file to be decoded.
@param  aUniqueId
        The object to open for reading. If the UniqueId is set to KNullDesC the entire file will be opened for reading with no transformation.
@param  aIntent
        The DRM Intent for image conversion
@param  aOptions
        The decoder options to use. See TOptions.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param  aDecoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return A pointer to the newly created decoder.

@leave  KErrUnderflow
        Not enough data in file to identify which plugin decoder to use.
@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        Either the appropriate plugin decoder for this file hasn't been found, or the file itself is missing.
@panic  ImageConversion 19
        No base type given for sub-type.

@see    TOptions
 */

CImageDecoder* CImageDecoder::FileNewImplL(RFs& aFs, const TDesC& aSourceFilename, const TDesC& aUniqueId, const TIntent aIntent, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);

	CContent* content = NULL;
	content = GetContentLC(aSourceFilename);
	CData* data = content->OpenContentL(aIntent, aUniqueId);
	CleanupStack::PopAndDestroy(content);

	CleanupStack::PushL(data);

	if (aImageType == KNullUid && aDecoderUid == KNullUid)
		{ 
		TBuf8<KMaxMimeLength> mimeType;
		if (data->GetMimeTypeL(mimeType))
			{
			// try to find a controller based on MIME type
			CImageDecoder::MimeTypeGetDecoderListL(decoderList, mimeType, aOptions);
			}
		if (decoderList.Count()==0)
			{
			// read header data
			TBuf8<KImageHeaderSize> imageHeader;
			User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));
			CImageDecoder::ImageTypeGetDecoderListL(decoderList, imageHeader, aImageType, aImageSubType, aDecoderUid, aOptions);
			}
		}
	else
		{
		TBuf8<KImageHeaderSize> imageHeader;
		User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));
		CImageDecoder::ImageTypeGetDecoderListL(decoderList, imageHeader, aImageType, aImageSubType, aDecoderUid, aOptions);
		}
	
	CleanupStack::PopAndDestroy(1, data); //data

	//Try to match by file extension only
	//1) If no plugin was found and 
	//2) No specific decoder or format was specified
	const TBool formatSpecified = (aImageType!=KNullUid || aImageSubType!=KNullUid || aDecoderUid!=KNullUid);
	if(decoderList.Count()==0 && !formatSpecified)
		{
		CImageDecoder::SuffixTypeGetDecoderListL(decoderList, aSourceFilename, aOptions);
		}

	CImageDecoder* decoder = NULL;
	decoder = CImageDecoder::FileFindDecoderNewL(decoderList, aFs, aSourceFilename, aOptions, aUniqueId);


	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);
	decoder->iRelay->SetIntent(aIntent);

	CleanupStack::Pop(decoder);
	CleanupStack::PopAndDestroy(&decoderList);
	return decoder;
	}


/**
Creates a decoder for the image in the source buffer. 

If the client supplies an image type (and sub-type, if applicable) or decoder UID, 
these will be used to try and select an appropriate plugin decoder. If not, then 
the selection will be done by matching the image header from the buffer. If it
finds a decoder, it is created and then used to scan the beginning of the image
buffer.

@param  aFs
        A reference to a file server session for the decoder to use.
@param  aSourceData
        The buffer containing the image to be decoded. Note that the framework 
        doesn't take a copy of the actual data, therefore both the descriptor
        object and the data must persist during decoding.
@param  aOptions
        Decoder options to use.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param	aDecoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return	Returns a pointer to the newly created decoder.

@leave  KErrUnderflow
        Not enough data in descriptor to identify which plugin decoder to use.
@leave  KErrNotFound
        No appropriate plugin decoder for this image has been found.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.

@see    TOptions
*/
EXPORT_C CImageDecoder* CImageDecoder::DataNewL(RFs& aFs, const TDesC8& aSourceData, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	//Get a list of decoders that will decode the image
	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);
	CImageDecoder::ImageTypeGetDecoderListL(decoderList, aSourceData, aImageType, aImageSubType, aDecoderUid, aOptions);

	CImageDecoder* decoder = NULL;
	decoder = CImageDecoder::DataFindDecoderNewL(decoderList, aFs, aSourceData, aOptions);
	ASSERT(decoder!=NULL);

	CleanupStack::PopAndDestroy(&decoderList);
	return decoder;


	}

/**
@internalTechnology

Called internally to create a CImageDecoder and associated iRelay body

This member is internal and not intended for use.
*/
CImageDecoder* CImageDecoder::NewL(CImageDecodeConstruct* aConstruct, TOptions aOptions)
	{
	CleanupStack::PushL(aConstruct); // we take ownership of this until can pass to relay
	CImageDecoder* self = aConstruct->NewDecoderL(); // typically will callback to CImageDecoder::NewL()
	CleanupStack::PushL(self);
	TBool alwaysThread = (aOptions & EOptionAlwaysThread)!=EFalse;
	self->iRelay = MImageDecoderRelay::NewL(aConstruct, alwaysThread);
	CleanupStack::Pop(2); // self and construct
	self->iRelay->TransferConstructOwnership();
	return self;
	}

/**
@internalTechnology

Actual factory function for CImageDecoder - ie. it creates the object
Called back plugin - to allow plugin to override if required

This member is internal and not intended for use.
*/
CImageDecoder* CImageDecoder::NewL()
	{
	CImageDecoder* self = new (ELeave) CImageDecoder;
	return self;
	}

/**
Constructor for this class.
@internalTechnology
*/
EXPORT_C CImageDecoder::CImageDecoder()
	{
	}

/**
Destructor for this class.

If using a local file session, it closes it.
It also informs ECom that has finished with the decoder instance.

Frees all resources owned by the object prior to its destruction.
*/
EXPORT_C CImageDecoder::~CImageDecoder()
	{
	Cancel();

	delete iRelay;
	}

/**
Start decoding an image frame asynchronously.

@pre
The destination bitmap aDestination, must be created before the call to
Convert() is made. aDestination must be large enough to contain the frame and
be set to the required display mode. FrameInfo() can be used to obtain
the size and display mode of the frame.

When the conversion is complete, successfully or otherwise, the status is 
returned in aRequestStatus. 

If the operations completes with KErrUnderflow, then there is insufficient 
information in the descriptor. In this situation, ContinueConvert() should be 
called repeatedly until the descriptor has accumulated enough information 
for ContinueConvert() to complete with KErrNone.It is the responsibility of the 
caller to ensure that the original data source used to create this decoder object 
gets enough information. If there is no data available then a caller can ignore 
this error code and use partially decoded image.

@param  aRequestStatus
        Request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
@param  aDestination
        A bitmap that will contain the decoded frame.
@param	aFrameNumber
        The frame in a multi-frame image to decode (optional, defaults to zero).

@note
As most codec plugins support downscaling the image but not upscaling, the standard behaviour (i.e. no requested
transformations) for codecs begins with the size of the destination bitmap passed to CImageDecoder::Convert being
inspected, and:

- 1. If the destination size matches the frame size of the image then the image is decoded full size into the destination
bitmap.
- 2. If the destination size is larger than the frame size of the image then the image is decoded full size into the
destination bitmap with no upscaling. The image origin is aligned with the top left corner of the bitmap and any area
in the bitmap to the bottom and right of image is left in its initialised state.
- 3. If the destination size is smaller than the frame size of the image then a reduction factor is calculated and the
image is scaled down (1/2, 1/4, 1/8 size) whilst maintaining the aspect ratio of the image. The size is the next
smallest size that will fit in the destination bitmap. The use case for this is when the client wants to pass in the
screen size of device and have the image scaled to fill as much of the screen as possible.
However, if the extension interfaces (clipping, scale, operation) are used then the additional behaviour below applies.
- 4. If the extension interfaces for clipping rectangle and/or operation are applied, but not scaling, then the size of
the destination bitmap for an unscaled image can be obtained via CImageDecoder::GetDestinationSize.
Lets call that SizeA. The same rules apply as given in 1, 2, 3 above resulting in a down-scaled destination if the
destination bitmap is smaller than SizeA.
- 5. If the extension interface for scaling is called via one of the two TImageConvScaler::SetScalingL(.. functions then
it is required that the destination size is obtained through CImageDecoder::GetDestinationSize and that a destination
bitmap of that size is passed to CImageDecoder::Convert. Failure to do this will cause the decoder to fail with KErrArgument.
This rule holds if clipping and/or operation is applied as well as scaling.
*/
EXPORT_C void CImageDecoder::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)
	{
	ASSERT(ValidProperties());
	iRelay->Convert(aRequestStatus, aDestination, aFrameNumber);
	}

/**
Set the source image type, this can be any value from TImageType. It can leave with a system wide
error. Typical leaves are documented below.
   
@param 	aImageType
		An image type from TImageType to denote source image that the decoder should use.
@leave  KErrNotFound
		If the image for the type specified is not found.
@leave  KErrCorrupt
		For a corrupt image. In the case of failing to change the source image from a valid 
		EImageTypeMain image to a corrupt EImageTypeThumbnail, the decoder resets the image 
		type back to the valid EImageTypeMain.
*/
EXPORT_C void CImageDecoder::SetImageTypeL(TInt aImageType)
	{
	ASSERT(ValidProperties());
	iRelay->SetImageTypeL(aImageType);
	}


/**
Start decoding an image frame and mask asynchronously.

@pre
The destination bitmap aDestination, must be created before the call to
Convert() is made. aDestination must be large enough to contain the frame and
be set to the required display mode. FrameInfo() can be used to obtain
the size and display mode of the frame. The destination mask aDestinationMask
must be created before the call to Convert() is made and must be large enough for
the mask. The display mode must be EGray2 or EGray256 and must be EGray256 if the
image contains alpha-blending information. This information can be obtained from
the iFlags property of TFrameInfo obtained from a FrameInfo() call.

When the conversion is complete, successfully or otherwise, the status is 
returned in aRequestStatus.

If the operations completes with KErrUnderflow, then there is insufficient 
information in the descriptor. In this situation, ContinueConvert() should be 
called repeatedly until the descriptor has accumulated enough information 
for ContinueConvert() to complete with KErrNone.It is the responsibility of the 
caller to ensure that the original data source used to create this decoder object 
gets enough information. If there is no data available then a caller can ignore 
this error code and use partially decoded image.

@param  aRequestStatus
        Request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
@param  aDestination
        A bitmap that will contain the decoded frame.
@param  aDestinationMask
        A bitmap that will contain the decoded frame mask.
@param  aFrameNumber
        The frame in multi-frame image to decode (optional, defaults to zero).

@see    TFrameInfo
*/
EXPORT_C void CImageDecoder::Convert(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber)
	{
	ASSERT(ValidProperties());
	iRelay->Convert(aRequestStatus, aDestination, aDestinationMask, aFrameNumber);
	}

/**
Continue decoding a frame and/or mask after new image data was added to
the source file or descriptor and a previous call to Convert() or
ContinueConvert() returned KErrUnderflow.

@param  aRequestStatus
        Request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
*/
EXPORT_C void CImageDecoder::ContinueConvert(TRequestStatus* aRequestStatus)
	{
	ASSERT(ValidProperties());
	iRelay->ContinueConvert(aRequestStatus);
	}

/**
Return the number of frames in the image being decoded. 

This function can be called immediately after the call to create the decoder, 
thus enabling the caller to know how many frames need to be converted. Client 
may have to call IsImageHeaderProcessingComplete() & ContinueProcessingHeaders() 
to ensure all all data is available.

@return The number of frames in the source image.
*/
EXPORT_C TInt CImageDecoder::FrameCount() const
	{
	ASSERT(ValidProperties());
	return iRelay->FrameCount();
	}

/**
Return the status of the image.

If the image is incomplete EFalse will be returned. The client should continue
to supply more data and call ContinueProcessingHeaders() until ETrue is returned.

Panic categories:  Many CImageDecoder functions are dependent upon this function
returning ETrue before they can successfully be called.  If the client calls one
of these dependent functions, then that function call may panic with either the 
'ImageConversion' panic category or a decoder plugin specific panic category,
for example: 'BMPConvertPlugin', 'GIFConvertPlugin', 'ICOConvertPlugin',
'JPEGConvertPlugin', 'MBMConvertPlugin', 'OTAConvertPlugin', 'PNGConvertPlugin',
'TIFFConvertPlugin', 'WBMPConvertPlugin', 'WMFConvertPlugin'.


@return Image status.
*/
EXPORT_C TBool CImageDecoder::IsImageHeaderProcessingComplete() const
	{
	return iRelay->IsImageHeaderProcessingComplete();
	}

/**
Continue processing image headers after new image data was added to
the source file or descriptor.

@see IsImageHeaderProcessingComplete()
*/
EXPORT_C void CImageDecoder::ContinueProcessingHeaderL()
	{
	ASSERT(ValidProperties());
	iRelay->ContinueProcessingHeaderL();
	}

/**
Cancels any conversions currently in progress (Cancel is synchronous).
*/
EXPORT_C void CImageDecoder::Cancel()
	{
	if(ValidProperties())
		iRelay->Cancel();
	}

/**
Return the frame info for a specified frame of the image. 

This function can be called immediately after the call to create 
the decoder, thus enabling the caller to know about each frame in 
advance of converting it.

The returned information contains details of the size of the image,
the dimensions of the frame, its colour depth and so on. More advanced 
information may be available for the image using FrameData().

Use FrameCount() to determine how many frames are contained in the image 
before using this function.

@param  aFrameNumber
        The frame number.

@return The returned information for the specified frame.

@panic  ImageConversion 10
        Frame number outside the range 0 to FrameCount()-1.
        See CImageDecoder::FrameCount().

@see	CImageDecoder::FrameCount()
*/
EXPORT_C const TFrameInfo& CImageDecoder::FrameInfo(TInt aFrameNumber) const
	{
	// Return the frame info for a particular frame
	ASSERT(ValidProperties());
	return iRelay->FrameInfo(aFrameNumber);
	}

/**
Returns additional plugin specific information on a specified frame.

The plugin specific information usually covers advanced image features such 
as image quality, advanced colour settings and so on.

Use FrameCount() to determine how many frames are contained in the 
image before using this function.

@param  aFrameNumber
        The frame number.

@return The data for the specified frame.

@panic  ImageConversion 10
        Frame number outside the range 0 to FrameCount()-1.
        See CImageDecoder::FrameCount().

@see	CImageDecoder::FrameCount()
*/
EXPORT_C const CFrameImageData& CImageDecoder::FrameData(TInt aFrameNumber) const
	{
	// Return the frame image data for a particular frame.
	ASSERT(ValidProperties());
	return iRelay->FrameData(aFrameNumber);
	}

/**
Return the number of comments attached to the image (as opposed to
a particular frame).

For further informantion on panic categories, please see the note in
CImageDecoder::IsImageHeaderProcessingComplete().

@panic  ImageConversion 13 Header processing has not completed.
		See CImageDecoder::IsImageHeaderProcessingComplete().

@see	IsImageHeaderProcessingComplete()

@return The number of comments attached to the image.
*/
EXPORT_C TInt CImageDecoder::NumberOfImageComments() const
	{
	ASSERT(ValidProperties());
	return iRelay->Plugin()->NumberOfImageComments();
	}

/**
Return a particular comment attached to the image.

Ownership of the returned buffer is transferred to the caller. Use NumberOfImageComments() 
to determine how many (if any) comments are contained within the image.

For further informantion on panic categories, please see the note in
CImageDecoder::IsImageHeaderProcessingComplete().

@param  aCommentNumber	The comment number.

@panic 	ImageConversion 12 Comments are not supported.
		See CImageDecoder::NumberOfImageComments().

@panic  ImageConversion 13 Header processing has not completed.
		See CImageDecoder::IsImageHeaderProcessingComplete().

@panic 	ImageConversion 14 aCommentNumber is not valid.
		See CImageDecoder::NumberOfImageComments().

@see	IsImageHeaderProcessingComplete()
@see	NumberOfImageComments()

@return A buffer containing the comment.
*/
EXPORT_C HBufC* CImageDecoder::ImageCommentL(TInt aCommentNumber) const
	{
	ASSERT(ValidProperties());
	return iRelay->Plugin()->ImageCommentL(aCommentNumber);
	}

/**
Return the number of comments attached to a given frame of the image
(as opposed to the whole image).

Use FrameCount() to retrieve the number of frames in the image to ensure 
that a valid aFrameNumber is used.

For further informantion on panic categories, please see the note in
CImageDecoder::IsImageHeaderProcessingComplete().

@param	aFrameNumber The frame number.

@panic 	ImageConversion 10 aFrameNumber is not valid.
		See CImageDecoder::FrameCount().

@panic  ImageConversion 13 Header processing has not completed.
		See CImageDecoder::IsImageHeaderProcessingComplete().

@see	IsImageHeaderProcessingComplete()
@see	FrameCount()

@return	The number of comments attached to a given frame of the image.
*/
EXPORT_C TInt CImageDecoder::NumberOfFrameComments(TInt aFrameNumber) const
	{
	ASSERT(ValidProperties());
	return iRelay->Plugin()->NumberOfFrameComments(aFrameNumber);
	}

/**
Return a particular comment attached to a given frame of the image.

The desired order of calling methods should be FrameCount(),NumberOfFrameComments() and then FrameCommentL().

Use FrameCount() to retrieve the number of frames in the image to ensure that a valid aFrameNumber is used.

Use NumberOfFrameComments() to retrieve the number of comments attached to a given frame
of the image (as opposed to the whole image),to ensure that a valid aCommentNumber is used.

Ownership of the returned buffer is transferred to the caller.

For further informantion on panic categories, please see the note in
CImageDecoder::IsImageHeaderProcessingComplete().

@param  aFrameNumber	The frame number within the image from which to retrieve the specified comment.

@param  aCommentNumber	The comment number to retrieve from the specified frame.

@panic 	ImageConversion 10 aFrameNumber is not valid.
		See CImageDecoder::FrameCount().

@panic  ImageConversion 13 Header processing has not completed.
		See CImageDecoder::IsImageHeaderProcessingComplete().
		
@panic	ImageConversion 14 aCommentNumber is not valid.
		See CImageDecoder::NumberOfFrameComments().

@see	IsImageHeaderProcessingComplete()
@see	FrameCount()
@see	NumberOfFrameComments()

@return A buffer containing the specified comment.
*/
EXPORT_C HBufC* CImageDecoder::FrameCommentL(TInt aFrameNumber, TInt aCommentNumber) const
	{
	ASSERT(ValidProperties());
	return iRelay->Plugin()->FrameCommentL(aFrameNumber, aCommentNumber);
	}

/**
Return the formatted frame information strings for a specific frame
and leave it on the cleanup stack.

Ownership is transferred to the caller.

@param  aFrameNumber
        The frame number from which to retrieve the formatted information string.

@return The formatted frame information strings.
*/
EXPORT_C CFrameInfoStrings* CImageDecoder::FrameInfoStringsLC(TInt aFrameNumber)
	{
	ASSERT(ValidProperties());
	return iRelay->FrameInfoStringsLC(aFrameNumber);
	}

/**
Return the formatted frame information strings for a specific frame.
Ownership is transferred to the caller.

@param  aFrameNumber
        The frame number from which to retrieve the formatted information string.

@return The formatted frame information strings.
*/
EXPORT_C CFrameInfoStrings* CImageDecoder::FrameInfoStringsL(TInt aFrameNumber)
	{
	CFrameInfoStrings* frameInfoStrings = FrameInfoStringsLC(aFrameNumber);
	CleanupStack::Pop();

	return frameInfoStrings;
	}

/**
Return the implementation UID of the decoder being used to decode the image.

@return	The implementation UID of the decoder.
*/
EXPORT_C TUid CImageDecoder::ImplementationUid() const
	{
	return iRelay->ImplementationUid();
	}

/**
Retrieves the image type and sub-type for a given frame of the image that
has just been decoded.

For further informantion on panic categories, please see the note in
CImageDecoder::IsImageHeaderProcessingComplete().

@param  aFrameNumber
        The frame number for which type information should be retreived.
@param  aImageType
        On return contains the image type UID for the specified frame.
@param  aImageSubType
        On return contains the image sub-type UID if there is one (or KNullUid if 
		there is not).

@panic 	ImageConversion 10 aFrameNumber is not valid.
		See CImageDecoder::FrameCount().

@see	IsImageHeaderProcessingComplete()
@see	FrameCount()
*/
EXPORT_C void CImageDecoder::ImageType(TInt aFrameNumber, TUid& aImageType, TUid& aImageSubType) const
	{
	ASSERT(ValidProperties());
	iRelay->Plugin()->ImageType(aFrameNumber, aImageType, aImageSubType);	
	}

/**
Calls CImageDecoderPlugin::HandleCustomSyncL(aParam) that executes user defined plugin 
specific functions. Subsequent behaviour depends on the CImageDecoderPlugin class.

This function is part of the support for extended codecs for use within classes 
derived from CImageDecoder.

Note: This function is intended for use by plugin writers only.

@param  aParam
        Interpretation dependent on plugin.
        
@see    CImageDecoderPlugin::HandleCustomSyncL()
*/

EXPORT_C void CImageDecoder::CustomSyncL(TInt aParam)
	{
	ASSERT(ValidProperties());
	iRelay->CustomSyncL(aParam);
	}

/**
Sets up background convert cycle, bypassing Convert().
A call to this will result in a call to the associated CImageDecoderPlugin::InitCustomAsyncL(aParam),
which if successful will start background processing. This function uses the same mechanism as Convert(),
and therefore cannot be used concurrently. Cancel() etc work as expected.

Note: This function is intended for use by plugin writers only.

@param  aRequestStatus
        Request status. On completion contains an error code.
        KErrNone if the bitmap was successfully decoded,
        otherwise another of the system-wide error codes.
@param  aParam
        Interpretation dependent on plugin.
*/
EXPORT_C void CImageDecoder::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	ASSERT(ValidProperties());
	iRelay->CustomAsync(aRequestStatus, aParam);
	}

/**
Returns associated CImageDecoderPlugin.

Allows the extended CImageDecoder object to talk to its CImageDecoderPlugin equivalent.

Note: This function is intendend for use by plugin writers only.

@return A pointer to the related CImageDecoderPlugin instance.
*/
EXPORT_C CImageDecoderPlugin* CImageDecoder::Plugin() const
	{
	ASSERT(ValidProperties());
	return iRelay->Plugin();	
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageDecoder::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageDecoder::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageDecoder::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageDecoder::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

/**
Function to calculate the reduction factor based on the input parameters.

@param  aOriginalSize
        A reference to the original size of an image.
@param  aReducedSize
        A reference to the new size of an image.
@return The reduction factor.
*/

EXPORT_C TInt CImageDecoder::ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const
	{
	ASSERT(ValidProperties());
	return iRelay->ReductionFactor(aOriginalSize, aReducedSize);	
	}

/**
Calculates reduced size of the decoded bitmap based on the input parameters and updates aReducedSize with this value.

@param  aOriginalSize
        A reference to the original size of an image.
@param  aReductionFactor
        The Reduction Factor to be applied
@param  aReducedSize
        A reference to the new size of the image.
@return An error code indicating if the function call was successful. KErrNone on success, otherwise
        KErrArgument.
*/
	
EXPORT_C TInt CImageDecoder::ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const	
	{
	ASSERT(ValidProperties());
	return iRelay->ReducedSize(aOriginalSize, aReductionFactor, aReducedSize);	
	}


/**
Set the decoder worker thread priority

@param  aPriority
		a new value for worker thread priority
@return KErrNotSupported 
		the decoder object doesn't use a worker thread.
		Other system-wide error codes.
@see	TThreadPriority
*/
EXPORT_C TInt CImageDecoder::SetDecoderThreadPriority(TThreadPriority aPriority)
	{
	return iRelay->SetDecoderThreadPriority( aPriority );
	}


/* IMAGE ENCODER */

/**
Returns a list of the basic image types that can be encoded, based on the
currently available encoder plugins.

The returned array contains entries for the supported image types. Each entry
consists of the "display string" as well as the UID for that image type. Since 
this function only returns basic image type UID's, the second UID which represents 
the sub-type will always be zero.

Ownership of the array is passed to the caller so, before the array goes out of 
scope in the client, the caller must call the array's ResetAndDestroy() method to free 
any entries.

@param  aImageTypeArray
        An empty array, into which this function will put a list of supported image types.
*/
EXPORT_C void CImageEncoder::GetImageTypesL(RImageTypeDescriptionArray& aImageTypeArray)
	{
	ImageEnDecoderUtils::DoGetImageTypesL(KImageEncoderInterfaceUid, aImageTypeArray);
	}

/**
@publishedAll
@released

Gets a list of the properties of a specific encoder plugin.

@param	aImplementationUid
        The encoder implementation UID for which the plugin properties need to be retrieved.
@param	aPropertiesArray
		The array of plugin properties owned by the specified encoder.
		The caller has the ownership of the array.
*/
EXPORT_C void CImageEncoder::GetPluginPropertiesL(const TUid aImplementationUid, RUidDataArray& aPropertiesArray)
	{
	ImageEnDecoderUtils::DoGetPluginPropertiesL(KImageEncoderInterfaceUid, aImplementationUid, aPropertiesArray);
	}

/**
@publishedAll
@released

Gets a list of encoder implementations UIDs that have some specific uids (properties, image type, image sub-type or class uids).

@param	aRequiredUids
        The array containing the UIDs of the required uids (properties, image type, image sub-type or class uids).
@param	aImplArray
		The array containing the implementation UIDs of the available encoder plugins with the required UIDs.
		The caller has the ownership of the array.
*/
EXPORT_C void CImageEncoder::GetInterfaceImplementationsL(const RUidDataArray& aRequiredUids, RUidDataArray& aImplArray)
	{
	ImageEnDecoderUtils::DoGetInterfaceImplementationsL(KImageEncoderInterfaceUid, aRequiredUids, aImplArray);
	}

/**
@publishedAll
@released

Gets a list of encoder implementations UIDs that have some specific required uids (properties, image type, image sub-type or class uids).

@param	aRequiredUids
        The array containing the UIDs of the required uids (properties, image type, image sub-type or class uids).
@param	aLength
		The length of aRequiredUids (number of required uids).
@param	aImplArray
		The array containing the implementation UIDs of the available encoder plugins with the required UIDs.
		The caller has the ownership of the array.
*/	
EXPORT_C void CImageEncoder::GetInterfaceImplementationsL(const TUid* aRequiredUids, const TInt aLength, RUidDataArray& aImplArray)
	{
	RUidDataArray requiredUids;
	CleanupClosePushL(requiredUids);
	for(TInt index = 0 ; index < aLength ; index++)
		{
		User::LeaveIfError(requiredUids.Append(aRequiredUids[index]));
		}
	ImageEnDecoderUtils::DoGetInterfaceImplementationsL(KImageEncoderInterfaceUid, requiredUids, aImplArray);
	CleanupStack::PopAndDestroy(1); //requiredUids
	}

/**
For a given basic image type, returns a list of the image sub-types that can
be encoded, based on the currently available encoder plugins.

Each entry in the returned array consists of the "display string" for
the plugin as well as the UID for the sub-type. The first UID represents
the basic type and is always set to aImageType.

Ownership of the array is passed to the caller, so before the
array goes out of scope in the client, the caller must call
the array's ResetAndDestroy() method to free any entries.

@param  aImageType
        The basic image type for which a list of sub-types should be returned.
@param  aSubTypeArray
        An empty array, into which this function will put a list of of supported
        image sub-types.
*/
EXPORT_C void CImageEncoder::GetImageSubTypesL(const TUid aImageType, RImageTypeDescriptionArray& aSubTypeArray)
	{
	ImageEnDecoderUtils::DoGetImageTypesL(KImageEncoderInterfaceUid, aSubTypeArray, aImageType);
	}

/**
Get a list of the file extensions that can be encoded and their corresponding
MIME types, based on the currently available encoder plugins.

@param  aFileTypeArray
        An empty array, into which this function will put a list of
        entries. Each entry will consist of a file extension string for
        which an encoder plugin has been found, accompanied by the
        primary MIME type and then any secondary MIME types
        (if present).
        
		Ownership of the array is passed to the caller, so before the
		array goes out of scope in the client, the caller must call
		the array's ResetAndDestroy() method to free any entries.
*/
EXPORT_C void CImageEncoder::GetFileTypesL(RFileExtensionMIMETypeArray& aFileTypeArray)
	{
	ImageEnDecoderUtils::DoGetFileTypesL(KImageEncoderInterfaceUid, aFileTypeArray);
	}


/**
Creates an encoder based on a specified MIME type and write output to a named file.

The client supplies a MIME type which will be used to try and select an appropriate
plugin encoder. If an appropriate encoder is found, it is created.

If any file related errors are encountered opening the specified file, this
function leaves with an appropriate file related leave code.

@param  aFs
        A reference to a file server session for the encoder to use.
@param  aDestinationFilename
        The name of the file into which to put the encoded image.
@param  aMIMEType
        The MIME type to use for the encoding.
@param  aOptions
        The encoder options to use.

@return Returns a pointer to the newly created encoder.

@see    TOptions
*/
EXPORT_C CImageEncoder* CImageEncoder::FileNewL(RFs& aFs, const TDesC& aDestinationFilename, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	//Get a sorted list of encoders
	RImplInfoPtrArray encoderList;
	CleanupResetAndDestroyPushL(encoderList);
	CImageEncoder::MimeTypeGetEncoderListL(encoderList, aMIMEType, aOptions);

	if(encoderList.Count() == 0)
		User::Leave(KErrNotFound);

	//Use the highest rated encoder.
	CImageEncodeConstruct* construct = NewEncodeConstructL(*encoderList[0], aOptions);
	CleanupStack::PopAndDestroy(&encoderList);

	CImageEncoder* encoderPtr = NewL(construct, aOptions); // NewL takes ownership of construct - no need to push on stack
	CleanupStack::PushL(encoderPtr);
	encoderPtr->iRelay->SetFileL(aFs, aDestinationFilename, aOptions);
	CleanupStack::Pop(encoderPtr); 

	return encoderPtr;
	}

/**
Creates a plugin encoder for a specified MIME type and writes output to a descriptor.

The client supplies a MIME type which will be used to try and select an appropriate
plugin encoder. If an appropriate encoder is found, it creates it.

@param  aDestinationData
        The buffer pointer into which to put the encoded image. This must be
        a NULL pointer. Memory will be allocated internally and ownership of
        the data passed to the caller.
@param  aMIMEType
        The MIME type to use for the encoding.
@param  aOptions
        Encoder options to use.

@return	A pointer to the newly created encoder.

@see    TOptions
*/
EXPORT_C CImageEncoder* CImageEncoder::DataNewL(HBufC8*&  aDestinationData, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	if (aDestinationData!=NULL)
		Panic(ENonNullDescriptorPassed);

	//Get a sorted list of encoders
	RImplInfoPtrArray encoderList;
	CleanupResetAndDestroyPushL(encoderList);
	CImageEncoder::MimeTypeGetEncoderListL(encoderList, aMIMEType, aOptions);

	if(encoderList.Count() == 0)
		User::Leave(KErrNotFound);

	//Use the highest rated encoder.
	CImageEncodeConstruct* construct = NewEncodeConstructL(*encoderList[0], aOptions);
	CleanupStack::PopAndDestroy(&encoderList);
	CImageEncoder* encoderPtr = NewL(construct, aOptions); // NewL takes ownership of construct - no need to push on stack
	CleanupStack::PushL(encoderPtr);
	encoderPtr->iRelay->SetDataL(aDestinationData, aOptions);
	CleanupStack::Pop(encoderPtr); 
	return encoderPtr;
	}

/**
@internalTechnology

Creates a list of encoders that support the specified MIME type.

@param  aEncoderList
        Create a list of encoders that support the given MIME type.
@param	aMIMEType
        The MIME type to encode to.
@param	aOptions
		Decoder options to use.
*/
void CImageEncoder::MimeTypeGetEncoderListL(RImplInfoPtrArray& aEncoderList, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	if (aMIMEType.Length() == 0)
		{ // Get out, empty MIME type string
		User::Leave(KErrArgument);
		}

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchMIMEType);
	customMatchData->SetMatchStringL(aMIMEType);
	customMatchData->SetExtensionOptions(aOptions);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);

	REComSession::ListImplementationsL(KImageEncoderInterfaceUid, resolverParams, KImageConvertResolverUid, aEncoderList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
	}

/**
@internalTechnology

Create a list of encoders that support the specified image type.

@param  aEncoderList
        A list of encoders that support the specified image type.
@param  aImageType
        The image base type.
@param  aImageSubType
        The image sub type.
@param  aEncoderUid
        The implementation UID for a specific codec or a decoder/encoder class UID.
@param	aOptions
		Encoder options to use.

@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface
*/
void CImageEncoder::ImageTypeGetEncoderListL(RImplInfoPtrArray& aEncoderList, const TUid aImageType, const TUid aImageSubType, const TUid aEncoderUid, const TOptions aOptions)
	{
	if ((aImageType == KNullUid) && (aEncoderUid == KNullUid))
		{ // Get out, no base type specified
		Panic(EIllegalImageType); 
		}

	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetBaseType(aImageType);
	customMatchData->SetSubType(aImageSubType);
	customMatchData->SetImplementationType(aEncoderUid);
	customMatchData->SetExtensionOptions(aOptions);

	HBufC8* package  = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();
	
	TEComResolverParams resolverParams; // Parameters on which to match
	resolverParams.SetDataType(packageDes);
   
	REComSession::ListImplementationsL(KImageEncoderInterfaceUid, resolverParams, KImageConvertResolverUid, aEncoderList);

	CleanupStack::PopAndDestroy(2); // package, customMatchData
   	}

/**
@internalTechnology

Create an encode construct using a image type, sub type and encoder
implementation UID.

@param  aEncoderInfo
        Implementation information for the encoder to be created.
@param	aOptions
		Encoder options to use.
@return Returns a pointer to the newly created encoder construct.

*/
CImageEncodeConstruct* CImageEncoder::NewEncodeConstructL(const CImplementationInformation& aEncoderInfo, const TOptions aOptions)
	{
	CCustomMatchData* customMatchData = CCustomMatchData::NewLC();
	COpaqueDataParse* parse = COpaqueDataParse::NewLC(aEncoderInfo.OpaqueData());

	customMatchData->SetMatchType(EMatchUids);
	customMatchData->SetBaseType(parse->ImageTypeUid());
	customMatchData->SetSubType(parse->ImageSubTypeUid());
	customMatchData->SetImplementationType(aEncoderInfo.ImplementationUid());
	customMatchData->SetExtensionOptions(aOptions);

	CleanupStack::PopAndDestroy(parse);

	HBufC8* package = customMatchData->NewPackLC();
	TPtr8 packageDes = package->Des();

	TEComResolverParams resolverParams;
	resolverParams.SetDataType(packageDes);

	CImageEncodeConstruct* construct = NULL;
	construct = STATIC_CAST(CImageEncodeConstruct*,
			REComSession::CreateImplementationL(KImageEncoderInterfaceUid,
			_FOFF(CImageEncodeConstruct, iDtorIDKey),
			resolverParams,
			KImageConvertResolverUid));
	ASSERT(construct!=NULL);

	CleanupStack::PopAndDestroy(2, customMatchData); //package, customMatchData

	return construct;
	}

/**
Creates an encoder based on a supplied parameters and writes output to a named file.

The client must supply a basic image type (and a sub-type, if applicable) or a specific
encoder implementation UID, which will be used to try and select an appropriate plugin
encoder. If an appropriate encoder is found, it is created.

Note:
Every image format has two IDs, known as the type and the sub-type (although generally
the sub-type is KNullUid). To retrieve a list of supported types and sub-types that can be
encoded, use the static functions GetImageTypesL() and GetImageSubTypesL().

If no plugin encoder can be found that matches the details provided in aImageType, aImageSubType
and possibly aEncoderUid this function leaves with KErrNotFound or KEComErrNoInterfaceIdentified.

If any file related errors are encountered opening the specified file, this function leaves
with an appropriate file related leave code.

@param  aFs
        A reference to a file server session for the encoder to use.
@param  aDestinationFilename
        The name of the file into which to put the encoded image.
@param  aOptions
        Options to use.
@param  aImageType
        The image type to use for the encoding (mandatory).
@param  aImageSubType
        The image sub-type to use for the encoding (only if applicable, defaults to KNullUid).
@param  aEncoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return A pointer to the newly created encoder.

@leave  KErrNotFound
        No appropriate plugin encoder for this image has been found.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.
@panic  ImageConversion 20
        No base type given for encoder implementation.

@see    TOptions
*/
EXPORT_C CImageEncoder* CImageEncoder::FileNewL(RFs& aFs, const TDesC& aDestinationFilename, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aEncoderUid)
	{
	//Get a sorted list of encoders
	RImplInfoPtrArray encoderList;
	CleanupResetAndDestroyPushL(encoderList);
	CImageEncoder::ImageTypeGetEncoderListL(encoderList, aImageType, aImageSubType, aEncoderUid, aOptions);

	if(encoderList.Count() == 0)
		User::Leave(KErrNotFound);

	//Use the highest rated encoder.
	CImageEncodeConstruct* construct = NewEncodeConstructL(*encoderList[0], aOptions);
	CleanupStack::PopAndDestroy(&encoderList);
	CImageEncoder* encoderPtr = NewL(construct, aOptions); // NewL takes ownership of construct - no need to push on cleanstack
	CleanupStack::PushL(encoderPtr);
	encoderPtr->iRelay->SetFileL(aFs, aDestinationFilename, aOptions);
	CleanupStack::Pop(); //encoderPtr
	return encoderPtr;
	}

/**
Creates a plugin encoder based on optional parameters and writes output to a descriptor.

The client must supply a basic image type (and a sub-type, if applicable) or specific encoder/class,
implementation UID which will be used to try and select an appropriate plugin
encoder. If an appropriate encoder is found, it is created.

Note: 
Every image format has two IDs, known as the type and the sub-type (although generally the sub-type 
is KNullUid). To retrieve a list of supported types and sub-types that can be encoded, use the 
static functions GetImageTypesL() and GetImageSubTypesL().

If no plugin encoder can be found that matches the details provided in aImageType, aImageSubType
and possibly aEncoderUid this function leaves with KErrNotFound or KEComErrNoInterfaceIdentified.

@param  aDestinationData
        The buffer pointer into which to put the encoded image.
@param  aOptions
        The encoder options to use.
@param  aImageType
        The image type to use for the encoding (mandatory).
@param  aImageSubType
        The image sub-type to use for the encoding (only if applicable, defaults to KNullUid).
@param  aEncoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return A pointer to the newly created encoder.

@leave  KErrNotFound
        No appropriate plugin encoder for this image has been found.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.
@panic  ImageConversion 20
        No base type given for encoder implementation.

@see    TOptions
*/
EXPORT_C CImageEncoder* CImageEncoder::DataNewL(HBufC8*& aDestinationData, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aEncoderUid)
	{
	if (aDestinationData!=NULL)
		Panic(ENonNullDescriptorPassed);

	//Get a sorted list of encoders
	RImplInfoPtrArray encoderList;
	CleanupResetAndDestroyPushL(encoderList);
	CImageEncoder::ImageTypeGetEncoderListL(encoderList, aImageType, aImageSubType, aEncoderUid, aOptions);

	if(encoderList.Count() == 0)
		User::Leave(KErrNotFound);

	//Use the highest rated encoder.
	CImageEncodeConstruct* construct = NewEncodeConstructL(*encoderList[0], aOptions);
	CleanupStack::PopAndDestroy(&encoderList);
	CImageEncoder* encoderPtr = NewL(construct, aOptions); // NewL takes ownership of construct - no need to push on cleanstack
	CleanupStack::PushL(encoderPtr);
	encoderPtr->iRelay->SetDataL(aDestinationData, aOptions);
	CleanupStack::Pop(); //encoderPtr
	return encoderPtr;
	}

/**
@internalTechnology

Called internally to create a CImageEncoder and associated iRelay body
*/
CImageEncoder* CImageEncoder::NewL(CImageEncodeConstruct* aConstruct, TOptions aOptions)
	{
	CleanupStack::PushL(aConstruct); // we take ownership of aConstruct
	CImageEncoder* self = aConstruct->NewEncoderL(); // typically will callback to CImageEncoder::NewL()
	CleanupStack::PushL(self);
	TBool alwaysThread = (aOptions & EOptionAlwaysThread)!=EFalse;
	self->iRelay = MImageEncoderRelay::NewL(aConstruct, alwaysThread); // ownership of aConstruct switches to properties
	CleanupStack::Pop(2); // self  and aConstruct
	self->iRelay->TransferConstructOwnership();
	return self;
	}

/**
@internalTechnology

Actual factory function for CImageEncoder - ie. it creates the object
Called back plugin - to allow plugin to override if required
*/
CImageEncoder* CImageEncoder::NewL()
	{
	CImageEncoder* self = new (ELeave) CImageEncoder;
	return self;
	}

/**
Constructor for this class.
*/
EXPORT_C CImageEncoder::CImageEncoder()
	{
	}

/**
Destructor for this class.

Closes the file. If using a local file session, it closes it.
Calls ECom to tell it the encoder instance is no longer required.

Frees all other resources owned by the object prior to its destruction.
*/
EXPORT_C CImageEncoder::~CImageEncoder()
	{
	Cancel();

	delete iRelay;
	}

/**
Encodes a bitmap asynchronously.

When encoding is complete, successfully or otherwise, the
status is returned in aRequestStatus.

@param  aRequestStatus
        The request status. On completion contains an error code.
        KErrNone if the bitmap was successfully encoded,
        otherwise another of the system-wide error codes.
@param  aSource
        A bitmap to encode.
@param  aFrameImageData
        The frame image data (optional, defaults to NULL).
		There exists format-specific image data variants that are used by 
		encoders to obtain image specific data. This behaviour is invoked by specifying 
		aFrameImageData. Otherwise, encoder specific defaults are invoked.
@see TBmpImageData
*/
EXPORT_C void CImageEncoder::Convert(TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData)
	{
	ASSERT(ValidProperties());
	iRelay->Convert(aRequestStatus, aSource, aFrameImageData);
	}

/**
Asynchronously cancels any conversions currently in progress.
*/
EXPORT_C void CImageEncoder::Cancel()
	{
	if(ValidProperties())
		iRelay->Cancel();
	}

/**
Returns the implementation UID of the encoder being used to encode the bitmap.

@return	The implementation UID of the encoder.
*/
EXPORT_C TUid CImageEncoder::ImplementationUid() const
	{
	return iRelay->ImplementationUid();
	}

/**
Calls CImageEncoderPlugin::HandleCustomSyncL() that executes user defined
plugin specific functions. Subsequent behaviour therefore depends on the 
CImageEncoderPlugin class.

Note:
For use by plugin writers only.

@param  aParam
        Interpretation determined by plugin.
*/
EXPORT_C void CImageEncoder::CustomSyncL(TInt aParam)
	{
	ASSERT(ValidProperties());
	iRelay->CustomSyncL(aParam);
	}

/**
Sets up background convert cycle, bypassing Convert().

Use this function to inititate CImageEncoderPlugin::InitCustomAsyncL(aParam),
which if successful will start background processing. Convert() uses the same
mechanism as CustomAsync(), and therefore cannot be used concurrently. Cancel() 
and other related functions still work as expected.

Note:
For use by plugin writers only.

@param  aRequestStatus
        The request status. On completion contains an error code.
        KErrNone if the bitmap was successfully encoded,
        otherwise another of the system-wide error codes.
@param  aParam
        Interpretation determined by plugin.
*/
EXPORT_C void CImageEncoder::CustomAsync(TRequestStatus* aRequestStatus, TInt aParam)
	{
	ASSERT(ValidProperties());
	iRelay->CustomAsync(aRequestStatus, aParam);
	}

/**
Returns the associated CImageEncoderPlugin.

This is part of support for extended codecs, for use within classes derived from 
CImageEncoder. Allows the extended CImageEncoder object to talk to its 
CImageEncoderPlugin equivalent.

Note:
For use by plugin writers only.
*/
EXPORT_C CImageEncoderPlugin* CImageEncoder::Plugin() const
	{
	ASSERT(ValidProperties());
	return iRelay->Plugin();
	}

/**
@internalTechnology

Intended for future proofing - will pannic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageEncoder::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will pannic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageEncoder::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will pannic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageEncoder::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will pannic if called

@panic  ImageConversion 30
*/
EXPORT_C void CImageEncoder::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

/**
Create a decoder for the image in the named file. The client supplies a
MIME type which will be used to try and select an appropriate plugin
decoder. If it finds a decoder it creates it and then goes on to use that
decoder to scan the beginning of the image file.

If any file related errors are encountered opening the specified file, this 
function leaves with an appropriate file related leave code.

@param  aFile
        The handle of the file to decode
@param  aMIMEType
        The MIME type of the image in the file.
@param  aIntent
		The DRM Intent to open the file with
@param	aOptions
        Decoder options to use.

@return	Returns a pointer to the newly created decoder.

@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        Either the appropriate plugin decoder for this file hasn't been found, or the file itself is missing.

@see    TOptions
*/
EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFile& aFile, const TDesC8& aMIMEType, TIntent aIntent, const TOptions aOptions)
	{
	return CImageDecoder::FileNewImplL(aFile, aMIMEType, KDefaultContentObject, aIntent, aOptions);
	}

/**
Creates a decoder for the image in the named file. If the client supplies an
image type (and sub-type, if applicable) or decoder UID, these will be used
to try and select an appropriate plugin decoder. If not, then the selection
will be done by matching the image header in the file. If it finds a decoder
it creates it and then goes on to use that decoder to scan the beginning of
the image file.

@param  aFile
        The handle of the file to decode
@param  aIntent
        The DRM Intent for image conversion.
@param  aOptions
        The decoder options to use. See TOptions.
@param  aImageType
        The image type of the image in the file (optional, defaults to KNullUid).
@param  aImageSubType
        The image sub-type of the image in the file (optional, defaults to KNullUid).
@param  aDecoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return	A pointer to the newly created decoder.

@leave  KErrUnderflow
        Not enough data in file to identify which plugin decoder to use.
@leave  KErrNotFound
        Either the appropriate plugin decoder for this file hasn't been found, or the file itself is missing.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.

@see     TOptions
*/

EXPORT_C CImageDecoder* CImageDecoder::FileNewL(RFile& aFile, TIntent aIntent, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	return CImageDecoder::FileNewImplL(aFile, KDefaultContentObject, aIntent, aOptions, aImageType, aImageSubType, aDecoderUid);
	}

CImageDecoder* CImageDecoder::FileNewImplL(RFile& aFile, const TDesC8& aMIMEType, const TDesC& aUniqueId, TIntent aIntent, const TOptions aOptions)
	{
	CContent* content = CContent::NewLC(aFile);
	CData* data = content->OpenContentL(aIntent, aUniqueId); // check file presence, evaluate (not execute) intent

	delete data; // close file
	CleanupStack::PopAndDestroy(content);

	//Get a sorted list of decoders that will decode the image
	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);
	CImageDecoder::MimeTypeGetDecoderListL(decoderList, aMIMEType, aOptions);

	CImageDecoder* decoder = NULL;
	decoder = CImageDecoder::FileFindDecoderNewL(decoderList, aFile, aOptions, aUniqueId);

	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);
	decoder->iRelay->SetIntent(aIntent);

	CleanupStack::Pop(decoder);
	CleanupStack::PopAndDestroy(&decoderList);
	return decoder;
	}
	
CImageDecoder* CImageDecoder::FileNewImplL(RFile& aFile, const TDesC& aUniqueId, const TIntent aIntent, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aDecoderUid)
	{
	if ((aImageType == KNullUid) && (aImageSubType != KNullUid))
		{ // Get out, no base type given for sub-type
		Panic(EIllegalImageSubType); 
		}

	RImplInfoPtrArray decoderList;
	CleanupResetAndDestroyPushL(decoderList);

	CContent* content = CContent::NewLC(aFile);
	CData* data = content->OpenContentL(aIntent, aUniqueId);
	CleanupStack::PushL(data);

	if (aImageType == KNullUid && aDecoderUid == KNullUid)
		{ 
		TBuf8<KMaxMimeLength> mimeType;
		if (data->GetMimeTypeL(mimeType))
			{
			// try to find a controller based on MIME type
			CImageDecoder::MimeTypeGetDecoderListL(decoderList, mimeType, aOptions);
			}
		if (decoderList.Count()==0)
			{
			// read header data
			TBuf8<KImageHeaderSize> imageHeader;
			User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));
			CImageDecoder::ImageTypeGetDecoderListL(decoderList, imageHeader, aImageType, aImageSubType, aDecoderUid, aOptions);
			}
		}
	else
		{
		TBuf8<KImageHeaderSize> imageHeader;
		User::LeaveIfError(data->Read(imageHeader, KImageHeaderSize));
		CImageDecoder::ImageTypeGetDecoderListL(decoderList, imageHeader, aImageType, aImageSubType, aDecoderUid, aOptions);
		}
	
	CleanupStack::PopAndDestroy(2, content); // content, data

	//Try to match by file extension only
	//1) If no plugin was found and 
	//2) No specific decoder or format was specified
	TBuf<256> filename;
	aFile.Name(filename);
	const TBool formatSpecified = (aImageType!=KNullUid || aImageSubType!=KNullUid || aDecoderUid!=KNullUid);
	if(decoderList.Count()==0 && !formatSpecified)
		{
		CImageDecoder::SuffixTypeGetDecoderListL(decoderList, filename, aOptions);
		}

	CImageDecoder* decoder = NULL;
	decoder = CImageDecoder::FileFindDecoderNewL(decoderList, aFile, aOptions, aUniqueId);
	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);
	decoder->iRelay->SetIntent(aIntent);

	CleanupStack::Pop(decoder);
	CleanupStack::PopAndDestroy(&decoderList);
	return decoder;
	}
	
/**
@internalTechnology

Scans a sorted list of decoders for the first one that can decode the image.

@param  aDecoderList
        A list of decoders that support the image format.
@param  aFile
        A file server session for the decoder to use.
@param  aOptions
        The options to use during decoding.
@param  aUniqueId
        Identifier of file within a multi-part archive.

@return A pointer to the decoder.
*/
CImageDecoder* CImageDecoder::FileFindDecoderNewL(const RImplInfoPtrArray& aDecoderList, RFile& aFile, const TOptions aOptions, const TDesC& aUniqueId)
	{
	TInt noOfDecoders = aDecoderList.Count();

	if(noOfDecoders == 0)
		User::Leave(KErrNotFound);

	CImageDecoder* decoder = NULL;
	TInt decoderNo = 0;
	TInt error = KErrNone;
	do
		{
		const CImplementationInformation& decoderInfo = *(aDecoderList[decoderNo++]);
		TRAP(error,decoder=FileDecoderNewL(decoderInfo, aFile, aOptions, aUniqueId));
		if (error != KErrCorrupt && error != KErrNotSupported && error != KErrNotFound)
			break;
		}
	while(decoderNo < noOfDecoders);

	if(error!=KErrNone)
		{
		ASSERT(decoder==NULL);
		if (error == KErrCorrupt || error == KErrNotSupported)
			error = KErrNotFound;
		User::Leave(error);
		}

	return decoder;
	}
	
/**
@internalTechnology

Create a construct that can create a decoder and call
functions to initialise the decoder with the image data.

@param  aDecoderInfo
        Implementation information for the decoder to be created.
@param  aFile
        A file server session for the decoder to use.
@param  aOptions
        Options the decoder must use.
@param  aUniqueId
        Identifier of file within a multi-part archive.

@return	A pointer to the decoder.
*/
CImageDecoder* CImageDecoder::FileDecoderNewL(const CImplementationInformation& aDecoderInfo, RFile& aFile, const TOptions aOptions, const TDesC& aUniqueId)
	{
	CImageDecodeConstruct* construct = NULL;
	construct = NewDecodeConstructL(aDecoderInfo, aOptions);
	ASSERT(construct!= NULL);

	CImageDecoder* decoder = NewL(construct, aOptions); // note NewL takes ownership of construct - don't push on stack
	ASSERT(decoder!=NULL);
	CleanupStack::PushL(decoder);

	decoder->iRelay->SetUniqueIdL(aUniqueId);

	decoder->iRelay->SetFileL(aFile, aOptions);
	decoder->iRelay->HandleNewlyOpenedImageL();

	CleanupStack::Pop(decoder);
	return decoder;
	}


/**
Creates an encoder based on a specified MIME type and write output to a named file.

The client supplies a MIME type which will be used to try and select an appropriate
plugin encoder. If an appropriate encoder is found, it is created.

If any file related errors are encountered opening the specified file, this
function leaves with an appropriate file related leave code.

@param  aFile
        The handle of an open file to write the encoded image to
@param  aMIMEType
        The MIME type to use for the encoding.
@param  aOptions
        The encoder options to use.

@return Returns a pointer to the newly created encoder.

@leave  KEComErrNoInterfaceIdentified
        ECom could not find the specified interface.
@leave  KErrNotFound
        No appropriate plugin encoder for this image has been found.

@see    TOptions
*/
EXPORT_C CImageEncoder* CImageEncoder::FileNewL(RFile& aFile, const TDesC8& aMIMEType, const TOptions aOptions)
	{
	//Get a sorted list of encoders
	RImplInfoPtrArray encoderList;
	CleanupResetAndDestroyPushL(encoderList);
	CImageEncoder::MimeTypeGetEncoderListL(encoderList, aMIMEType, aOptions);

	if(encoderList.Count() == 0)
		User::Leave(KErrNotFound);

	//Use the highest rated encoder.
	CImageEncodeConstruct* construct = NewEncodeConstructL(*encoderList[0], aOptions);
	CleanupStack::PopAndDestroy(&encoderList);

	CImageEncoder* encoderPtr = NewL(construct, aOptions); // NewL takes ownership of construct - no need to push on stack
	CleanupStack::PushL(encoderPtr);
	encoderPtr->iRelay->SetFileL(aFile, aOptions);
	CleanupStack::Pop(encoderPtr); 
	return encoderPtr;
	}
	
/**
Creates an encoder based on a supplied parameters and writes output to a named file.

The client must supply a basic image type (and a sub-type, if applicable) or a specific
encoder/class implementation UID, which will be used to try and select an appropriate plugin
encoder. If an appropriate encoder is found, it is created.

Note:
Every image format has two IDs, known as the type and the sub-type (although generally
the sub-type is KNullUid). To retrieve a list of supported types and sub-types that can be
encoded, use the static functions GetImageTypesL() and GetImageSubTypesL().

If no plugin encoder can be found that matches the details provided in aImageType, aImageSubType
and possibly aEncoderUid this function leaves with KErrNotFound or KEComErrNoInterfaceIdentified.

If any file related errors are encountered opening the specified file, this function leaves
with an appropriate file related leave code.

@param  aFile
        The handle of an open file to write the encoded image to
@param  aOptions
        Options to use.
@param  aImageType
        The image type to use for the encoding (mandatory).
@param  aImageSubType
        The image sub-type to use for the encoding (only if applicable, defaults to KNullUid).
@param  aEncoderUid
		The implementation UID for a specific codec or a decoder/encoder class UID (optional, defaults to KNullUid).
		If this option is selected for a specific codec the image type and image sub type for the displayer must be supplied.
		When loading plugins by class UID the image type and image subtype are not mandatory and the first valid plugin from 
		the list of available plugins with the specified class UID will be loaded.
@see	KUidICLJpegEXIFInterface
@see	KUidICLJpegImageFrameInterface

@return A pointer to the newly created encoder.

@leave  KErrNotFound
        No appropriate plugin encoder for this image has been found.
@leave  KEComErrNoInterfaceIdentified
		ECom could not find the specified interface.		

@panic  ImageConversion 19
        No base type given for sub-type.
@panic  ImageConversion 20
        No base type given for encoder implementation.

@see    TOptions
*/
EXPORT_C CImageEncoder* CImageEncoder::FileNewL(RFile& aFile, const TOptions aOptions, const TUid aImageType, const TUid aImageSubType, const TUid aEncoderUid)
	{
	//Get a sorted list of encoders
	RImplInfoPtrArray encoderList;
	CleanupResetAndDestroyPushL(encoderList);
	CImageEncoder::ImageTypeGetEncoderListL(encoderList, aImageType, aImageSubType, aEncoderUid, aOptions);

	if(encoderList.Count() == 0)
		User::Leave(KErrNotFound);

	//Use the highest rated encoder.
	CImageEncodeConstruct* construct = NewEncodeConstructL(*encoderList[0], aOptions);
	CleanupStack::PopAndDestroy(&encoderList);
	CImageEncoder* encoderPtr = NewL(construct, aOptions); // NewL takes ownership of construct - no need to push on cleanstack
	CleanupStack::PushL(encoderPtr);
	encoderPtr->iRelay->SetFileL(aFile, aOptions);
	CleanupStack::Pop(); //encoderPtr
	return encoderPtr;
	}

	
/**
Select to encode or not the thumbnail

@param  aDoGenerateThumbnail
		EFalse if no generation of thumbnail
*/
EXPORT_C void CImageEncoder::SetThumbnail(TBool aDoGenerateThumbnail)
	{
	ASSERT(ValidProperties());
	iRelay->SetThumbnail(aDoGenerateThumbnail);
	}
	
/**
Set the encoder worker thread priority

@param  aPriority
		a new value for worker thread priority
@return KErrNotSupported 
		the encoder object doesn't use a worker thread.
		Other system-wide error codes.
@see	TThreadPriority
*/
EXPORT_C TInt CImageEncoder::SetEncoderThreadPriority(TThreadPriority aPriority)
	{
	return iRelay->SetEncoderThreadPriority( aPriority );
	}

/**
@publishedAll
@released

Optional call from client which may be made on the encoder to allow analysis of image prior to calling
Convert.

Should be called once encode is fully set up e.g. any encode operations defined.

@param	aRequestStatus
		Request status. On completion this contains an error code. This is KErrNone if the frame
		was analyzed successfully, KErrNotSupported if the codec does not support analysis, or a
		system-wide error code.

@see CImageEncoder::Convert
*/
EXPORT_C void CImageEncoder::Prepare(TRequestStatus* aRequestStatus)
	{
	ASSERT(ValidProperties());
	ASSERT(aRequestStatus);

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvPrepare* prepare = cache->Prepare();

	if(!prepare)
		{
		MImageConvExtension* prepareExtension = NULL;
		TRAPD(err,
			{
			iRelay->GetExtensionL(KICLPrepareUid, prepareExtension);
			prepare = new (ELeave) TImageConvPrepare();
			prepare->SetExtension(prepareExtension);
			cache->SetPrepareExtension(prepare);
			});
		if ( err != KErrNone )
			{
			User::RequestComplete(aRequestStatus, err);
			return;
			}
		}
	prepare->Prepare(aRequestStatus);
	}

CContent* CImageDecoder::GetContentLC(const TDesC& aSourceFilename)
	{
	CContent* content = NULL;
	TRAPD(err,content = CContent::NewL(aSourceFilename, EContentShareReadOnly));
	if(err != KErrNone && err != KErrNoMemory)
		{
		content = CContent::NewL(aSourceFilename, EContentShareReadWrite);
		}
	User::LeaveIfNull(content);
	CleanupStack::PushL(content);
	return content;
	}

/**
@publishedAll
@released


Get the extension interface for operations on the image.
@return Interface to image conversion operations.
@leave KErrNotSupported if loaded codec plugin does not support this optional extension.
@leave Other system wide errors

@note The order of post processing operations when applied for decoding is

- 1. Clip the image 
- 2. Scale the image 
- 3. Operate on the image 

@see CImageDecoder::SetClippingRectL()
@see TImageConvScaler
@see TImageConvOperation
*/
EXPORT_C TImageConvOperation* CImageDecoder::OperationL()
	{
	ASSERT(ValidProperties());

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvOperation* operation = cache->Operation();

	if(!operation)
		{
		MImageConvExtension* operationExtension = NULL;
		iRelay->GetExtensionL(KICLOperationUid, operationExtension);
		operation = new (ELeave) TImageConvOperation();
		operation->SetExtension(operationExtension);
		cache->SetOperationExtension(operation);
		}
	return operation;
	}

/**
@publishedAll
@released


Get the extension interface for scaling the image. @see TImageConvScaler
@return Interface to image conversion scaler.
@leave KErrNotSupported if loaded codec plugin does not support this optional extension.
@leave Other system wide errors.

@see note under CImageDecoder::OperationL.
*/
EXPORT_C TImageConvScaler* CImageDecoder::ScalerL()
	{
	ASSERT(ValidProperties());

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvScaler* scaler = cache->Scaler();

	if(!scaler)
		{
		MImageConvExtension* scalerExtension = NULL;
		iRelay->GetExtensionL(KICLScalerUid, scalerExtension);
		scaler = new (ELeave) TImageConvScaler();
		scaler->SetExtension(scalerExtension);
		cache->SetScalerExtension(scaler);
		}
	return scaler;
	}

/**
@publishedAll
@released


Get the extension interface for block streaming on the image. @see TImageConvStreamedDecode
@return Interface to image conversion block streamer.
@leave KErrNotSupported if loaded codec plugin does not support this optional extension.
@leave Other system wide errors.
*/
EXPORT_C TImageConvStreamedDecode* CImageDecoder::BlockStreamerL()
	{
	ASSERT(ValidProperties());

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvStreamedDecode* streamer = cache->DecodeBlockStreamer();

	if(!streamer)
		{
		MImageConvExtension* streamExtension = NULL;
		iRelay->GetExtensionL(KICLStreamedDecodeUid, streamExtension);
		streamer = new (ELeave) TImageConvStreamedDecode();
		streamer->SetExtension(streamExtension);
		cache->SetBlockStreamerExtension(streamer);
		}
	return streamer;
	}

/**
@publishedAll
@released


Sets the area of interest of the image to be decoded.  This function can leave with
any of the system-wide error codes.

@param aClipRect	A pointer to a TRect that specifies the
					location and size of the region to be decoded.  This
					rectangle must have positive width and height values as
					per TRect::IsNormalized() and TRect::Normalize().
					Passing in a NULL value will clear the clipping rectangle.
					Note that a clipping rectangle may not be valid for all frames of an image.

@leave KErrNotSupported		This function is not supported.
@leave KErrArgument			Returned if the clipping rectangle:
							a) is empty (i.e. IsEmpty() returns ETrue)
							b) is not normalised (i.e. IsNormalized() returns EFalse)
							c) has coordinates that are not located within, or on,
							the coodinates of at least one frame of the original image.
							d) has a width or a height of 0

@see	TRect::IsEmpty()
@see	TRect::IsNormalized()
@see	TRect::Normalize()

@see note under CImageDecoder::OperationL
*/
EXPORT_C void CImageDecoder::SetClippingRectL(const TRect* aClipRect)
	{
	ASSERT(ValidProperties());
	iRelay->SetClippingRectL(aClipRect);
	}

/**
@publishedAll
@released


Optional call from client which may be made on the decoder to allow analysis of image prior to calling
Convert.

Should be called once decode is fully set up e.g. clipping rectangle set.

@param	aRequestStatus
		Request status. On completion this contains an error code. This is KErrNone if the frame
		was analyzed successfully, KErrNotSupported if the codec does not support analysis, or a
		system-wide error code.

@see CImageDecoder::Convert
*/
EXPORT_C void CImageDecoder::Prepare(TRequestStatus* aRequestStatus)
	{
	ASSERT(ValidProperties());
	ASSERT(aRequestStatus);

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvPrepare* prepare = cache->Prepare();

	if(!prepare)
		{
		MImageConvExtension* prepareExtension = NULL;
		TRAPD(err,
			{
			iRelay->GetExtensionL(KICLPrepareUid, prepareExtension);
			prepare = new (ELeave) TImageConvPrepare();
			prepare->SetExtension(prepareExtension);
			cache->SetPrepareExtension(prepare);
			});
		if ( err != KErrNone )
			{
			User::RequestComplete(aRequestStatus, err);
			return;
			}
		}
	prepare->Prepare(aRequestStatus);
	}

/**
@publishedAll
@released


Get the size of the decoded image for the given frame. The calculation will account for any clipping rectangle set,
scaling applied through the TImageConvScaler extension and any operation applied through TImageConvOperation.
If TImageConvScaler::SetScalingL(.. has been called then the size of the bitmap passed to CImageDecoder::Convert must match the size returned from
this function.

@param aSize 
	   Returns the size of the decoded image.
@param aFrameNumber
	   The frame number.
	   
@return KErrArgument if an error in calculation is detected e.g. if clipping rectangle is outside of the overall frame boundary.
@return Other system wide errors.
*/
EXPORT_C TInt CImageDecoder::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	ASSERT(ValidProperties());
	return iRelay->GetDestinationSize(aSize, aFrameNumber);
	}

/**
@publishedAll
@released


Get the extension interface for operations on image. @see TImageConvOperation
@return Interface to image conversion operations (rotate/mirror over axis).
@leave KErrNotSupported if loaded codec plugin does not support this optional extension.
@leave Other system wide errors
*/
EXPORT_C TImageConvOperation* CImageEncoder::OperationL()
	{
	ASSERT(ValidProperties());

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvOperation* operation = cache->Operation();

	if(!operation)
		{
		MImageConvExtension* operationExtension = NULL;
		iRelay->GetExtensionL(KICLOperationUid, operationExtension);
		operation = new (ELeave) TImageConvOperation();
		operation->SetExtension(operationExtension);
		cache->SetOperationExtension(operation);
		}
	return operation;
	}

/**
@publishedAll
@released


Get the extension interface for block streaming on image. @see TImageConvStreamedEncode
@return Interface to image conversion operations.
@leave KErrNotSupported if loaded codec plugin does not support this optional extension.
@leave Other system wide errors
*/
EXPORT_C TImageConvStreamedEncode* CImageEncoder::BlockStreamerL()
	{
	ASSERT(ValidProperties());

	CImageConvExtensionCache* cache = &iRelay->ExtensionCache();
	TImageConvStreamedEncode* streamer = cache->EncodeBlockStreamer();

	if(!streamer)
		{
		MImageConvExtension* streamExtension = NULL;
		iRelay->GetExtensionL(KICLStreamedEncodeUid, streamExtension);
		streamer = new (ELeave) TImageConvStreamedEncode();
		streamer->SetExtension(streamExtension);
		cache->SetBlockStreamerExtension(streamer);
		}
	return streamer;
	}



