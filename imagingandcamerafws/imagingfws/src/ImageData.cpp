// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "icl/ImageData.h"
#include "icl/ImageCodecData.h"
#include "ImageData.inl"

/**
Constructs and initialises the TFrameDataBlock object to the data type specified by aDataType.

@param  aDataType
        The data type.

@see ImageCodecData.h
 */
EXPORT_C TFrameDataBlock::TFrameDataBlock(TUid aDataType)
: iDataType(aDataType)
	{
	}

/**
Returns the data type of the current TFrameDataBlock object.

@return The data type.

@see ImageCodecData.h
*/
EXPORT_C TUid TFrameDataBlock::DataType() const
	{
	return iDataType;
	}


/**
Constructs and initialises the TImageDataBlock object to the data type specified by aDataType.

@param  aDataType
        The data type.

@see ImageCodecData.h
*/
EXPORT_C TImageDataBlock::TImageDataBlock(TUid aDataType)
: iDataType(aDataType)
	{
	}

/**
Returns the data type of the current TImageDataBlock object.

@return The data type.

@see ImageCodecData.h
*/
EXPORT_C TUid TImageDataBlock::DataType() const
	{
	return iDataType;
	}


/**
 *
 * Default constructor for this class.
 */
EXPORT_C TBmpImageData::TBmpImageData()
	: TImageDataBlock(KBMPImageDataUid)
	{
	}

TImageDataBlock* TBmpImageData::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TBmpImageData* bmpImageData = new(ELeave) TBmpImageData(*this);
	return bmpImageData;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TBmpCompression::TBmpCompression()
	: TImageDataBlock(KBMPCompressionUid)
	{
	}

TImageDataBlock* TBmpCompression::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TBmpCompression* bmpCompression = new(ELeave) TBmpCompression(*this);
	return bmpCompression;
	}


/**
 *
 * Default constructor for this class.
 */
EXPORT_C TJpegImageData::TJpegImageData()
	: TImageDataBlock(KJPGImageDataUid),
	iSampleScheme(EColor420),
	iQualityFactor(75)
	{
	}

TImageDataBlock* TJpegImageData::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TJpegImageData* jpegImageData = new(ELeave) TJpegImageData(*this);
	return jpegImageData;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TJpegQTable::TJpegQTable():
	TImageDataBlock(KJPGQTableUid),
	iTableIndex(ELumaTable),
	iEntries(KJpgQTableEntries)
	{
	}

TImageDataBlock* TJpegQTable::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TJpegQTable* jpegQTable = new(ELeave) TJpegQTable(*this);
	return jpegQTable;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TJpegComment::TJpegComment():
	TImageDataBlock(KJPGCommentUid),
	iComment(NULL)
	{
	}

TImageDataBlock* TJpegComment::DuplicateL(CFrameImageData& aFrameImageData) const
	{
	TJpegComment* jpegComment = new(ELeave) TJpegComment(*this);
	CleanupStack::PushL(jpegComment);

	jpegComment->iComment = jpegComment->iComment->AllocLC();

	User::LeaveIfError(aFrameImageData.AppendImageBuffer(jpegComment->iComment));
	CleanupStack::Pop(2); // jpegComment, jpegComment->iComment;
	return jpegComment;
	}


/**
 *
 * Default constructor for this class.
 */
EXPORT_C TGifBackgroundColor::TGifBackgroundColor()
	: TImageDataBlock(KGIFBackgroundColorUid)
	{
	}

TImageDataBlock* TGifBackgroundColor::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TGifBackgroundColor* gifBackgroundColor = new(ELeave) TGifBackgroundColor(*this);
	return gifBackgroundColor;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TGifImageDescriptor::TGifImageDescriptor()
	: TFrameDataBlock(KGIFImageDescriptorUid)
	{
	}

TFrameDataBlock* TGifImageDescriptor::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TGifImageDescriptor* gifImageDescriptor = new(ELeave) TGifImageDescriptor(*this);
	return gifImageDescriptor;
	}

/**
 * Constructor for this class.
 * Set the data type for this class to KGIFColorTableUid
 */
EXPORT_C TGifColorTable::TGifColorTable()
	: TFrameDataBlock(KGIFColorTableUid)
	{
	}

/**
Create a copy of this class.

@param	aFrameImageData
	Not used
@return	A copy of the class. Ownership is passed to the caller.
*/
TFrameDataBlock* TGifColorTable::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TGifColorTable* gifColorTable = new(ELeave) TGifColorTable(*this);
	return gifColorTable;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TGifImageControl::TGifImageControl()
	: TFrameDataBlock(KGIFImageControlUid)
	{
	}

