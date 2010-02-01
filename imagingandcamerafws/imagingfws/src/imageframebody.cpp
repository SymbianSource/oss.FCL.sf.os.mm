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

#include "imageframebody.h"
#include "ImageClientMain.h"

/**
Destructor for CImageFrame::CBody class.
*/	
CImageFrame::CBody::~CBody()
	{
	delete iFormat;
	delete iLayout;	
	}

/**
Returns the format of the CImageFrame::CBody.

@return A const reference to the TFrameFormatBase object which describes the CImageFrame::CBody object format.
*/	    
const TFrameFormatBase& CImageFrame::CBody::FrameFormat() const
	{
	return *iFormat;
	}	
	
/**
Set the format of the CImageFrame::CBody.

@param aFormat
	   A const reference to TFrameFormatBase object used to set CImageFrame::CBody format.
*/	
void CImageFrame::CBody::SetFrameFormatL(const TFrameFormatBase& aFormat)
	{
	if (iFormat!=NULL)
		{
		delete iFormat;
		iFormat = NULL;
		}
	iFormat = static_cast<TFrameFormatBase*>(aFormat.DuplicateL());	
	}

/**
Returns the layout of the CImageFrame::CBody associated image data.

@return A const reference to TFrameLayoutBase which describes CImageFrame::CBody object particular layout.
*/
const TFrameLayoutBase& CImageFrame::CBody::FrameLayout() const
	{
	return *iLayout;
	}

/**
Set the layout of the CImageFrame::CBody associated image data.

@param aFrameLayout
	   A const reference to TFrameLayoutBase object used to set CImageFrame::CBody layout.

@leave KErrNoMemory.
*/		
void CImageFrame::CBody::SetFrameLayoutL(const TFrameLayoutBase& aFrameLayout)
	{
	if (iLayout!=NULL)
		{
		delete iLayout;
		iLayout = NULL;
		}
	iLayout = static_cast<TFrameLayoutBase*>(aFrameLayout.DuplicateL());
	}

/**
Returns the size in pixels of the image the CImageFrame::CBody refers to.

@return image size in pixels.
*/
const TSize& CImageFrame::CBody::FrameSizeInPixels() const
	{
	return iFrameSize;
	}
	
/**
Set the size in pixels of the image CImageFrame::CBody refers to.

@param aFrameSize
	   A const reference to TSize used to set CImageFrame::CBody format.
*/	
void CImageFrame::CBody::SetFrameSizeInPixels(const TSize& aFrameSize)
	{
	iFrameSize = aFrameSize;
	}

/**
Provides a writable TDes8 reference to the data buffer. 
Suitable both for reading and writing.
 
@return writable TDes8 reference.
*/	
TDes8& CImageFrame::CBody::Data()
	{
	return iBufferPtr;
	}
	
/**
Provides a TDesC8 reference to the data buffer. 
Suitable for reading of the data only.
 
@return const TDesC8 reference.
*/	
const TDesC8& CImageFrame::CBody::Data() const
	{
	return iBufferPtr;
	}

/**
Provides a RChunk reference. It will panic if the frame does not contain a chunk.
 
@return RChunk reference.
*/	
RChunk& CImageFrame::CBody::DataChunk()
	{
	__ASSERT_ALWAYS((iDataChunk!=NULL), Panic(EInvalidValue));
	return *iDataChunk;
	}

/**
Returns the offset of the CImageFrame::CBody specific data within the buffer object.
 
@return  data offset from CimageFrame buffer start.
*/
TInt CImageFrame::CBody::DataOffset() const
	{
	return iDataOffset;
	}

/**
Returns whether the specific CImageFrame object contains a Chunk or not. 

@return ETrue if the buffer is a chuk and EFalse of not. 
*/
TBool CImageFrame::CBody::IsChunk() const
	{
	return iIsChunk;
	}
	
/**
Returns the maximum buffer size as specified by the constructing entity.

@return the maximum buffer size. 
*/
TInt CImageFrame::CBody::MaxBufferSize() const
	{
	return iMaxBufferSize;
	}
		
