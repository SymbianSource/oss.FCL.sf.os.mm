// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "TestFullImageFrame.h"

// CImageFrame type definition for CImageFrameDesc
static const TUid KUidIclImageFrameDesc   	= {0x10273755}; //dummy value

// CImageFrame type definition for CImageFrameRChunk
static const TUid KUidIclImageFrameRChunk	= {0x10273756}; //dummy value

// CImageFrame type definition for TFrameFormatTest
static const TUid KUidIclImageFrameFormatTest	= {0x10273757}; //dummy value

// CImageFrame type definition for TFrameLayoutTest 
static const TUid KUidIclImageFrameLayoutTest	= {0x10273758}; //dummy value


TFrameLayoutTest::TFrameLayoutTest(TInt aPlanes) : TFrameLayoutBase(KUidIclImageFrameLayoutTest), iPlanes(aPlanes)
	{
	}
	
TFrameLayoutBase* TFrameLayoutTest::DuplicateL() const
	{
	// need to put on stack
	TFrameLayoutBase* copy =  new  (ELeave) TFrameLayoutTest(iPlanes);
	
	CleanupStack::PushL(copy);
	// copy this one to the new instance
	Mem::Copy(copy, this, sizeof(TFrameLayout));
	CleanupStack::Pop();

	return copy;
	}

TFrameFormatTest::TFrameFormatTest() : TFrameFormatBase(KUidIclImageFrameFormatTest)
	{
	}

TFrameFormatBase* TFrameFormatTest::DuplicateL() const
	{
	TFrameFormatBase* copy = new (ELeave) TFrameFormatTest();
	return copy;
	}	
	
/*
Constructor for CImageFrameTest class.
@param  aDataType
        UID which specifies the type of the CImageFrameTest implementation.
*/	
CImageFrameTest::CImageFrameTest(TUid aDataType): iImageFrameType(aDataType), 
										  iFrameSize(0,0)								   
	{
	}

/*
Destructor for CImageFrameTest class.
*/	
CImageFrameTest::~CImageFrameTest()
	{
	}

/*
Returns the Uid for the current type of the CImageFrameTest.

@return the UID for the type of the CImageFrameTest object.
*/	
TUid CImageFrameTest::ImageFrameType()
	{
	return iImageFrameType;
	}

/*
Returns the format of the CImageFrameTest.

@return A const reference to the TFrameFormat object which describes the CImageFrameTest object format.
*/	    
const TFrameFormatBase& CImageFrameTest::FrameFormat() const
	{
	return *iFormat;
	}	

/*
Set the format of the CImageFrameTest.

@param aFormat
	   A const reference to TFrameFormat object used to set CImageFrameTest format.
*/	
void CImageFrameTest::SetFrameFormatL(const TFrameFormatBase& aFormat)
	{
	delete iFormat;
	iFormat = (TFrameFormatBase*)aFormat.DuplicateL();	
	}

/*
Returns the layout of the CImageFrameTest associated image data.

@return A const reference to TFrameLayout which describes CImageFrameTest object particular layout.
*/
const TFrameLayoutBase& CImageFrameTest::FrameLayout() const
	{
	return *iLayout;
	}

/*
Set the layout of the CImageFrameTest associated image data.

@param aFrameLayout
	   A const reference to TFrameLayout object used to set CImageFrameTest format.

@leave KErrArgument
       when the new data layout is inconsistent with CImageFrameTest object other parameters.
*/		
void CImageFrameTest::SetFrameLayoutL(const TFrameLayoutBase& aLayout)
	{
	delete iLayout;
	iLayout = (TFrameLayoutBase*)aLayout.DuplicateL();	
	}

/*
Returns the size in pixels of the image the CImageFrameTest refers to.

@return image size in pixels.
*/
const TSize& CImageFrameTest::FrameSizeInPixels() const
	{
	return iFrameSize;
	}
	
/*
Set the size in pixels of the image CImageFrameTest refers to.

@param aFrameSize
	   A const reference to TSize used to set CImageFrameTest format.
*/	
void CImageFrameTest::SetFrameSizeInPixels(const TSize& aFrameSize)
	{
	iFrameSize = aFrameSize;
	}

