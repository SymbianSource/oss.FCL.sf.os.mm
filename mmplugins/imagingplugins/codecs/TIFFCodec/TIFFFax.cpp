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

#include "TIFFFax.h"

const TInt KMaxHuffmanCodeLength = 13;
LOCAL_C inline TUint32 GetMostSignificantBits(TUint32 aBits,TInt aNumBits)
	{
	return(aBits>>(32-aNumBits));
	}


// CTiffFax1dDecoder.
CTiffFax1dDecoder::~CTiffFax1dDecoder()
	{
	delete []iRunData;
	delete iImageProcessor;
	}

void CTiffFax1dDecoder::ConstructL(const TTiffImageData& aImageData)
	{
	iImageData = &aImageData;

	if((iImageData->iBitsPerSample>1) || (iImageData->iSamplesPerPixel>1))
		User::Leave(KErrNotSupported);

	if(iImageData->iFillOrder==2)
		iReverseBitsInByte = ETrue;

	// note officially should check for iImageData->iPhotometricInterpretation==EBlackIsZero
	// and reverse black, white interpretation, but some TIFF writers break this rule and other
	// readers ignore it
	iBlack = KRgbBlack;
	iWhite = KRgbWhite;

	iRowWidth = iImageData->iSizeInPixels.iWidth;
	iRowNumber = 0;
	iRunData = new (ELeave) TInt[iRowWidth+2];
	}

void CTiffFax1dDecoder::DoNewFrameL(CFbsBitmap& aBitmap, TBool aDisableErrorDiffusion, TInt aReductionFactor)
	{
	delete iImageProcessor;
	iImageProcessor = NULL;

	iImageProcessor = ImageProcessorUtility::NewImageProcessorL(aBitmap, aReductionFactor, EGray2, aDisableErrorDiffusion);
	iImageProcessor->PrepareL(aBitmap,iImageData->iSizeInPixels);
	}

void CTiffFax1dDecoder::NewStripL(TInt aNumBytes)
	{
	if(!aNumBytes)
		aNumBytes = KMaxTInt;

	iRowsRemaining = iImageData->iRowsPerStrip;

	TInt maxRows = iImageData->iSizeInPixels.iHeight-iRowNumber;
	if( (iRowsRemaining>maxRows) || (iRowsRemaining<=0) )
		iRowsRemaining = maxRows;

	iPixelsStoredInRow = 0;

	iBitCache = 0;
	iBitCacheLength = 0;

	iCodeType = ETiffFaxWhiteRun;

	CTiffFax1dDecoder::NewRow();
	}

void CTiffFax1dDecoder::NewRow()
	{
	iColor = ETiffFaxWhiteRun;
	iRunDataIndex = 0;
	iRunData[0] = 0;
	}

TBool CTiffFax1dDecoder::ProcessStripL(TBufPtr8* aSrc)
	{
	const TUint8* sourceBuffer = aSrc->Ptr();
	iSourcePtr = sourceBuffer;
	iSourcePtrLimit = iSourcePtr+aSrc->Length();

	UseBits(0);

	FOREVER
		{
		switch(ProcessNextCode())
			{
		case ETiffFaxUnderflow:
			aSrc->Shift(iSourcePtr-sourceBuffer);
			return(EFalse);

		case ETiffFaxInvalid:
			User::Leave(KErrCorrupt);

		case ETiffFaxEndOfLine:
			if(iPixelsStoredInRow==0)
				continue;

			if(iPixelsStoredInRow!=iRowWidth)
				User::Leave(KErrCorrupt);

			NewRow();

			iPixelsStoredInRow = 0;
			iRowNumber++;
			iRowsRemaining--;
			continue;

		case ETiffFaxPixelRun:
			if(!iCodeCount)
				continue;

			{
			TInt n = iPixelsStoredInRow+iCodeCount;

			if(n>iRowWidth)
				User::Leave(KErrCorrupt);

			if(iCodeType==ETiffFaxBlackRun)
				{
				iImageProcessor->SetMonoPixelRun(0,iCodeCount);

				if(!(iRunDataIndex&1))
					iRunDataIndex++;
				}
			else
				{
				iImageProcessor->SetMonoPixelRun(255,iCodeCount);

				if(iRunDataIndex&1)
					iRunDataIndex++;
				}

			iPixelsStoredInRow = n;
			iRunData[iRunDataIndex] = n;

			if((n!=iRowWidth) || (iRowsRemaining!=1))	// Continue if not last pixel of image
				continue;
			}

			// else drop through....

		case ETiffFaxEndOfFax:
			iImageProcessor->FlushPixels();
			return(ETrue);
			}
		}
	}

