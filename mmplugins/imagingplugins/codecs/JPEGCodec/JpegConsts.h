// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __JPEGCONSTS_H__
#define __JPEGCONSTS_H__

	
// Constants.
const TUint16 KJpgMarker = 0xff00;
const TUint16 KJpgMarkerMask = 0xff00;
const TUint16 KJpgSOISignature = 0xffd8;
const TUint16 KJpgApp0Signature = 0xffe0;
const TUint16 KJpgApp1Signature = 0xffe1;
const TUint16 KJpgAppESignature = 0xffee;
const TUint16 KJpgDHTSignature = 0xffc4;
const TUint16 KJpgDQTSignature = 0xffdb;
const TUint16 KJpgBaselineDCTSOFSignature = 0xffc0;
const TUint16 KJpgExtendedDCTSOFSignature = 0xffc1;
const TUint16 KJpgProgressiveDCTSOFSignature = 0xffc2;
const TUint16 KJpgLosslessDCTSOFSignature = 0xffc3;
const TUint16 KJpgSOSSignature = 0xffda;
const TUint16 KJpgRestartIntervalSignature = 0xffdd;
const TUint16 KJpgEOISignature = 0xffd9;
const TUint16 KJpgCommentSignature = 0xfffe;
const TUint16 KJpgAppSignatureMask = 0xfff0;
const TInt KJpgPixelRatio = 1;
const TUint8  KJpgMarkerByte = ( KJpgMarker >> 8 );

const TInt KJpgHuffmanLookAhead = 8;
const TInt KJpgHuffmanMaxCodeSize = 16;
const TUint32 KJpgHuffmanLookAheadMask = (1<<KJpgHuffmanLookAhead)-1;

_LIT8(KJfifId,"JFIF");
_LIT8(KJfifExtId,"JFXX");
_LIT8(KAdobeId,"Adobe");
const TInt KJpgApp0IdSize = 4;
const TInt KJpgAppEIdSize = 5;

const TUint16 KJfifVersion0100 = 0x0100;
const TUint16 KJfifVersion0101 = 0x0101;
const TUint16 KJfifVersion0102 = 0x0102;

const TInt KAdobeColorTransformYCbCr = 1;
const TInt KAdobeColorTransformRGB = 0;

const TInt KJfifInitialHeaderSize = 6;
const TInt KJfifApp0DataSize = 16;
const TInt KJfifHeaderBlockSizeInBytes = 4096;
const TInt KJfifDataBlockSizeInBytes = 1024*8; // 8K
const TInt KJfifLengthOffset = 4;
const TInt KJfifThumbnailXYOffset = 18;

const TInt KJpgNumberOfComponents = 3;
const TInt KJpgMinNumberOfComponents = 1;
const TInt KJpgMaxNumberOfTables = 4;
const TInt KJpgHuffmanTableBitsSize = 16;
const TInt KJpgHuffmanTableSize = 256;
const TInt KJpgDCTBlockWidth = 8;
const TInt KJpgDCTBlockHeight = 8;
const TInt KJpgDCTBlockSize = 64;
const TInt KJpgMinSampleFactor = 1;
const TInt KJpgMaxSampleFactor = 4;
const TInt KJpgZeroRunValue = 0xf0;
const TInt KJpgMaxNumOfDataUnits = 6;
const TInt KJpgMonochromeDataUnitCount = 1;
const TInt KJpgEColor444DataUnitCount = 3;
const TInt KJpgColor422DataUnitCount = 4;
const TInt KJpgColor420DataUnitCount = 6;

const TUint KRotation90DegreesClockwise = 0x01;
const TUint KRotation180DegreesClockwise = 0x02;
const TUint KRotation270DegreesClockwise = 0x04;
const TUint KMirrorHorizontalAxis = 0x08;
const TUint KMirrorVerticalAxis = 0x10;

const TUint KJpgEncoderCapabilities = 0x1F;

class TAlignedZigZag
    {
public:
    union 
        {
        TUint8  iZigZag[KJpgDCTBlockSize];
        TUint64 iAligment;    
        };
    };

extern const TAlignedZigZag KZigZagSequence;
	
const TUint KJPEGInterchangeFormat = 0x0201;
const TUint KJPEGInterchangeFormatLength = 0x0202;

const TInt KYComp = 0;	// index of Y component
const TInt KUComp = 1;	// index of U component
const TInt KVComp = 2;	// index of V component

/**
Capabilities for different color sampling schemes.
*/
const TInt KSamplingMonoMCUWidthInPixels = 8;
const TInt KSamplingMonoMCUHeightInPixels = 8;
/**
The optimal input pixel buffer width was calculated emprically by determining the average number of compressed MCUs that can fit in the 4096(KMaxDstBufferSize) encoded 
destination buffer provided by framework.
The calculation is roughly: (KMaxDstBufferSize) / (Size of Image in bytes/(MCU count x * MCU count y))
*/

const TInt KSampling422MCUWidthInPixels = 16;
const TInt KSampling422MCUHeightInPixels = 8;

const TInt KSampling420MCUWidthInPixels = 16;
const TInt KSampling420MCUHeightInPixels = 16;

const TInt KOptimalBlockSize = 256;

#if defined(__ARMCC__)
#define FORCEDINLINE __forceinline
#else
#define FORCEDINLINE inline
#endif

#endif // __JPEGCONSTS_H__