/*
Returns whether this is or not a Chunk based storage.

@return boolean to indicate whether this image frame is using a chunk.
*/
TBool CImageFrameTest::IsChunk() const
	{
	return iIsChunk;
	}
	
/*
Returns MaxBuferSize.

@return maximum buffer size reserved for this image.
*/	
TInt CImageFrameTest::MaxBufferSize() const
	{
	return iMaxBufferSize;
	}
	
/*
Initialise CImageFrameTest object.

@param aSize
	   a reference to a TSize object containing image size in pixels. 
@param aFormat
	   a reference to a TFrameFormat object containing image format information. 	
@param aLayout
	   a reference to TSize used  set CImageFrameTest format.

@leave KErrArgument
*/	
void CImageFrameTest::ConstructL(const TSize& aSize, 
		 				     const TFrameFormatBase& aFormat,								   
						     const TFrameLayoutBase& aLayout)
	{
	iFrameSize = aSize;
	iFormat = (TFrameFormatBase*)aFormat.DuplicateL();
	iLayout = (TFrameLayoutBase*)aLayout.DuplicateL();	
	CheckInternalConsistencyL();
	}

/*
Checks that the data contained in the individual parameters is consistent
with each other, so that the object is usable. 

@leave  KErrArgument if the basic check was not passed.
*/
void CImageFrameTest::CheckInternalConsistencyL()
	{
	// do need  leave if data is conflicting
	if (iFrameSize.iHeight<=0 || iFrameSize.iWidth <= 0)
		{
		User::Leave(KErrArgument);
		}
	}

/*
Static factory function used to create a CImageFrameDesc object.

@param aBuffer
       a reference to the TDesc8 object the CImageFrameDesc wraps around

@return a pointer to a valid CImageFrameDesc object.

@leave  KErrMemory or other system wide error codes. 
*/
CImageFrameDesc* CImageFrameDesc::NewL(TDes8& aBuffer)
	{
	CImageFrameDesc* self = new(ELeave) CImageFrameDesc(aBuffer); 
	return self;
	}

/*
Static factory function used to create a CImageFrameDesc object.

@param aBuffer
       a reference to the TDesc8 object the CImageFrameDesc wraps around.
       
@param aFrameSize
       a reference to the TSize object  initilialise the CImageFrameDesc object.
       
@param aFrameFormat
       a reference to the TFrameFormat  initilialise the CImageFrameDesc object.
       
@param aFrameLayout
       a reference to the TFrameLayout  initilialise the CImageFrameDesc object.

@return a pointer to a valid CImageFrameDesc object.       
              
@leave  KErrArgument
@leave  KErrMemory or other system wide error code.
*/						  
CImageFrameDesc* CImageFrameDesc::NewL(TDes8& aBuffer, 
							   					 const TSize& aFrameSize, 
											 	 const TFrameFormatBase& aFrameFormat,
												 const TFrameLayoutBase& aFrameLayout)
	{
	CImageFrameDesc* self =  new(ELeave) CImageFrameDesc(aBuffer); 
	CleanupStack::PushL(self);
	self->ConstructL(aFrameSize, aFrameFormat, aFrameLayout);
	CleanupStack::Pop();
	return self;
	}
	
/*
Constructor for CImageFrameDesc class.

param  aBuffer
       the buffer as a TDesc8 reference.
*/
CImageFrameDesc::CImageFrameDesc(TDes8& aBuffer) : CImageFrameTest(KUidIclImageFrameDesc), 			   
												   iBufferDes8(aBuffer) 
	{
	}

/*
Destructor for CImageFrameDesc class.
*/	
CImageFrameDesc::~CImageFrameDesc()
	{
	}	

/*
Provides a writable TDes8 reference to the data buffer. 
Suitable both for reading and writing.
 
@return writable TDes8 reference.
*/	
TDes8& CImageFrameDesc::Data()
	{
	return iBufferDes8;
	}
	