LOCAL_D const TUint8 KByteFlip[256] =
	{
	0x00,0x80,0x40,0xc0,0x20,0xa0,0x60,0xe0,0x10,0x90,0x50,0xd0,0x30,0xb0,0x70,0xf0,
	0x08,0x88,0x48,0xc8,0x28,0xa8,0x68,0xe8,0x18,0x98,0x58,0xd8,0x38,0xb8,0x78,0xf8,
	0x04,0x84,0x44,0xc4,0x24,0xa4,0x64,0xe4,0x14,0x94,0x54,0xd4,0x34,0xb4,0x74,0xf4,
	0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,0x5c,0xdc,0x3c,0xbc,0x7c,0xfc,
	0x02,0x82,0x42,0xc2,0x22,0xa2,0x62,0xe2,0x12,0x92,0x52,0xd2,0x32,0xb2,0x72,0xf2,
	0x0a,0x8a,0x4a,0xca,0x2a,0xaa,0x6a,0xea,0x1a,0x9a,0x5a,0xda,0x3a,0xba,0x7a,0xfa,
	0x06,0x86,0x46,0xc6,0x26,0xa6,0x66,0xe6,0x16,0x96,0x56,0xd6,0x36,0xb6,0x76,0xf6,
	0x0e,0x8e,0x4e,0xce,0x2e,0xae,0x6e,0xee,0x1e,0x9e,0x5e,0xde,0x3e,0xbe,0x7e,0xfe,
	0x01,0x81,0x41,0xc1,0x21,0xa1,0x61,0xe1,0x11,0x91,0x51,0xd1,0x31,0xb1,0x71,0xf1,
	0x09,0x89,0x49,0xc9,0x29,0xa9,0x69,0xe9,0x19,0x99,0x59,0xd9,0x39,0xb9,0x79,0xf9,
	0x05,0x85,0x45,0xc5,0x25,0xa5,0x65,0xe5,0x15,0x95,0x55,0xd5,0x35,0xb5,0x75,0xf5,
	0x0d,0x8d,0x4d,0xcd,0x2d,0xad,0x6d,0xed,0x1d,0x9d,0x5d,0xdd,0x3d,0xbd,0x7d,0xfd,
	0x03,0x83,0x43,0xc3,0x23,0xa3,0x63,0xe3,0x13,0x93,0x53,0xd3,0x33,0xb3,0x73,0xf3,
	0x0b,0x8b,0x4b,0xcb,0x2b,0xab,0x6b,0xeb,0x1b,0x9b,0x5b,0xdb,0x3b,0xbb,0x7b,0xfb,
	0x07,0x87,0x47,0xc7,0x27,0xa7,0x67,0xe7,0x17,0x97,0x57,0xd7,0x37,0xb7,0x77,0xf7,
	0x0f,0x8f,0x4f,0xcf,0x2f,0xaf,0x6f,0xef,0x1f,0x9f,0x5f,0xdf,0x3f,0xbf,0x7f,0xff
	};

void CTiffFax1dDecoder::UseBits(TInt aNumBits)
	{
	ASSERT(aNumBits<=iBitCacheLength);

	TInt		validBits = iBitCacheLength-aNumBits;
	TUint32		bits = iBitCache<<aNumBits;

	if(validBits>=KMaxHuffmanCodeLength)
		{
		iBitCache = bits;
		iBitCacheLength = validBits;
		return;
		}

	TInt shift = 32-validBits;
	const TUint8* ptr = iSourcePtr;

	TInt numBytes = shift>>3;
	TInt maxBytes = iSourcePtrLimit-ptr;
	if(numBytes>maxBytes)
		numBytes = maxBytes;

	iBitCacheLength = validBits+numBytes*8;

	const TUint8* ptrLimit = ptr+numBytes;

	if(iReverseBitsInByte)
		{
		while(ptr<ptrLimit)
			{
			TInt byte = *(ptr++);
			byte = KByteFlip[byte]; // reverse bits within byte
			shift -= 8;
			bits |= byte<<shift;
			}
		}
	else
		{
		while(ptr<ptrLimit)
			{
			TInt byte = *(ptr++);
			shift -= 8;
			bits |= byte<<shift;
			}
	}

	iBitCache = bits;
	iSourcePtr = ptr;

	return;
	}

TTiffFaxState CTiffFax1dDecoder::Decode1d()
	{
	if(iBitCache&0xff000000)
		{
		if(iColor==ETiffFaxWhiteRun)
			return(DecodeWhite());
		else
			return(DecodeBlack());
		}
	else
		return(DecodeEndOfLine());
	}

#define HUFFMAN_DECODE_ENTRY(aCount,aNumBits) (aCount<<4)+aNumBits
LOCAL_D const TUint16 White0000[16] =
	{
		HUFFMAN_DECODE_ENTRY(0		,0),
		HUFFMAN_DECODE_ENTRY(0		,0),
		HUFFMAN_DECODE_ENTRY(29		,8),
		HUFFMAN_DECODE_ENTRY(30		,8),
		HUFFMAN_DECODE_ENTRY(45		,8),
		HUFFMAN_DECODE_ENTRY(46		,8),
		HUFFMAN_DECODE_ENTRY(22		,7),
		HUFFMAN_DECODE_ENTRY(22		,7),

		HUFFMAN_DECODE_ENTRY(23		,7),
		HUFFMAN_DECODE_ENTRY(23		,7),
		HUFFMAN_DECODE_ENTRY(47		,8),
		HUFFMAN_DECODE_ENTRY(48		,8),
		HUFFMAN_DECODE_ENTRY(13		,6),
		HUFFMAN_DECODE_ENTRY(13		,6),
		HUFFMAN_DECODE_ENTRY(13		,6),
		HUFFMAN_DECODE_ENTRY(13		,6)
	};

