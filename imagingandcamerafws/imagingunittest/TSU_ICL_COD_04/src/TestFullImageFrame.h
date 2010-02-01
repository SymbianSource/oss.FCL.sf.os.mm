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

#ifndef TESTFULLIMAGEFRAME_H
#define TESTFULLIMAGEFRAME_H

#include <e32std.h>
#include <e32base.h> 
#include <imageframe.h>


class TFrameFormatTest: public TFrameFormatBase
	{
public:			
	enum TColourSpace
		{
		EColourSpaceNone,//default
		EColourSpaceCustom, 
		EYCbCr,
		EYuv,
		ERgb
		};

	enum TColorSampling
		{
		EColorSamplingNone,//default
		ESamplingCustom, 
		EMonochrome,
		EColor444,
		EColor422,
		EColor420,
		EColor411
		};

	enum TFormatPacking
		{
		EFormatPackingNone,//default
		EPackingCustom, 
		EPackingSymbian
		};    	

public: 
	TFrameFormatTest();
	TFrameFormatBase* DuplicateL() const;
		
public: 
	TColourSpace	iColourSpace;
	TColorSampling 	iSampling;
	TFormatPacking	iFormatPacking;
	TUint32 		iFormatCode; // associated format code
	};

class TFrameLayoutTest: public TFrameLayoutBase
	{	
public:
	TFrameLayoutTest(TInt aPlanes);
	TFrameLayoutBase* DuplicateL() const;
	
public:
	/** The number of planes in this image. Value < KMaxPlanesInFrame. */ 
	TInt iPlanes;				   
	/** The offset of each plane from the start of the memory  referenced by this image frame */ 
	TInt iStart[KMaxPlanesInFrame]; 
	/** The length of each image plane in bytes. */ 
	TInt iLength[KMaxPlanesInFrame]; 
	/** The length of the data stored in each image plane in bytes. */ 
	TInt iCurrentLength[KMaxPlanesInFrame];	
	/* the width of the stride for each plane */           
	TInt iScanLength[KMaxPlanesInFrame];	
	};
	

class CImageFrameTest : public CImageFrame 
    {
public:  
	TUid ImageFrameType();
    
	const TFrameFormatBase& FrameFormat() const;	
	void SetFrameFormatL(const TFrameFormatBase& aFormat);
	
	const TFrameLayoutBase& FrameLayout() const;	
	void SetFrameLayoutL(const TFrameLayoutBase& aFrameLayout);

	const TSize& FrameSizeInPixels() const;	
	void SetFrameSizeInPixels(const TSize& aFrameSize);

    TInt MaxBufferSize() const;
    TBool IsChunk() const;

	// data access from descriptor
	virtual TDes8& Data() = 0;
	virtual const TDesC8& Data() const = 0;

    ~CImageFrameTest();

protected:
	CImageFrameTest(TUid aDataType);
	
	void ConstructL(const TSize& aFrameSize,
 				    const TFrameFormatBase& aFrameFormat,								   
				    const TFrameLayoutBase& aFrameLayout); 
				    
	void CheckInternalConsistencyL();

protected:
	TUid 				iImageFrameType;           	 
   	TSize 				iFrameSize;		 
    TFrameLayoutBase*	iLayout; 		  
	TFrameFormatBase*	iFormat;
    TInt                iMaxBufferSize;
    TBool               iIsChunk;
   
   };

/*
	Class CImageFrameDesc subclasses CImageFrame and is implemented as a wrapper 
	around a TDesc8 buffer.
	Provides implementation for reading and writing to TDesc8 buffer.
*/	
class CImageFrameDesc : public CImageFrameTest
    {
public:
	static CImageFrameDesc* NewL(TDes8& aBuffer);
 								  
    static CImageFrameDesc* NewL(TDes8& aBuffer, 
			 	   				  const TSize& aFrameSize, 
			 					  const TFrameFormatBase& aFrameFormat,
								  const TFrameLayoutBase& aFrameLayout); 

	~CImageFrameDesc();
	
	
	virtual TDes8& Data();	
	virtual const TDesC8& Data() const;	

protected:	
	CImageFrameDesc(TDes8& aBuffer);

protected:	
  	TDes8& iBufferDes8;
  	};

/*
	Class CImageFrameRChunk is a subclass ofCImageFrame class, implemented as a wrapper around RChunk buffer.
	Provides implementation for reading and writing to RChunk buffer.
	Allows direct access to the RChunk handle.
*/
class CImageFrameRChunk : public CImageFrameTest
    {
public:
	static CImageFrameRChunk* NewL(RChunk& aBuffer, 
						   					TInt aDataOffset);

	static CImageFrameRChunk* NewL(RChunk& aBuffer, 
								  	TInt aDataOffset, 
		 					 	  	const TFrameFormatBase& aFrameFormat,									
		 					 	  	const TSize& aFrameSize,									    	
		 					 	  	const TFrameLayoutBase& aFrameLayout); 

	~CImageFrameRChunk();

	virtual TDes8& Data();	
	virtual const TDesC8& Data() const;

	RChunk& GetRChunk();
	RChunk& GetRChunk() const;
	
	TInt DataOffset();

protected:	
	CImageFrameRChunk(RChunk& aBuffer, TInt aDataOffset);

protected:	
    RChunk& 	iDataChunk;
    TInt    	iDataOffset;
    TPtr8		iBufferPtr;
    };
    
#endif //FULLIMAGEFRAMETEST_H