/**
Static factory function used to create a CImageFrame::CBody object.

@param aHandle
       a reference to the owning CImageFrame object.

@param aBuffer
       a reference to the TDesc8 object the CImageFrame::CBody wraps around

@param aMaxBufferSize
	   the maximum buffer size. 

@return a pointer to a valid CImageFrame::CBody object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error codes. 
*/
CImageFrame::CBody* CImageFrame::CBody::NewL(CImageFrame& aHandle, 
 											 const TDes8& aBuffer,
 											 TInt aMaxBufferSize)
	{
	CImageFrame::CBody* self =  new(ELeave) CImageFrame::CBody(aHandle, aBuffer); 
	CleanupStack::PushL(self);
	self->SetMaxSizeL(aMaxBufferSize);
	self->SetDefaultFormatL();
	CleanupStack::Pop();
	return self;	
	}

/**
Static factory function used to create a CImageFrame::CBody object.

@param aHandle
       a reference to the owning CImageFrame object.

@param aBuffer
       a reference to the TDesc8 object the CImageFrame::CBody wraps around.

@param aMaxBufferSize
	   the maximum buffer size. 

@param aFrameSize
       a reference to the TSize object to initialise the CImageFrame::CBody object.      

@param aFrameFormat
       a reference to the TFrameFormatBase to initialise the CImageFrame::CBody object.    

@param aFrameLayout
       a reference to the TFrameLayoutBase to initialise the CImageFrame::CBody object.

@return a pointer to a valid CImageFrame::CBody object.       
              
@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/						  
CImageFrame::CBody* CImageFrame::CBody::NewL(CImageFrame& aHandle, 
										 	 const TDes8& aBuffer, TInt aMaxBufferSize,
						   					 const TSize& aFrameSize, 
										 	 const TFrameFormatBase& aFrameFormat,
											 const TFrameLayoutBase& aFrameLayout)
	{
	CImageFrame::CBody* self =  new(ELeave) CImageFrame::CBody(aHandle, aBuffer); 
	CleanupStack::PushL(self);
	self->SetMaxSizeL(aMaxBufferSize);
	self->StoreAddParamL(aFrameSize, aFrameFormat, aFrameLayout);
	CleanupStack::Pop();
	return self;
	}

/**
Static factory function used to create a CImageFrameRChunk object.

@param aHandle
       a reference to the owning CImageFrame object.

@param aBuffer
       a reference to the RChunk object the CImageFrameRChunk wraps around.

@param aMaxBufferSize
	   the maximum buffer size.       

@param aDataOffset
       The offset at which the data for this frame starts.       

@return a pointer to a valid CImageFrame::CBody object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
CImageFrame::CBody* CImageFrame::CBody::NewL(CImageFrame& aHandle, 
											const RChunk* aBuffer, 
											TInt aMaxBufferSize, 
											TInt aDataOffset)
	{
	CImageFrame::CBody* self = new(ELeave) CImageFrame::CBody(aHandle); 
	CleanupStack::PushL(self);
	self->StoreChunkL(aBuffer, aMaxBufferSize, aDataOffset);
	self->SetDefaultFormatL();
	CleanupStack::Pop();
	return self;	
	}

/**
Static factory function used to create a CImageFrameRChunk object.

@param aHandle
       a reference to the owning CImageFrame object.

@param aBuffer
       a reference to the RChunk object the CImageFrameRChunk wraps around.

@param aMaxBufferSize
	   the maximum buffer size.            

@param aDataOffset
       The offset at which the data for this frame starts. 

@param aFrameSize
       a reference to the TSize object to initialise the CImageFrameRChunk object.

@param aFrameFormat
       a reference to the TFrameFormatBase to initialise the CImageFrameRChunk object. 

@param aFrameLayout
       a reference to the TFrameLayoutBase to initialise the CImageFrameRChunk object.      

@return a pointer to a valid CImageFrame::CBody object.

@leave  KErrArgument
@leave  KErrNoMemory or other system wide error code.
*/
CImageFrame::CBody* CImageFrame::CBody::NewL(CImageFrame& aHandle,const RChunk* aBuffer, 
											TInt aMaxBufferSize,
											TInt aDataOffset, 
											const TSize& aFrameSize,
											const TFrameFormatBase& aFrameFormat,																	    	
											const TFrameLayoutBase& aFrameLayout)
	{
	CImageFrame::CBody* self = new(ELeave) CImageFrame::CBody(aHandle); 
	CleanupStack::PushL(self);
	self->StoreChunkL(aBuffer, aMaxBufferSize, aDataOffset);
	self->StoreAddParamL(aFrameSize, aFrameFormat, aFrameLayout);
	CleanupStack::Pop();
	return self;
	}