LOCAL_D const TUint16 White0001[16] =
	{
		HUFFMAN_DECODE_ENTRY(20		,7),
		HUFFMAN_DECODE_ENTRY(20		,7),
		HUFFMAN_DECODE_ENTRY(33		,8),
		HUFFMAN_DECODE_ENTRY(34		,8),
		HUFFMAN_DECODE_ENTRY(35		,8),
		HUFFMAN_DECODE_ENTRY(36		,8),
		HUFFMAN_DECODE_ENTRY(37		,8),
		HUFFMAN_DECODE_ENTRY(38		,8),

		HUFFMAN_DECODE_ENTRY(19		,7),
		HUFFMAN_DECODE_ENTRY(19		,7),
		HUFFMAN_DECODE_ENTRY(31		,8),
		HUFFMAN_DECODE_ENTRY(32		,8),
		HUFFMAN_DECODE_ENTRY(1		,6),
		HUFFMAN_DECODE_ENTRY(1		,6),
		HUFFMAN_DECODE_ENTRY(1		,6),
		HUFFMAN_DECODE_ENTRY(1		,6)
	};

LOCAL_D const TUint16 White0010[16] =
	{
		HUFFMAN_DECODE_ENTRY(12		,6),
		HUFFMAN_DECODE_ENTRY(12		,6),
		HUFFMAN_DECODE_ENTRY(12		,6),
		HUFFMAN_DECODE_ENTRY(12		,6),
		HUFFMAN_DECODE_ENTRY(53		,8),
		HUFFMAN_DECODE_ENTRY(54		,8),
		HUFFMAN_DECODE_ENTRY(26		,7),
		HUFFMAN_DECODE_ENTRY(26		,7),

		HUFFMAN_DECODE_ENTRY(39		,8),
		HUFFMAN_DECODE_ENTRY(40		,8),
		HUFFMAN_DECODE_ENTRY(41		,8),
		HUFFMAN_DECODE_ENTRY(42		,8),
		HUFFMAN_DECODE_ENTRY(43		,8),
		HUFFMAN_DECODE_ENTRY(44		,8),
		HUFFMAN_DECODE_ENTRY(21		,7),
		HUFFMAN_DECODE_ENTRY(21		,7)
	};

LOCAL_D const TUint16 White0011[16] =
	{
		HUFFMAN_DECODE_ENTRY(28		,7),
		HUFFMAN_DECODE_ENTRY(28		,7),
		HUFFMAN_DECODE_ENTRY(61		,8),
		HUFFMAN_DECODE_ENTRY(62		,8),
		HUFFMAN_DECODE_ENTRY(63		,8),
		HUFFMAN_DECODE_ENTRY(0		,8),
		HUFFMAN_DECODE_ENTRY(320	,8),
		HUFFMAN_DECODE_ENTRY(384	,8),

		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5),
		HUFFMAN_DECODE_ENTRY(10		,5)
	};

LOCAL_D const TUint16 White0100[32] =
	{
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),

		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),
		HUFFMAN_DECODE_ENTRY(11		,5),

		HUFFMAN_DECODE_ENTRY(27		,7),
		HUFFMAN_DECODE_ENTRY(27		,7),
		HUFFMAN_DECODE_ENTRY(27		,7),
		HUFFMAN_DECODE_ENTRY(27		,7),
		HUFFMAN_DECODE_ENTRY(59		,8),
		HUFFMAN_DECODE_ENTRY(59		,8),
		HUFFMAN_DECODE_ENTRY(60		,8),
		HUFFMAN_DECODE_ENTRY(60		,8),

		HUFFMAN_DECODE_ENTRY(1472	,9),
		HUFFMAN_DECODE_ENTRY(1536	,9),
		HUFFMAN_DECODE_ENTRY(1600	,9),
		HUFFMAN_DECODE_ENTRY(1728	,9),
		HUFFMAN_DECODE_ENTRY(18		,7),
		HUFFMAN_DECODE_ENTRY(18		,7),
		HUFFMAN_DECODE_ENTRY(18		,7),
		HUFFMAN_DECODE_ENTRY(18		,7)
	};

LOCAL_D const TUint16 White0101[16] =
	{
		HUFFMAN_DECODE_ENTRY(24		,7),
		HUFFMAN_DECODE_ENTRY(24		,7),
		HUFFMAN_DECODE_ENTRY(49		,8),
		HUFFMAN_DECODE_ENTRY(50		,8),
		HUFFMAN_DECODE_ENTRY(51		,8),
		HUFFMAN_DECODE_ENTRY(52		,8),
		HUFFMAN_DECODE_ENTRY(25		,7),
		HUFFMAN_DECODE_ENTRY(25		,7),

		HUFFMAN_DECODE_ENTRY(55		,8),
		HUFFMAN_DECODE_ENTRY(56		,8),
		HUFFMAN_DECODE_ENTRY(57		,8),
		HUFFMAN_DECODE_ENTRY(58		,8),
		HUFFMAN_DECODE_ENTRY(192	,6),
		HUFFMAN_DECODE_ENTRY(192	,6),
		HUFFMAN_DECODE_ENTRY(192	,6),
		HUFFMAN_DECODE_ENTRY(192	,6)
	};

