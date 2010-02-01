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

/**
 @file
 @publishedAll
 @released
*/

#include <imageframe.h>
#include "imageframebody.h"
#include "ImageClientMain.h"

/**
Constructor for the TFrameFormatBase class.

@param aType
	   The UID used to identify a specific format class.  
	   It is used as a runtime identification of the particular subclass.
*/
EXPORT_C TFrameFormatBase::TFrameFormatBase(TUid aType): iType(aType)
	{	
	}

/**
The type of the TFrameFormatBase subclass.

@return The UID used to identify a specific subclass.
*/	
EXPORT_C TUid TFrameFormatBase::Type() const
	{
	return iType;
	}

// for future development
EXPORT_C void TFrameFormatBase::Reserved1()
	{
	Panic(EReservedCall);
	}

// for future development
EXPORT_C void TFrameFormatBase::Reserved2()
	{
	Panic(EReservedCall);
	}	

// for future development
EXPORT_C void TFrameFormatBase::Reserved3()
	{
	Panic(EReservedCall);
	}	
	
// for future development
EXPORT_C void TFrameFormatBase::Reserved4()
	{
	Panic(EReservedCall);
	}	

/**
Constructor for the TFrameFormat class. 

@param aFormatCode
	   Unique UID specifying an image format code. The list of recognised formats is stored in file imageframeformats.hrh. 

@see imageframeformats.hrh
@note If the format code is not recognised, the format code, colour space and sampling are initialised as KNullUid.	    
*/
EXPORT_C TFrameFormat::TFrameFormat(TUid aFormatCode):TFrameFormatBase(KUidIclImageFrameFormat), iFormatCode(aFormatCode)
	{
	switch(aFormatCode.iUid)
		{
		case KFormatYUVMonochromeUidValue:
			{
			iColourSpace 	= KUidColourSpaceYCbCr;
			iSampling  		= KUidSamplingMonochrome;
			break;
			}
		case KFormatYUV422InterleavedUidValue:
		case KFormatYUV422InterleavedReversedUidValue:			
		case KFormatYYUV422InterleavedUidValue:			
		case KFormatYUV422PlanarUidValue:			
			{
			iColourSpace 	= KUidColourSpaceYCbCr;
			iSampling  		= KUidSamplingColor422;
			break;
			}	
		case KFormatYUV420PlanarUidValue:
		case KFormatYUV420PlanarReversedUidValue:
		case KFormatYUV420SemiPlanarUidValue:
		case KFormatYUV420InterleavedUidValue:
			{
			iColourSpace 	= KUidColourSpaceYCbCr;
			iSampling  		= KUidSamplingColor420;
			break;
			}	
		case KFormat16bitRGB444InterleavedUidValue:
		case KFormat16BitRGB565InterleavedUidValue:
		case KFormat32BitRGB888InterleavedUidValue:
			{
			iColourSpace 	= KUidColourSpaceRGB;
			iSampling  		= KUidSamplingColor444;
			break;
			}
		case KFormatYUV444InterleavedUidValue:
		case KFormatYUV444PlanarUidValue:
			{
			iColourSpace 	= KUidColourSpaceYCbCr;
			iSampling 		= KUidSamplingColor444;
			break;
			}		
		default:
			{
			iFormatCode 	= KNullUid;
			iColourSpace 	= KNullUid;
			iSampling  		= KNullUid;
			}
		}
	}

/** 
Creates an identical object to itself and places it on the heap.

@return The pointer to the newly created object cast as its base class TFrameFormatBase.

@leave KErrNoMemory.
*/ 	
EXPORT_C TFrameFormatBase* TFrameFormat::DuplicateL() const
	{
	TFrameFormatBase* copy = new (ELeave) TFrameFormat(iFormatCode);
	return copy;
	}

/** 
Returns the colour space for the TFrameFormat object.
@note File imageframeconst.h contains the recognised colour space codes. 
These are named KUidColourSpaceXXX.

@return The UID of the colour space.

@see imageframeconst.h 
*/ 	
EXPORT_C TUid TFrameFormat::ColourSpace() const
	{
	return iColourSpace;
	}
	
/** 
Returns the sampling format for the TFrameFormat object.
@note File imageframeconst.h contains the recognised sampling codes. 
These are named KUidSamplingColorXXX.

@return The UID of the sampling format.

@see imageframeconst.h 
*/ 	
EXPORT_C TUid TFrameFormat::Sampling() const
	{
	return iSampling;
	}
	
