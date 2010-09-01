
// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ImageConstruct.cpp - classes used during construction of decoders/encoders
// 
//

#include "icl/ImageConstruct.h"

#include "ImageConversion.h"
#include "ImageConversionPriv.h"
#include "ImageClientMain.h"

#include <ecom/ecom.h>

// CImageDecodeConstruct

/**
Default constructor
*/
EXPORT_C CImageDecodeConstruct::CImageDecodeConstruct()
	{
	}

/**
Destructor
*/
EXPORT_C CImageDecodeConstruct::~CImageDecodeConstruct()
	{
	// Tell ECom that we've deleted this instance of the implementation
	REComSession::DestroyedImplementation(iDtorIDKey);
	}

/**
Second stage constructor. This should be called during
construction of the derived class.
*/
EXPORT_C void CImageDecodeConstruct::ConstructL()
	{
	}

/**
Returns the decoder implementation UID.

@return	The implementation UID.
*/
EXPORT_C TUid CImageDecodeConstruct::ImplementationUid() const
	{
	TUid implementationUid = TUid::Null();
	TInt err = KErrNone;
	TRAP(err, implementationUid = REComSession::GetImplementationUidL(iDtorIDKey));
	return implementationUid;
	}

/**
Default version creates a basic CImageDecoder

@return A pointer to a fully constructed CImageDecoder
*/
EXPORT_C CImageDecoder* CImageDecodeConstruct::NewDecoderL() const
	{
	return CImageDecoder::NewL();
	}

/**
Requests a threaded decode. 

The default version of the virtual function returns EFalse, that is, it does not request a threaded decode.

@return ETrue a threaded decode is requested, otherwise, EFalse.
*/
EXPORT_C TBool CImageDecodeConstruct::RequestThread() const
	{
	return EFalse;
	}

/**
 * Intended for future proofing - will panic if called
 * @internalTechnology
 * @panic	"EReservedCall"
 */
EXPORT_C void CImageDecodeConstruct::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
 * Intended for future proofing - will panic if called
 * @internalTechnology
 * @panic	"EReservedCall"
 */
EXPORT_C void CImageDecodeConstruct::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
 * Intended for future proofing - will panic if called
 * @internalTechnology
 * @panic	"EReservedCall"
 */
EXPORT_C void CImageDecodeConstruct::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
 * Intended for future proofing - will panic if called
 * @internalTechnology
 * @panic	"EReservedCall"
 */
EXPORT_C void CImageDecodeConstruct::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

// CImageEncodeConstruct

/**
Default constructor
*/
EXPORT_C CImageEncodeConstruct::CImageEncodeConstruct()
	{
	}

/**
Destructor
*/
EXPORT_C CImageEncodeConstruct::~CImageEncodeConstruct()
	{
	// Tell ECom that we've deleted this instance of the implementation
	REComSession::DestroyedImplementation(iDtorIDKey);
	}

/**
Second stage constructor. This should be called during
construction of the derived class.
*/
EXPORT_C void CImageEncodeConstruct::ConstructL()
	{
	}

/**
Retrieve the encoder implementation UID.

@return	The implementation UID.
*/
EXPORT_C TUid CImageEncodeConstruct::ImplementationUid() const
	{
	TUid implementationUid = TUid::Null();
	TInt err = KErrNone;
	TRAP(err, implementationUid = REComSession::GetImplementationUidL(iDtorIDKey));
	return implementationUid;
	}

/**
Default version creates a basic CImageEncoder.

@return	Pointer to a fully constructed CImageEncoder object.
*/
EXPORT_C CImageEncoder* CImageEncodeConstruct::NewEncoderL() const
	{
	return CImageEncoder::NewL();
	}

/**
Requests a threaded encode.

Note:
The default version of this virtual function returns EFalse and does
not request a threaded encode.
*/
EXPORT_C TBool CImageEncodeConstruct::RequestThread() const
	{
	return EFalse;
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageEncodeConstruct::ReservedVirtual1()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageEncodeConstruct::ReservedVirtual2()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageEncodeConstruct::ReservedVirtual3()
	{
	Panic(EReservedCall);
	}

/**
@internalTechnology

Intended for future proofing - will panic if called

@panic  EReservedCall
*/
EXPORT_C void CImageEncodeConstruct::ReservedVirtual4()
	{
	Panic(EReservedCall);
	}

