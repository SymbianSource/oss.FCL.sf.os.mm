// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef FWEXTTESTCODEC_H
#define FWEXTTESTCODEC_H

#include <icl/imagecodec.h>
#include "fwexttestconvert.h"

// Forward declaration
class CFwExtTestDecoderPlugin;

// Read Codec
class CFwExtReadCodec : public CImageReadCodec
	{
public:
	static CFwExtReadCodec* NewL(CFwExtTestDecoderPlugin& aDecoder);
	
	// From CImageReadCodec
	void InitFrameL(TFrameInfo& aFrameInfo, CFrameImageData& aFrameImageData, TBool aDisableErrorDiffusion, CFbsBitmap& aDestination, CFbsBitmap* aDestinationMask);
	void InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& aFrameData);
	TFrameState ProcessFrameL(TBufPtr8& aSrc);
	void SetPrepareProcessing(); // set flag that Prepare in progress
	
private:
	CFwExtReadCodec(CFwExtTestDecoderPlugin& aDecoder);
	
private:
	TInt iLoops;
	TBool iPrepareProcessing; // to distinguish between Prepare and Convert
	CFwExtTestDecoderPlugin* iDecoder; // not owned
	};

// Write Codec
class CFwExtWriteCodec : public CImageWriteCodec
	{
public:	
	static CFwExtWriteCodec* NewL(CFwExtTestEncoderPlugin& aEncoder);
		
	void ResetCount();
	void Complete();
	
	// From CImageWriteCodec
	TFrameState ProcessFrameL(TBufPtr8& aDst);
	void SetPrepareProcessing(); // set flag that Prepare in progress
	
private:
	CFwExtWriteCodec(CFwExtTestEncoderPlugin& aEncoder);
	
private:
	TInt iLoops;
	TBool iPrepareProcessing; // to distinguish between Prepare and Convert/AddBlocks
	TBool iComplete;
	CFwExtTestEncoderPlugin* iEncoder; // not owned
	};

#endif // FWEXTTESTCODEC_H