/** 
Returns the format code which uniquely identifies the TFrameFormat object.
@note File imageframeconst.h contains the recognised format codes. 
These are named KUidFormatXXX.

@return The UID of the format code.

@see imageframeconst.h
*/ 	
EXPORT_C TUid TFrameFormat::FormatCode() const
	{
	return iFormatCode;
	}
	
/** 
Sets a new colour space.
@note File imageframeconst.h contains the recognised colour space codes. 
These are named KUidColourSpaceXXX.

@param aColourSpace
		The UID of the new colour space.
		
@see imageframeconst.h		
*/	
EXPORT_C void TFrameFormat::SetColourSpace(TUid aColourSpace)
	{
	iColourSpace = aColourSpace;
	}
	
/**
Constructor for the TFrameLayoutBase class.

@param aType
	   The UID used to identify a specific layout class.  
	   It is used as a runtime identification of the particular subclass.
*/
EXPORT_C TFrameLayoutBase::TFrameLayoutBase(TUid aType): iType(aType)
	{
	}
	
/**
The type of the TFrameLayoutBase subclass. 

@return The UID used to identify a specific subclass.
*/	
EXPORT_C TUid TFrameLayoutBase::Type() const
	{
	return iType;
	}
	
// for future development
EXPORT_C void TFrameLayoutBase::Reserved1()
	{
	Panic(EReservedCall);
	}
	
// for future development
EXPORT_C void TFrameLayoutBase::Reserved2()
	{
	Panic(EReservedCall);
	}	

// for future development
EXPORT_C void TFrameLayoutBase::Reserved3()
	{
	Panic(EReservedCall);
	}	
	
// for future development 
EXPORT_C void TFrameLayoutBase::Reserved4()
	{
	Panic(EReservedCall);
	}	
		
/**
Constructor for the TFrameLayout class.

@param aPlanes
	   The number of planes into which the image data is organised. 
	   It should be a positive integer less than or equal to KMaxPlanesInFrame 
	   (defined in imageframeconst.h).
	   
@panic EInvalidValue if the value of aPlanes is negative or greater than KMaxPlanesInFrame.
@see imageframeconst.h
*/
EXPORT_C TFrameLayout::TFrameLayout(TInt aPlanes) : TFrameLayoutBase(KUidIclImageFrameLayout), iPlanes(aPlanes)
	{
	__ASSERT_ALWAYS((iPlanes >= 0) && (iPlanes <= KMaxPlanesInFrame), Panic(EInvalidValue));
	}

/** 
Creates an identical object to itself and places it on the heap.

@return The pointer to the newly created object base class.

@leave KErrNoMemory.
*/ 	
EXPORT_C TFrameLayoutBase* TFrameLayout::DuplicateL() const
	{
	TFrameLayoutBase* copy =  new  (ELeave) TFrameLayout(iPlanes);
	
	CleanupStack::PushL(copy);
	// copy the contents of this one to the new instance
	Mem::Copy(copy, this, sizeof(TFrameLayout));
	CleanupStack::Pop();

	return copy;
	}

/**
Returns the number of planes in the current layout.

@return The number of planes.
*/
EXPORT_C TInt TFrameLayout::Planes() const
	{
	return iPlanes;
	}
	
/**
Returns the start of a plane identified by its index. The value is the byte offset from the start 
of the CImageFrame object memory.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@return The start position of the plane as a byte offset. 
@panic  EInvalidIndex if the index is not in the allowed range.       
*/
EXPORT_C TInt TFrameLayout::Start(TInt aIndex) const
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	return iStart[aIndex];
	}
	
/**
Returns the maximum length in bytes of a specific plane.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@return The maximum length of the plane in bytes.   
@panic  EInvalidIndex if the index is not in the allowed range.            
*/
EXPORT_C TInt TFrameLayout::Length(TInt aIndex) const
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	return iLength[aIndex];
	}

/**
Returns the current length in bytes of a specific plane.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@return The current length of the plane in bytes.  
@panic  EInvalidIndex if the index is not in the allowed range.         
*/
EXPORT_C TInt TFrameLayout::CurrentLength(TInt aIndex) const
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	return iCurrentLength[aIndex];
	}

/**
Returns the scan length in bytes of a specific plane.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@return The scan length of the plane in bytes.   
@panic  EInvalidIndex if the index is not in the allowed range.        
*/
EXPORT_C TInt TFrameLayout::ScanLength(TInt aIndex)const
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	return iScanLength[aIndex];
	}

