// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef EXIFTRANSFORMDATAACCESSOR_H
#define EXIFTRANSFORMDATAACCESSOR_H

/**
   @file
   @internalComponent
*/

#include <e32base.h>
#include <icl/imagecodec.h>
#include <imageconversion.h>

class CExifTransform;

/**
	Base class representing the source from where ExifEditUtility
	should read its input.  

	Encapsulates either a file or a descriptor.
*/
class MExifSource
	{
public:
	/**
	Does initialisation specific to the destination type.
	*/
	virtual void InitL() = 0;
	
	/**
	Sets the read position in the source.
	
	@param  aPosition
	 	The desired read position
	*/
	virtual TInt SetReadPosition(TInt aPosition) = 0;
	  
	/**
	Reads a TUint16 value from the source at the current position.
	
	@return
		The read TUint16
	*/
	virtual TUint16 ReadUint16L() const = 0;	
	
	/**
	Reads the Exif metadata block of the specified length from the source, 
	starting at the specified position, and converts it into a new CExifData
	object to be owned by the caller.
	
	It is expected that aPos is correct for non-zero lengths.
	
	@param  aPos
		The position in the source where the start of the Exif block is located.
	@param  aLength
		The length of the Exif metadata block. If source has no Exif metadata then use a length of zero.
		
	@ returns
		A new CExifData* to be owned by the caller.
	*/
	virtual CExifTransform* ReadAndConvertExifDataL(TUint aPos, TUint aLength) const = 0;

	/**
	Reads aLength bytes of data from the source, starting at position aPos, into the
	the object's HBufC8. Updates aDes to point to this HBufC8.  Ownership
	of the HBufC8 is not transferred.
	
	@param  aPos
		The position in the source where the data is to be read.
	@param  aDes
		Updated to point to memory (not owned) containing the read data.
	@param  aLength
		The number of bytes to read.
	@param aStatus
		Active object status.  Updated.
	*/
	virtual void ReadL(TUint aPos,TBufPtr8& aDes,TInt aLength, TRequestStatus& aStatus) const = 0;
	
	/**
	Reads aLength bytes of data from the source into the HBufC8 to which aDes currently points.
	The source is read from its current position.
	
	ReadL must have been called at some point beforehand to read the first block
	and set the position correctly.
	
	@param  aDes
		Updated to point to memory (not owned) containing the read data.
	@param  aLength
		The number of bytes to read.
	@param aStatus
		Active object status.  Updated.
	*/
	virtual void NextReadL(TBufPtr8& aDes,TInt aLength,TRequestStatus& aStatus) const = 0;
	
	/**
	Updates aSize with the size of the source data.
	
	@param  aSize
		Updated with the size of the source data.
		
	@return error codes
		KErrNone on success.
	*/
	virtual TInt Size(TInt& aSize) const = 0;
	
	/**
	Creates a new CImageDecoder object based on the source type and returns it to
	the caller.  It will be owned by the caller.
	
	@return
		New CImageDecoder* which the caller will own.
	*/
	virtual CImageDecoder* CreateImageDecoderL(CImageDecoder::TOptions aDecoderOptions) const = 0;
	
	/**
	Called by CExifEditUtility::DoCancel() when a transform is cancelled.
	
	@return error codes
		KErrNone on success.
	*/
	virtual TInt Cancel() = 0;
	
	/**
	Called from ExifEditUtility::RunError before EReadComplete state reached.
	*/
	virtual void CleanupAfterEarlyError() = 0;
	
	/**
	Called from ExifEditUtility::RunError after EReadComplete state reached.
	*/
	virtual void CleanupAfterLateError() = 0;
	
	/**
	Called to destroy the object when one only has a pointer to the abstract class.
	*/
	virtual void Destroy() = 0;
	};
	

	
/**
	Base class representing the destination to which ExifEditUtility
	should write its output.  

	Encapsulates either a file or a descriptor.
*/
class MExifDest
	{
public:
	/**
	Does initialisation specific to the destination type.
	
	@return error codes
		KErrNone if successful.
	*/
	virtual TInt Init() = 0;
	
	/**
	Writes aLength bytes of aDes to the destination & updates aStatus.
	
	@param  aDes
		Source descriptor containing data to be written to the destination.
	@param  aLength
		Number of bytes to of aDes to write to the destination.
	@param aStatus
		Active object status.  Updated.
	*/
	virtual void WriteL(const TDesC8& aDes,TInt aLength,TRequestStatus& aStatus) = 0;
	
	/**
	Writes the internal buffer owned by the object to the destination.
	
	@param  aStatus
		Active object status.  Updated.
	*/
	virtual void WriteDestBufferL(TRequestStatus& aStatus) = 0;
	
	/**
	WriteComplete finalises the destination after all data has been written to it.
	It is called by ExifEditUtility prior to ExifEditUtility becoming inactive.
	*/
	virtual void WriteComplete() = 0;
	
	/**
	Transfers ownership of aBuffer to this object.
	
	@param  aBuffer
		HBufC8* which this object will take ownership of.
	*/
	virtual void SetDestBuffer(HBufC8* aBuffer) = 0;
	
	/**
	Called when transform is cancelled to clean up this object.
	
	@return error codes
		KErrNone if successful.
	*/
	virtual TInt Cancel() = 0;
	
	/**
	Called from ExifEditUtility::RunError before EReadComplete state reached.
	*/
	virtual void CleanupAfterEarlyError() = 0;
	
	/**
	Called from ExifEditUtility::RunError after EReadComplete state reached.
	*/
	virtual void CleanupAfterLateError() = 0;
	
	/**
	Called to destroy the object when one only has a pointer to the abstract class.
	*/
	virtual void Destroy() = 0;
	};	

	
#endif //EXIFTRANSFORMDATAACCESSOR_H