/**
CImageFrame::CBody constructor.

@param aHandle
       a reference to the owning CImageFrame object.      
*/			
CImageFrame::CBody::CBody(CImageFrame& aHandle) : 
						iHandle(aHandle),
	  					iBufferPtr(0,0),
	  					iIsChunk(ETrue)							
	{
	}
	
/**
CImageFrame::CBody constructor.

@param aHandle
       a reference to the owning CImageFrame object.

@param aBuffer
       a reference to the TDes8 object the CImageFrame::Body object contains.      
*/	
CImageFrame::CBody::CBody(CImageFrame& aHandle, const TDes8& aBuffer) : 
												iHandle(aHandle),			   
												iBufferPtr((TUint8*)(aBuffer.Ptr()), aBuffer.Size(), aBuffer.MaxLength()),
												iIsChunk(EFalse)
												
	{
	}

/**
Stores the maximum size of the buffer

@param aMaxBufferSize
       buffer maximum size.

@leave KErrArgument.      
*/	
void CImageFrame::CBody::SetMaxSizeL(TInt aMaxBufferSize)
	{
	if (aMaxBufferSize <= 0)
		{
		User::Leave(KErrArgument);
		}
	iMaxBufferSize = aMaxBufferSize;
	}

/** 
Function used to store the chunk relevant information.

@param aBuffer
	   a pointer to a valid RChunk object.

@param aMaxBufferSize
		buffer maximum size.

@param aDataOffset
	   The offset of the data in the CImageFrame::CBody object from the start of the chunk. 

@leave KErrArgument
*/
void CImageFrame::CBody::StoreChunkL(const RChunk* aBuffer, TInt aMaxBufferSize, TInt aDataOffset)
	{
	if (aBuffer == NULL)
		{
		User::Leave(KErrArgument);
		}
		
	if (aDataOffset < 0)
		{
		User::Leave(KErrArgument);
		}
			
	SetMaxSizeL(aMaxBufferSize);	
	iDataOffset = aDataOffset;
	iDataChunk = (RChunk*)aBuffer;
	iBufferPtr.Set((iDataChunk->Base() + iDataOffset), iDataChunk->Size(), iDataChunk->MaxSize());
	}
	
/**
Initialise CImageFrame::CBody object.

@param aSize
	   a reference to a TSize object containing image size in pixels. 

@param aFormat
	   a reference to a TFrameFormatBase object containing image format information. 	

@param aLayout
	   a reference to FrameLayoutBase object containing image layout information.

@leave KErrNoMemory
*/	
void CImageFrame::CBody::StoreAddParamL( const TSize& aSize, 
					 				     const TFrameFormatBase& aFormat,								   
									     const TFrameLayoutBase& aLayout)
	{
	iFrameSize = aSize;
	if (iFormat!=NULL)
		{
		delete iFormat;
		iFormat = NULL;
		}
	iFormat = (TFrameFormatBase*)aFormat.DuplicateL();
	
	if (iLayout!=NULL)
		{
		delete iLayout;
		iLayout = NULL;
		}
	iLayout = (TFrameLayoutBase*)aLayout.DuplicateL();
	}

/**
Sets default Frame Format and Frame Layout for this version of Image Frame

@leave KErrNoMemory
*/	
void CImageFrame::CBody::SetDefaultFormatL()
	{
	iFormat = new (ELeave) TFrameFormat(KNullUid);
	iLayout = new (ELeave) TFrameLayout(0);
	}
	
/*
Used for padding the BWINS def file.
*/
#ifdef __WINS__
EXPORT_C void Reserved1()
	{
	}
#endif // __WINS__