/**
Sets the start offset for a specific plane in the current layout.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@param aStart
       The new start offset position in bytes.  

@panic EInvalidIndex if the index is not in the allowed range.        
@panic EInvalidValue if the value of aStart is negative.                 
*/	
EXPORT_C void TFrameLayout::SetStart(TInt aIndex, TInt aStart)
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	__ASSERT_ALWAYS((aStart >= 0), Panic(EInvalidValue));
	iStart[aIndex] = aStart;
	}

/**
Sets the maximum length of a specific plane in the current layout.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@param aLength
       The new length in bytes. 

@panic EInvalidIndex if the index is not in the allowed range.        
@panic EInvalidValue if the value of aLength is negative.                          
*/
EXPORT_C void TFrameLayout::SetLength(TInt aIndex, TInt aLength)
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	__ASSERT_ALWAYS((aLength >= 0), Panic(EInvalidValue));
	iLength[aIndex] = aLength;
	}

/**
Sets the current length for a specific plane in the current layout.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@param aCurrentLength
       The new value for current length in bytes
       .
@panic EInvalidIndex if the index is not in the allowed range.        
@panic EInvalidValue if the value of aCurrentLength is negative.                        
*/
EXPORT_C void TFrameLayout::SetCurrentLength(TInt aIndex, TInt aCurrentLength)
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	__ASSERT_ALWAYS((aCurrentLength >= 0), Panic(EInvalidValue));
	iCurrentLength[aIndex] = aCurrentLength;
	}
	
/**
Set a new value for the scan length for a specific plane in the current layout.

@param aIndex
       The index of the plane. The value is non negative and must be less than the value 
       returned by TFrameLayout::Planes().
       
@param aScanLength
       The new value for the scan length of the plane in bytes.  
       
@panic EInvalidIndex if the index is not in the allowed range.        
@panic EInvalidValue if the value of aScanLength is negative.                                   
*/
EXPORT_C void TFrameLayout::SetScanLength(TInt aIndex, TInt aScanLength)
	{
	__ASSERT_ALWAYS((aIndex >= 0) && (aIndex < iPlanes), Panic(EInvalidIndex));
	__ASSERT_ALWAYS((aScanLength >= 0), Panic(EInvalidValue));
	iScanLength[aIndex] = aScanLength;
	}

/**
Constructor for the CImageFrame class.
*/	
EXPORT_C CImageFrame::CImageFrame()						   
	{
	}

/**
Destructor for the CImageFrame class.
*/	
EXPORT_C CImageFrame::~CImageFrame()
	{
	delete iBody;
	}

/**
Returns the format of the CImageFrame object. 

@return A reference to a TFrameFormatBase object that describes the format of the CImageFrame object.
*/	    
EXPORT_C const TFrameFormatBase& CImageFrame::FrameFormat() const
	{
	return iBody->FrameFormat();
	}
		
/**
Sets the format of the CImageFrame object.

@param aFormat
	   A reference to a TFrameFormatBase object that is used to set the format of the CImageFrame.
	   
@leave KErrNoMemory
*/	
EXPORT_C void CImageFrame::SetFrameFormatL(const TFrameFormatBase& aFormat)
	{
	iBody->SetFrameFormatL(aFormat);
	}

/**
Returns the memory layout of the CImageFrame object.

@return A reference to a TFrameLayoutBase object that describes the memory layout of the CImageFrame object.
*/
EXPORT_C const TFrameLayoutBase& CImageFrame::FrameLayout() const
	{
	return iBody->FrameLayout();
	}

/**
Sets the layout of the CImageFrame object.

@param aFrameLayout
	   A reference to a TFrameLayoutBase object that is used to set the memory layout of the CImageFrame object.
	   
@leave KErrNoMemory
*/		
EXPORT_C void CImageFrame::SetFrameLayoutL(const TFrameLayoutBase& aFrameLayout)
	{
	iBody->SetFrameLayoutL(aFrameLayout);
	}

/**
Returns the size in pixels of the image the CImageFrame refers to.
@return The image size in pixels.
*/
EXPORT_C const TSize& CImageFrame::FrameSizeInPixels() const
	{
	return iBody->FrameSizeInPixels();
	}
	
/**
Sets the size in pixels of the image CImageFrame refers to.

@param aFrameSize
	   A reference to a TSize object used to set the image size (in pixels) for the CImageFrame object.
	   
@panic EInvalidValue if the framesize value is invalid.
*/	
EXPORT_C void CImageFrame::SetFrameSizeInPixels(const TSize& aFrameSize)
	{
	__ASSERT_ALWAYS((aFrameSize.iWidth >= 0) && (aFrameSize.iHeight >= 0), Panic(EInvalidValue));
	iBody->SetFrameSizeInPixels(aFrameSize);
	}

