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

#ifndef IFDGENERAL_H
#define IFDGENERAL_H

#include <e32base.h>
#include "ExifTagDescriptions.h"

class CIfdGeneralEntry;
class CIfdGeneral : public CBase
	{
public:
	friend class CExifVerifier;

public:
	~CIfdGeneral();

	TInt GetIntegerParam(const TUint aTag, TInt& aParam) const;
	TInt GetShortParam(const TUint aTag, TUint16& aParam) const;
	TInt GetRationalParam(const TUint aTag, TInt& aNumer, TInt& aDenom) const;
	TInt GetParam8(const TUint aTag, HBufC8*& aParam) const;
	TInt GetParam16(const TUint aTag, HBufC16*& aParam) const;

	void GetIntegerArrayParamL(const TUint aTag, CArrayFix<TInt>& aParam) const;
	void GetShortArrayParamL(const TUint aTag, CArrayFix<TUint16>& aParam) const;
	void GetRationalArrayParamL(const TUint aTag, CArrayFix<TRational>& aParam) const;

	// Returns the number of bytes stored by an Ifd.
	TInt Size() const;
	
	// Can only be called by the 0th Ifd (otherwise returns KErrNotSupported).
	TInt GetOffsetToFirstIfd(TUint& aOffset) const;
	
	TInt Ifd() const; // returns the identifying Ifd.

	virtual TUint8* CreateIfdBlockL(TUint aOffset);
	
	TUint EntryCount() const;
	
	// Set the data of existing tag entries.
	TInt SetParam8(const TUint aTag, HBufC8* aParam);
	TInt SetParam16(const TUint aTag, HBufC16* aParam);
	TInt SetIntegerParam(const TUint aTag, const TInt aParam);
	TInt SetShortParam(const TUint aTag, const TUint16 aParam);
	TInt SetRationalParam(const TUint aTag, const TInt aNumerator, const TInt aDenominator);	
	void SetIntegerArrayParamL(const TUint aTag, const CArrayFix<TInt>& aParam);		
	void SetRationalArrayParamL(const TUint aTag, const CArrayFix<TRational>& aParam);
	void SetShortArrayParamL(const TUint aTag, const CArrayFix<TUint16>& aParam);
	
	// Add a new entry.
	void AddParam8L(const TUint aTag, const TUint aFormat, const TUint aComponentCount, HBufC8* aParam);
	void AddParam16L(const TUint aTag, const TUint aFormat, const TUint aComponentCount, HBufC16* aParam);
	void AddIntegerParamL(const TUint aTag, const TUint aFormat, const TUint aComponentCount, TInt aParam);
	void AddShortParamL(const TUint aTag, const TUint aFormat, const TUint aComponentCount, TUint16 aParam);
	void AddRationalParamL(const TUint aTag, const TUint aFormat, const TUint aComponentCount, const TInt aNumerator, const TInt aDenominator);
	void AddIntegerArrayParamL(const TUint aTag, const TUint aFormat, const CArrayFix<TInt>& aParam);	
	void AddRationalArrayParamL(const TUint aTag, const TUint aFormat, const CArrayFix<TRational>& aParam);
	void AddShortArrayParamL(const TUint aTag, const TUint aFormat, const CArrayFix<TUint16>& aParam);
	
	// Does the given tag exist in this Ifd?
	TBool EntryExists(const TUint aTag) const;
	
	TInt GetFormat(TUint aTag, TUint& aFormat);
	TInt GetComponentCount(TUint aTag, TUint& aComponentCount);

	TBool FindTag(const TUint aTag);
	
	// Remove an entry
	void RemoveEntryL(const TUint aTag);
	
	virtual void CheckMandatoryEntriesL();
protected:
	CIfdGeneral(const TInt aIfdNumber, const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);
	void ConstructL();
	
	virtual TInt GetTagIndex(const TUint aTag);
	virtual TInt GetNumberOfValidTags()=0;
	virtual const TUint* GetTagInformation(TInt aIndex)=0;
	
	virtual TBool AllowNullTags();	
	//check if the 	ifd entry count is valid with respect to minimum and maximum value.
	virtual TBool IsValidIfdEntryCount(TUint16 aIfdEntryCount) const;
private: // Functions
	void SetupDirectoryEntriesL(); 
	void AddAllIfdEntriesL();
	CIfdGeneralEntry* CreateIfdEntryLC(const TUint& aTag, const TUint& aFormat, const TUint& aComponentCount, const TUint8* aValuePtr, TUint aValueOffset, TUint aTotalSize, TBool& aUnknownTag);
	TInt LocateEntry(const TUint16 aTag) const;
	
	void WriteIfdDirEntriesL(TUint8* aBlock, TUint aOffset);
	void WriteIfdData(TUint8* aBlock);

	
protected: // Member variables.
	const TUint iIfdNumber;
	const TBool iIntel;
	const TUint iOffsetToIfd;
	const TUint8* iBase; // base pointer to ifd entry memory. Not owned
	const TUint iExifDataLength;// size of EXIF data (i.e. from iBase to the end of
								// the EXIF data) used for offset checking.
								
	TUint16	iIfdEntryCount; // The number of tags a decoded IFD header thinks it has.
	RPointerArray<CIfdGeneralEntry> iIfdEntries; // iIfdEntries.Count() is the number of valid tags.
	};
	
	
