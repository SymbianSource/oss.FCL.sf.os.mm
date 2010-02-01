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

/**
 @file
 @internalTechnology
*/
#ifndef IMAGEFRAMEBODY_H
#define IMAGEFRAMEBODY_H

#include <imageframe.h>

/**
CBody class implements the functionality declared in CImageFrame class.
It can be created as a wrapper around a RChunk or a descriptor.
*/
NONSHARABLE_CLASS( CImageFrame::CBody ): public CBase 
    {
    friend class CImageFrame;
    
private: 
    static CBody* NewL(CImageFrame& aHandle, const TDes8& aBuffer, TInt aMaxBufferSize);
 								  
    static CBody* NewL(CImageFrame& aHandle, const TDes8& aBuffer, TInt aMaxBufferSize,
				 	   	const TSize& aFrameSize, const TFrameFormatBase& aFrameFormat, const TFrameLayoutBase& aFrameLayout); 
										  
	static CBody* NewL(CImageFrame& aHandle, const RChunk* aBuffer, TInt aMaxBufferSize, TInt aDataOffset);

	static CBody* NewL(CImageFrame& aHandle, const RChunk* aBuffer, TInt aMaxBufferSize,
					  TInt aDataOffset,	const TSize& aFrameSize, const TFrameFormatBase& aFrameFormat,const TFrameLayoutBase& aFrameLayout); 
 	~CBody();
    
	const TFrameFormatBase& FrameFormat() const;	
	void SetFrameFormatL(const TFrameFormatBase& aFormat);
	
	const TFrameLayoutBase& FrameLayout() const;	
	void SetFrameLayoutL(const TFrameLayoutBase& aFrameLayout);

	const TSize& FrameSizeInPixels() const;	
	void SetFrameSizeInPixels(const TSize& aFrameSize);

	TDes8& Data();
	
	const TDesC8& Data() const;

	RChunk& DataChunk();
	
	TInt DataOffset() const;
	TInt MaxBufferSize() const;
	TBool IsChunk() const;

    explicit CBody(CImageFrame& aHandle);
	CBody(CImageFrame& aHandle, const TDes8& aBuffer);
	
	void StoreChunkL(const RChunk* aBuffer, TInt aMaxBufferSize, TInt aDataOffset);
	void SetMaxSizeL(TInt aMaxBufferSize);	 
	void StoreAddParamL(const TSize& aFrameSize, const TFrameFormatBase& aFrameFormat, const TFrameLayoutBase& aFrameLayout);  
	void SetDefaultFormatL();
	

private:
	CImageFrame& 		iHandle; 
	//  Frame size in pixels        	 
   	TSize 				iFrameSize;
   	//  Image Frame specific layout		 
	TFrameLayoutBase* 	iLayout; 
	//  Image Frame format	  
	TFrameFormatBase*	iFormat; 
	//  pointer to the RChunk, represented by the CImageFrameRChunk object
    RChunk* 			iDataChunk;
    //  offset of the data from the start of the chunk
    TInt    			iDataOffset;
    //  8-bit pointer to the data.
    TPtr8				iBufferPtr;
    //  The maximum size allocated to this object by its creator
    TInt                iMaxBufferSize;
    // stores the info about the type of the present object
    TBool               iIsChunk;
    };
	  
#endif //IMAGEFRAMEBODY_H