LOCAL_D const TUint16 White0110[32] =
	{
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),
		HUFFMAN_DECODE_ENTRY(1664	,6),

		HUFFMAN_DECODE_ENTRY(448	,8),
		HUFFMAN_DECODE_ENTRY(448	,8),
		HUFFMAN_DECODE_ENTRY(512	,8),
		HUFFMAN_DECODE_ENTRY(512	,8),
		HUFFMAN_DECODE_ENTRY(704	,9),
		HUFFMAN_DECODE_ENTRY(768	,9),
		HUFFMAN_DECODE_ENTRY(640	,8),
		HUFFMAN_DECODE_ENTRY(640	,8),

		HUFFMAN_DECODE_ENTRY(576	,8),
		HUFFMAN_DECODE_ENTRY(576	,8),
		HUFFMAN_DECODE_ENTRY(832	,9),
		HUFFMAN_DECODE_ENTRY(896	,9),
		HUFFMAN_DECODE_ENTRY(960	,9),
		HUFFMAN_DECODE_ENTRY(1024	,9),
		HUFFMAN_DECODE_ENTRY(1088	,9),
		HUFFMAN_DECODE_ENTRY(1152	,9),

		HUFFMAN_DECODE_ENTRY(1216	,9),
		HUFFMAN_DECODE_ENTRY(1280	,9),
		HUFFMAN_DECODE_ENTRY(1344	,9),
		HUFFMAN_DECODE_ENTRY(1408	,9),
		HUFFMAN_DECODE_ENTRY(256	,7),
		HUFFMAN_DECODE_ENTRY(256	,7),
		HUFFMAN_DECODE_ENTRY(256	,7),
		HUFFMAN_DECODE_ENTRY(256	,7)
	};

LOCAL_D const TUint16 White0111[1] =
	{
		HUFFMAN_DECODE_ENTRY(2		,4)
	};

LOCAL_D const TUint16 White1000[1] =
	{
		HUFFMAN_DECODE_ENTRY(3		,4)
	};

LOCAL_D const TUint16 White1001[2] =
	{
		HUFFMAN_DECODE_ENTRY(128	,5),
		HUFFMAN_DECODE_ENTRY(8		,5)
	};

LOCAL_D const TUint16 White1010[4] =
	{
		HUFFMAN_DECODE_ENTRY(9		,5),
		HUFFMAN_DECODE_ENTRY(9		,5),
		HUFFMAN_DECODE_ENTRY(16		,6),
		HUFFMAN_DECODE_ENTRY(17		,6)
	};

LOCAL_D const TUint16 White1011[1] =
	{
		HUFFMAN_DECODE_ENTRY(4		,4)
	};

LOCAL_D const TUint16 White1100[1] =
	{
		HUFFMAN_DECODE_ENTRY(5		,4)
	};

LOCAL_D const TUint16 White1101[4] =
	{
		HUFFMAN_DECODE_ENTRY(14		,6),
		HUFFMAN_DECODE_ENTRY(15		,6),
		HUFFMAN_DECODE_ENTRY(64		,5),
		HUFFMAN_DECODE_ENTRY(64		,5)
	};

LOCAL_D const TUint16 White1110[1] =
	{
		HUFFMAN_DECODE_ENTRY(6		,4)
	};

LOCAL_D const TUint16 White1111[1] =
	{
		HUFFMAN_DECODE_ENTRY(7		,4)
	};

LOCAL_D const TUint16 Black0001[4] =
	{
		HUFFMAN_DECODE_ENTRY(9		,6),
		HUFFMAN_DECODE_ENTRY(8		,6),
		HUFFMAN_DECODE_ENTRY(7		,5),
		HUFFMAN_DECODE_ENTRY(7		,5)
	};

LOCAL_D const TUint16 Black0010[1] =
	{
		HUFFMAN_DECODE_ENTRY(6		,4)
	};

LOCAL_D const TUint16 Black0011[1] =
	{
		HUFFMAN_DECODE_ENTRY(5		,4)
	};

LOCAL_D const TUint16 Black010x[1] =
	{
		HUFFMAN_DECODE_ENTRY(1		,3)
	};

LOCAL_D const TUint16 Black011x[1] =
	{
		HUFFMAN_DECODE_ENTRY(4		,3)
	};

LOCAL_D const TUint16 Black10xx[1] =
	{
		HUFFMAN_DECODE_ENTRY(3		,2)
	};

LOCAL_D const TUint16 Black11xx[1] =
	{
		HUFFMAN_DECODE_ENTRY(2		,2)
	};

LOCAL_D const TUint16 Black0000_0010[32] =
	{
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),
		HUFFMAN_DECODE_ENTRY(18		,10),

		HUFFMAN_DECODE_ENTRY(52		,12),
		HUFFMAN_DECODE_ENTRY(52		,12),
		HUFFMAN_DECODE_ENTRY(640	,13),
		HUFFMAN_DECODE_ENTRY(704	,13),
		HUFFMAN_DECODE_ENTRY(768	,13),
		HUFFMAN_DECODE_ENTRY(832	,13),
		HUFFMAN_DECODE_ENTRY(55		,12),
		HUFFMAN_DECODE_ENTRY(55		,12),

		HUFFMAN_DECODE_ENTRY(56		,12),
		HUFFMAN_DECODE_ENTRY(56		,12),
		HUFFMAN_DECODE_ENTRY(1280	,13),
		HUFFMAN_DECODE_ENTRY(1344	,13),
		HUFFMAN_DECODE_ENTRY(1408	,13),
		HUFFMAN_DECODE_ENTRY(1472	,13),
		HUFFMAN_DECODE_ENTRY(59		,12),
		HUFFMAN_DECODE_ENTRY(59		,12),

		HUFFMAN_DECODE_ENTRY(60		,12),
		HUFFMAN_DECODE_ENTRY(60		,12),
		HUFFMAN_DECODE_ENTRY(1536	,13),
		HUFFMAN_DECODE_ENTRY(1600	,13),
		HUFFMAN_DECODE_ENTRY(24		,11),
		HUFFMAN_DECODE_ENTRY(24		,11),
		HUFFMAN_DECODE_ENTRY(24		,11),
		HUFFMAN_DECODE_ENTRY(24		,11)
	};

