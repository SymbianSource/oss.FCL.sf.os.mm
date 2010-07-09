// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32mem.h>
#include "ImageResolverAPI.h"
#include "../inc/icl/ICL_PropertyUIDS.h"

const TInt KMaxDescriptorLength = 128;	// The biggest descriptor that will be
										// built when internalizing the object

/*
 *
 * Default constructor
 *
 */
CCustomMatchData::CCustomMatchData()
	{
	}

/*
 *
 * Default destructor
 *
 */
CCustomMatchData::~CCustomMatchData()
	{
	delete iMatchString;
	delete iFileSuffix;
	iMatchReqUidsArray.Close();
	iOptionsUidsArray.Close();
	}

/*
 *
 * Standard, safe constructor that leaves nothing on the cleanup stack
 *
 * @return "CCustomMatchData*"
 *         A pointer to the newly constructed match data object
 *
 */
CCustomMatchData* CCustomMatchData::NewL()
	{
	CCustomMatchData* self = CCustomMatchData::NewLC();
	CleanupStack::Pop(self); 
	return self;
	}
/*
 *
 * Standard, safe constructor that leaves nothing on the cleanup stack
 *
 * @param  "const TDesC8& aPackage"
 *		   A reference to a descriptor created using <code>PackL()</code>
 *         used to initialize the object.
 * @return "CCustomMatchData*"
 *         A pointer to the newly constructed match data object
 *
 */
CCustomMatchData* CCustomMatchData::NewL(const TDesC8& aPackage)
	{
	CCustomMatchData* self = CCustomMatchData::NewLC(aPackage);
	CleanupStack::Pop(self);
	return self;
	}

/*
 *
 * Standard, safe constructor that leaves the pointer to the newly constructed
 * match data object on the cleanup stack
 *
 * @return	"CCustomMatchData*"
 *			A pointer to the newly constructed match data object
 *
 */
CCustomMatchData* CCustomMatchData::NewLC()
	{
	CCustomMatchData* self = new(ELeave) CCustomMatchData;
	CleanupStack::PushL(self);
    return self;
	}

/*
 *
 * Standard, safe constructor that leaves the pointer to the newly constructed
 * match data object on the cleanup stack
 *
 * @param	"const TDesC8& aPackage"
 *			A reference to a descriptor created using <code>PackL()</code>
 *			used to initialize the object.
 * @return	"CCustomMatchData*"
 *			A pointer to the newly constructed match data object
 *
 */
CCustomMatchData* CCustomMatchData::NewLC(const TDesC8& aPackage)
	{
	CCustomMatchData* self = new(ELeave) CCustomMatchData;
	CleanupStack::PushL(self);
	self->ConstructL(aPackage);
    return self;
	}

void CCustomMatchData::ConstructL(const TDesC8& aPackage)
	{
	UnPackL(aPackage);
	}

/*
 *
 * Externalize the object to a stream.
 * All the member variables will be written to the stream, and if no MatchString
 * is provided a null descriptor is used. The stream must be able to accomodate 
 * the streaming operation or it will leave with <code>KErrOverFlow</code>.
 *
 * @param	"RWriteStream& aStream"
 *          A reference to the stream to which the member variables will
 *          be written.
 *
 */
void CCustomMatchData::ExternalizeL(RWriteStream& aStream) const
	{
	aStream.WriteInt32L(iMatchType);
	aStream.WriteInt32L(iBaseType.iUid);
	aStream.WriteInt32L(iSubType.iUid);
	aStream.WriteInt32L(iImplementationType.iUid);
	aStream.WriteInt32L(iMatchReqUidsArray.Count());

	for (TInt index = 0; index < iMatchReqUidsArray.Count(); index++)
		{
		aStream.WriteInt32L((iMatchReqUidsArray[index]).iUid);
		}
	
	// if Match String is not used send empty descriptor
	if (iMatchString)
		aStream << *iMatchString;
	else
		aStream << KNullDesC8;

	// if File Suffix is not used send empty descriptor
	if (iFileSuffix)
		aStream << *iFileSuffix;
	else
		aStream << KNullDesC8;
	
	aStream.WriteInt32L(iExtensionOptions);

	aStream.WriteInt32L(iOptionsUidsArray.Count());
	
	for (TInt index = 0; index < iOptionsUidsArray.Count(); index++)
		{
		aStream.WriteInt32L((iOptionsUidsArray[index]).iUid);
		}

	aStream.WriteInt32L(iOptions);
	}