TFrameDataBlock* TGifImageControl::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TGifImageControl* gifImageControl = new(ELeave) TGifImageControl(*this);
	return gifImageControl;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TGifLZWInfo::TGifLZWInfo()
	: TFrameDataBlock(KGIFLZWInfoUid),
	iCompressedBytes(0)
	{
	}

TFrameDataBlock* TGifLZWInfo::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TGifLZWInfo* gifLZWInfo = new(ELeave) TGifLZWInfo(*this);
	return gifLZWInfo;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TGifComment::TGifComment()
	: TImageDataBlock(KGIFCommentUid),
	iComment(NULL)
	{
	}

TImageDataBlock* TGifComment::DuplicateL(CFrameImageData& aFrameImageData) const
	{
	TGifComment* gifComment = new(ELeave) TGifComment(*this);
	CleanupStack::PushL(gifComment);

	gifComment->iComment = gifComment->iComment->AllocLC();

	User::LeaveIfError(aFrameImageData.AppendImageBuffer(gifComment->iComment));
	CleanupStack::Pop(2); // gifComment, gifComment->iComment;
	return gifComment;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TGifLoopIterations::TGifLoopIterations()
    : TImageDataBlock(KGIFLoopIterationsUid),
    iLoopIterations(0)
    {
    }

TImageDataBlock* TGifLoopIterations::DuplicateL(CFrameImageData& /*aFrameImageData*/) const
    {
    TGifLoopIterations* gifIterations = new(ELeave) TGifLoopIterations(*this);
    return gifIterations;
    }

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TMbmDecodeData::TMbmDecodeData()
	: TFrameDataBlock(KMBMDecodeDataUid)
	{
	}

TFrameDataBlock* TMbmDecodeData::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TMbmDecodeData* mbmDecodeData = new(ELeave) TMbmDecodeData(*this);
	return mbmDecodeData;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TMbmEncodeData::TMbmEncodeData()
	: TFrameDataBlock(KMBMEncodeDataUid)
	{
	}

TFrameDataBlock* TMbmEncodeData::DuplicateL(CFrameImageData& /* aFrameImageData */ ) const
	{
	TMbmEncodeData* mbmEncodeData = new(ELeave) TMbmEncodeData(*this);
	return mbmEncodeData;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TPngEncodeData::TPngEncodeData()
	: TFrameDataBlock(KPNGEncodeDataUid)
	{
	}

TFrameDataBlock* TPngEncodeData::DuplicateL(CFrameImageData& /* aFrameImageData */) const
	{
	TPngEncodeData* pngEncodeData = new(ELeave) TPngEncodeData(*this);
	return pngEncodeData;
	}

/**
 *
 * Default constructor for this class.
 */
EXPORT_C TTiffImageData::TTiffImageData():
	TFrameDataBlock(KTIFFImageDataUid),
	iStripInfo(NULL)
	{
	}

TFrameDataBlock* TTiffImageData::DuplicateL(CFrameImageData& aFrameImageData) const
	{
	TTiffImageData* tiffImageData = new(ELeave) TTiffImageData(*this);
	CleanupStack::PushL(tiffImageData);

	tiffImageData->iStripInfo = tiffImageData->iStripInfo->AllocLC();

	User::LeaveIfError(aFrameImageData.AppendImageBuffer(tiffImageData->iStripInfo));
	CleanupStack::Pop(2); // tiffImageData, tiffImageData->iStripInfo;
	return tiffImageData;
	}

//
// TFrameInfo
//

/**
Returns the current frame state.

@return The frame state.
*/
EXPORT_C TFrameInfo::TFrameInfoState TFrameInfo::CurrentFrameState() const
	{
	return iCurrentFrameState;
	}

/**
Sets the current frame state to the supplied value.

@param  aFrameInfoState
        The frame state.
*/
EXPORT_C void TFrameInfo::SetCurrentFrameState(TFrameInfoState aFrameInfoState)
	{
	iCurrentFrameState = aFrameInfoState;
	}

/**
Returns the current data offset.

@return The data offset.
*/
EXPORT_C TInt TFrameInfo::CurrentDataOffset() const
	{
	return iCurrentDataOffset;
	}

/**
Sets the current data offset to the supplied value.

@param  aOffset
        The data offset.
*/
EXPORT_C void TFrameInfo::SetCurrentDataOffset(TInt aOffset)
	{
	iCurrentDataOffset = aOffset;
	}

/**
Returns the frame data offset.

@return The frame data offset.
*/
EXPORT_C TInt TFrameInfo::FrameDataOffset() const
	{
	return iFrameDataOffset;
	}

/**
Sets the frame data offset to the supplied value.

@param  aOffset
        The frame data offset.
*/
EXPORT_C void TFrameInfo::SetFrameDataOffset(TInt aOffset)
	{
	iFrameDataOffset = aOffset;
	}