/**
Returns a reference to the chunk encapsulated by the CImageFrame object.

@note The function will panic if the object does not encapsulate an RChunk. Before calling this function 
call IsChunk() to check if the CImageFrame object encapsulates a chunk.
 
@return A reference to the chunk.
@panic EInvalidValue if the CImageFrame object does not encapsulate an RChunk.
*/	
EXPORT_C RChunk& CImageFrame::DataChunk()
	{
	return iBody->DataChunk();
	}

/**
Returns the offset at which the data for this frame starts. For CImageFrame 
objects encapsulating descriptors the offset is zero.
 
@return The offset at which the data for this frame starts in bytes.
*/
EXPORT_C TInt CImageFrame::DataOffset() const
	{
	return iBody->DataOffset();
	}
	
/**
Returns whether CImageFrame object encapsulates a chunk.

@return ETrue if the CImageFrame object uses a chunk, otherwise EFalse.
*/
EXPORT_C TBool CImageFrame::IsChunk() const
	{
	return iBody->IsChunk();
	}
	
/**
Provides a writeable TDes8 reference to the data buffer. The reference can be used for reading and writing the data.
 
@return A writeable TDes8 reference to the data buffer.
*/	
EXPORT_C TDes8& CImageFrame::Data()
	{
	return iBody->Data();
	}
	
/**
Provides a TDesC8 reference to the data buffer. The reference can only be used for reading the data. 
 
@return A TDesC8 reference to the data buffer for reading only.
*/	
EXPORT_C const TDesC8& CImageFrame::Data() const
	{
	return iBody->Data();
	}

/**
Returns the maximum space reserved for this frame. 

@return The maximum space in bytes reserved for this frame.       
*/
EXPORT_C TInt CImageFrame::MaxBufferSize() const 
	{
	return iBody->MaxBufferSize();
	}