/*
 *
 * Internalize the object from a stream.
 * All the member variables will be read from the streamed.
 * If there is not enougth data in the stream or it will 
 * leave with <code>KErrEof</code>.
 *
 * @param	"RReadStream& aStream"
 *          A reference to the stream from which data will be read to
 *          setup the member variables.
 *
 */
void CCustomMatchData::InternalizeL(RReadStream& aStream)
	{
	iMatchType      = STATIC_CAST(TResolverMatchType, aStream.ReadInt32L());
	iBaseType.iUid  = aStream.ReadInt32L();
	iSubType.iUid   = aStream.ReadInt32L();
	iImplementationType.iUid = aStream.ReadInt32L();
	
	TInt matchReqUidsCount = aStream.ReadInt32L();
	if (matchReqUidsCount > 0)
		{
		iMatchReqUidsArray.Reset();
		for (TInt index = 0; index < matchReqUidsCount; index++)
			{			
			User::LeaveIfError(iMatchReqUidsArray.Append(TUid::Uid(aStream.ReadInt32L())));
			}
		}

	delete iMatchString; iMatchString = NULL;
	iMatchString = HBufC8::NewL(aStream, KMaxDescriptorLength); //KErrOverflow

	delete iFileSuffix; iFileSuffix = NULL;
	iFileSuffix = HBufC::NewL(aStream, KMaxDescriptorLength); //KErrOverflow
	
	iExtensionOptions = aStream.ReadInt32L();

	TInt optionsUidsCount = aStream.ReadInt32L();
	if (optionsUidsCount > 0)
		{
		iOptionsUidsArray.Reset();
		for (TInt index = 0; index < optionsUidsCount; index++)
			{			
			User::LeaveIfError(iOptionsUidsArray.Append(TUid::Uid(aStream.ReadInt32L())));
			}
		}

	iOptions = aStream.ReadInt32L();
	}

/*
 *
 * Set a string to match against.
 *
 * @param	"const TDesC8& aMIMEType"
 *          A reference to a descriptor to match against.
 *
 */
void CCustomMatchData::SetMatchStringL(const TDesC8& aMIMEType)
	{
	delete iMatchString; iMatchString = NULL;
	iMatchString = aMIMEType.AllocL();
	}

/*
 *
 * Sets the match type, base type and sub type to match against
 *
 * @param	"const TResolverMatchType& aMatchType"
 *			The type of match requested.
 * @param	"const TUid& aBaseType"
 *			The base type to match against
 * @param	"const TUid& aSubType"
 *			The sub type to match against
 *
 */
void CCustomMatchData::SetTypes(const TResolverMatchType& aMatchType, const TUid& aBaseType, const TUid& aSubType)
	{
	iMatchType = aMatchType;
	iBaseType  = aBaseType;
	iSubType   = aSubType;
	}

/*
 *
 * Sets the match type to match against
 *
 * @param	"const TResolverMatchType& aMatchType"
 *			The type of match requested.
 *
 */
void CCustomMatchData::SetMatchType(const TResolverMatchType& aMatchType)
	{
	iMatchType = aMatchType;
	}

/*
 *
 * Sets the base type to match against
 *
 * @param	"const TUid& aBaseType"
 *			The base type to match against
 *
 */
void CCustomMatchData::SetBaseType(const TUid& aBaseType)
	{
	iBaseType  = aBaseType;
	}

/*
 *
 * Sets the sub type to match against
 *
 * @param	"const TUid& aSubType"
 *			The sub type to match against
 *
 */
void CCustomMatchData::SetSubType(const TUid& aSubType)
	{
	iSubType   = aSubType;
	}

/*
 *
 * Sets the implementation type to match against
 *
 * @param	"const TUid& aImplementationType"
 *			The implementation uid of a specific codec to match against
 *
 */
