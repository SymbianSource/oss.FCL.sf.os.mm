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
//

#include <s32mem.h>
#include "ImageTransformResolverAPI.h"

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
	aStream.WriteInt32L(iPluginFlagsNeeded);
	aStream.WriteInt32L(iBaseType.iUid);
	aStream.WriteInt32L(iSubType.iUid);
	aStream.WriteInt32L(iImplementationType.iUid);

	// if Match String is not used send empty descriptor
	if(iMatchString)
		aStream << *iMatchString;
	else
		aStream << KNullDesC8;

	// if File Suffix is not used send empty descriptor
	if(iFileSuffix)
		aStream << *iFileSuffix;
	else
		aStream << KNullDesC8;
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
	iMatchType      = static_cast<TResolverMatchType>(aStream.ReadInt32L());
	iPluginFlagsNeeded      = static_cast<TPluginFlagsNeeded>(aStream.ReadInt32L());
	iBaseType.iUid  = aStream.ReadInt32L();
	iSubType.iUid   = aStream.ReadInt32L();
	iImplementationType.iUid = aStream.ReadInt32L();

	delete iMatchString; iMatchString = NULL;
	iMatchString = HBufC8::NewL(aStream,KMaxDescriptorLength); //KErrOverflow

	delete iFileSuffix; iFileSuffix = NULL;
	iFileSuffix = HBufC::NewL(aStream,KMaxDescriptorLength); //KErrOverflow
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

/* set the flags that the plugin must support
 */
void CCustomMatchData::SetPluginFlagsNeeded(const TUint aPluginFlags)
	{
	iPluginFlagsNeeded = aPluginFlags;
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
 * Retrieves the plugin flags that are needed
 *
 * @return	"TResolverMatchType"
 *			The type of match requested.
 *
 */
TUint CCustomMatchData::PluginFlagsNeeded() const
	{
	return iPluginFlagsNeeded;
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

	size += 6*sizeof(TInt32);	// The size necessary for 
								//		iMatchType
								//		iPluginFlagsNeeded
								//		iBaseType
								//		iSubType
								//		iInterfaceUid
								// and the size of the descriptor (Max TInt32)
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
