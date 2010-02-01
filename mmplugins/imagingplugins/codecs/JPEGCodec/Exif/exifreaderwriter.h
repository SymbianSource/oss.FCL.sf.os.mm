// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef EXIFREADERWRITER_H
#define EXIFREADERWRITER_H

#include <iclexif.h> // For MExifMetadata declaration

// Forward declarations
class CIfdGeneral;
class CIfd0;
class CIfd1;
class CExifIfd;
class CInteropIfd;
class CGpsIfd;

class CExifReaderWriter : public CBase, public MExifMetadata
	{
public:
	friend class CExifVerifier;
		
public:
	~CExifReaderWriter();
	
	// Functions derived from MExifMetaData
	TInt GetIntegerParam(TUint aTag, TUint aIfd, TInt& aParam) const;
	TInt GetShortParam(TUint aTag, TUint aIfd, TUint16& aParam) const;
	TInt GetRationalParam(TUint aTag, TUint aIfd, TInt& aNumer, TInt& aDenom) const;
	TInt GetParam8(TUint aTag, TUint aIfd, HBufC8*& aParam) const;
	TInt GetParam16(TUint aTag, TUint aIfd, HBufC16*& aParam) const;
	TInt GetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam) const;
	TInt GetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam) const;
	TInt GetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam) const;
	
	TInt SetParam8(TUint aTag, TUint aIfd, HBufC8* aParam);
	TInt SetParam16(TUint aTag, TUint aIfd, HBufC16* aParam);
	TInt SetIntegerParam(TUint aTag, TUint aIfd, TInt aParam);
	TInt SetShortParam(TUint aTag, TUint aIfd, TUint16 aParam);
	TInt SetRationalParam(TUint aTag, TUint aIfd, TInt aNumerator, TInt aDenominator);	
	TInt SetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam);
	TInt SetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam);
	TInt SetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam);
	
	
	virtual HBufC8* GetJpegThumbnailData();
	virtual void SetThumbnailData(HBufC8* aJpegData);
	
	TBool IsExifDataModified();
	void ResetExifDataModified();
	
	// Helper Function
	static TUint ReadUint32(const TBool aIntelByteAlign, const TUint8* aPtr);
	// Helper Function	
	static TUint16 ReadUint16(const TBool aIntelByteAlign, const TUint8* aPtr);
	
protected:
	void SetUpIfdsL();
	virtual void SetIfd1L()=0;
	TInt CheckExifOffset(const TUint aOffset);
	TBool FindIfdIndex(TInt aRealIfd, TInt& aIfdIndex) const;
	CIfdGeneral* GetIfd(TInt aIndex);
   	void RemoveThumbnailData();

	CExifReaderWriter();
	
	void InitializeIfdsL(TBool aAlwaysCreateIfd1, TBool aCreateGpsIfd);
	void InitializeIfdsL(TUint8* aData, TUint aApp1Size);	

	void SetThumbnailDataBaseL(CIfdGeneral* ifd1, TBool aKeepIfd1);
	HBufC8* CreateExifHeaderBaseL();
	TInt GetThumbnailLengthOffsetBase(TUint& aThumbnailLengthOffset);
	  
private: // functions
	
	void ReadHeaderL();
	TInt InternalIfd(const TUint aTag, const TInt aExternalIfd) const;
	TInt CheckTagL(const TUint aTag, const TUint aIfd) const;
	void FindInternalIfdDataL(const TUint aTag, const TUint aExternalIfdNumber, TInt& aInternalIfdNumber, CIfdGeneral*& aInternalIfd);
	CIfdGeneral* FindIfd(const TUint aTag, const TUint aExternalIfd) const;
	TInt ProcessGpsIfdAddParamResult(CIfdGeneral*& aGpsIfd, TInt aAddParamResult);
	CIfdGeneral* CreateAndAppendSubIfdL(CIfdGeneral& aIfd, TUint aTag);

	// thumbnail helper functions
	virtual void WriteExifFileHeaderBaseL(const TUint aApp1Size, TUint8*& aPtr);
	virtual void SetFirstIfdOffsetBase();

	TInt DoSetParam8(TUint aTag, TUint aIfd, HBufC8* aParam);
	TInt DoSetParam16(TUint aTag, TUint aIfd, HBufC16* aParam);
	TInt DoSetIntegerParam(TUint aTag, TUint aIfd, TInt aParam);
	TInt DoSetShortParam(TUint aTag, TUint aIfd, TUint16 aParam);
	TInt DoSetRationalParam(TUint aTag, TUint aIfd, TInt aNumerator, TInt aDenominator);	
	TInt DoSetIntegerArrayParam(TUint aTag, TUint aIfd, CArrayFix<TInt>& aParam);
	TInt DoSetShortArrayParam(TUint aTag, TUint aIfd, CArrayFix<TUint16>& aParam);
	TInt DoSetRationalArrayParam(TUint aTag, TUint aIfd, CArrayFix<TRational>& aParam);
		
protected:		
	RPointerArray<CIfdGeneral> iIfds; //own IFDs
	TBool iEncodeThumbnail;
	TUint8* iData;
	TBool iIntel;
	const TUint8* iBase;
	TUint16 iExifDataLength;	// used to check that offsets are within a valid range
	
private: // member variables
	TUint iOffsetToZerothIfd;	
	HBufC8* iJpegThumbnailData;
	TBool iExifDataModified;
	};

#endif // EXIFREADERWRITER_H
