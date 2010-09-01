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

#ifndef EXIFEDITUTILITY_H
#define EXIFEDITUTILITY_H

#include <e32base.h>
#include <icl/imagecodec.h>

const TUint KBlockIdLength = 2;		// jpeg block id is two bytes long
const TUint KBlockSizeLength = 2;	// jpeg block size is two bytes long
const TUint KBlockIdAndSizeLength = KBlockIdLength + KBlockSizeLength;

enum TConvertState
	{
	EEmpty,			// nothing has been done on the file
	EReadComplete,	// use to say that the file has been read and can be written
	
	// write enums
	EStartWrite,
	EWriteReadHeader,
	EWriteWriteHeader,
	EReadThumbnail,
	EWriteReadThumbnail,
	EWriteConvertExif,
	EWriteWriteExif,
	EWriteTrailer,
	EWriteReadMainImage,
	ECancelled,
	ECopying
	};

enum TCopyState
	{
	ERead,
	EWrite,
	EReadJpegBlockInfo
	};

//ExifTag class defines the Tags found in the Exif specification.
class CExifData;
class CScaledJpegGenerator;
class MExifSource;
class MExifDest;

class CExifEditUtility : public CActive
	{
public:
	static CExifEditUtility* NewL(MExifSource* aSource, MExifDest* aDest, TBool aIgnoreExifMetadataProcessing = EFalse);
	~CExifEditUtility();

	void ReadSourceL();
	void WriteDestL(TBool aEncodeThumbnail, const TSize& aSize, TBool aPreserveImage, TBool aMaintainAspectRatio, TRequestStatus* aNotifier);
	
	MExifMetadata* ExifMetadata();

protected:
	CExifEditUtility(TBool aIgnoreExifMetadataProcessing);

	void ProcessCommandL();
	void DoCancel();

private:
	void ConstructL(MExifSource* aSource, MExifDest* aDest);

	void InitReadL();

	// asynchronous notification methods

	TBool DoWriteConvertExifL();
	TBool DoWriteReadThumbnailL();
	TBool DoReadThumbnailL();
	TBool DoWriteComplete();
	TBool DoCreateMainImageL();
	TConvertState DoCopyNextBlockL();

	TBool DoCopyBlockL(TUint aStart, TInt aLength, TConvertState aNextStep, TBool aCombineTables = EFalse);
	TBool DoWriteScaledImageL();

	TBool DoWriteDestBufferL();

	void RunL();
	TInt RunError(TInt aError);
	
	void DoReadL();
	void SetUpTablesL();	
	void AppendTableDataL(RBuf8& aTables, const TDesC8& aData);
	void HandleJpegBlockL(const TDesC8& aPrependedData);
	void UpdateImageSizeTagsL();
	
private:
	// configuration data
	MExifSource* iSource;		/**< The source that will be read*/
	MExifDest* iDest;			/**< The destination */
	TSize iSize;				/**< size of the image in the destination */
	TBool iPreserveImage; 		/**< ETrue if the image should not be changed*/
	TBool iMaintainAspectRatio; 		/**< ETrue if the aspect ratio should be maintained*/
	TBool iEncodeThumbnail;		/**< ETrue if the destination file should embed a thumbnail*/
	TRequestStatus* iNotifier;	/**< the Active object to notify when the transform is performed*/

	CExifTransform* iExifData;
	CScaledJpegGenerator* iScaledJpegGenerator;

	TUint iExifBlockSize;		// size of the exif block in the source	
	TUint iHeaderBlockSize;		// size of the chunk of data that is before the exif data
	TInt iTrailerBlockSize;		// size of the chunk of data that is after the exif data
	TUint iTrailerOffset;		// offset to the trailing data in the source
	
	TBufPtr8 iIOBufferPtr;

	TConvertState iState;

	TCopyState iCurCopyState;
	TConvertState iNextStep;
	TInt iCopyLength;
	TUint iCurrentWriteBufferIndex;
	
	RBuf8 iDHTTables;			// stores the DHT tables that must be combined into one for exif
	RBuf8 iDQTTables;			// stores the DQT tables that must be combined into one for exif
	
	TInt iCurrentJpegBlockType;
	TUint iCurrentJpegBlockSize;
	TBool iDestIsExif;
	TBuf8<KBlockIdAndSizeLength> iLookupSlackBuff; // for holding of block id slack when copying main image
	RBuf8 iTempCopyBuffer;  // this buffer is used when we need to combine possibly incorrect block data
	TBool iIgnoreExifMetadataProcessing; //don't parse any EXIF data if this flag is set, and writes transparently from src to dest.
	TBool iConvertToJfif; //Source is to be converted to a JFIF by the transform (usually because source Exif block is corrupt).
	};

#endif //EXIFEDITUTILITY_H