LOCAL_D const TUint16 Black0000_0011[32] =
	{
		HUFFMAN_DECODE_ENTRY(25		,11),
		HUFFMAN_DECODE_ENTRY(25		,11),
		HUFFMAN_DECODE_ENTRY(25		,11),
		HUFFMAN_DECODE_ENTRY(25		,11),
		HUFFMAN_DECODE_ENTRY(1664	,13),
		HUFFMAN_DECODE_ENTRY(1728	,13),
		HUFFMAN_DECODE_ENTRY(320	,12),
		HUFFMAN_DECODE_ENTRY(320	,12),

		HUFFMAN_DECODE_ENTRY(384	,12),
		HUFFMAN_DECODE_ENTRY(384	,12),
		HUFFMAN_DECODE_ENTRY(448	,12),
		HUFFMAN_DECODE_ENTRY(448	,12),
		HUFFMAN_DECODE_ENTRY(512	,13),
		HUFFMAN_DECODE_ENTRY(576	,13),
		HUFFMAN_DECODE_ENTRY(53		,12),
		HUFFMAN_DECODE_ENTRY(53		,12),

		HUFFMAN_DECODE_ENTRY(54		,12),
		HUFFMAN_DECODE_ENTRY(54		,12),
		HUFFMAN_DECODE_ENTRY(896	,13),
		HUFFMAN_DECODE_ENTRY(960	,13),
		HUFFMAN_DECODE_ENTRY(1024	,13),
		HUFFMAN_DECODE_ENTRY(1088	,13),
		HUFFMAN_DECODE_ENTRY(1152	,13),
		HUFFMAN_DECODE_ENTRY(1216	,13),

		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10),
		HUFFMAN_DECODE_ENTRY(64		,10)
	};

LOCAL_D const TUint16 Black0000_0100[1] =
	{
		HUFFMAN_DECODE_ENTRY(13		,8)
	};

LOCAL_D const TUint16 Black0000_0101[16] =
	{
		HUFFMAN_DECODE_ENTRY(23		,11),
		HUFFMAN_DECODE_ENTRY(23		,11),
		HUFFMAN_DECODE_ENTRY(50		,12),
		HUFFMAN_DECODE_ENTRY(51		,12),
		HUFFMAN_DECODE_ENTRY(44		,12),
		HUFFMAN_DECODE_ENTRY(45		,12),
		HUFFMAN_DECODE_ENTRY(46		,12),
		HUFFMAN_DECODE_ENTRY(47		,12),

		HUFFMAN_DECODE_ENTRY(57		,12),
		HUFFMAN_DECODE_ENTRY(58		,12),
		HUFFMAN_DECODE_ENTRY(61		,12),
		HUFFMAN_DECODE_ENTRY(256	,12),
		HUFFMAN_DECODE_ENTRY(16		,10),
		HUFFMAN_DECODE_ENTRY(16		,10),
		HUFFMAN_DECODE_ENTRY(16		,10),
		HUFFMAN_DECODE_ENTRY(16		,10)
	};

LOCAL_D const TUint16 Black0000_0110[16] =
	{
		HUFFMAN_DECODE_ENTRY(17		,10),
		HUFFMAN_DECODE_ENTRY(17		,10),
		HUFFMAN_DECODE_ENTRY(17		,10),
		HUFFMAN_DECODE_ENTRY(17		,10),
		HUFFMAN_DECODE_ENTRY(48		,12),
		HUFFMAN_DECODE_ENTRY(49		,12),
		HUFFMAN_DECODE_ENTRY(62		,12),
		HUFFMAN_DECODE_ENTRY(63		,12),

		HUFFMAN_DECODE_ENTRY(30		,12),
		HUFFMAN_DECODE_ENTRY(31		,12),
		HUFFMAN_DECODE_ENTRY(32		,12),
		HUFFMAN_DECODE_ENTRY(33		,12),
		HUFFMAN_DECODE_ENTRY(40		,12),
		HUFFMAN_DECODE_ENTRY(41		,12),
		HUFFMAN_DECODE_ENTRY(22		,11),
		HUFFMAN_DECODE_ENTRY(22		,11)
	};

LOCAL_D const TUint16 Black0000_0111[1] =
	{
		HUFFMAN_DECODE_ENTRY(14		,8)
	};

LOCAL_D const TUint16 Black0000_100x[1] =
	{
		HUFFMAN_DECODE_ENTRY(10		,7)
	};