void CCustomMatchData::SetImplementationType(const TUid& aImplementationType)
	{
	iImplementationType = aImplementationType;
	}
/*
 *
 * Set a file extension to match against.
 *
 * @param	"const TDesC8& aFileSuffix"
 *          A reference to a file suffix to match against.
 *
 */
void CCustomMatchData::SetFileSuffixL(const TDesC& aFileSuffix)
	{
	delete iFileSuffix; iFileSuffix = NULL;
	iFileSuffix = aFileSuffix.AllocL();
	}

/*
 *
 * Retrieves the match type, base type and sub type to match against
 *
 * @param	"TResolverMatchType& aMatchType"
 *			The type of match requested.
 * @param	"TUid& aBaseType"
 *			The base type to match against
 * @param	"TUid& aSubType"
 *			The sub type to match against
 *
 */
void CCustomMatchData::GetTypes(TResolverMatchType& aMatchType, TUid& aBaseType, TUid& aSubType) const
	{
	aMatchType = iMatchType;
	aBaseType  = iBaseType;
	aSubType   = iSubType;
	}
/*
 *
 * Retrieves the match type to match against
 *
 * @return	"TResolverMatchType"
 *			The type of match requested.
 *
 */
TResolverMatchType CCustomMatchData::MatchType() const
	{
	return iMatchType;
	}

/*
 *
 * Retrieves the base type to match against
 *
 * @return	"TUid"
 *			The base type to match against
 *
 */
TUid CCustomMatchData::BaseType() const
	{
	return iBaseType;
	}

/*
 *
 * Retrieves the sub type to match against
 *
 * @return	"TUid"
 *			The sub type to match against
 *
 */
TUid CCustomMatchData::SubType() const
	{
	return iSubType;
	}
/*
 *
 * Retrieves the implementation type to match against
 *
 * @return	"TUid"
 *			The implementation of a specific codec to match against
 *
 */

TUid CCustomMatchData::ImplementationType() const
	{
	return iImplementationType;
	}

/*
 *
 * Retrieves the string to match against. If no string is set
 * a null descriptor is returned.
 *
 * @return	"TPtrC8"
 *			The string to match against
 *
 */
const TPtrC8 CCustomMatchData::MatchString() const
	{
	TPtrC8 result;

	if(iMatchString)
		result.Set(*iMatchString);
	else
		result.Set(KNullDesC8);

	return result;
	}

/*
 *
 * Retrieves the file extension to match against. If no suffix is set
 * a null descriptor is returned.
 *
 * @return	"TPtrC8"
 *			The file suffix to match against
 *
 */
const TPtrC CCustomMatchData::FileSuffix() const
	{
	TPtrC result;

	if(iFileSuffix)
		result.Set(*iFileSuffix);
	else
		result.Set(KNullDesC);

	return result;
	}

/* Used to package up the object into a descriptor so
 * that it can be placed in the <code>DataType</code> descriptor of a
 * <code>TEComResolverParams</code> before being passed across the client
 * server boundary within ECom and handed to the custom resolver.
 *
 * @return	"const HBufC8*"
 *			A pointer to a desctriptor with the object packed in it.
 *			The object does not take responsibility for the pointer
 *			and the external code must ensure that the pointer is deleted.
 *			The pointer is left on the cleanupstack.	
 *
 */
HBufC8* CCustomMatchData::NewPackLC() const
	{
	//Calculate the size necessary size for the descriptor to pack the object
	TInt size = 0;
	if(iMatchString)
		size = iMatchString->Size();
	if(iFileSuffix)
		size += iFileSuffix->Size();
	
	size += iMatchReqUidsArray.Count() * sizeof(TUid);
	size += iOptionsUidsArray.Count() * sizeof(TUid);
		
	size += 9 * sizeof(TInt32);	// The size necessary for 
								//		iMatchType
								//		iBaseType
								//		iSubType
								//		iInterfaceUid
								//		iMatchReqUidsArray.Count()
								// and the size of the descriptor (Max TInt32)
								//		iExtensionOptions
								//		iOptionsUidsArray.Count()
								//		iOptions
	HBufC8* package = HBufC8::NewLC(size);
	TPtr8   packageDes = package->Des();

	RDesWriteStream stream(packageDes);
	ExternalizeL(stream);
	stream.Close();

	return package;
	}