/*
Provides a TDesC8 reference to the data buffer. 
Suitable for reading of the data only.
 
@return const TDesC8 reference.
*/	
const TDesC8& CImageFrameDesc::Data() const
	{
	return iBufferDes8;
	}

/*
Static factory function used to create a CImageFrameRChunk object.

@param aBuffer
       a reference to the RChunk object the CImageFrameRChunk wraps around.
       
@param aDataOffset
       The offset at which the data for this frame starts.       

@return a pointer to a valid CImageFrameRChunk object.

@leave  KErrMemory or other system wide error code.
*/
CImageFrameRChunk* CImageFrameRChunk::NewL(RChunk& aBuffer, 
					   								TInt aDataOffset)
	{
	CImageFrameRChunk* self = new(ELeave) CImageFrameRChunk(aBuffer, aDataOffset); 
	return self;
	}

/*
Static factory function used to create a CImageFrameRChunk object.

@param aBuffer
       a reference to the RChunk object the CImageFrameRChunk wraps around.
       
@param aDataOffset
       The offset at which the data for this frame starts. 

@param aFrameSize
       a reference to the TSize object to initilialise the CImageFrameRChunk object.
       
@param aFrameFormat
       a reference to the TFrameFormat to initilialise the CImageFrameRChunk object.
       
@param aFrameLayout
       a reference to the TFrameLayout to initilialise the CImageFrameRChunk object.      

@return a pointer to a valid CImageFrameRChunk object.

@leave  KErrArgument
@leave  KErrMemory or other system wide error code.
*/
CImageFrameRChunk* CImageFrameRChunk::NewL(RChunk& aBuffer, 
										  	TInt aDataOffset, 
									 	  	const TFrameFormatBase& aFrameFormat,									
									 	  	const TSize& aFrameSize,									    	
									 	  	const TFrameLayoutBase& aFrameLayout)
	{
	CImageFrameRChunk* self = new(ELeave) CImageFrameRChunk(aBuffer, aDataOffset); 
	CleanupStack::PushL(self);
	self->ConstructL(aFrameSize, aFrameFormat, aFrameLayout);
	CleanupStack::Pop();
	return self;
	}

/*
CImageFrameRChunk constructor.

@param aBuffer
       a reference to the RChunk object the CImageFrameRChunk wraps around.
       
@param aDataOffset
       The offset at which the data for this frame starts.  
*/			
CImageFrameRChunk::CImageFrameRChunk(RChunk& aBuffer, TInt aDataOffset) : 
	  								CImageFrameTest(KUidIclImageFrameRChunk), 
									iDataChunk(aBuffer), 
	 								iDataOffset(aDataOffset),
									iBufferPtr(TPtr8(iDataChunk.Base() + iDataOffset, aBuffer.Size(), aBuffer.MaxSize()))									
	{

	}
	
/*
CImageFrameRChunk destructor.
*/	
CImageFrameRChunk::~CImageFrameRChunk()
	{
	
	}

/*
Provides a writable TDes8 reference to the data buffer. 
Suitable both for reading and writing.
 
@return writable TDes8 reference.
*/	
TDes8& CImageFrameRChunk::Data()
	{
	return iBufferPtr;
	}
	
/*
Provides a TDesC8 reference to the data buffer. 
Suitable for reading of the data only.
 
@return TDesC8 reference.
*/	
const TDesC8& CImageFrameRChunk::Data() const
	{
	return iBufferPtr;
	}

/*
Provides a RChunk reference.
 
@return RChunk reference.
*/	
RChunk& CImageFrameRChunk::GetRChunk()
	{
	return iDataChunk;
	}

/*
Provides a RChunk reference.
 
@return RChunk reference.
*/	
RChunk& CImageFrameRChunk::GetRChunk() const
	{
	return iDataChunk;
	}	

/*
Returns the offset of the CImageFrameTest specific data within the RChunk object.
 
@return RChunk data offset from RChunk start.
*/
TInt CImageFrameRChunk::DataOffset()
	{
	return iDataOffset;
	}
