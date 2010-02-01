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

#include <fbs.h>
#include "TestExifDerived.h"


/*static*/
CTestExifDerivedDecoder* CTestExifDerivedDecoder::NewL()
	{
	CTestExifDerivedDecoder* self=new (ELeave) CTestExifDerivedDecoder();
	CleanupStack::PushL(self);
	self->CJPEGExifDecoder::ConstructL();
	CleanupStack::Pop(self);
	return self;
	}



/*static*/
CTestExifDerivedEncoder* CTestExifDerivedEncoder::NewL()
	{
	CTestExifDerivedEncoder* self=new (ELeave) CTestExifDerivedEncoder();
	CleanupStack::PushL(self);
	self->CJPEGExifEncoder::ConstructL();
	CleanupStack::Pop(self);
	return self;	
	}
	
/*static*/
CDummyExifDecoder* CDummyExifDecoder::NewL()
	{
	return new (ELeave) CDummyExifDecoder();
	}
	
MExifMetadata* CDummyExifDecoder::ExifMetadata()
	{
	return reinterpret_cast<MExifMetadata*>(1);
	}
	
void CDummyExifDecoder::ImageType(TInt /*aFrameNumber*/, TUid& /*aImageType*/, TUid& /*aImageSubType*/) const
	{
	}
	
void CDummyExifDecoder::ScanDataL()
	{
	}
	
CFrameInfoStrings* CDummyExifDecoder::FrameInfoStringsL(RFs& /*aFs*/, TInt /*aFrameNumber*/)
	{
	return NULL;
	}

/*static*/
CDummyExifEncoder* CDummyExifEncoder::NewL()
	{
	return new (ELeave) CDummyExifEncoder();
	}
	
MExifMetadata* CDummyExifEncoder::ExifMetadata()
	{
	return reinterpret_cast<MExifMetadata*>(1);
	}
	
void CDummyExifEncoder::PrepareEncoderL(const CFrameImageData* /*aFrameImageData*/)
	{
	}
	
void CDummyExifEncoder::UpdateHeaderL()
	{
	}