LOCAL_D const TUint16 Black0000_101x[1] =
	{
		HUFFMAN_DECODE_ENTRY(11		,7)
	};

LOCAL_D const TUint16 Black0000_1100[16] =
	{
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),
		HUFFMAN_DECODE_ENTRY(15		,9),

		HUFFMAN_DECODE_ENTRY(128	,12),
		HUFFMAN_DECODE_ENTRY(192	,12),
		HUFFMAN_DECODE_ENTRY(26		,12),
		HUFFMAN_DECODE_ENTRY(27		,12),
		HUFFMAN_DECODE_ENTRY(28		,12),
		HUFFMAN_DECODE_ENTRY(29		,12),
		HUFFMAN_DECODE_ENTRY(19		,11),
		HUFFMAN_DECODE_ENTRY(19		,11)
	};

LOCAL_D const TUint16 Black0000_1101[16] =
	{
		HUFFMAN_DECODE_ENTRY(20		,11),
		HUFFMAN_DECODE_ENTRY(20		,11),
		HUFFMAN_DECODE_ENTRY(34		,12),
		HUFFMAN_DECODE_ENTRY(35		,12),
		HUFFMAN_DECODE_ENTRY(36		,12),
		HUFFMAN_DECODE_ENTRY(37		,12),
		HUFFMAN_DECODE_ENTRY(38		,12),
		HUFFMAN_DECODE_ENTRY(39		,12),

		HUFFMAN_DECODE_ENTRY(21		,11),
		HUFFMAN_DECODE_ENTRY(21		,11),
		HUFFMAN_DECODE_ENTRY(42		,12),
		HUFFMAN_DECODE_ENTRY(43		,12),
		HUFFMAN_DECODE_ENTRY(0		,10),
		HUFFMAN_DECODE_ENTRY(0		,10),
		HUFFMAN_DECODE_ENTRY(0		,10),
		HUFFMAN_DECODE_ENTRY(0		,10)
	};

LOCAL_D const TUint16 Black0000_111x[1] =
	{
		HUFFMAN_DECODE_ENTRY(12		,7)
	};

LOCAL_D const TUint16 Common0000_0001[16] =
	{
		HUFFMAN_DECODE_ENTRY(1792	,11),
		HUFFMAN_DECODE_ENTRY(1792	,11),
		HUFFMAN_DECODE_ENTRY(1984	,12),
		HUFFMAN_DECODE_ENTRY(2048	,12),
		HUFFMAN_DECODE_ENTRY(2112	,12),
		HUFFMAN_DECODE_ENTRY(2176	,12),
		HUFFMAN_DECODE_ENTRY(2240	,12),
		HUFFMAN_DECODE_ENTRY(2304	,12),
		HUFFMAN_DECODE_ENTRY(1856	,11),
		HUFFMAN_DECODE_ENTRY(1856	,11),
		HUFFMAN_DECODE_ENTRY(1920	,11),
		HUFFMAN_DECODE_ENTRY(1920	,11),
		HUFFMAN_DECODE_ENTRY(2368	,12),
		HUFFMAN_DECODE_ENTRY(2432	,12),
		HUFFMAN_DECODE_ENTRY(2496	,12),
		HUFFMAN_DECODE_ENTRY(2560	,12)
	};

class THuffmanTable
	{
public:
	const TUint16*	iCodes;
	TInt			iIndexBits;	// Number of bits to use for indexing into iCodes
	};

LOCAL_D const THuffmanTable WhiteTables[32] =
	{
		{White0000,4},
		{White0001,4},
		{White0010,4},
		{White0011,4},
		{White0100,5},
		{White0101,4},
		{White0110,5},
		{White0111,0},
		{White1000,0},	
		{White1001,1},	
		{White1010,2},	
		{White1011,0},	
		{White1100,0},	
		{White1101,2},
		{White1110,0},	
		{White1111,0},	
	};

LOCAL_D const THuffmanTable BlackTables[16] =
	{
		{NULL,0},
		{Black0001,2},
		{Black0010,0},
		{Black0011,0},
		{Black010x,0},
		{Black010x,0},
		{Black011x,0},
		{Black011x,0},
		{Black10xx,0},
		{Black10xx,0},
		{Black10xx,0},
		{Black10xx,0},
		{Black11xx,0},
		{Black11xx,0},
		{Black11xx,0},
		{Black11xx,0},
	};

LOCAL_D const THuffmanTable BlackTables0000[16] =
	{
		{NULL,0},
		{Common0000_0001,4},
		{Black0000_0010,5},
		{Black0000_0011,5},
		{Black0000_0100,0},
		{Black0000_0101,4},
		{Black0000_0110,4},
		{Black0000_0111,0},
		{Black0000_100x,0},
		{Black0000_100x,0},
		{Black0000_101x,0},
		{Black0000_101x,0},
		{Black0000_1100,4},
		{Black0000_1101,4},
		{Black0000_111x,0},
		{Black0000_111x,0},
	};