/**
Factory function used to create a CImageFrame object encapsulating a descriptor.

@param aBuffer
       A reference to the TDes8 object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.       

@return A pointer to the newly created CImageFrame object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C  CImageFrame* CImageFrame::NewL(const TDes8& aBuffer, 
										 TInt aMaxBufferSize)
	{
	CImageFrame* self =  new(ELeave) CImageFrame(); 
	CleanupStack::PushL(self);
	self->ConstructL(aBuffer, aMaxBufferSize);
	CleanupStack::Pop();
	return self;	
	}

/**
Factory function used to create a CImageFrame object encapsulating a descriptor.

@param aBuffer
       A reference to the TDes8 object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.       

@param aFrameSize
       A reference to a TSize object that defines the frame size in pixels of the CImageFrame object.
       
@param aFrameFormat
       A reference to a TFrameFormatBase object that defines the format of the CImageFrame object.
       
@param aFrameLayout
       A reference to a TFrameLayoutBase object that defines the memory layout of the CImageFrame object.      

@return A pointer to the newly created CImageFrame object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C CImageFrame* CImageFrame::NewL(const TDes8& aBuffer, 
										TInt aMaxBufferSize,
					   					const TSize& aFrameSize, 
									 	const TFrameFormatBase& aFrameFormat,
										const TFrameLayoutBase& aFrameLayout)
	{
	CImageFrame* self =  new(ELeave) CImageFrame(); 
	CleanupStack::PushL(self);
	self->ConstructL(aBuffer, aMaxBufferSize, aFrameSize, aFrameFormat, aFrameLayout);
	CleanupStack::Pop();
	return self;
	}

/**
Factory function used to create a CImageFrame object encapsulating a chunk.

@param aBuffer
       A pointer to the RChunk object encapsulated by the CImageFrame object.

@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.        
       
@param aDataOffset
       The offset in bytes at which the data for this frame starts.       

@return A pointer to the newly created CImageFrame object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C CImageFrame* CImageFrame::NewL(const RChunk* aBuffer, 
										TInt aMaxBufferSize, 
										TInt aDataOffset)
	{
	CImageFrame* self = new(ELeave) CImageFrame(); 
	CleanupStack::PushL(self);
	self->ConstructL(aBuffer, aMaxBufferSize, aDataOffset);
	CleanupStack::Pop();
	return self;
	}
	
/**
Factory function used to create a CImageFrame object encapsulating a chunk.

@param aBuffer
       A pointer to the RChunk object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.         
       
@param aDataOffset
       The offset in bytes at which the data for this frame starts from the chunk start. 

@param aFrameSize
       A reference to a TSize object that defines the frame size in pixels of the CImageFrame object.
       
@param aFrameFormat
       A reference to a TFrameFormatBase object that defines the format of the CImageFrame object.
       
@param aFrameLayout
       A reference to a TFrameLayoutBase object that defines the memory layout of the CImageFrame object.    

@return A pointer to the newly created CImageFrame object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C CImageFrame* CImageFrame::NewL(const RChunk* aBuffer, 
										TInt aMaxBufferSize,
										TInt aDataOffset, 
										const TSize& aFrameSize,									    	
										const TFrameFormatBase& aFrameFormat,									
										const TFrameLayoutBase& aFrameLayout)
	{
	CImageFrame* self = new(ELeave) CImageFrame(); 
	CleanupStack::PushL(self);
	self->ConstructL(aBuffer, aMaxBufferSize, aDataOffset, aFrameSize, aFrameFormat, aFrameLayout);
	CleanupStack::Pop();
	return self;
	}

/** Function used to initialise a CImageFrame object encapsulating a chunk.

@param aBuffer
       A pointer to the RChunk object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.         
       
@param aDataOffset
       The offset in bytes at which the data for this frame starts from the chunk start. 

@param aFrameSize
       A reference to a TSize object that defines the frame size in pixels of the CImageFrame object.
       
@param aFrameFormat
       A reference to a TFrameFormatBase object that defines the format of the CImageFrame object.
       
@param aFrameLayout
       A reference to a TFrameLayoutBase object that defines the memory layout of the CImageFrame object.          

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C void CImageFrame::ConstructL(const	RChunk* aBuffer, 
							 TInt aMaxBufferSize,
							 TInt aDataOffset, 
						 	 const TSize& aFrameSize,
						 	 const TFrameFormatBase& aFrameFormat,									
						 	 const TFrameLayoutBase& aFrameLayout)
	{
	iBody = CBody::NewL(*this, aBuffer, aMaxBufferSize, aDataOffset, aFrameSize, aFrameFormat, aFrameLayout);
	}

/**
Function used to initialise a CImageFrame object encapsulating a chunk.

@param aBuffer
       A pointer to the RChunk object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.         
       
@param aDataOffset
       The offset in bytes at which the data for this frame starts from the chunk start.
       
@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C void CImageFrame::ConstructL(const	RChunk* aBuffer, 
							 	TInt aMaxBufferSize,
							 	TInt aDataOffset)
	{
	iBody = CBody::NewL(*this, aBuffer, aMaxBufferSize, aDataOffset);
	}


/**
Function used to initialise a CImageFrame object encapsulating a descriptor.

@param aBuffer
       A reference to the TDes8 object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.       

@param aFrameSize
       A reference to a TSize object that defines the frame size in pixels of the CImageFrame object.
       
@param aFrameFormat
       A reference to a TFrameFormatBase object that defines the format of the CImageFrame object.
       
@param aFrameLayout
       A reference to a TFrameLayoutBase object that defines the memory layout of the CImageFrame object.      

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C void CImageFrame::ConstructL(const TDes8& aBuffer, 
							 TInt aMaxBufferSize,
					   		 const TSize& aFrameSize, 
							 const TFrameFormatBase& aFrameFormat,
							 const TFrameLayoutBase& aFrameLayout)
	{
	iBody = CBody::NewL(*this, aBuffer, aMaxBufferSize, aFrameSize, aFrameFormat, aFrameLayout);
	}
	
/**
Function used to initialise a CImageFrame object encapsulating a descriptor.

@param aBuffer
       A reference to the TDes8 object encapsulated by the CImageFrame object.
       
@param aMaxBufferSize 
	   The maximum space in bytes reserved for this frame.       

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
EXPORT_C void CImageFrame::ConstructL(const TDes8& aBuffer, 
							 TInt aMaxBufferSize)
	{
	iBody = CBody::NewL(*this, aBuffer, aMaxBufferSize);
	}	


// for future development
EXPORT_C void CImageFrame::Reserved1()
	{
	Panic(EReservedCall);
	}
	
// for future development
EXPORT_C void CImageFrame::Reserved2()
	{
	Panic(EReservedCall);
	}	

// for future development
EXPORT_C void CImageFrame::Reserved3()
	{
	Panic(EReservedCall);
	}	
	
// for future development
EXPORT_C void CImageFrame::Reserved4()
	{
	Panic(EReservedCall);
	}	
	