/* Used to unpack a descriptor packed using <code>PackL()</code> and set the
 * member variables to the data contained in the descriptor. The descriptor 
 * is passed to the custom resolver by ECom and can be retrieved from the <code>DataType</code> 
 * of <code>TEComResolverParams</code>.
 *
 * @param	"const TDes8& aPackage"
 *			A reference to a desctriptor created using <code>PackL()</code> to which
 *			the member variables will be set.
 *
 */
void CCustomMatchData::UnPackL(const TDesC8& aPackage)
	{
	RDesReadStream stream(aPackage);
	InternalizeL(stream);
	stream.Close();
	}

/* Sets the array of required uids by copying all the elements of the array in parameter,
 * so the custom match data doesn't take ownership of the array specified.
 *
 * @param	"const RUidDataArray& aMatchReqUidsArray"
 *			The new required uids array.
 *
 */
void CCustomMatchData::SetMatchReqUidsL(const RUidDataArray& aMatchReqUidsArray)
	{
	iMatchReqUidsArray.Reset();
	for(TInt index = 0 ; index < aMatchReqUidsArray.Count() ; index++)
		{
		User::LeaveIfError(iMatchReqUidsArray.Append(aMatchReqUidsArray[index]));
		}
	}

/* Gets the array of required uids by copying all the elements of the member data array
 * in the array in parameter.
 *
 * @param	"RUidDataArray& aMatchReqUidsArray"
 *			The new required uids array to copy data to.
 *
 */
void CCustomMatchData::GetMatchReqUidsL(RUidDataArray& aMatchReqUidsArray)
	{
	aMatchReqUidsArray.Reset();
	for(TInt index = 0 ; index < iMatchReqUidsArray.Count() ; index++)
		{
		User::LeaveIfError(aMatchReqUidsArray.Append(iMatchReqUidsArray[index]));
		}
	}

/* Gets the array of required options uids by copying all the elements of the member data array
 * in the array in parameter.
 *
 * @param	"RUidDataArray& aOptionsUidsArray"
 *			The new required options uids array to copy data to.
 *
 */
void CCustomMatchData::GetOptionsUidsL(RUidDataArray& aOptionsUidsArray)
	{
	aOptionsUidsArray.Reset();
	for(TInt index = 0 ; index < iOptionsUidsArray.Count() ; index++)
		{
		User::LeaveIfError(aOptionsUidsArray.Append(iOptionsUidsArray[index]));
		}
	}

/*
 * Request that plugins support all extension interfaces interoperably.
 * 
 * @param TInt aExtensionBits
 *        Set of extensions (@see CImageDecoder::TOptions or @see CImageEncoder::TOptions)
 */
void CCustomMatchData::SetExtensionOptions(TUint aExtensionBits)
	{
	iExtensionOptions = (aExtensionBits & KIclExtensionOptionsMask);
	}

/*
 * Request that plugins support specific options.
 * 
 * @param TInt aOptions
 *        Set of CImageDecoder options (@see CImageDecoder::TOptions)
 */
void CCustomMatchData::SetOptions(CImageDecoder::TOptions aOptions)
	{
	if ((aOptions & CImageDecoder::EOptionAutoRotate) != 0) 
		{
		iOptionsUidsArray.Append(KUidJPGAutoRotateSupport);
		}
	if((aOptions & CImageDecoder::EOptionOptimisedPartialImageDecoding) != 0)
	    {
        iOptionsUidsArray.Append(KUidJPGOptimisedPartialDecodingSupport);
	    }
	iOptions = aOptions;
	}
	
/*
* Get list of all options required 
*
* @return TUint
*         Set of options to match (@see CImagDecoder::TOptions)
*/
TUint CCustomMatchData::Options() 
	{
	return iOptions;
	}

/*
 * Get list of all extension interfaces required.
 * 
 * @return TInt
 *         Set of extensions to match (@see CImageDecoder::TOptions or @see CImageEncoder::TOptions)
 * 
 */
TUint CCustomMatchData::ExtensionOptions()
	{
	return iExtensionOptions;
	}