TTiffFaxState CTiffFax1dDecoder::DecodeWhite()
	{
	TUint32	bits = iBitCache;
	TUint16	code;

	if(bits>=0x02000000)
		{
		const THuffmanTable* table = &WhiteTables[GetMostSignificantBits(bits,4)];
		bits &= ~0xf0000000; // zero the 4 most significant bits
		code  = table->iCodes[GetMostSignificantBits(bits,4+table->iIndexBits)];
		}
	else if(bits>=0x01000000)
		{
		bits &= ~0xff000000; // zero the 8 most significant bits
		code = Common0000_0001[GetMostSignificantBits(bits,8+4)];
		}
	else
		return(ETiffFaxInvalid);

	TInt numBits = code&15;

	if(numBits>iBitCacheLength)
		return(ETiffFaxUnderflow);

	UseBits(numBits);

	iCodeType = ETiffFaxWhiteRun;
	iCodeCount = code>>4;

	if(iCodeCount<64)
		iColor = ETiffFaxBlackRun;

	return(ETiffFaxPixelRun);
	}

TTiffFaxState CTiffFax1dDecoder::DecodeBlack()
	{
	TUint32	bits = iBitCache;
	TUint16	code;

	if(bits&0xf0000000)
		{
		const THuffmanTable* table = &BlackTables[GetMostSignificantBits(bits,4)];
		bits &= ~0xf0000000; // zero the 4 most significant bits
		code  = table->iCodes[GetMostSignificantBits(bits,4+table->iIndexBits)];
		}
	else if(bits&0xff000000)
		{
		const THuffmanTable* table = &BlackTables0000[GetMostSignificantBits(bits,8)];
		bits &= ~0xff000000; // zero the 8 most significant bits
		code  = table->iCodes[GetMostSignificantBits(bits,8+table->iIndexBits)];
		}
	else
		return(ETiffFaxInvalid);

	TInt numBits = code&15;

	if(numBits>iBitCacheLength)
		return(ETiffFaxUnderflow);

	UseBits(numBits);

	iCodeType = ETiffFaxBlackRun;
	iCodeCount = code>>4;

	if(iCodeCount<64)
		iColor = ETiffFaxWhiteRun;

	return(ETiffFaxPixelRun);
	}

TTiffFaxState CTiffFax1dDecoder::DecodeEndOfLine()
	{
	if(iBitCache>=0x00200000)
		return(ETiffFaxInvalid);

	FOREVER
		{
		if(iBitCacheLength<12)
			return(ETiffFaxUnderflow);
		if(iBitCache&0x00100000)
			break;
		UseBits(1);
		}

	UseBits(12);

	iColor = ETiffFaxWhiteRun;

	iCodeCount = 1;

	if(iCodeType==ETiffFaxEndOfLineCode)
		return(ETiffFaxEndOfFax);

	iCodeType = ETiffFaxEndOfLineCode;
	return(ETiffFaxEndOfLine);
	}


// CTiffFax2dDecoder.
CTiffFax2dDecoder::~CTiffFax2dDecoder()
	{
	delete []iReferenceRunData;
	}

void CTiffFax2dDecoder::ConstructL(const TTiffImageData& aImageData)
	{
	CTiffFax1dDecoder::ConstructL(aImageData);
	iReferenceRunData = new (ELeave) TInt[iRowWidth+2];
	}

void CTiffFax2dDecoder::NewStripL(TInt aNumBytes)
	{
	CTiffFax1dDecoder::NewStripL(aNumBytes);
	CTiffFax2dDecoder::NewRow();
	}

void CTiffFax2dDecoder::NewRow()
	{
	if(iRunDataIndex&1)
		iRunDataIndex++;
	iRunData[iRunDataIndex] = iRowWidth;
	iRunData[iRunDataIndex+1] = iRowWidth;

	TInt* temp = iReferenceRunData;
	iReferenceRunData = iRunData;
	iRunData = temp;

	iReferenceRunDataIndex = 0;
	iHorizontalCodeCount = 0;
	iA0 = 0;

	CTiffFax1dDecoder::NewRow();
	}

TTiffFaxState CTiffFax2dDecoder::Decode2d()
	{
	if(iHorizontalCodeCount)
		return(DecodeHorizontal());

	TUint32	bits = iBitCache;
	TInt validBits;
	TInt verticalOffset;

	switch(GetMostSignificantBits(bits,3))
		{
	case 0:

		switch(GetMostSignificantBits(bits,7))
			{
		case 0:
		case 1:
			return(DecodeEndOfLine());
		case 2:
			verticalOffset = -3;
			validBits = 7;
			break;
		case 3:
			verticalOffset = 3;
			validBits = 7;
			break;
		case 4:
		case 5:
			verticalOffset = -2;
			validBits = 6;
			break;
		case 6:
		case 7:
			verticalOffset = 2;
			validBits = 6;
			break;
		default:
			if(iBitCacheLength<4)
				return(ETiffFaxUnderflow);
			UseBits(4);
			return(DecodePass());
			}

		break;

	case 1:
		if(iBitCacheLength<3)
			return(ETiffFaxUnderflow);
		UseBits(3);
		iHorizontalCodeCount = 2;
		return(DecodeHorizontal());

	case 2:

		verticalOffset = -1;
		validBits = 3;
		break;

	case 3:

		verticalOffset = 1;
		validBits = 3;
		break;

	default:

		verticalOffset = 0;
		validBits = 1;
		break;
		}

	if(validBits>iBitCacheLength)
		return(ETiffFaxUnderflow);

	UseBits(validBits);

	return(DecodeVertical(verticalOffset));
	}

