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

#ifndef __EXIFGENERALCONSTS_H__
#define __EXIFGENERALCONSTS_H__

#include <gdi.h>
#include <iclexif.h>

// Header file containing general constants used by the Exif Encoder and Decoder.

// Constants for size.
const TUint K32BitIntegerByteCount = 4;
const TUint K16BitIntegerByteCount = 2;
const TUint K8BitIntegerByteCount = 1;

const TUint16 KApp1Marker = 0xE1FF; // The Application 1 marker (APP1).
const TUint16 KApp1MarkerByteCount = K16BitIntegerByteCount; // Number of bytes in the APP1 marker.
const TUint16 KBlockEndMarker = 0xD9FF; // The Application 1 marker (APP1).
const TUint16 KBlockEndMarkerByteCount = K16BitIntegerByteCount; // Number of bytes in the APP1 marker.

const TUint KOffsetToApp1Size = K16BitIntegerByteCount; // Two bytes in from the start of the Exif header.
const TUint16 KApp1SizeLength = K16BitIntegerByteCount; // The byte count of the APP1 header length field.

const TUint KExifMarker = 0x66697845; // 'Exif' string.
const TUint16 KExifMarkerByteCount = K32BitIntegerByteCount; // i.e. number of bytes of 'Exif'
const TUint16 KExifMarkerEnd = 0x0000; // Always follows 'Exif'.
const TUint16 KExifMarkerEndByteCount = K16BitIntegerByteCount;
const TInt KExifHeaderLength = K32BitIntegerByteCount + K16BitIntegerByteCount;

const TUint16 KIntelByteAlignment = 0x4949; // Indicates little-endian byte alignment.
const TUint16 KMotorolaByteAlignment = 0x4D4D;
const TUint16 KByteAlignmentByteCount = K16BitIntegerByteCount;
const TUint KByteCountToByteAlignment = KApp1MarkerByteCount +
										KApp1SizeLength +
										KExifHeaderLength; // Number of bytes from KApp1Marker through to KIntel.
const TInt KOffsetFromExifHeaderToByteAlignment = KExifHeaderLength;

const TUint16 KFortyTwo = 0x002A;
const TUint16 KFortyTwoByteCount = K16BitIntegerByteCount;

const TUint KIfdOffset = 0x00000008; // Offset to 0th IFD.
const TUint16 KIfdOffsetByteCount = K32BitIntegerByteCount;
const TUint KOffsetToFirstIfdSize = K32BitIntegerByteCount; // A 4 byte field containing an offset to the 1st Ifd.

const TUint16 KMaxApp1Size = 0xFFFF; // Maximum size of the APP1 header.

const TUint KSizeOfEntryCount = K16BitIntegerByteCount; // Each Ifd has an entry count of 2 bytes.

const TInt KJPEGEncodedThumbnail = 6; //

// External user will call functions using either of these two constants as the Ifd argument.
const TInt16 KExternalZerothIfd = 0;
const TInt16 KExternalFirstIfd = 1;

const TInt KOffsetToZerothOffset = 	KByteAlignmentByteCount +
									KFortyTwoByteCount +
									KIfdOffsetByteCount +
									KSizeOfEntryCount; // Number of bytes from byte alignment to the offset to the 0th Ifd.

const TUint16 KExifFileHeaderLength = 	KApp1MarkerByteCount +
										KApp1SizeLength +
										KExifMarkerByteCount +
										KExifMarkerEndByteCount +
										KByteAlignmentByteCount + 
										KFortyTwoByteCount + 
										KIfdOffsetByteCount; // Length of the standard Exif Header.

// Literals used in the encoder/decoder.										
_LIT8(KUnicode, "UNICODE\0"); // The first 8 bytes of the User Comment tag, if using Unicode.
_LIT8(KExifHeader, "Exif\0\0");

const TInt KThumbnailHeight = 120;
const TInt KThumbnailWidth = 160;
const TDisplayMode KThumbnailDisplayMode= EColor256;
const TInt KIOBlockSize=4096;	// size of blocks used for IO operations

enum TIfd
	{
	EZeroth, // Main image.	
	EExifSub, // Main image.
	EInterop, // Main image.
	EFirst, // Thumbnail.
	EGpsSub = KExifGpsIfd >> 16 // (==4) Main image.		
	};

enum TTagValueIndexes
	{
	ETagValue,
	EFormatValue,
	EComponentCountValue,
	EValueOffsetValue,
	EIfdValue,
	ESupportLevelValue
	};
	
// Is the tag Mandatory, Optional or 
enum TSupportLevel
	{
	EOptional,
	EConditionallyMandatory,
	EMandatory
	};
	
// The format field in the Ifd entry is a two byte field, with an identifying short.
enum TFormat
	{
	EByte = 1,
	EAscii = 2,
	EUnsignedShort = 3,
	EUnsignedLong = 4,
	EUnsignedRational = 5,
	EUndefined = 7,
	ESignedLong = 9,
	ESignedRational = 10
	};
	
// Related to TFormat. Stores size (in bytes) of each enumeration, i.e. EByte size is 1 byte, 
// EUnsignedRational size is 8 bytes etc etc.
const TInt KDataFormatSize[] = {0,1,1,2,4,8, 0,1, 0,4,8};
const TUint KDataFormatSizeLength = sizeof(KDataFormatSize)/sizeof(KDataFormatSize[0]); // The length of the KDataFormatsize array.

// Used by Ifd entry
const TUint KMinimumIfdEntrySize = 12; // The number of bytes in a standard ifd entry.
const TInt KSizeOfTagField = 2; // The number of bytes in the tag field.
const TInt KSizeOfFormatField = 2; // The number of bytes in format field.
const TInt KSizeOfComponentCountField = 4; // The number of bytes in the component count field.
const TUint KSizeOfValueOffsetField = 4; // The number of bytes in the value/offset field.
const TInt KSizeOfRational = 8; // The number of bytes in a rational number.
const TInt KTagFieldPosition = 0;
const TInt KFormatFieldPosition = 2; // The start position of the format field.
const TInt KComponentCountFieldPosition = 4; // The start position of the component count field.
const TInt KValueOffsetFieldPosition = 8; // The start position of the value/offset field.
const TUint8 KThreeBytes = 3;

#endif // __EXIFGENERALCONSTS_H__