class CIfd0 : public CIfdGeneral
	{
public:		
	static CIfd0* NewLC(const TUint aOffsetToIfd=0, const TUint8* aBase=NULL, const TBool aIntel=EFalse, const TUint aExifDataLength=0);
	virtual TInt GetNumberOfValidTags();
	virtual const TUint* GetTagInformation(TInt aIndex);
	~CIfd0();
protected:
	void ConstructL();
	virtual TBool IsValidIfdEntryCount(TUint16 aIfdEntryCount) const;
	//Gets information about manufacturer name, model name and UI platform - depending on the tagvalue
	HBufC8* GetDeviceInfo(TUint aTagValue);
private:
	CIfd0(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);
protected:
	//buffer for storing manufaturer, model and UI platform
	HBufC8* iManufacturerName;
	HBufC8* iModelName;
	HBufC8* iUIPlatform;
	TUint iTag010F[6];
	TUint iTag0110[6];
	TUint iTag0131[6];
	};
	
class CExifIfd : public CIfdGeneral
	{
public:		
	static CExifIfd* NewLC(const TUint aOffsetToIfd=0, const TUint8* aBase=NULL, const TBool aIntel=EFalse, const TUint aExifDataLength=0);
	static CExifIfd* NewL(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);
	virtual TInt GetNumberOfValidTags();
	virtual const TUint* GetTagInformation(TInt aIndex);
    virtual void CheckMandatoryEntriesL();
    TBool CheckImageSizeTags();
    void UpdateImageSizeTagsL(const TSize& aSize);
private:
	CExifIfd(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);	
	};
	
class CInteropIfd : public CIfdGeneral
	{
public:		
	static CInteropIfd* NewLC(const TUint aOffsetToIfd=0, const TUint8* aBase=NULL, const TBool aIntel=EFalse, const TUint aExifDataLength=0);
	static CInteropIfd* NewL(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);
	virtual TInt GetNumberOfValidTags();
	virtual const TUint* GetTagInformation(TInt aIndex);  
    
protected:
	virtual TBool IsValidIfdEntryCount(TUint16 aIfdEntryCount) const;

private:
	CInteropIfd(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);	
	};
	
class CIfd1 : public CIfdGeneral
	{
public:		
	static CIfd1* NewLC(const TUint aOffsetToIfd=0, const TUint8* aBase=NULL, const TBool aIntel=EFalse, const TUint aExifDataLength=0);
	virtual TInt GetNumberOfValidTags();
	virtual const TUint* GetTagInformation(TInt aIndex);    
    
private:
	CIfd1(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);	
	};

class CGpsIfd : public CIfdGeneral
	{
public:		
	static CGpsIfd* NewLC(const TUint aOffsetToIfd=0, const TUint8* aBase=NULL, const TBool aIntel=EFalse, const TUint aExifDataLength=0);
	static CGpsIfd* NewL(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);
	virtual TInt GetNumberOfValidTags();
	virtual const TUint* GetTagInformation(TInt aIndex);
   
protected:
	virtual TBool IsValidIfdEntryCount(TUint16 aIfdEntryCount) const;

	TBool AllowNullTags();
    
private:
	CGpsIfd(const TUint aOffsetToIfd, const TUint8* aBase, const TBool aIntel, const TUint aExifDataLength);	
	};


#endif // IFDGENERAL_H