TTiffFaxState CTiffFax2dDecoder::DecodeHorizontal()
	{
	TTiffFaxState returnValue;

	returnValue = Decode1d();
	if(returnValue!=ETiffFaxPixelRun)
		return(returnValue);

	iA0 += iCodeCount;

	if(iCodeCount<64)
		iHorizontalCodeCount--;

	if(iHorizontalCodeCount)
		return(ETiffFaxPixelRun);

	TInt n = iA0;
	if(n>=iRowWidth)
		n = iRowWidth-1;

	TInt i = iReferenceRunDataIndex;

	while(iReferenceRunData[i]<=n)
		i++;

	if(iColor==ETiffFaxBlackRun)
		{
		if(!(i&1))
			i++;
		}
	else
		{
		if(i&1)
			i++;
		}

	iReferenceRunDataIndex = i;

	return(ETiffFaxPixelRun);
	}

TTiffFaxState CTiffFax2dDecoder::DecodeVertical(TInt aDifference)
	{
	iA0 = iReferenceRunData[iReferenceRunDataIndex]+aDifference;

	iCodeCount = iA0-iPixelsStoredInRow;
	iCodeType = iColor;

	if(iColor==ETiffFaxWhiteRun)
		iColor = ETiffFaxBlackRun;
	else
		iColor = ETiffFaxWhiteRun;

	TInt n = iA0;
	if(n>=iRowWidth)
		n = iRowWidth-1;

	TInt i = iReferenceRunDataIndex;

	if(i)
		i--;
	else
		i++;

	while(iReferenceRunData[i]<=n)
		i += 2;

	iReferenceRunDataIndex = i;

	return(ETiffFaxPixelRun);
	}

TTiffFaxState CTiffFax2dDecoder::DecodePass()
	{
	iA0 = iReferenceRunData[iReferenceRunDataIndex+1];
	iReferenceRunDataIndex += 2;

	iCodeCount = iA0-iPixelsStoredInRow;
	iCodeType = iColor;

	return(ETiffFaxPixelRun);
	}


// CTiffGroup3Fax1dDecoder.
CTiffGroup3Fax1dDecoder* CTiffGroup3Fax1dDecoder::NewL(const TTiffImageData& aImageData)
	{
	if(aImageData.iT4Options & ~TTiffIfdEntry::ET4ContainsPaddedEols)
		User::Leave(KErrNotSupported);

	CTiffGroup3Fax1dDecoder* decoder = new (ELeave) CTiffGroup3Fax1dDecoder;

	CleanupStack::PushL(decoder);
	decoder->ConstructL(aImageData);
	CleanupStack::Pop();

	return(decoder);
	}

TTiffFaxState CTiffGroup3Fax1dDecoder::ProcessNextCode()
	{
	return(Decode1d());
	}


// CTiffGroup3Fax2dDecoder.
CTiffGroup3Fax2dDecoder* CTiffGroup3Fax2dDecoder::NewL(const TTiffImageData& aImageData)
	{
	if( (aImageData.iT4Options&~TTiffIfdEntry::ET4ContainsPaddedEols) != TTiffIfdEntry::ET4TwoDimentionalCoding )
		User::Leave(KErrNotSupported);

	CTiffGroup3Fax2dDecoder* decoder = new (ELeave) CTiffGroup3Fax2dDecoder;

	CleanupStack::PushL(decoder);
	decoder->ConstructL(aImageData);
	CleanupStack::Pop();

	return(decoder);
	}

void CTiffGroup3Fax2dDecoder::NewStripL(TInt aNumBytes)
	{
	CTiffFax2dDecoder::NewStripL(aNumBytes);
	iState = E1d;
	}

TTiffFaxState CTiffGroup3Fax2dDecoder::ProcessNextCode()
	{
	if(iState!=ESelect1d2d)
		{
		TTiffFaxState returnValue;

		if(iState==E1d)
			returnValue = Decode1d();
		else
			returnValue = Decode2d();

		if(returnValue!=ETiffFaxEndOfLine)
			return(returnValue);

		iState = ESelect1d2d;
		}
	
	if(!iBitCacheLength)
		return(ETiffFaxUnderflow);

	if(iBitCache&((TUint32)1<<31))
		iState = E1d;
	else
		iState = E2d;

	UseBits(1);

	return(ETiffFaxEndOfLine);
	}

void CTiffGroup3Fax2dDecoder::NewRow()
	{
	if(iState==E1d)
		CTiffFax1dDecoder::NewRow();
	else
		CTiffFax2dDecoder::NewRow();
	}


// CTiffGroup4FaxDecoder.
CTiffGroup4FaxDecoder* CTiffGroup4FaxDecoder::NewL(const TTiffImageData& aImageData)
	{
	if(aImageData.iT6Options!=0)
		User::Leave(KErrNotSupported);

	CTiffGroup4FaxDecoder* decoder = new (ELeave) CTiffGroup4FaxDecoder;

	CleanupStack::PushL(decoder);
	decoder->ConstructL(aImageData);
	CleanupStack::Pop();

	return(decoder);
	}

TTiffFaxState CTiffGroup4FaxDecoder::ProcessNextCode()
	{
	if((iA0==iRowWidth) && (iHorizontalCodeCount==0))
		return(ETiffFaxEndOfLine);

	return(Decode2d());
	}

